#include "RGBLeds.h"



#define BASE_COLOR_DRIVER_ADR 0x60 // blue led is at 0x60, green led is at 0x61, red led is at 0x62
#define BASE_LED_SELECTOR_ADR 0x06 // LED0-3 Selector

// Initialization
void init_RGBLEDS()
{
    uint16_t UNIT_OFF = 0x0000;

    // Software reset enable
    UCB2CTLW0 = UCSWRST;

    // Initialize I2C master
    // Set as master, I2C mode, Clock sync, SMCLK source, Transmitter
    UCB2CTLW0 |= (UCMST | UCMODE_3 | UCSYNC | UCSSEL__SMCLK | UCTR);

    // Set the Fclk as 400khz.
    // Presumes that the SMCLK is selected as source and Fsmclk is 12 MHz..
    UCB2BRW = 30;

    // In conjunction with the next line, this sets the pins as I2C mode.
    // (Table found on p160 of SLAS826E)
    // Set P3.6 as UCB2_SDA and 3.7 as UCB2_SLC by making SEL1=0 and SEL1=1, respectively
    P3SEL0 |= GPIO_PIN7 | GPIO_PIN6;
    P3SEL1 &= ~(GPIO_PIN7 | GPIO_PIN6);

    // Bitwise anding of all bits except UCSWRST to finish i2c config.
    UCB2CTLW0 &= ~UCSWRST;

    // Clear previous led values
    LP3943_LedModeSet(RED, UNIT_OFF);
    LP3943_LedModeSet(GREEN, UNIT_OFF);
    LP3943_LedModeSet(BLUE, UNIT_OFF);
}

/*
 * LP3943_LedModeSet
 * This function will set each of the LEDs to the desired operating
 * mode.
 * Unit indicates color, led data indicates which led will be modified
 * The operating modes are on, off, PWM1 and PWM2.
 * Registers you will be using in this part:
 * eUSCI_Bx I2C Slave Address Register.
 * eUSCI_Bx Control Word Register 0.
 * eUSCI_Bx Transmit Buffer Register.
 * eUSCI_Bx Interrupt Flag Register.

 * The units that can be written to are:
 * UNIT | 0 | Red
 * UNIT | 1 | Blue
 * UNIT | 2 | Green
 *
 * The registers to be written to are:
 * --------
 * | LS0  | LED0-3 Selector        |
 * | LS1  | LED4-7 Selector        |
 * | LS2  | LED8-11 Selector       |
 * | LS3  | LED12-16 Selector      |
 * --------
 */
void LP3943_LedModeSet(uint32_t unit, uint16_t LED_DATA)
{
    uint32_t selected_color_driver;
    uint8_t led_setting_nibble;
    uint16_t led_settings[4] = {0,0,0,0}; // settings for LS0, LS1, LS2, LS3

    // Generate data you want send via I2C.
    uint8_t i;
    for(i=0; i < 4; ++i){
        led_setting_nibble = LED_DATA >> (4*i); //grab next nibble
        if(led_setting_nibble & BIT0){
            led_settings[i] |= BIT0; // set bit 0 to turn on first led in register
        }
        if(led_setting_nibble & BIT1){
            led_settings[i] |= BIT2; // set bit 2 to turn on second led in register
        }
        if(led_setting_nibble & BIT2){
            led_settings[i] |= BIT4; // set bit 4 to turn on third led in register
        }
        if(led_setting_nibble & BIT3){
            led_settings[i] |= BIT6; // set bit 6 to turn on fourth led in register
        }
    }

    selected_color_driver = unit | BASE_COLOR_DRIVER_ADR; // get desired color driver address

    // Set initial slave address since we are master.
    UCB2I2CSA = selected_color_driver; //I2C slave address for selected driver
    // Generate START condition.
    UCB2CTLW0 |= UCTXSTT;
    while(UCB2CTLW0 & UCTXSTT); // UCTXSTT is automatically cleared after START condition and address information is transmitted.

    // Wait for buffer availability just in case.
    while(!(UCB2IFG & UCTXIFG0));
    UCB2TXBUF = BASE_LED_SELECTOR_ADR | BIT4; // Send first led selector register address with autoincrement bit set

    // Fill TXBUF with the data for the LP3943.
    for(i=0; i < 4; ++i){
        // Wait for buffer availability.
        while(!(UCB2IFG & UCTXIFG0)); // wait for previously transmitted value to finish sending
          UCB2TXBUF = led_settings[i];
    }

    while (!(UCB2IFG & UCTXIFG0)); // Make sure we transmitted last value before writing stop condition

    // Generate STOP condition.
    UCB2CTLW0 |= UCTXSTP;
}

void set_purp_blu_gre_red_array(uint16_t val){
    LP3943_LedModeSet(RED,   (val  & 0xF00F) );
    LP3943_LedModeSet(GREEN, (val  & 0x0F00) );
    LP3943_LedModeSet(BLUE,  (val  & 0xF0F0) );
}
