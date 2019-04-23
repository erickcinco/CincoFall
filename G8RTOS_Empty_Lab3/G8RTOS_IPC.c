/*
 * G8RTOS_IPC.c
 *
 *  Created on: Jan 10, 2017
 *      Author: Daniel Gonzalez
 */
#include <stdint.h>
#include "msp.h"
#include "G8RTOS_IPC.h"
#include "G8RTOS_Semaphores.h"



/*********************************************** Defines ******************************************************************************/


/*********************************************** Data Structures Used *****************************************************************/

/*
 * FIFO struct will hold
 *  - buffer
 *  - head
 *  - tail
 *  - lost data
 *  - current size
 *  - mutex
 */

/* Create FIFO struct here */
typedef struct FIFO_T FIFO_T;
struct FIFO_T{
    int32_t Buffer[FIFOSIZE];
    int32_t *Head;
    int32_t *Tail;
    uint32_t LostData;
    semaphore_t CurrentSize;
    semaphore_t Mutex;
};

//typedef struct FIFO_ball FIFO_ball;
//struct FIFO_ball{
//    Ball Buffer[FIFOSIZE];
//    Ball *Head;
//    Ball *Tail;
//    uint32_t LostData;
//    semaphore_t CurrentSize;
//    semaphore_t Mutex;
//};



/* Array of FIFOS */
static FIFO_T FIFOs[MAX_NUMBER_OF_FIFOS];
//static FIFO_ball BallFIFOs[MAX_NUMBER_OF_FIFOS];

/*********************************************** Data Structures Used *****************************************************************/

/*
 * Initializes FIFO Struct
 */
int G8RTOS_InitFIFO(uint32_t FIFOIndex)
{
    if(FIFOIndex > MAX_NUMBER_OF_FIFOS - 1){
        return -1;
    }
    // read from head
    // write at tail
    FIFOs[FIFOIndex].Head = &(FIFOs[FIFOIndex].Buffer[0]); // point read location to beginning of fifo buff because size is 0
    FIFOs[FIFOIndex].Tail = &(FIFOs[FIFOIndex].Buffer[0]); // point write location to beginning of fifo buff because size is 0
    FIFOs[FIFOIndex].LostData = 0; // no lost data yet!
    G8RTOS_InitSemaphore(&(FIFOs[FIFOIndex].CurrentSize), 0); // init current size semaphore of fifo buff to 0
    G8RTOS_InitSemaphore(&(FIFOs[FIFOIndex].Mutex), 1); // init fifo mutex to 1 aka available
    return 0;
}

/*
 * Initializes FIFO Struct
 */
//int G8RTOS_InitBallFIFO(uint32_t FIFOIndex)
//{
//    if(FIFOIndex > MAX_NUMBER_OF_FIFOS - 1){
//        return -1;
//    }
    // read from head
    // write at tail
//    BallFIFOs[FIFOIndex].Head = &(BallFIFOs[FIFOIndex].Buffer[0]); // point read location to beginning of fifo buff because size is 0
//    BallFIFOs[FIFOIndex].Tail = &(BallFIFOs[FIFOIndex].Buffer[0]); // point write location to beginning of fifo buff because size is 0
//    BallFIFOs[FIFOIndex].LostData = 0; // no lost data yet!
//    G8RTOS_InitSemaphore(&(BallFIFOs[FIFOIndex].CurrentSize), 0); // init current size semaphore of fifo buff to 0
//    G8RTOS_InitSemaphore(&(BallFIFOs[FIFOIndex].Mutex), 1); // init fifo mutex to 1 aka available
//    return 0;
//}

/*
 * Reads FIFO
 *  - Waits until CurrentSize semaphore is greater than zero
 *  - Gets data and increments the head ptr (wraps if necessary)
 * Param: "FIFOChoice": chooses which buffer we want to read from
 * Returns: uint32_t Data from FIFO
 */
uint32_t readFIFO(uint32_t FIFOChoice)
{
    uint32_t fifo_data;
    G8RTOS_WaitSemaphore(&(FIFOs[FIFOChoice].CurrentSize)); // currentsize semaphore minus 1 decrease buffer semaphore because we read data
    G8RTOS_WaitSemaphore(&(FIFOs[FIFOChoice].Mutex)); // buffer mutex minus 1 aka will no longer be available
    // see if any data is available
    fifo_data = *(FIFOs[FIFOChoice].Head++); // read from head post increment head ptr
    // handle the case where the head ptr reaches end of buffer location
    if(FIFOs[FIFOChoice].Head == &(FIFOs[FIFOChoice].Buffer[(FIFOSIZE-1)])){
        FIFOs[FIFOChoice].Head = &(FIFOs[FIFOChoice].Buffer[0]); // point back to front of fifo
    }

    G8RTOS_SignalSemaphore(&(FIFOs[FIFOChoice].Mutex)); // release the buffer mutex aka now can write
    return fifo_data;
}

/*
 * Reads FIFO
 *  - Waits until CurrentSize semaphore is greater than zero
 *  - Gets data and increments the head ptr (wraps if necessary)
 * Param: "FIFOChoice": chooses which buffer we want to read from
 * Returns: uint32_t Data from FIFO
 */
//Ball readBallFIFO(uint32_t FIFOChoice)
//{
//    Ball fifo_data;
//    G8RTOS_WaitSemaphore(&(BallFIFOs[FIFOChoice].CurrentSize)); // currentsize semaphore minus 1 decrease buffer semaphore because we read data
//    G8RTOS_WaitSemaphore(&(BallFIFOs[FIFOChoice].Mutex)); // buffer mutex minus 1 aka will no longer be available
//    // see if any data is available
//    fifo_data = *(BallFIFOs[FIFOChoice].Head++); // read from head post increment head ptr
//    // handle the case where the head ptr reaches end of buffer location
//    if(BallFIFOs[FIFOChoice].Head == &(BallFIFOs[FIFOChoice].Buffer[(FIFOSIZE-1)])){
//        BallFIFOs[FIFOChoice].Head = &(BallFIFOs[FIFOChoice].Buffer[0]); // point back to front of fifo
//    }
//
//    G8RTOS_SignalSemaphore(&(BallFIFOs[FIFOChoice].Mutex)); // release the buffer mutex aka now can write
//    return fifo_data;
//}

/*
 * Writes to FIFO
 *  Writes data to Tail of the buffer if the buffer is not full
 *  Increments tail (wraps if ncessary)
 *  Param "FIFOChoice": chooses which buffer we want to read from
 *        "Data': Data being put into FIFO
 *  Returns: error code for full buffer if unable to write
 */
int writeFIFO(uint32_t FIFOChoice, uint32_t Data)
{
    G8RTOS_WaitSemaphore(&(FIFOs[FIFOChoice].Mutex)); // wait for fifo mutex to become available so we can write
    // provide 1 buffer cell in case interrupt happens b/w reading fifo and incrementing head
    if(FIFOs[FIFOChoice].CurrentSize == FIFOSIZE-1){
        FIFOs[FIFOChoice].LostData++;
        // TODO
//        G8RTOS_SignalSemaphore(&(FIFOs[FIFOChoice].Mutex)); // release the fifo mutex because we are done writing
        // TODO
        return -1; //return error code bc lost data
    }
    *(FIFOs[FIFOChoice].Tail++) = Data; // Write data to tail and post increment

    // if we reach end of buffer update tail to beginning of buffer
    if(FIFOs[FIFOChoice].Tail == &(FIFOs[FIFOChoice].Buffer[(FIFOSIZE-1)])){
        FIFOs[FIFOChoice].Tail = &(FIFOs[FIFOChoice].Buffer[0]);
    }

    G8RTOS_SignalSemaphore(&(FIFOs[FIFOChoice].CurrentSize)); // increments Current size semaphore meaning we have new data
    G8RTOS_SignalSemaphore(&(FIFOs[FIFOChoice].Mutex)); // release the fifo mutex because we are done writing

    return 0;
}


//int writeBallFIFO(uint32_t FIFOChoice, Ball Data)
//{
//    G8RTOS_WaitSemaphore(&(BallFIFOs[FIFOChoice].Mutex)); // wait for fifo mutex to become available so we can write
//    // provide 1 buffer cell in case interrupt happens b/w reading fifo and incrementing head
//    if(BallFIFOs[FIFOChoice].CurrentSize == FIFOSIZE-1){
//        BallFIFOs[FIFOChoice].LostData++;
//        // TODO
////        G8RTOS_SignalSemaphore(&(FIFOs[FIFOChoice].Mutex)); // release the fifo mutex because we are done writing
//        // TODO
//        return -1; //return error code bc lost data
//    }
//    *(BallFIFOs[FIFOChoice].Tail++) = Data; // Write data to tail and post increment
//
//    // if we reach end of buffer update tail to beginning of buffer
//    if(BallFIFOs[FIFOChoice].Tail == &(BallFIFOs[FIFOChoice].Buffer[(FIFOSIZE-1)])){
//        BallFIFOs[FIFOChoice].Tail = &(BallFIFOs[FIFOChoice].Buffer[0]);
//    }
//
//    G8RTOS_SignalSemaphore(&(BallFIFOs[FIFOChoice].CurrentSize)); // increments Current size semaphore meaning we have new data
//    G8RTOS_SignalSemaphore(&(BallFIFOs[FIFOChoice].Mutex)); // release the fifo mutex because we are done writing
//
//    return 0;
//}

