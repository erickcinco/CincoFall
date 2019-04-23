/*
 * G8RTOS_IPC.h
 *
 *  Created on: Jan 10, 2017
 *      Author: Daniel Gonzalez
 */
#include "LCDLib.h"

#ifndef G8RTOS_G8RTOS_IPC_H_
#define G8RTOS_G8RTOS_IPC_H_

/*********************************************** Defines ******************************************************************************/

#define FIFOSIZE 16
#define MAX_NUMBER_OF_FIFOS 4

/*********************************************** Error Codes **************************************************************************/

/*********************************************** Error Codes **************************************************************************/

/*********************************************** Public Functions *********************************************************************/

/*
 * Initializes One to One FIFO Struct
 */
int G8RTOS_InitFIFO(uint32_t FIFOIndex);
int G8RTOS_InitBallFIFO(uint32_t FIFOIndex);

/*
 * Reads FIFO
 *  - Waits until CurrentSize semaphore is greater than zero
 *  - Gets data and increments the head ptr (wraps if necessary)
 * Param "FIFOChoice": chooses which buffer we want to read from
 * Returns: uint32_t Data from FIFO
 */
uint32_t readFIFO(uint32_t FIFO);
//Ball readBallFIFO(uint32_t FIFOChoice);

/*
 * Writes to FIFO
 *  Writes data to Tail of the buffer if the buffer is not full
 *  Increments tail (wraps if ncessary)
 *  Param "FIFOChoice": chooses which buffer we want to read from
 *        "Data': Data being put into FIFO
 *  Returns: error code for full buffer if unable to write
 */
int writeFIFO(uint32_t FIFO, uint32_t data);
//int writeBallFIFO(uint32_t FIFOChoice, Ball Data);

/*********************************************** Public Functions *********************************************************************/


#endif /* G8RTOS_G8RTOS_IPC_H_ */
