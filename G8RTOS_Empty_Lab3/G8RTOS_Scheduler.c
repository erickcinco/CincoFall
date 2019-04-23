/*
 * G8RTOS_Scheduler.c
 */

/*********************************************** Dependencies and Externs *************************************************************/

#include <stdint.h>
#include <stdbool.h>
#include "msp.h"
#include "G8RTOS_Scheduler.h"
#include "G8RTOS_Structures.h"
#include "BSP.h"
#include "G8RTOS_CriticalSection.h"
#include "G8RTOS_Semaphores.h"

//#include <string.h>


/*
 * G8RTOS_Start exists in asm
 */
extern void G8RTOS_Start();

/* System Core Clock From system_msp432p401r.c */
extern uint32_t SystemCoreClock;

/*
 * Pointer to the currently running Thread Control Block
 */
extern tcb_t *CurrentlyRunningThread;

/*********************************************** Dependencies and Externs *************************************************************/

#define ICSR (*((volatile unsigned int*)(0xE000ED04)))
#define ICSR_PENDSVSET (1 << 28)

/*********************************************** Defines ******************************************************************************/

/* Status Register with the Thumb-bit Set */
#define THUMBBIT 0x01000000

/*********************************************** Defines ******************************************************************************/


/*********************************************** Data Structures Used *****************************************************************/

/* Thread Control Blocks
 *	- An array of thread control blocks to hold pertinent information for each thread
 */
static tcb_t threadControlBlocks[MAX_THREADS];

/* Thread Stacks
 *	- An array of arrays that will act as invdividual stacks for each thread
 */
static int32_t threadStacks[MAX_THREADS][STACKSIZE];

/* Periodic Event Threads
 * - An array of periodic events to hold pertinent information for each thread
 */
static ptcb_t Pthread[MAX_PTHREADS];

sched_ErrCode_t SchedulerError;

/*********************************************** Data Structures Used *****************************************************************/


/*********************************************** Private Variables ********************************************************************/

/*
 * Current Number of Threads currently in the scheduler
 */
static uint32_t NumberOfThreads;


/*
 * Current Number of Periodic Threads currently in the scheduler
 */
static uint32_t NumberOfPthreads;

/*********************************************** Private Variables ********************************************************************/


/*********************************************** Private Functions ********************************************************************/

/*
 * Initializes the Systick and Systick Interrupt
 * The Systick interrupt will be responsible for starting a context switch between threads
 * Param "numCycles": Number of cycles for each systick interrupt
 */
static void InitSysTick(uint32_t numCycles)
{
    SysTick_Config(numCycles); // systick will fire every 1 ms
    SysTick_enableInterrupt();
}

/*
 * Chooses the next thread to run.
 * Lab 4 Scheduling Algorithm:
 * 	- Priority scheduler:
 * iterate through all the other threads
 * if tempNextThread is neither sleeping or blocked, we check if its priority value is less
 * than a currentMaxPriority value (initial currentMaxPriority value will be 256)
 * if it is, we set the CurrentRunningThread equal to the thread with the higher priority,
 * and reinitialize the currentMaxPriority
 * set the next thread to check equal to the next thread in the linked list to keep checking
 * for the rest of the threads (potentially of higher priority).
 */
void G8RTOS_Scheduler()
{
    tcb_t * tempNextThread;
    tempNextThread = CurrentlyRunningThread->next_TCB; // set tempNextThread to be the next thread in the linked list
    uint8_t currentMaxPriority = 255; // initialize currentMaxPriority to lowest aka 256

    // iterate through every thread once to make sure all priorities have been check
    for(uint32_t check_thread = 0; check_thread < NumberOfThreads; check_thread++){
        //if not blocked or sleeping check priority
        if(!(tempNextThread->blocked) && !(tempNextThread->asleep)){
            if((tempNextThread->priority) < currentMaxPriority){
                CurrentlyRunningThread = tempNextThread; // set currentlyRunningThread to thread with higher priority
                currentMaxPriority = CurrentlyRunningThread->priority; // reinitialize currentMaxPriority
            }
        }
        tempNextThread = tempNextThread->next_TCB; // iterate through rest of threads (potentially of higher priority)
    }
}


/*
 * SysTick Handler
 * The Systick Handler now will increment the system time,
 * set the PendSV flag to start the scheduler,
 * and be responsible for handling sleeping and periodic threads
 */
void SysTick_Handler()
{
    SystemTime++; //increment the system time
    tcb_t *tcb_ptr = CurrentlyRunningThread; //retrieve current thread
    ptcb_t *pt_ptr = &Pthread[0]; // retrieve periodic threads

    // update periodic threads by iterating through
    for(uint16_t i=0; i<NumberOfPthreads; i++){
        // if execute time matches system time we will update the next execute time and run attached function
        if(pt_ptr->execute_time == SystemTime){
            pt_ptr->execute_time = pt_ptr->period + SystemTime;
            (*(pt_ptr->Handler))(); //execute attached handler
        }
        pt_ptr = pt_ptr->next_PTCB; // move to next pthread
    }

    // update threads and check for sleep
    for(uint16_t i=0; i<NumberOfThreads; i++){
        // if asleep check current count
        if(tcb_ptr->asleep){ //check if this syntax works else check if it is == 1
            // wake up if system time has passed our sleep count
            if(tcb_ptr->sleep_count <= SystemTime){
                tcb_ptr->asleep = 0; // set asleep to false;
            }
        }
        // grab next tcb
        tcb_ptr = tcb_ptr->next_TCB;
    }
    // set the PendSV flag to start the scheduler
    ICSR |= ICSR_PENDSVSET;
}

/*********************************************** Private Functions ********************************************************************/


/*********************************************** Public Variables *********************************************************************/

/* Holds the current time for the whole System */
uint32_t SystemTime;
// Current set of id's
static uint16_t IDCounter;
static uint16_t tcbToInitialize;
uint8_t max_priority_index = 0; // will hold the index of max priority
/*********************************************** Public Variables *********************************************************************/


/*********************************************** Public Functions *********************************************************************/

/*
 * Sets variables to an initial state (system time and number of threads)
 * Enables board for highest speed clock and disables watchdog
 */
void G8RTOS_Init()
{
    // rtos config
    SystemTime = 0; // init system time to 0
    NumberOfThreads = 0; // init number of threads to 0
    NumberOfPthreads = 0; // init number of periodic threads to 0
    IDCounter = 0;
    tcbToInitialize = 0;

    for(uint16_t i=0; i<MAX_THREADS; i++){
        threadControlBlocks[i].is_alive = false;
        threadControlBlocks[i].priority = 255;
    }

    uint32_t newVTORTable = 0x20000000;
    memcpy((uint32_t *)newVTORTable, (uint32_t *)SCB->VTOR, 57*4); // 57 interrupt vectors to copy
    SCB->VTOR = newVTORTable;

    // hardware config
    BSP_InitBoard();
}

/*
 * Starts G8RTOS Scheduler
 * 	- Initializes the Systick
 * 	- Sets Context to first thread
 * Returns: Error Code for starting scheduler. This will only return if the scheduler fails
 */
int G8RTOS_Launch()
{
    NVIC_SetPriority (SysTick_IRQn,6);
    NVIC_SetPriority (PendSV_IRQn,7);

    CurrentlyRunningThread = &threadControlBlocks[0]; // set tempNextThread to first thread in linked list

    G8RTOS_Scheduler();

    InitSysTick(ClockSys_GetSysFreq()/1000); // systick will fire every 1 ms
    G8RTOS_Start();
    return 0; // only reached if there is a scheduler failure
}


/*
 * Adds threads to G8RTOS Scheduler
 * 	- Checks if there are stil available threads to insert to scheduler
 * 	- Initializes the thread control block for the provided thread
 * 	- Initializes the stack for the provided thread to hold a "fake context"
 * 	- Sets stack tcb stack pointer to top of thread stack
 * 	- Sets up the next and previous tcb pointers in a round robin fashion
 * Param "threadToAdd": Void-Void Function to add as preemptable main thread
 * Returns: Error code for adding threads
 */

int G8RTOS_AddThread(void (*threadToAdd)(void), uint8_t thread_priority, char * name)
{
    int32_t add_thread_state = StartCriticalSection(); // start crit section because adding while os running
    uint16_t dead_thread_index = 0;
    bool dead_thread_found = false;
//    threadId_t unique_id;

    if(NumberOfThreads == MAX_THREADS){
        EndCriticalSection(add_thread_state);
        return THREAD_LIMIT_REACHED; // error we cannot add more threads maxed out
    }

    NumberOfThreads++; // Increase the total number of threads currently being used

    // iterate through tcbs for first dead tcb
    for(uint16_t i=0; i<MAX_THREADS; i++){
        if(threadControlBlocks[i].is_alive == false){
            dead_thread_index = i;
            // add current thread to thread stack
            // PSR to some default value that has the thumb-bit set
            threadStacks[dead_thread_index][STACKSIZE-1] = THUMBBIT; //bit 32 thumb-bit is set PSR
            // PC (R15) to thread’s function pointer (the void-void function pointer that the Add_Threadfunction receives)
            threadStacks[dead_thread_index][STACKSIZE-2] = (int32_t) (threadToAdd);
            // rest of thread stack is init automatically during context switch

            threadControlBlocks[dead_thread_index].asleep = 0;// init sleep to false
            threadControlBlocks[dead_thread_index].is_alive = true;// init sleep to false
            threadControlBlocks[dead_thread_index].priority = thread_priority; // set priority to user value
            threadControlBlocks[dead_thread_index].thread_id = ((IDCounter++)<<16 | tcbToInitialize); // generate a unique id
            threadControlBlocks[dead_thread_index].blocked = 0; // initially not blocked
            threadControlBlocks[dead_thread_index].sleep_count = 0; // initial sleep count is 0
            threadControlBlocks[dead_thread_index].stack_ptr = &threadStacks[dead_thread_index][STACKSIZE-16];
            dead_thread_found = true;
            break;
        }
    }

    if(!dead_thread_found){
        EndCriticalSection(add_thread_state);
        return THREADS_INCORRECTLY_ALIVE;
    }

    // set thread name by iterating name string till we hit null terminator
    for(uint16_t i=0; *name!= 0; i++){
        threadControlBlocks[dead_thread_index].thread_name[i] = *name; // set thread name
        name++;
    }

    // update next and prev tcb's

    // handle first thread case... have it point to itself
    if(NumberOfThreads == 1){
        threadControlBlocks[dead_thread_index].next_TCB = &threadControlBlocks[dead_thread_index];
        threadControlBlocks[dead_thread_index].previous_TCB = &threadControlBlocks[dead_thread_index];
        EndCriticalSection(add_thread_state);
        return NO_ERROR;
    }

    // handle lowest priority and highest priority case
    if(threadControlBlocks[dead_thread_index].priority <= threadControlBlocks[max_priority_index].priority || threadControlBlocks[dead_thread_index].priority >= threadControlBlocks[max_priority_index].previous_TCB->priority){
        threadControlBlocks[dead_thread_index].next_TCB = &threadControlBlocks[max_priority_index]; // set next tcb to max priority tcb
        threadControlBlocks[dead_thread_index].previous_TCB = threadControlBlocks[max_priority_index].previous_TCB; // set prev tcb to the prev tcb of max priority
        // update highest priority thread next and prev
        threadControlBlocks[max_priority_index].previous_TCB->next_TCB = &threadControlBlocks[dead_thread_index]; // set next thread of max priority previous tcb to found dead thread
        threadControlBlocks[max_priority_index].previous_TCB = &threadControlBlocks[dead_thread_index]; // set previous thread of max priority to current dead thread

        // update thread with highest priority if dead thread priority exceeds the current max priority threads priority
        if(threadControlBlocks[dead_thread_index].priority <= threadControlBlocks[max_priority_index].priority){
            max_priority_index = dead_thread_index;
        }
    }

    EndCriticalSection(add_thread_state);
    return NO_ERROR;
}


/*
 * Adds periodic threads to G8RTOS Scheduler
 * Function will initialize a periodic event struct to represent event.
 * The struct will be added to a linked list of periodic events
 * Param Pthread To Add: void-void function for P thread handler
 * Param period: period of P thread to add
 * Returns: Error code for adding threads
 */
int G8RTOS_AddPeriodicEvent(void (*PthreadToAdd)(void), uint32_t period)
{
    int32_t add_Pthread_state = StartCriticalSection(); // start crit section because adding while os running

    if(NumberOfPthreads == MAX_PTHREADS){
        return -1; // we have reached max periodic threads return error
    }

    Pthread[NumberOfPthreads].Handler = PthreadToAdd; // attach thread
    Pthread[NumberOfPthreads].period = period; // set period paramenter of ptcb
    Pthread[NumberOfPthreads].current_time = 0; // initialize time to 0
    NumberOfPthreads++; //increment number of pthreads

    // pthread list
    for(uint16_t i=0; i < NumberOfPthreads; i++){
        Pthread[i].next_PTCB = &Pthread[(i+1) % NumberOfPthreads]; // set next zzzzzzzzcb ptr with wrap around
//         TODO should it be NumberOfThreads + i - 1
        Pthread[i].previous_PTCB = &Pthread[(NumberOfPthreads + i - 1) % NumberOfPthreads]; // set previous ptcb ptr with wrap around
        // if we have not reached end of list check if there is a pthread with multiple of new pthread period, if so offset it by 1
        if(i != NumberOfPthreads-1){
            if(Pthread[i].period%period == 0 || period%Pthread[i].period == 0){
                Pthread[NumberOfPthreads-1].current_time++;
            }
        }
    }
    // handle the newly added thread which is Num-1 because we incremented the number of pthreads
    // Set the execution time to new period + previous current time + system time
    Pthread[NumberOfPthreads-1].execute_time = period + Pthread[NumberOfPthreads-1].current_time + SystemTime;

    EndCriticalSection(add_Pthread_state);

    return 0; //all good
}

threadId_t G8RTOS_GetThreadId()
{
    return CurrentlyRunningThread->thread_id;
}

sched_ErrCode_t G8RTOS_KillThread(threadId_t threadId)
{
    int32_t kill_thread_state = StartCriticalSection(); // start crit section because adding while os running
    if(NumberOfThreads == 1){
        EndCriticalSection(kill_thread_state);
        return CANNOT_KILL_LAST_THREAD;
    }

    tcbToInitialize++;
    //search for thread with same threadId
    uint16_t kill_index;
    for(kill_index=0; kill_index<MAX_THREADS; kill_index++){
        // thread to kill has been found
        if(threadControlBlocks[kill_index].thread_id == threadId){
            threadControlBlocks[kill_index].is_alive = false;
            threadControlBlocks[kill_index].next_TCB->previous_TCB = threadControlBlocks[kill_index].previous_TCB; // set tcb to be killed next tcb's previous tcb to the next tcb of tcb to be killed
            threadControlBlocks[kill_index].previous_TCB->next_TCB = threadControlBlocks[kill_index].next_TCB; // set tcb to be killed previous tcb's next tcb to the next tcb of tcb to be killed

            // check if we killed the thread with the max priority index and if so update with newest max priority index
            for(uint16_t new_max_priority_index=0; new_max_priority_index < MAX_THREADS; new_max_priority_index++){
                if(threadControlBlocks[kill_index].next_TCB->thread_id == threadControlBlocks[new_max_priority_index].thread_id){
                    if(threadControlBlocks[new_max_priority_index].priority <= threadControlBlocks[max_priority_index].priority){
                        max_priority_index = new_max_priority_index; // update index for thread with highest priority
                    }
                }
            }
            NumberOfThreads--; // decrement the current number of active threads
            // thread being killed is equal to currently running thread we need to context switch once crit section ends
            if(CurrentlyRunningThread == &threadControlBlocks[kill_index]){
                EndCriticalSection(kill_thread_state); // see if we can get rid of this
                // set the PendSV flag to start the scheduler
                ICSR |= ICSR_PENDSVSET;
                return NO_ERROR;
            }
            EndCriticalSection(kill_thread_state);
            return NO_ERROR;
        }
    }
    // If it makes it through the for loop then the thread does not exist
    EndCriticalSection(kill_thread_state);
    return THREAD_DOES_NOT_EXIST;
}

sched_ErrCode_t G8RTOS_KillSelf()
{
    return G8RTOS_KillThread(CurrentlyRunningThread->thread_id);
}

sched_ErrCode_t G8RTOS_KillAllOtherThreads(){
    int32_t kill_all_other_threads_state = StartCriticalSection(); // start crit section because adding while os running
    if(NumberOfThreads == 1){
        EndCriticalSection(kill_all_other_threads_state);
        return CANNOT_KILL_LAST_THREAD;
    }
    uint16_t error = NO_ERROR;
    //kill all threads that do not have matching id threadId
    uint16_t kill_index;
    for(kill_index=0; kill_index<MAX_THREADS; kill_index++){
        // thread to kill has been found
        if(threadControlBlocks[kill_index].thread_id != CurrentlyRunningThread->thread_id && threadControlBlocks[kill_index].is_alive){
            error = G8RTOS_KillThread(threadControlBlocks[kill_index].thread_id);
        }
    }
    // If it makes it through the for loop then the thread does not exist
    EndCriticalSection(kill_all_other_threads_state);
    return NO_ERROR;
}


sched_ErrCode_t G8RTOS_AddAPeriodicEvent(void (*AthreadToAdd)(void), uint8_t priority, IRQn_Type IRQn)
{
    int32_t add_APer_event_state = StartCriticalSection(); // start crit section because adding while os running

    // verify the irqn is less than the last exception (PSS_IRQn) and greater than the last acceptable userIRQn(port6_irqn)
    if((IRQn < PSS_IRQn) || (IRQn > PORT6_IRQn)){
        EndCriticalSection(add_APer_event_state);
        return IRQn_INVALID;
    }
    // verify priority is not greater than 6, the greatest user priority number
    if(priority > 6){
        EndCriticalSection(add_APer_event_state);
        return HWI_PRIORITY_INVALID;
    }

    __NVIC_SetVector(IRQn, (uint32_t) AthreadToAdd);
    __NVIC_SetPriority(IRQn, priority);
    __NVIC_EnableIRQ(IRQn);
    EndCriticalSection(add_APer_event_state);
    return NO_ERROR;
}


/*
 * Puts the current thread into a sleep state.
 *  param durationMS: Duration of sleep time in ms
 */
void sleep(uint32_t durationMS)
{
    CurrentlyRunningThread->asleep = 1; // set sleep flag for current thread
    CurrentlyRunningThread->sleep_count = durationMS + SystemTime; // wait till system time matches our sleep count
    ICSR |= ICSR_PENDSVSET; // call systick handler

}





/*********************************************** Public Functions *********************************************************************/
