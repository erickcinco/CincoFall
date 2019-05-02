/*
 * LCDLib.c
 *
 *  Created on: Mar 2, 2017
 *      Author: Danny
 */

#include "LCDLib.h"
#include "msp.h"
#include "driverlib.h"
#include "AsciiLib.h"

Calib calibration;
uint16_t write_pink_flag = 0;



/************************************  Private Functions  *******************************************/

/*
 * Delay x ms
 */
static void Delay(unsigned long interval)
{
    while(interval > 0)
    {
        __delay_cycles(48000);
        interval--;
    }
}

void PORT4_IRQHandler(void){
    // TODO
    P4IFG &= ~BIT0; // must clear IFG flag

    uint16_t val;

    Delay(4);// delay for 4 ms to allow settle
    val = P4IN & BIT0;

    if(!write_pink_flag && !val){
        write_pink_flag = 1;
    }
}

/*******************************************************************************
 * Function Name  : LCD_initSPI
 * Description    : Configures LCD Control lines
 * Input          : None
 * Output         : None
 * Return         : None
 * Attention      : None
 *******************************************************************************/
static void LCD_initSPI()
{
    // Step 1: Set UCSWRST software reset enable
    UCB3CTLW0 = EUSCI_B_CTLW0_SWRST;
    // Step 2: Init all eUSCI registers with UCSWRST = 1 (including UCxCTL1)
    // SPI - 3 Pin, 8 bit spi master, high polarity for inactive state, 12 MHZ
    UCB3CTLW0 &= ~(EUSCI_B_CTLW0_MODE1 | EUSCI_B_CTLW0_MODE0 | EUSCI_B_CTLW0_SEVENBIT); // 00 for 3-pin spi, 0 for 8-bit data
    UCB3CTLW0 |= (EUSCI_B_CTLW0_MSB | EUSCI_B_CTLW0_MST | EUSCI_B_CTLW0_CKPL | EUSCI_B_CTLW0_SSEL__SMCLK); // 1 for master mode, 1 for inactive state high polarity, SMCLK

    // TODO - Remove once working to operate at max speed
    // Set the Fclk as 4Mhz. ---- for testing
    // Presumes that the SMCLK is selected as source and Fsmclk is 12 MHz..
//    UCB3BRW = 3;

    // Step 3: Configure ports: P10.1 - CLK - UCB3CLK, P10.2 - MOSI, P10.3 - MISO, P10.4 - LCD CS, P10.5 - TP CS

    /* P10.1 - CLK - UCB3CLK
     * DIR = X
     * SEL1 = 0, SEL0 = 1
     */
    P10SEL1 &= ~BIT1;
    P10SEL0 |= BIT1;

    /* P10.2 - MOSI
     * DIR = X
     * SEL1 = 0, SEL0 = 1
     */
    P10SEL1 &= ~BIT2;
    P10SEL0 |= BIT2;

    /* P10.3 - MISO
     * DIR = X
     * SEL1 = 0, SEL0 = 1
     */
    P10SEL1 &= ~BIT3;
    P10SEL0 |= BIT3;

    /* P10.4 - LCD CS
     * DIR = 1 GPIO OUTPUT
     * SEL1 = 0, SEL0 = 0
     */
    P10DIR |= BIT4;
    P10SEL1 &= ~BIT4;
    P10SEL0 &= ~BIT4;

    /* P10.5 - TP CS
     * DIR = 1 GPIO OUTPUT
     * SEL1 = 0, SEL0 = 0
     */
    P10DIR |= BIT5;
    P10SEL1 &= ~BIT5;
    P10SEL0 &= ~BIT5;

    // Step 4: Clear UCSWRST
    UCB3CTLW0 &= ~UCSWRST;

    // Step 5: Enable interrupts (optional) with UCRXIE or UCTXIE
    // N/A ?



}

/*******************************************************************************
 * Function Name  : LCD_reset
 * Description    : Resets LCD
 * Input          : None
 * Output         : None
 * Return         : None
 * Attention      : Uses P10.0 for reset
 *******************************************************************************/
static void LCD_reset()
{
    P10DIR |= BIT0;
    P10OUT |= BIT0;  // high
    Delay(100);
    P10OUT &= ~BIT0; // low
    Delay(100);
    P10OUT |= BIT0;  // high
}

/************************************  Private Functions  *******************************************/


/************************************  Public Functions  *******************************************/

/*******************************************************************************
 * Function Name  : LCD_DrawRectangle
 * Description    : Draw a rectangle as the specified color
 * Input          : xStart, xEnd, yStart, yEnd, Color
 * Output         : None
 * Return         : None
 * Attention      : Must draw from left to right, top to bottom!
 *******************************************************************************/
void LCD_DrawRectangle(int16_t xStart, int16_t xEnd, int16_t yStart, int16_t yEnd, uint16_t Color)
{
    // Optimization complexity: O(64 + 2N) Bytes Written 

    /* Check special cases for out of bounds
     * If out of bounds set to the respective max/min
     */
    if(xStart < 0){
        xStart = 0;
    }
    if(xEnd > MAX_SCREEN_X){
        xEnd = MAX_SCREEN_X;
    }
    if(yStart < 0){
        yStart = 0;
    }
    if(yEnd > MAX_SCREEN_Y){
        yEnd = MAX_SCREEN_Y;
    }
    /* Set window area for high-speed RAM write */
    LCD_WriteReg(HOR_ADDR_START_POS, yStart);     /* Horizontal GRAM Start Address */
    LCD_WriteReg(HOR_ADDR_END_POS, (yEnd - 1));  /* Horizontal GRAM End Address */
    LCD_WriteReg(VERT_ADDR_START_POS, xStart);    /* Vertical GRAM Start Address */
    LCD_WriteReg(VERT_ADDR_END_POS, (xEnd - 1)); /* Vertical GRAM End Address */

    /* Set cursor */ 
    LCD_SetCursor(xStart, yStart);
    /* Set index to GRAM */ 
    LCD_WriteIndex(GRAM);
    /* Send out data only to the entire area */
    SPI_CS_LOW; // set chip select upon start
    SPISendRecvByte(SPI_START | SPI_WR | SPI_DATA); // start condition   /* Write : RS = 1, RW = 0       */
    for(uint16_t i=xStart; i<xEnd; i++){
        for(uint16_t i=yStart; i<yEnd; i++){
            LCD_Write_Data_Only(Color);
        }
    }
    SPI_CS_HIGH;
}

void LCD_Draw_Sprite(int16_t xStart, int16_t xEnd, int16_t yStart, int16_t yEnd, uint16_t *color_array)
{
    // Optimization complexity: O(64 + 2N) Bytes Written

    /* Check special cases for out of bounds
     * If out of bounds set to the respective max/min
     */
    if(xStart < 0){
        xStart = 0;
    }
    if(xEnd > MAX_SCREEN_X){
        xEnd = MAX_SCREEN_X;
    }
    if(yStart < 0){
        yStart = 0;
    }
    if(yEnd > MAX_SCREEN_Y){
        yEnd = MAX_SCREEN_Y;
    }
    /* Set window area for high-speed RAM write */
    LCD_WriteReg(HOR_ADDR_START_POS, yStart);     /* Horizontal GRAM Start Address */
    LCD_WriteReg(HOR_ADDR_END_POS, (yEnd - 1));  /* Horizontal GRAM End Address */
    LCD_WriteReg(VERT_ADDR_START_POS, xStart);    /* Vertical GRAM Start Address */
    LCD_WriteReg(VERT_ADDR_END_POS, (xEnd - 1)); /* Vertical GRAM End Address */

    /* Set cursor */
    LCD_SetCursor(xStart, yStart);
    /* Set index to GRAM */
    LCD_WriteIndex(GRAM);
    /* Send out data only to the entire area */
    SPI_CS_LOW; // set chip select upon start
    SPISendRecvByte(SPI_START | SPI_WR | SPI_DATA); // start condition   /* Write : RS = 1, RW = 0       */
    for(uint16_t i=xStart; i<xEnd; i++){
        for(uint16_t j=yStart; j<yEnd; j++){
            LCD_Write_Data_Only(*color_array);
                color_array++;
        }
    }
    SPI_CS_HIGH;
}


/******************************************************************************
 * Function Name  : PutChar
 * Description    : Lcd screen displays a character
 * Input          : - Xpos: Horizontal coordinate
 *                  - Ypos: Vertical coordinate
 *                  - ASCI: Displayed character
 *                  - charColor: Character color
 * Output         : None
 * Return         : None
 * Attention      : None
 *******************************************************************************/
inline void PutChar( uint16_t Xpos, uint16_t Ypos, uint8_t ASCI, uint16_t charColor)
{
    uint16_t i, j;
    uint8_t buffer[16], tmp_char;
    GetASCIICode(buffer,ASCI);  /* get font data */
    for( i=0; i<16; i++ )
    {
        tmp_char = buffer[i];
        for( j=0; j<8; j++ )
        {
            if( (tmp_char >> 7 - j) & 0x01 == 0x01 )
            {
                LCD_SetPoint( Xpos + j, Ypos + i, charColor );  /* Character color */
            }
        }
    }
}

/******************************************************************************
 * Function Name  : GUI_Text
 * Description    : Displays the string
 * Input          : - Xpos: Horizontal coordinate
 *                  - Ypos: Vertical coordinate
 *                  - str: Displayed string
 *                  - charColor: Character color
 * Output         : None
 * Return         : None
 * Attention      : None
 *******************************************************************************/
void LCD_Text(uint16_t Xpos, uint16_t Ypos, uint8_t *str, uint16_t Color)
{
    uint8_t TempChar;

    /* Set area back to span the entire LCD */
    LCD_WriteReg(HOR_ADDR_START_POS, 0x0000);     /* Horizontal GRAM Start Address */
    LCD_WriteReg(HOR_ADDR_END_POS, (MAX_SCREEN_Y - 1));  /* Horizontal GRAM End Address */
    LCD_WriteReg(VERT_ADDR_START_POS, 0x0000);    /* Vertical GRAM Start Address */
    LCD_WriteReg(VERT_ADDR_END_POS, (MAX_SCREEN_X - 1)); /* Vertical GRAM Start Address */
    do
    {
        TempChar = *str++;
        PutChar( Xpos, Ypos, TempChar, Color);
        if( Xpos < MAX_SCREEN_X - 8)
        {
            Xpos += 8;
        }
        else if ( Ypos < MAX_SCREEN_X - 16)
        {
            Xpos = 0;
            Ypos += 16;
        }
        else
        {
            Xpos = 0;
            Ypos = 0;
        }
    }
    while ( *str != 0 );
}


/*******************************************************************************
 * Function Name  : LCD_Clear
 * Description    : Fill the screen as the specified color
 * Input          : - Color: Screen Color
 * Output         : None
 * Return         : None
 * Attention      : None
 *******************************************************************************/
void LCD_Clear(uint16_t Color)
{
    /* Set area back to span the entire LCD */
    LCD_WriteReg(HOR_ADDR_START_POS, 0x0000);     /* Horizontal GRAM Start Address */
    LCD_WriteReg(HOR_ADDR_END_POS, (MAX_SCREEN_Y - 1));  /* Horizontal GRAM End Address */
    LCD_WriteReg(VERT_ADDR_START_POS, 0x0000);    /* Vertical GRAM Start Address */
    LCD_WriteReg(VERT_ADDR_END_POS, (MAX_SCREEN_X - 1)); /* Vertical GRAM Start Address */
    
    /* Set cursor to (0,0) */ 
    LCD_SetCursor(0, 0);
    /* Set write index to GRAM */     
    LCD_WriteIndex(GRAM);
    /* Start data transmittion */ 
    LCD_Write_Data_Start();

    LCD_DrawRectangle(0, MAX_SCREEN_X, 0, MAX_SCREEN_Y, Color);
}

/******************************************************************************
 * Function Name  : LCD_SetPoint
 * Description    : Drawn at a specified point coordinates
 * Input          : - Xpos: Row Coordinate
 *                  - Ypos: Line Coordinate
 * Output         : None
 * Return         : None
 * Attention      : 18N Bytes Written
 *******************************************************************************/
void LCD_SetPoint(int16_t Xpos, int16_t Ypos, uint16_t color)
{
    uint8_t out_of_bounds = 0; // debug purposes
    /* Should check for out of bounds */ 
    if(Xpos < 0 || Xpos > MAX_SCREEN_X || Ypos < 0 || Ypos > MAX_SCREEN_Y){
        out_of_bounds = 1;
        return;
    }
    /* Set cursor to Xpos and Ypos */ 
    LCD_SetCursor(Xpos, Ypos);
    /* Write color to GRAM reg */
    LCD_WriteReg(GRAM, color);
}

/*******************************************************************************
 * Function Name  : LCD_Write_Data_Only
 * Description    : Data writing to the LCD controller
 * Input          : - data: data to be written
 * Output         : None
 * Return         : None
 * Attention      : None
 *******************************************************************************/
inline void LCD_Write_Data_Only(uint16_t data)
{
    uint8_t data_msb;
    uint8_t data_lsb;

    /* Send out MSB */ 
    data_msb = (data >> 8);
    SPISendRecvByte(data_msb);

    /* Send out LSB */ 
    data_lsb = (data & 0xFF);
    SPISendRecvByte(data_lsb);
}

/*******************************************************************************
 * Function Name  : LCD_WriteData
 * Description    : LCD write register data
 * Input          : - data: register data
 * Output         : None
 * Return         : None
 * Attention      : None
 *******************************************************************************/
inline void LCD_WriteData(uint16_t data)
{
    SPI_CS_LOW;

    SPISendRecvByte(SPI_START | SPI_WR | SPI_DATA);    /* Write : RS = 1, RW = 0       */
    SPISendRecvByte((data >>   8));                    /* Write D8..D15                */
    SPISendRecvByte((data & 0xFF));                    /* Write D0..D7                 */

    SPI_CS_HIGH;
}


/*******************************************************************************
 * Function Name  : LCD_WriteIndex
 * Description    : LCD write register address
 * Input          : - index: register address
 * Output         : None
 * Return         : None
 * Attention      : None
 *******************************************************************************/
inline void LCD_WriteIndex(uint16_t index)
{
    SPI_CS_LOW;

    /* SPI write data */
    SPISendRecvByte(SPI_START | SPI_WR | SPI_INDEX);   /* Write : RS = 0, RW = 0  */
    SPISendRecvByte(0);
    SPISendRecvByte(index);

    SPI_CS_HIGH;
}

/*******************************************************************************
 * Function Name  : LCD_WriteReg
 * Description    : Writes to the selected LCD register.
 * Input          : - LCD_Reg: address of the selected register.
 *                  - LCD_RegValue: value to write to the selected register.
 * Output         : None
 * Return         : None
 * Attention      : None
 *******************************************************************************/
inline void LCD_WriteReg(uint16_t LCD_Reg, uint16_t LCD_RegValue)
{
    /* Write 16-bit Index */
    LCD_WriteIndex(LCD_Reg);
    /* Write 16-bit Reg Data */
    LCD_WriteData(LCD_RegValue);
}

/*******************************************************************************
 * Function Name  : SPISendRecvByte
 * Description    : Send one byte then receive one byte of response
 * Input          : uint8_t: byte
 * Output         : None
 * Return         : Recieved value 
 * Attention      : None
 *******************************************************************************/
inline uint8_t SPISendRecvByte (uint8_t byte)
{

    /* Send byte of data - do it by writing to the TXBUF*/
    UCB3TXBUF = byte;
    /* Wait as long as busy*/
    // TODO - if this does not work try UCB3STATW & EUSCI_B_STATW_SPI_BUSY
    while(!(UCB3IFG & EUSCI_B_IFG_RXIFG0)); // wait for eUSCI to finish transmitting/receiving just in case
    /* Receive byte of data */
    /* Return received value*/
    return (uint8_t) UCB3RXBUF; // read rx buffer
}

/*******************************************************************************
 * Function Name  : LCD_Write_Data_Start
 * Description    : Start of data writing to the LCD controller
 * Input          : None
 * Output         : None
 * Return         : None
 * Attention      : None
 *******************************************************************************/
inline void LCD_Write_Data_Start(void)
{
    SPISendRecvByte(SPI_START | SPI_WR | SPI_DATA);    /* Write : RS = 1, RW = 0 */
}

/*******************************************************************************
 * Function Name  : LCD_ReadData
 * Description    : LCD read data
 * Input          : None
 * Output         : None
 * Return         : return data
 * Attention      : Diagram (d) in datasheet
 *******************************************************************************/
inline uint16_t LCD_ReadData()
{
    uint16_t value;
    SPI_CS_LOW;
    SPISendRecvByte(SPI_START | SPI_RD | SPI_DATA);   /* Read: RS = 1, RW = 1   */
    SPISendRecvByte(0);                               /* Dummy read 1           */
    value = (SPISendRecvByte(0) << 8);                /* Read D8..D15           */
    value |= SPISendRecvByte(0);                      /* Read D0..D7            */

    SPI_CS_HIGH;
    return value;
}

/*******************************************************************************
 * Function Name  : LCD_ReadReg
 * Description    : Reads the selected LCD Register.
 * Input          : None
 * Output         : None
 * Return         : LCD Register Value.
 * Attention      : None
 *******************************************************************************/
inline uint16_t LCD_ReadReg(uint16_t LCD_Reg)
{
    /* Write 16-bit Index */
    LCD_WriteIndex(LCD_Reg);
    /* Return 16-bit Reg using LCD_ReadData() */
    return LCD_ReadData();
}




/*******************************************************************************
 * Function Name  : LCD_SetCursor
 * Description    : Sets the cursor position.
 * Input          : - Xpos: specifies the X position.
 *                  - Ypos: specifies the Y position.
 * Output         : None
 * Return         : None
 * Attention      : None
 *******************************************************************************/
inline void LCD_SetCursor(uint16_t Xpos, uint16_t Ypos )
{
    /* Should just be two LCD_WriteReg to appropriate registers */ 

    /* Set horizonal GRAM coordinate (Ypos) */ 
    LCD_WriteReg(GRAM_HORIZONTAL_ADDRESS_SET, Ypos);
    /* Set vertical GRAM coordinate (Xpos) */
    LCD_WriteReg(GRAM_VERTICAL_ADDRESS_SET, Xpos);
}

/*******************************************************************************
 * Function Name  : LCD_Init
 * Description    : Configures LCD Control lines, sets whole screen black
 * Input          : bool usingTP: determines whether or not to enable TP interrupt 
 * Output         : None
 * Return         : None
 * Attention      : None
 *******************************************************************************/
void LCD_Init(bool usingTP)
{
    LCD_initSPI(); //

    if (usingTP)
    {
        /* Configure low true interrupt on P4.0 for TP */ 
        P4DIR &= ~BIT0; // Set P4 as input
        P4IFG &= ~BIT0; // P4.0 IFG cleared
        P4IE  |= BIT0;  // Enable interrupt on P4.0
        P4IES |= BIT0;  // high-to-low transition
        P4REN |= BIT0;  // Pull-up resistor
        P4OUT |= BIT0;  // Sets res to pull-up

        NVIC_EnableIRQ(PORT4_IRQn); // enable the interrupt

    }

    LCD_reset();

    LCD_WriteReg(0xE5, 0x78F0); /* set SRAM internal timing */
    LCD_WriteReg(DRIVER_OUTPUT_CONTROL, 0x0100); /* set Driver Output Con_trol */
    LCD_WriteReg(DRIVING_WAVE_CONTROL, 0x0700); /* set 1 line inversion */
    LCD_WriteReg(ENTRY_MODE, 0x1038); /* set GRAM write direction and BGR=1 */
    LCD_WriteReg(RESIZING_CONTROL, 0x0000); /* Resize register */
    LCD_WriteReg(DISPLAY_CONTROL_2, 0x0207); /* set the back porch and front porch */
    LCD_WriteReg(DISPLAY_CONTROL_3, 0x0000); /* set non-display area refresh cycle ISC[3:0] */
    LCD_WriteReg(DISPLAY_CONTROL_4, 0x0000); /* FMARK function */
    LCD_WriteReg(RGB_DISPLAY_INTERFACE_CONTROL_1, 0x0000); /* RGB interface setting */
    LCD_WriteReg(FRAME_MARKER_POSITION, 0x0000); /* Frame marker Position */
    LCD_WriteReg(RGB_DISPLAY_INTERFACE_CONTROL_2, 0x0000); /* RGB interface polarity */

    /* Power On sequence */
    LCD_WriteReg(POWER_CONTROL_1, 0x0000); /* SAP, BT[3:0], AP, DSTB, SLP, STB */
    LCD_WriteReg(POWER_CONTROL_2, 0x0007); /* DC1[2:0], DC0[2:0], VC[2:0] */
    LCD_WriteReg(POWER_CONTROL_3, 0x0000); /* VREG1OUT voltage */
    LCD_WriteReg(POWER_CONTROL_4, 0x0000); /* VDV[4:0] for VCOM amplitude */
    LCD_WriteReg(DISPLAY_CONTROL_1, 0x0001);
    Delay(200);

    /* Dis-charge capacitor power voltage */
    LCD_WriteReg(POWER_CONTROL_1, 0x1090); /* SAP, BT[3:0], AP, DSTB, SLP, STB */
    LCD_WriteReg(POWER_CONTROL_2, 0x0227); /* Set DC1[2:0], DC0[2:0], VC[2:0] */
    Delay(50); /* Delay 50ms */
    LCD_WriteReg(POWER_CONTROL_3, 0x001F);
    Delay(50); /* Delay 50ms */
    LCD_WriteReg(POWER_CONTROL_4, 0x1500); /* VDV[4:0] for VCOM amplitude */
    LCD_WriteReg(POWER_CONTROL_7, 0x0027); /* 04 VCM[5:0] for VCOMH */
    LCD_WriteReg(FRAME_RATE_AND_COLOR_CONTROL, 0x000D); /* Set Frame Rate */
    Delay(50); /* Delay 50ms */
    LCD_WriteReg(GRAM_HORIZONTAL_ADDRESS_SET, 0x0000); /* GRAM horizontal Address */
    LCD_WriteReg(GRAM_VERTICAL_ADDRESS_SET, 0x0000); /* GRAM Vertical Address */

    /* Adjust the Gamma Curve */
    LCD_WriteReg(GAMMA_CONTROL_1,    0x0000);
    LCD_WriteReg(GAMMA_CONTROL_2,    0x0707);
    LCD_WriteReg(GAMMA_CONTROL_3,    0x0307);
    LCD_WriteReg(GAMMA_CONTROL_4,    0x0200);
    LCD_WriteReg(GAMMA_CONTROL_5,    0x0008);
    LCD_WriteReg(GAMMA_CONTROL_6,    0x0004);
    LCD_WriteReg(GAMMA_CONTROL_7,    0x0000);
    LCD_WriteReg(GAMMA_CONTROL_8,    0x0707);
    LCD_WriteReg(GAMMA_CONTROL_9,    0x0002);
    LCD_WriteReg(GAMMA_CONTROL_10,   0x1D04);

    /* Set GRAM area */
    LCD_WriteReg(HOR_ADDR_START_POS, 0x0000);     /* Horizontal GRAM Start Address */
    LCD_WriteReg(HOR_ADDR_END_POS, (MAX_SCREEN_Y - 1));  /* Horizontal GRAM End Address */
    LCD_WriteReg(VERT_ADDR_START_POS, 0x0000);    /* Vertical GRAM Start Address */
    LCD_WriteReg(VERT_ADDR_END_POS, (MAX_SCREEN_X - 1)); /* Vertical GRAM Start Address */
    LCD_WriteReg(GATE_SCAN_CONTROL_0X60, 0x2700); /* Gate Scan Line */
    LCD_WriteReg(GATE_SCAN_CONTROL_0X61, 0x0001); /* NDL,VLE, REV */
    LCD_WriteReg(GATE_SCAN_CONTROL_0X6A, 0x0000); /* set scrolling line */

    /* Partial Display Control */
    LCD_WriteReg(PART_IMAGE_1_DISPLAY_POS, 0x0000);
    LCD_WriteReg(PART_IMG_1_START_END_ADDR_0x81, 0x0000);
    LCD_WriteReg(PART_IMG_1_START_END_ADDR_0x82, 0x0000);
    LCD_WriteReg(PART_IMAGE_2_DISPLAY_POS, 0x0000);
    LCD_WriteReg(PART_IMG_2_START_END_ADDR_0x84, 0x0000);
    LCD_WriteReg(PART_IMG_2_START_END_ADDR_0x85, 0x0000);

    /* Panel Control */
    LCD_WriteReg(PANEL_ITERFACE_CONTROL_1, 0x0010);
    LCD_WriteReg(PANEL_ITERFACE_CONTROL_2, 0x0600);
    LCD_WriteReg(DISPLAY_CONTROL_1, 0x0133); /* 262K color and display ON */
    Delay(50); /* delay 50 ms */

    LCD_Clear(LCD_BLACK);
}


uint16_t LCD_ReadPixelColor(uint16_t x, uint16_t y)
{
    LCD_WriteIndex(GRAM);
    LCD_SetCursor(x, y);
    return LCD_ReadData();
}

void calibrate_tp()
{
    Point calib_point;
    Point user_point;
    uint8_t test_string[] = "Calibrate by touching square...";
    // calib point in center of touchscreen
    calib_point.x = 160;
    calib_point.y = 120;
    // generate a known location for user to tap
    LCD_Clear(LCD_BLACK);
    LCD_Text(0, 60, test_string, LCD_WHITE);
    LCD_DrawRectangle( ((calib_point.x)-5), ((calib_point.x)+5), ((calib_point.y)-5), ((calib_point.y)+5), LCD_YELLOW);
    Delay(100);
    while(!write_pink_flag); // wait for user to touch screen
    user_point.x = TP_ReadX();
    user_point.y = TP_ReadY();
    LCD_Clear(LCD_BLACK);
    write_pink_flag = 0;
    calibration.x_offset = 160-user_point.x;
    calibration.y_offset = 120-user_point.y;
}



/*******************************************************************************
 * Function Name  : TP_ReadXY
 * Description    : Obtain X and Y touch coordinates
 * Input          : None
 * Output         : None
 * Return         : Pointer to "Point" structure
 * Attention      : None
 *******************************************************************************/
Point TP_ReadXY()
{
    Point current_read; // init a point struct
//    SPI_CS_LOW;
    /* Read X coord. */ 
    current_read.x = TP_ReadX()+calibration.x_offset; // TODO subtract offset
    /* Read Y coord. */ 
    current_read.y = TP_ReadY()+calibration.y_offset; // TODO subtract offset
    /* Return point  */ 
    return current_read;
}

uint16_t TP_ReadX()
{
    uint16_t x_avg = 0;
    uint16_t current_x = 0;
    float x_calc = 0;
    // TODO
    UCB3BRW = 3; // TODO see if this works


    SPI_CS_TP_LOW;
    SPISendRecvByte(CHX); // send start condition


    x_avg = (SPISendRecvByte(0) << 5);                /* Read D4..D11           */
    x_avg |= (SPISendRecvByte(0) >> 3);             /* Read D0..D3            */
    SPI_CS_TP_HIGH;
    UCB3BRW = 1; // TODO see if this works

    x_calc = (float) x_avg;
    x_calc = (x_calc / 4095.0); // normalize
    current_x = (uint16_t) (x_calc * MAX_SCREEN_X); // get position
    return current_x;
}

uint16_t TP_ReadY()
{
    uint16_t y_avg=0;
    uint16_t current_y=0;
    float y_calc=0;
    // TODO

    UCB3BRW = 3; // TODO see if this works

    SPI_CS_TP_LOW;
    current_y = SPISendRecvByte(CHY);

    y_avg = (SPISendRecvByte(0) << 5);                /* Read D4..D11           */
    y_avg |= (SPISendRecvByte(0) >> 3);             /* Read D0..D3            */
    SPI_CS_TP_HIGH;
    UCB3BRW = 1; // TODO see if this works

    y_calc = (float) y_avg;
    y_calc = (y_calc / 4095.0); // normalize
    current_y = (uint16_t) (y_calc * MAX_SCREEN_Y); // get position
    return current_y;
}

/*
BallShape create_ball_shape(Point desired_center_point, int16_t length, int16_t height){
    BallShape ball_shape;
    Point start;
    Point end;

    start.x = desired_center_point.x - length;
    end.x = desired_center_point.x + length;
    start.y = desired_center_point.y - height;
    end.y = desired_center_point.y + height;

    // handle wrap around
    if(start.x > MAX_SCREEN_X || end.x > MAX_SCREEN_X){
        desired_center_point.x = MIN_SCREEN_X + length;
        start.x = MIN_SCREEN_X;
        end.x = desired_center_point.x + length;
    }
    if(start.y > MAX_SCREEN_Y || end.y > MAX_SCREEN_Y){
        desired_center_point.y = MIN_SCREEN_Y + length;
        start.y = MIN_SCREEN_Y;
        end.y = desired_center_point.y + length;
    }

    if(start.x < MIN_SCREEN_X || end.x < MIN_SCREEN_X){
        desired_center_point.x = MAX_SCREEN_X-length;
        start.x = desired_center_point.x-length;
        end.x = MAX_SCREEN_X;
    }
    if(start.y < MIN_SCREEN_Y || end.y < MIN_SCREEN_Y){
        desired_center_point.y = MAX_SCREEN_Y-height;
        start.y = desired_center_point.y-height;
        end.y = MAX_SCREEN_Y;
    }

    ball_shape.center_point = desired_center_point; // sets center pt of ball
    ball_shape.x_start = start.x; // handle x dimensions of ball
    ball_shape.x_end = end.x;
    ball_shape.y_start = start.y; // handle y dimensions of ball
    ball_shape.y_end = end.y;
    ball_shape.length = length; // set length and height params
    ball_shape.height = height;
    return ball_shape;
}

Ball create_ball(Point desired_center_point, int16_t length, int16_t height, uint16_t color, Velocity speed, uint32_t thread_id){
    Ball new_ball;
    new_ball.current_ball = create_ball_shape(desired_center_point, length, height); // set current ball to user def
    new_ball.previous_ball = new_ball.current_ball; // set previous ball to same as current ball
    new_ball.color = color;
    new_ball.velocity = speed;
    new_ball.is_alive = true;
    new_ball.ball_thread_id = thread_id;
    return new_ball;
}

Ball update_ball_state(Ball user_ball){

    BallShape updated_ball_shape;
    Point updated_center_point;

    updated_center_point.x = (user_ball.current_ball.center_point.x) + (user_ball.velocity.x_step);
    updated_center_point.y = (user_ball.current_ball.center_point.y) + (user_ball.velocity.y_step);

    updated_ball_shape = create_ball_shape(updated_center_point, (user_ball.current_ball.length), (user_ball.current_ball.height));

    user_ball.previous_ball = user_ball.current_ball; //set previous_ball equal to current ball
    user_ball.current_ball = updated_ball_shape; // update current ball
    return user_ball;
}


Ball lcd_draw_ball(Ball ball_object){
    ball_object = update_ball_state(ball_object); // update ball state with new shape
    // update ball on lcd - clear last ball position write new ball position
    LCD_DrawRectangle(ball_object.previous_ball.x_start, ball_object.previous_ball.x_end, ball_object.previous_ball.y_start, ball_object.previous_ball.y_end, LCD_BLACK);
    LCD_DrawRectangle(ball_object.current_ball.x_start, ball_object.current_ball.x_end, ball_object.current_ball.y_start, ball_object.current_ball.y_end, ball_object.color);
    return ball_object;
}

Ball lcd_kill_ball(Ball ball_object){
    ball_object.is_alive = false;
    LCD_DrawRectangle(ball_object.current_ball.x_start, ball_object.current_ball.x_end, ball_object.current_ball.y_start, ball_object.current_ball.y_end, LCD_BLACK);
    return ball_object;
}

Point find_wrap_around(Point initial_point, Velocity speed, wrapType wrap){
    Point new_point;
    if(wrap == EXCEEDED_MAX_X){

    }
    else if(wrap == EXCEEDED_MIN_X){
        new_point.x = MAX_SCREEN_Y + MAX_SCREEN_X-initial_point.y + speed.x_step;
        new_point.y = MAX_SCREEN_Y - speed.y_step;
    }
    else if(wrap == EXCEEDED_MAX_Y){

    }
    else if(wrap == EXCEEDED_MIN_Y){
        new_point.x = MAX_SCREEN_Y + initial_point.x - MIN_SCREEN_Y + speed.x_step;
        new_point.y = MIN_SCREEN_Y + speed.y_step;
    }


    return new_point;
}
*/
/************************************  Public Functions  *******************************************/

