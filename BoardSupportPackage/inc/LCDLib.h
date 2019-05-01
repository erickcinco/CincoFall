/*
 * LCDLib.h
 *
 *  Created on: Mar 2, 2017
 *      Author: Danny
 */

#ifndef LCDLIB_H_
#define LCDLIB_H_

#include <stdbool.h>
#include <stdint.h>
/************************************ Defines *******************************************/


/* Screen size */
#define MAX_SCREEN_X     320
#define MAX_SCREEN_Y     240
#define MIN_SCREEN_X     0
#define MIN_SCREEN_Y     0
#define SCREEN_SIZE      76800
#define TEXT_CHAR_SIZE 10


/* Register details */
#define SPI_START   (0x70)     /* Start byte for SPI transfer        */
#define SPI_RD      (0x01)     /* WR bit 1 within start              */
#define SPI_WR      (0x00)     /* WR bit 0 within start              */
#define SPI_DATA    (0x02)     /* RS bit 1 within start byte         */
#define SPI_INDEX   (0x00)     /* RS bit 0 within start byte         */

/* CS LCD*/
#define SPI_CS_LOW P10OUT &= ~BIT4
#define SPI_CS_HIGH P10OUT |= BIT4

/* CS Touchpanel */
#define SPI_CS_TP_LOW P10OUT &= ~BIT5
#define SPI_CS_TP_HIGH P10OUT |= BIT5

/* XPT2046 registers definition for X and Y coordinate retrieval */
#define CHX         0x90
#define CHY         0xD0

//#define SPI_START_TP (0x80) // start byte for spi tp transfer
//#define SPI_ADDR

/* LCD colors */
#define LCD_WHITE          0xFFFF
#define LCD_BLACK          0x0000
#define LCD_BLUE           0x0197
#define LCD_RED            0xF800
#define LCD_MAGENTA        0xF81F
#define LCD_GREEN          0x07E0
#define LCD_CYAN           0x7FFF
#define LCD_YELLOW         0xFFE0
#define LCD_GRAY           0x2104
#define LCD_PURPLE         0xF11F
#define LCD_ORANGE         0xFD20
#define LCD_PINK           0xfdba
#define LCD_OLIVE          0xdfe4

/* ILI 9325 registers definition */
#define READ_ID_CODE                        0x00
#define DRIVER_OUTPUT_CONTROL               0x01
#define DRIVING_WAVE_CONTROL                0x02
#define ENTRY_MODE                          0x03
#define RESIZING_CONTROL                    0x04
#define DISPLAY_CONTROL_1                   0x07
#define DISPLAY_CONTROL_2                   0x08
#define DISPLAY_CONTROL_3                   0x09
#define DISPLAY_CONTROL_4                   0x0A
#define RGB_DISPLAY_INTERFACE_CONTROL_1     0x0C
#define FRAME_MARKER_POSITION               0x0D
#define RGB_DISPLAY_INTERFACE_CONTROL_2     0x0F
#define POWER_CONTROL_1                     0x10
#define POWER_CONTROL_2                     0x11
#define POWER_CONTROL_3                     0x12
#define POWER_CONTROL_4                     0x13
#define GRAM_HORIZONTAL_ADDRESS_SET         0x20
#define GRAM_VERTICAL_ADDRESS_SET           0x21
#define DATA_IN_GRAM                        0x22
#define POWER_CONTROL_7                     0x29
#define FRAME_RATE_AND_COLOR_CONTROL        0x2B

#define GAMMA_CONTROL_1                     0x30
#define GAMMA_CONTROL_2                     0x31
#define GAMMA_CONTROL_3                     0x32
#define GAMMA_CONTROL_4                     0x35
#define GAMMA_CONTROL_5                     0x36
#define GAMMA_CONTROL_6                     0x37
#define GAMMA_CONTROL_7                     0x38
#define GAMMA_CONTROL_8                     0x39
#define GAMMA_CONTROL_9                     0x3C
#define GAMMA_CONTROL_10                    0x3D

#define HOR_ADDR_START_POS                  0x50
#define HOR_ADDR_END_POS                    0x51
#define VERT_ADDR_START_POS                 0x52
#define VERT_ADDR_END_POS                   0x53
#define GATE_SCAN_CONTROL_0X60              0x60
#define GATE_SCAN_CONTROL_0X61              0x61
#define GATE_SCAN_CONTROL_0X6A              0x6A
#define PART_IMAGE_1_DISPLAY_POS            0x80
#define PART_IMG_1_START_END_ADDR_0x81      0x81
#define PART_IMG_1_START_END_ADDR_0x82      0x81
#define PART_IMAGE_2_DISPLAY_POS            0x83
#define PART_IMG_2_START_END_ADDR_0x84      0x84
#define PART_IMG_2_START_END_ADDR_0x85      0x85
#define PANEL_ITERFACE_CONTROL_1            0x90
#define PANEL_ITERFACE_CONTROL_2            0x92
#define PANEL_ITERFACE_CONTROL_4            0x95

#define GRAM                                0x22
#define HORIZONTAL_GRAM_SET                 0x20
#define VERTICAL_GRAM_SET                   0x21

/************************************ Defines *******************************************/



/********************************** Structures ******************************************/
typedef struct Point {
    int16_t x;
    int16_t y;
}Point;

typedef struct Calib {
    uint16_t x_offset;
    uint16_t y_offset;
}Calib;


//typedef struct Velocity{
//    int16_t x_step;
//    int16_t y_step;
//}Velocity;

//typedef struct BallShape{
//    Point center_point; // center of ball shape
//    int16_t x_start; // x start of ball
//    int16_t x_end; // x end of ball
//    int16_t y_start; // y start of ball
//    int16_t y_end; // y end of ball
//    int16_t length;
//    int16_t height;
//} BallShape;
//
//typedef struct Ball{
//    BallShape current_ball; // current state of ball
//    BallShape previous_ball; // previous state of ball
//    uint16_t color; // ball color
//    Velocity velocity;
//    uint32_t ball_thread_id;
//
//    bool is_alive;
//} Ball;
//
//typedef enum wrapType{
//    EXCEEDED_MIN_X = 0,
//    EXCEEDED_MAX_X = 1,
//    EXCEEDED_MAX_Y = 2,
//    EXCEEDED_MIN_Y = 3,
//}wrapType;

/********************************** Structures ******************************************/

/************************************ Public Functions  *******************************************/

/*******************************************************************************
 * Function Name  : LCD_DrawRectangle
 * Description    : Draw a rectangle as the specified color
 * Input          : xStart, xEnd, yStart, yEnd, Color
 * Output         : None
 * Return         : None
 * Attention      : None
 *******************************************************************************/
void LCD_DrawRectangle(int16_t xStart, int16_t xEnd, int16_t yStart, int16_t yEnd, uint16_t Color);

void LCD_Draw_Sprite(int16_t xStart, int16_t xEnd, int16_t yStart, int16_t yEnd, uint16_t *color_array);


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
inline void PutChar( uint16_t Xpos, uint16_t Ypos, uint8_t ASCI, uint16_t charColor);

/******************************************************************************
* Function Name  : LCD_Text
* Description    : Displays the string
* Input          : - Xpos: Horizontal coordinate
*                  - Ypos: Vertical coordinate
*                  - str: Displayed string
*                  - charColor: Character color
* Output         : None
* Return         : None
* Attention      : None
*******************************************************************************/
void LCD_Text(uint16_t Xpos, uint16_t Ypos, uint8_t *str,uint16_t Color);

/*******************************************************************************
* Function Name  : LCD_Write_Data_Only
* Description    : Data writing to the LCD controller
* Input          : - data: data to be written
* Output         : None
* Return         : None
* Attention      : None
*******************************************************************************/
inline void LCD_Write_Data_Only(uint16_t data);

/*******************************************************************************
* Function Name  : LCD_Clear
* Description    : Fill the screen as the specified color
* Input          : - Color: Screen Color
* Output         : None
* Return         : None
* Attention      : None
*******************************************************************************/
void LCD_Clear(uint16_t Color);

/******************************************************************************
* Function Name  : LCD_SetPoint
* Description    : Drawn at a specified point coordinates
* Input          : - Xpos: Row Coordinate
*                  - Ypos: Line Coordinate
* Output         : None
* Return         : None
* Attention      : 18N Bytes Written
*******************************************************************************/
void LCD_SetPoint(int16_t Xpos, int16_t Ypos, uint16_t color);

/*******************************************************************************
* Function Name  : LCD_WriteData
* Description    : LCD write register data
* Input          : - data: register data
* Output         : None
* Return         : None
* Attention      : None
*******************************************************************************/
inline void LCD_WriteData(uint16_t data);

/*******************************************************************************
* Function Name  : LCD_WriteReg
* Description    : Reads the selected LCD Register.
* Input          : None
* Output         : None
* Return         : LCD Register Value.
* Attention      : None
*******************************************************************************/
inline uint16_t LCD_ReadReg(uint16_t LCD_reg);

/*******************************************************************************
* Function Name  : LCD_WriteIndex
* Description    : LCD write register address
* Input          : - index: register address
* Output         : None
* Return         : None
* Attention      : None
*******************************************************************************/
inline void LCD_WriteIndex(uint16_t index);

/*******************************************************************************
 * Function Name  : SPISendRecvTPByte
 * Description    : Send one byte then receive one byte of response from Touchpanel
 * Input          : uint8_t: byte
 * Output         : None
 * Return         : None
 * Attention      : None
 *******************************************************************************/
inline uint8_t SPISendRecvTPByte (uint8_t byte);

/*******************************************************************************
* Function Name  : SPISendRecvByte
* Description    : Send one byte then recv one byte of response
* Input          : uint8_t: byte
* Output         : None
* Return         : Recieved value 
* Attention      : None
*******************************************************************************/
inline uint8_t SPISendRecvByte(uint8_t byte);

/*******************************************************************************
* Function Name  : LCD_Write_Data_Start
* Description    : Start of data writing to the LCD controller
* Input          : None
* Output         : None
* Return         : None
* Attention      : None
*******************************************************************************/
inline void LCD_Write_Data_Start(void);

/*******************************************************************************
* Function Name  : LCD_ReadData
* Description    : LCD read data
* Input          : None
* Output         : None
* Return         : return data
* Attention  : None
*******************************************************************************/
inline uint16_t LCD_ReadData();

/*******************************************************************************
* Function Name  : LCD_WriteReg
* Description    : Writes to the selected LCD register.
* Input          : - LCD_Reg: address of the selected register.
*                  - LCD_RegValue: value to write to the selected register.
* Output         : None
* Return         : None
* Attention      : None
*******************************************************************************/
inline void LCD_WriteReg(uint16_t LCD_Reg, uint16_t LCD_RegValue);

/*******************************************************************************
* Function Name  : LCD_SetCursor
* Description    : Sets the cursor position.
* Input          : - Xpos: specifies the X position.
*                  - Ypos: specifies the Y position.
* Output         : None
* Return         : None
* Attention      : None
*******************************************************************************/
inline void LCD_SetCursor(uint16_t Xpos, uint16_t Ypos );

/*******************************************************************************
* Function Name  : LCD_Init
* Description    : Configures LCD Control lines, sets whole screen black
* Input          : bool usingTP: determines whether or not to enable TP interrupt
* Output         : None
* Return         : None
* Attention      : None
*******************************************************************************/
void LCD_Init(bool usingTP);


uint16_t LCD_ReadPixelColor(uint16_t x, uint16_t y);


/*******************************************************************************
 * Function Name  : TP_ReadXY
 * Description    : Obtain X and Y touch coordinates
 * Input          : None
 * Output         : None
 * Return         : Point structure
 * Attention      : None
 *******************************************************************************/
Point TP_ReadXY();

/*******************************************************************************
 * Function Name  : TP_ReadX
 * Description    : Obtain X touch coordinate
 * Input          : None
 * Output         : None
 * Return         : X Coordinate
 * Attention      : None
 *******************************************************************************/
uint16_t TP_ReadX();

/*******************************************************************************
 * Function Name  : TP_ReadY
 * Description    : Obtain Y touch coordinate
 * Input          : None
 * Output         : None
 * Return         : Y Coordinate
 * Attention      : None
 *******************************************************************************/
uint16_t TP_ReadY();


void calibrate_tp();

//BallShape create_ball_shape(Point desired_center_point, int16_t length, int16_t height);
//Ball create_ball(Point desired_center_point, int16_t length, int16_t height, uint16_t color, Velocity speed, uint32_t thread_id);
//Ball update_ball_state(Ball user_ball);
//
//
//Ball lcd_draw_ball(Ball ball_object);
//Ball lcd_kill_ball(Ball ball_object);
//Point find_wrap_around(Point initial_point, Velocity speed, wrapType wrap);
//

#ifndef stage_block_size
#define stage_block_size 8
#endif
void draw_stage_block(uint16_t block_colors[stage_block_size][stage_block_size], int16_t xStart,
                      int16_t yStart);

/************************************ Public Functions  *******************************************/

#endif /* LCDLIB_H_ */
