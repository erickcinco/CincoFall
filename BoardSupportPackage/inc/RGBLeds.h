#ifndef RGBLEDS_H_
#define RGBLEDS_H_

#include <stdio.h>
#include "driverlib.h"
#include "msp.h"



/* Enums for RGB LED driver offset */
typedef enum device
{
    BLUE = 0,
    GREEN = 1,
    RED = 2,
} unit_desig;


/*
 * LP3943_ColorSet
 * This function will set the frequencies and PWM duty cycle
 * for each register of the specified unit
 */
static void LP3943_ColorSet(uint32_t unit, uint32_t PWM_DATA);


/*
 * LP3943_LedModeSet
 * This function will set each of the LEDs to the desired operating
 * mode. The operating modes are on, off, PWM1, and PWM2.
 */
void LP3943_LedModeSet(uint32_t unit, uint16_t LED_DATA);


/*
 * Performs necessary initializations for RGB LEDs
 */
void init_RGBLEDS();

/*
 * Writes to array with colors defined in the following order:
 * MSNibble Purple >  Green > Blue > LSNibble Red
 */
void set_purp_blu_gre_red_array(uint16_t val);

#endif // RGBLEDS_H_
