/*
 * G8RTOS_Structure.h
 *
 *  Created on: Jan 12, 2017
 *      Author: Raz Aloni
 */

#ifndef G8RTOS_STRUCTURES_H_
#define G8RTOS_STRUCTURES_H_

#include "G8RTOS.h"
#include <stdbool.h>


/*********************************************** Data Structure Definitions ***********************************************************/

/*
 *  Thread Control Block:
 *      - Every thread has a Thread Control Block
 *      - The Thread Control Block holds information about the Thread Such as the Stack Pointer, Priority Level, and Blocked Status
 *      - For Lab 2 the TCB will only hold the Stack Pointer, next TCB and the previous TCB (for Round Robin Scheduling)
 */

/* Create tcb struct here */
typedef struct tcb_t tcb_t;
struct tcb_t {
    int32_t *stack_ptr; // Points to the stack for corresponding thread
    tcb_t *next_TCB; // Points to the next tcb structure
    tcb_t *previous_TCB; // Points to the previous tcb structure
    semaphore_t *blocked; // blocked semaphore contains a 0 not blocked or the semaphore that the thread is currently waiting on
    uint32_t sleep_count; // duration + system time
    uint8_t asleep; // sleep flag tells if thread is asleep
    uint8_t priority; // could hold priority

    bool is_alive; // thread dead or alive
    char thread_name[MAX_NAME_LENGTH];
    threadId_t thread_id;
};

/*
 *  Periodic Thread Control Block:
 *      - Holds a function pointer that points to the periodic thread to be executed
 *      - Has a period in us
 *      - Holds Current time
 *      - Contains pointer to the next periodic event - linked list
 */

/* Create periodic thread struct here */

/*********************************************** Data Structure Definitions ***********************************************************/
typedef struct ptcb_t ptcb_t;
struct ptcb_t {
    void(*Handler)(void); // function for pthread
    uint32_t period; // executes at this time period
    uint32_t execute_time; // must execute in this amount of time
    uint32_t current_time; // current time
    ptcb_t *previous_PTCB; // Points to the previous ptcb structure
    ptcb_t *next_PTCB; // Points to the next ptcb structure

};


/*********************************************** Public Variables *********************************************************************/

tcb_t * CurrentlyRunningThread;

/*********************************************** Public Variables *********************************************************************/




#endif /* G8RTOS_STRUCTURES_H_ */
