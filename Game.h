/*
 * Game.h
 *
 *  Created on: Feb 27, 2017
 *      Author: danny
 */

#ifndef GAME_H_
#define GAME_H_

/*********************************************** Includes ********************************************************************/
#include <stdbool.h>
#include <stdint.h>
//#include "../G8RTOS_Lab5/G8RTOS.h"
#include "cc3100_usage.h"
#include "LCDLib.h"

#include "fighter_cat_gif_color_array_no_background.h"

/*********************************************** Includes ********************************************************************/

/*********************************************** Externs ********************************************************************/

/* Semaphores here */ 

/*********************************************** Externs ********************************************************************/

/*********************************************** Global Defines ********************************************************************/

// Pick which map and texture pack you want, currently there are only 0 or 1
#define MAP_CHOICE 1

#define MAX_NUM_OF_PLAYERS  2
#define MAX_NUM_OF_BALLS    10

/* Size of the texture square measured in one direction */
#define TEXTURE_SIZE 16

// This game can actually be played with 4 players... a little bit more challenging, but doable! 
#define NUM_OF_PLAYERS_PLAYING 2

/* Size of game arena */
#define ARENA_MIN_X                  TEXTURE_SIZE
#define ARENA_MAX_X                  MAX_SCREEN_X - TEXTURE_SIZE
#define ARENA_MIN_Y                  TEXTURE_SIZE
#define ARENA_MAX_Y                  MAX_SCREEN_Y - TEXTURE_SIZE

/* Size of objects */
#define PADDLE_LEN                   64
#define PADDLE_LEN_D2                (PADDLE_LEN >> 1)
#define PADDLE_WID                   4
#define PADDLE_WID_D2                (PADDLE_WID >> 1)
#define BALL_SIZE                    4
#define BALL_SIZE_D2                 (BALL_SIZE >> 1)
#define BOUNDARY_WIDTH               5

// Size of players
#define PLAYER_LEN                   FIGHTER_CAT_SPRITE_WIDTH
#define PLAYER_LEN_D2                (PLAYER_LEN >> 1)
#define PLAYER_WID                   FIGHTER_CAT_SPRITE_HEIGHT
#define PLAYER_WID_D2                (PLAYER_WID >> 1)

// number of lives initially
#define NUM_INITIAL_LIVES 3

// Initial player positions
#define HOST_PLAYER_START_X 25
#define HOST_PLAYER_START_Y 25
#define CLIENT_PLAYER_START_X (MAX_SCREEN_X - HOST_PLAYER_START_X - PLAYER_WID)
#define CLIENT_PLAYER_START_Y HOST_PLAYER_START_Y

// start out in the middle of respective side of stage
#define PLAYER_1_CENTER              MAX_SCREEN_X >> 2
#define PLAYER_2_CENTER              MAX_SCREEN_X - (MAX_SCREEN_X >> 2) - FIGHTER_CAT_SPRITE_WIDTH

/* Centers for paddles at the center of the sides */
#define PADDLE_X_CENTER              MAX_SCREEN_X >> 1

/* Edge limitations for player's center coordinate */
#define HORIZ_CENTER_MAX_PL          (ARENA_MAX_X - PLAYER_LEN_D2)
#define HORIZ_CENTER_MIN_PL          (ARENA_MIN_X + PLAYER_LEN_D2)

//#define HORIZ_CENTER_MAX_PL          (ARENA_MAX_X - PADDLE_LEN_D2)
//#define HORIZ_CENTER_MIN_PL          (ARENA_MIN_X + PADDLE_LEN_D2)

/* Constant enters of each player */
#define TOP_PLAYER_CENTER_Y          (ARENA_MIN_Y + PLAYER_LEN_D2)
#define BOTTOM_PLAYER_CENTER_Y       (ARENA_MAX_Y - PLAYER_LEN_D2)
//#define TOP_PLAYER_CENTER_Y          (ARENA_MIN_Y + PADDLE_WID_D2)
//#define BOTTOM_PLAYER_CENTER_Y       (ARENA_MAX_Y - PADDLE_WID_D2)

/* Edge coordinates for paddles */
#define TOP_PADDLE_EDGE              (ARENA_MIN_Y + PADDLE_WID)
#define BOTTOM_PADDLE_EDGE           (ARENA_MAX_Y - PADDLE_WID)


/* Amount of allowable space for collisions with the sides of paddles */
#define WIGGLE_ROOM                  2

/* Value for velocities from contact with paddles */
#define _1_3_PADDLE                  11

/* Defines for Minkowski Alg. for collision */
#define WIDTH_TOP_OR_BOTTOM          ((PADDLE_LEN + BALL_SIZE) >> 1) + WIGGLE_ROOM
#define HEIGHT_TOP_OR_BOTTOM         ((PADDLE_WID + BALL_SIZE) >> 1) + WIGGLE_ROOM

/* Edge limitations for ball's center coordinate */
#define HORIZ_CENTER_MAX_BALL        (ARENA_MAX_X - BALL_SIZE_D2)
#define HORIZ_CENTER_MIN_BALL        (ARENA_MIN_X + BALL_SIZE_D2)
#define VERT_CENTER_MAX_BALL         (ARENA_MAX_Y - BALL_SIZE_D2)
#define VERT_CENTER_MIN_BALL         (ARENA_MIN_Y + BALL_SIZE_D2)

/* Maximum ball speed */
#define MAX_BALL_SPEED               4

/* Background color - Black */
#define BACK_COLOR                   LCD_BLACK

/* Offset for printing player to avoid blips from left behind ball */
#define PRINT_OFFSET                10

/* Used as status LEDs for Wi-Fi */
#define BLUE_LED BIT2
#define RED_LED BIT0

#define MAX_LED_SCORE 8

/* Enums for player colors */
typedef enum
{
    PLAYER_RED = LCD_RED,
    PLAYER_BLUE = LCD_BLUE
}playerColor;

/* Enums for player numbers */
typedef enum
{
    BOTTOM = 0,
    TOP = 1
}playerPosition;

typedef enum
{
    NOTHING = 0,
    RESTART = 1,
    QUIT = 2
}endGameAction;


typedef enum {BALL_none, BALL_UP_RIGHT, BALL_UP_LEFT, BALL_DOWN_RIGHT, BALL_DOWN_LEFT, BALL_RIGHT, BALL_LEFT, BALL_UP, BALL_DOWN} ball_dir;


/*********************************************** Global Defines ********************************************************************/

/*********************************************** Data Structures ********************************************************************/
#pragma pack ( push, 1)
/*
 * Struct to be sent from the client to the host
 */
typedef struct
{
    uint32_t IP_address;
    int16_t displacement_x;
    int16_t displacement_y;
    uint8_t playerNumber;
    ball_dir ball_direction;
    bool ready;
    bool joined;
    bool acknowledge;
    bool spawn_ball;
} SpecificPlayerInfo_t;

/*
 * General player info to be used by both host and client
 * Client responsible for translation
 */
typedef struct
{
    uint16_t color;
    uint16_t *texture;
    int16_t x;
    int16_t y;
} GeneralPlayerInfo_t;

/*
 * Struct of all the balls, only changed by the host
 */
typedef struct
{
    int16_t currentCenterX;
    int16_t currentCenterY;
    uint16_t color;
    bool alive;
    bool kill_me;
} Ball_t;

/*
 * Struct to be sent from the host to the client
 */
typedef struct
{
    SpecificPlayerInfo_t player;
    GeneralPlayerInfo_t players[MAX_NUM_OF_PLAYERS];
    Ball_t balls[MAX_NUM_OF_BALLS];
    uint16_t numberOfBalls;
    bool winner;
    bool gameDone;
    uint8_t LEDScores[2];
    uint8_t overallScores[2];
} GameState_t;
#pragma pack ( pop )

/*
 * Struct of all the previous ball locations, only changed by self for drawing!
 */
typedef struct
{
    int16_t CenterX;
    int16_t CenterY;
}PrevBall_t;

/*
 * Struct of all the previous players locations, only changed by self for drawing
 */
typedef struct
{
    int16_t x;
    int16_t y;
}PrevPlayer_t;
/*********************************************** Data Structures ********************************************************************/


/*********************************************** Client Threads *********************************************************************/
/*
 * Thread for client to join game
 */
void JoinGame();

/*
 * Thread that receives game state packets from host
 */
void ReceiveDataFromHost();

/*
 * Thread that sends UDP packets to host
 */
void SendDataToHost();

/*
 * Thread to read client's joystick
 */
void ReadJoystickClient();

/*
 * End of game for the client
 */
void EndOfGameClient();

/*********************************************** Client Threads *********************************************************************/


/*********************************************** Host Threads *********************************************************************/
/*
 * Thread for the host to create a game
 */
void CreateGame();

/*
 * Thread that sends game state to client
 */
void SendDataToClient();

/*
 * Thread that receives UDP packets from client
 */
void ReceiveDataFromClient();

/*
 * Generate Ball thread
 */
void GenerateBall();

/*
 * Thread to read host's joystick
 */
void ReadJoystickHost();

/*
 * Thread to move a single ball
 */
void MoveBall_host();
void MoveBall_client();

/*
 * End of game for the host
 */
void EndOfGameHost();

/*********************************************** Host Threads *********************************************************************/


/*********************************************** Common Threads *********************************************************************/
/*
 * Idle thread
 */
void IdleThread();

/*
 * Thread to draw all the objects in the game
 */
void DrawObjects();

/*
 * Thread to update LEDs based on score
 */
void MoveLEDs();

/*********************************************** Common Threads *********************************************************************/


/*********************************************** Public Functions *********************************************************************/
/*
 * Determine whether this instance will be Host or Client depending on button press
 */
void AcquirePlayerRole();

/*
 * Returns either Host or Client depending on button press
 */
playerType GetPlayerRole();

/*
 * Return the Client's IP address
 */
uint32_t GetClientIP();

/*
 * Return the Host's IP address
 */
uint32_t GetHostIP();

/*
 * Draw players given center X center coordinate
 */
void DrawPlayer(GeneralPlayerInfo_t * player);

/*
 * Updates player's paddle based on current and new center
 */
void UpdatePlayerOnScreen(PrevPlayer_t * prevPlayerIn, GeneralPlayerInfo_t * outPlayer);

/*
 * Function updates ball position on screen
 */
void UpdateBallOnScreen(PrevBall_t * previousBall, Ball_t * currentBall, uint16_t outColor);

/*
 * Initializes and prints initial game state
 */
void InitBoardState();

void host_end_game_screen(void);
void client_end_game_screen();
void quit_screen_host(void);
void quit_screen_client(void);
void color_screen(uint16_t winner_color);
void fight_button_press(void);
void FireWait(void);
/*********************************************** Public Functions *********************************************************************/


#endif /* GAME_H_ */
