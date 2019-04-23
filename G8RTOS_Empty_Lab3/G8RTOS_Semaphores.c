/*
 * G8RTOS_Semaphores.c
 */

/*********************************************** Dependencies and Externs *************************************************************/

#include <stdint.h>
#include "msp.h"
#include "G8RTOS_Semaphores.h"
#include "G8RTOS_CriticalSection.h"
#include "G8RTOS_Scheduler.h"
#include "G8RTOS_Structures.h"
/*********************************************** Dependencies and Externs *************************************************************/
#define ICSR (*((volatile unsigned int*)(0xe000ed04)))
#define ICSR_PENDSVSET (1 << 28)

/*********************************************** Public Functions *********************************************************************/

/*
 * Initializes a semaphore to a given value
 * Param "s": Pointer to semaphore
 * Param "value": Value to initialize semaphore to
 * THIS IS A CRITICAL SECTION
 */
void G8RTOS_InitSemaphore(semaphore_t *s, int32_t value)
{
    int32_t semaphore_state = StartCriticalSection(); //Saves the state of the current PRIMASK (I-bit), Disables interrupts Returns: The current PRIMASK State
    *s = value; // Set the semaphore to desired initial state
    EndCriticalSection(semaphore_state); //Restores the state of the PRIMASK given an a PRIMASK State to update
}

/*
 * No longer waits for semaphore
 *  - Decrements semaphore
 *  - Blocks thread is sempahore is unavalible
 * Param "s": Pointer to semaphore to wait on
 * THIS IS A CRITICAL SECTION
 */
void G8RTOS_WaitSemaphore(semaphore_t *s)
{
    int32_t semaphore_state = StartCriticalSection();

    // semaphore is available decrement it making it unavailable
    (*s)--;

    // if semaphore is less than zero it is unavailable and thread becomes blocked
    if((*s) < 0){
        // block thread
        CurrentlyRunningThread->blocked = s; // set block pointer to semaphore to wait on
        // Yield to allow another thread to run
        ICSR |= ICSR_PENDSVSET; // Initiates context switch by setting pendsv in icsr register
    }

    EndCriticalSection(semaphore_state);
}

/*
 * Signals the completion of the usage of a semaphore
 *  - Increments the semaphore value by 1
 *  - Unblocks any threads waiting on that semaphore
 * Param "s": Pointer to semaphore to be signaled
 * THIS IS A CRITICAL SECTION
 */
void G8RTOS_SignalSemaphore(semaphore_t *s)
{
    int32_t semaphore_state = StartCriticalSection();
    // increment the semaphore value
    (*s)++; // semaphore has been released

    // unblock any threads that may be waiting
    if((*s) <= 0){
        // retrieve the next tcb ptr by looking at current tcb
        tcb_t *tcb_ptr = CurrentlyRunningThread->next_TCB;
        // search for threads which are blocked by this semaphore
        while(tcb_ptr-> blocked != s){
            tcb_ptr = tcb_ptr->next_TCB;
        }
        // release the thread
        tcb_ptr->blocked = 0;
    }
    EndCriticalSection(semaphore_state);
}

/*********************************************** Public Functions *********************************************************************/


