#include "threads.h"
#include <BSP.h>
#include "driverlib.h"
#include "LCDLib.h"
#include "G8RTOS_CriticalSection.h"
#include "cc3100_usage.h"
#include "Game.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "G8RTOS.h"

// instantiate mutexs
semaphore_t sensor_I2C;
semaphore_t led_I2C;
semaphore_t lcd_SPI;
semaphore_t WiFi_mutex;
semaphore_t led_mutex;

int test;


/*
 * Stable Rx/Tx between client & host
 *  >Issue with SendDataToHost()
 *     -Causes client to crash
 *
 * Have EndGames conditions written
 */

int16_t joystick_host_x_coor = 0;
int16_t joystick_host_y_coor = 0;
int16_t joystick_client_x_coor = 0;
int16_t joystick_client_y_coor = 0;

int8_t accelerometer_x = 0;
int8_t accelerometer_y = 0;

static GameState_t game_state;

static SpecificPlayerInfo_t new_client_data;
static SpecificPlayerInfo_t client_player;

int16_t client_displacement; // global variable for host

char display_score_buffer_0[10];
char display_score_buffer_1[10];

void ball_thread(void);
void idle_thread(void);
void DrawScore(void);
void DrawBoundary(void);
void end_game_button_press(void);



bool waiting_on_player_type_designation = true;
playerType player_type = Host;

typedef enum {left, right, up, down} direction;

bool update_game_score = false;
//bool led_mutex_score = false;
bool check_end_game_buttons = false;

playerColor winner;
endGameAction end_game_action = NOTHING;

void DrawScore(void){

    char zero_text[] = "0";
    sprintf(display_score_buffer_0, "%d", game_state.overallScores[0]);
    sprintf(display_score_buffer_1, "%d", game_state.overallScores[1]);

    // clear text area
    G8RTOS_WaitSemaphore(&lcd_SPI);
    LCD_DrawRectangle(1, ARENA_MIN_X - BOUNDARY_WIDTH - 1, 10, 30, LCD_BLACK);
    G8RTOS_SignalSemaphore(&lcd_SPI);

    G8RTOS_WaitSemaphore(&lcd_SPI);
    LCD_DrawRectangle(1, ARENA_MIN_X - BOUNDARY_WIDTH - 1, MAX_SCREEN_Y - 20, MAX_SCREEN_Y, LCD_BLACK);
    G8RTOS_SignalSemaphore(&lcd_SPI);

    // update score
    if(game_state.overallScores[1] < 10)
    {
        G8RTOS_WaitSemaphore(&lcd_SPI);
        LCD_Text(1, 10, zero_text, LCD_WHITE);
        LCD_Text(1+TEXT_CHAR_SIZE, 10, display_score_buffer_1, LCD_WHITE);
        G8RTOS_SignalSemaphore(&lcd_SPI);
    }
    else
    {
        G8RTOS_WaitSemaphore(&lcd_SPI);
        LCD_Text(1, 10, display_score_buffer_1, LCD_WHITE);
        G8RTOS_SignalSemaphore(&lcd_SPI);
    }

    if(game_state.overallScores[0] < 10)
    {
        G8RTOS_WaitSemaphore(&lcd_SPI);
        LCD_Text(1, MAX_SCREEN_Y - 20, zero_text, LCD_WHITE);
        LCD_Text(1+TEXT_CHAR_SIZE, MAX_SCREEN_Y - 20, display_score_buffer_0, LCD_WHITE);
        G8RTOS_SignalSemaphore(&lcd_SPI);
    }
    else
    {
        G8RTOS_WaitSemaphore(&lcd_SPI);
        LCD_Text(1, MAX_SCREEN_Y - 20, display_score_buffer_0, LCD_WHITE);
        G8RTOS_SignalSemaphore(&lcd_SPI);
    }
}

void DrawBoundary(void){
    //LEFT
    G8RTOS_WaitSemaphore(&lcd_SPI);
    LCD_DrawRectangle(ARENA_MIN_X - BOUNDARY_WIDTH, ARENA_MIN_X, ARENA_MIN_Y, ARENA_MAX_Y, LCD_WHITE);
    LCD_DrawRectangle(ARENA_MAX_X, ARENA_MAX_X + BOUNDARY_WIDTH, ARENA_MIN_Y, ARENA_MAX_Y, LCD_WHITE);
    G8RTOS_SignalSemaphore(&lcd_SPI);
}

void DrawPlayer(GeneralPlayerInfo_t * player){
    int16_t y_start;
    int16_t y_end;
    int16_t x_start = player->currentCenter - PADDLE_LEN_D2;
    int16_t x_end = player->currentCenter + PADDLE_LEN_D2;

    // handle boundary case for x direction
    if(x_start < ARENA_MIN_X)
    {
        player->currentCenter = HORIZ_CENTER_MIN_PL;
        x_start = ARENA_MIN_X;
        x_end = ARENA_MIN_X + PADDLE_LEN;
    }
    if(x_end > ARENA_MAX_X)
    {
        player->currentCenter = HORIZ_CENTER_MAX_PL;
        x_start = ARENA_MAX_X-PADDLE_LEN;
        x_end = ARENA_MAX_X;
    }
    // handle y start and end
    if(player->position == BOTTOM)
    {
        y_start = BOTTOM_PADDLE_EDGE;
        y_end = ARENA_MAX_Y;
    }
    if(player->position == TOP)
    {
        y_start = ARENA_MIN_Y;
        y_end = TOP_PADDLE_EDGE;
    }

    G8RTOS_WaitSemaphore(&lcd_SPI);
    LCD_DrawRectangle(x_start, x_end, y_start, y_end, player->color);
    G8RTOS_SignalSemaphore(&lcd_SPI);
}

void DrawObjects(void){
    PrevPlayer_t prev_red_x;
    PrevPlayer_t prev_blue_x;
    prev_red_x.Center = game_state.players[1].currentCenter;
    prev_blue_x.Center = game_state.players[0].currentCenter;
    PrevBall_t prev_ball_array[MAX_NUM_OF_BALLS];

    // Init prev ball array
    for(uint16_t i=0; i<MAX_NUM_OF_BALLS; i++)
    {
        prev_ball_array[i].CenterX = game_state.balls[i].currentCenterX;
        prev_ball_array[i].CenterY = game_state.balls[i].currentCenterY;
    }

    while(1)
    {
        // update balls
        for(uint_fast8_t j=0; j<MAX_NUM_OF_BALLS; j++){
            if(game_state.balls[j].alive == true){
                if( (prev_ball_array[j].CenterX != game_state.balls[j].currentCenterX) ||
                    (prev_ball_array[j].CenterY != game_state.balls[j].currentCenterY) )
                {
                    G8RTOS_WaitSemaphore(&lcd_SPI);
    //                    uint32_t PRIMASK_state = StartCriticalSection();

                    LCD_DrawRectangle(prev_ball_array[j].CenterX - BALL_SIZE_D2 +
                                      (1 - (BALL_SIZE % BALL_SIZE_D2)),
                                      prev_ball_array[j].CenterX + BALL_SIZE_D2,
                                      prev_ball_array[j].CenterY - BALL_SIZE_D2 +
                                      (1 - (BALL_SIZE % BALL_SIZE_D2)),
                                      prev_ball_array[j].CenterY + BALL_SIZE_D2,
                                      BACK_COLOR);
                    G8RTOS_SignalSemaphore(&lcd_SPI);

                    G8RTOS_WaitSemaphore(&lcd_SPI);
                    LCD_DrawRectangle(game_state.balls[j].currentCenterX -
                                      BALL_SIZE_D2 +
                                      (1 - (BALL_SIZE % BALL_SIZE_D2)),
                                      game_state.balls[j].currentCenterX +
                                      BALL_SIZE_D2,
                                      game_state.balls[j].currentCenterY -
                                      BALL_SIZE_D2 +
                                      (1 - (BALL_SIZE % BALL_SIZE_D2)),
                                      game_state.balls[j].currentCenterY +
                                      BALL_SIZE_D2,
                                      game_state.balls[j].color);

    //                    EndCriticalSection(PRIMASK_state);
                    G8RTOS_SignalSemaphore(&lcd_SPI);
                    prev_ball_array[j].CenterX = game_state.balls[j].currentCenterX;
                    prev_ball_array[j].CenterY = game_state.balls[j].currentCenterY;
                }
                if(game_state.balls[j].kill_me)
                {
                    G8RTOS_WaitSemaphore(&lcd_SPI);
                //                uint32_t PRIMASK_state = StartCriticalSection();
                    LCD_DrawRectangle(game_state.balls[j].currentCenterX - BALL_SIZE_D2 +
                                      (1 - (BALL_SIZE % BALL_SIZE_D2)),
                                      game_state.balls[j].currentCenterX + BALL_SIZE_D2,
                                      game_state.balls[j].currentCenterY - BALL_SIZE_D2 +
                                      (1 - (BALL_SIZE % BALL_SIZE_D2)),
                                      game_state.balls[j].currentCenterY + BALL_SIZE_D2,
                                      BACK_COLOR);
                //                EndCriticalSection(PRIMASK_state);
                    G8RTOS_SignalSemaphore(&lcd_SPI);
                    game_state.numberOfBalls--;
                    game_state.balls[j].alive = false;
                    game_state.balls[j].kill_me = false;
                }
            }
        }

        // update paddles
        if(game_state.players[1].currentCenter != prev_red_x.Center){
            UpdatePlayerOnScreen(&prev_red_x, &(game_state.players[1]));
        }
        if(game_state.players[0].currentCenter != prev_blue_x.Center){
            UpdatePlayerOnScreen(&prev_blue_x, &(game_state.players[0]));
        }
        prev_red_x.Center = game_state.players[1].currentCenter;
        prev_blue_x.Center = game_state.players[0].currentCenter;
        sleep(20); // sleep for 20ms (reasonable refresh rate)
    }
}

void UpdatePlayerOnScreen(PrevPlayer_t * prevPlayerIn, GeneralPlayerInfo_t * outPlayer){
    // only update if new center is different than past center
    int16_t y_start;
    int16_t y_end;
    int16_t x_old_start = (prevPlayerIn->Center) - PADDLE_LEN_D2;
    int16_t x_old_end = (prevPlayerIn->Center) + PADDLE_LEN_D2;
    int16_t x_new_start = (outPlayer->currentCenter) - PADDLE_LEN_D2;
    int16_t x_new_end = (outPlayer->currentCenter) + PADDLE_LEN_D2;
    // handle boundary case for x direction
    if(x_new_start < ARENA_MIN_X + 2)
    {
        outPlayer->currentCenter = HORIZ_CENTER_MIN_PL + 2;
        x_new_start = ARENA_MIN_X + 2;
        x_new_end = ARENA_MIN_X + 2 + PADDLE_LEN;
    }
    if(x_new_end >= ARENA_MAX_X - 2)
    {
        outPlayer->currentCenter = HORIZ_CENTER_MAX_PL - 2;
        x_new_start = ARENA_MAX_X - 2 - PADDLE_LEN;
        x_new_end = ARENA_MAX_X - 2;
    }
    // handle y start and end
    if(outPlayer->position == BOTTOM)
    {
        y_start = BOTTOM_PADDLE_EDGE;
        y_end = ARENA_MAX_Y;
    }
    if(outPlayer->position == TOP)
    {
        y_start = ARENA_MIN_Y;
        y_end = TOP_PADDLE_EDGE;
    }

    // clear stale paddle data
    if( (outPlayer->currentCenter) < (prevPlayerIn->Center) )
    {
        G8RTOS_WaitSemaphore(&lcd_SPI);
        LCD_DrawRectangle(x_new_end, x_old_end + WIGGLE_ROOM, y_start, y_end, BACK_COLOR);

        LCD_DrawRectangle(x_new_start, x_old_start + WIGGLE_ROOM, y_start, y_end, outPlayer->color);
        G8RTOS_SignalSemaphore(&lcd_SPI);
    }
    else{
        G8RTOS_WaitSemaphore(&lcd_SPI);
        LCD_DrawRectangle(x_old_start - WIGGLE_ROOM, x_new_start, y_start, y_end, BACK_COLOR);
        LCD_DrawRectangle(x_old_end - WIGGLE_ROOM, x_new_end, y_start, y_end, outPlayer->color);
        G8RTOS_SignalSemaphore(&lcd_SPI);

    }


}

void MoveLEDs(void){
    uint16_t red_score;
    uint16_t blue_score;
    while(1){
        G8RTOS_WaitSemaphore(&led_mutex);

        switch(game_state.LEDScores[1])
        {
            case 0:
                red_score = 0;
                break;
            case 1:
                red_score = BIT0 ;
                break;
            case 2:
                red_score = BIT0 | BIT1;
                break;
            case 3:
                red_score = BIT0 | BIT1 | BIT2;
                break;
            case 4:
                red_score = BIT0 | BIT1 | BIT2 | BIT3;
                break;
            case 5:
                red_score = BIT0 | BIT1 | BIT2 | BIT3 | BIT4;
                break;
            case 6:
                red_score = BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5;
                break;
            case 7:
                red_score = BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6;
                break;
            case 8:
                red_score = BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7;
                break;
        }

        switch(game_state.LEDScores[0])
        {
            case 0:
                blue_score = 0;
                break;
            case 1:
                blue_score = BITF;
                break;
            case 2:
                blue_score = BITF | BITE;
                break;
            case 3:
                blue_score = BITF | BITE | BITD;
                break;
            case 4:
                blue_score = BITF | BITE | BITD | BITC;
                break;
            case 5:
                blue_score = BITF | BITE | BITD | BITC | BITB;
                break;
            case 6:
                blue_score = BITF | BITE | BITD | BITC | BITB | BITA;
                break;
            case 7:
                blue_score = BITF | BITE | BITD | BITC | BITB | BITA | BIT9;
                break;
            case 8:
                blue_score = BITF | BITE | BITD | BITC | BITB | BITA | BIT9 | BIT8;
                break;
        }

        LP3943_LedModeSet(RED, red_score);
        LP3943_LedModeSet(BLUE, blue_score);
//        G8RTOS_KillSelf();
    }
}

void idle_thread(void) { // idle thread
    while(1);
}


extern void CreateGame(void){

    // Initialize the game state
    game_state.numberOfBalls = 0;
    // should I init ball aray?
    for(uint16_t i=0; i<MAX_NUM_OF_BALLS; i++)
    {
        game_state.balls[i].currentCenterX = MAX_SCREEN_X/2;
        game_state.balls[i].currentCenterY = (VERT_CENTER_MAX_BALL - VERT_CENTER_MIN_BALL)/2;
        game_state.balls[i].color = LCD_WHITE;
        game_state.balls[i].alive = false;
        game_state.balls[i].kill_me = false;
    }

    game_state.winner = false;
    game_state.gameDone = false;
    for(uint16_t i=0; i<MAX_NUM_OF_PLAYERS; i++)
    {
        game_state.players[i].currentCenter = PADDLE_X_CENTER; // start with paddles in center of screen
        game_state.LEDScores[i] = 0;
    }
    game_state.overallScores[0] = 0;
    game_state.overallScores[1] = 0;
    game_state.LEDScores[0] = 0;
    game_state.LEDScores[1] = 0;
    game_state.players[0].color = PLAYER_BLUE;
    game_state.players[0].position = BOTTOM;
    game_state.players[1].color = PLAYER_RED;
    game_state.players[1].position = TOP;

    // draw init board (draw arena, players, and scores)
    DrawBoundary();
    DrawScore();
    for(uint16_t i=0; i<MAX_NUM_OF_PLAYERS; i++)
    {
        DrawPlayer(&(game_state.players[i])); // put the player at the center of the screen upon starting game
    }

    // add any semaphores
    G8RTOS_InitSemaphore(&led_mutex, 0);
    // semaphore init shit
    // LED I2C mutex
    G8RTOS_InitSemaphore(&led_I2C, 0);

    // sensor I2C mutex
    G8RTOS_InitSemaphore(&sensor_I2C, 1);

    // LCD/Touchpad mutex
    G8RTOS_InitSemaphore(&lcd_SPI, 1);

    // Wifi mutex
    G8RTOS_InitSemaphore(&WiFi_mutex, 1); // can I move this to the other semaphore init area

    // Add threads: GenerateBall, DrawObjects, ReadJoystickHost, SendDataToClient,ReceiveDataFromClient, MoveLEDs(lower priority), Idle
    G8RTOS_AddThread(idle_thread, 254, "idle");
    G8RTOS_AddThread(MoveLEDs, 1, "MoveLeds");
    G8RTOS_AddThread(GenerateBall, 1, "GenBall");
    G8RTOS_AddThread(DrawObjects, 1, "DrawObj");
    G8RTOS_AddThread(ReadJoystickHost, 1, "JoyHost");
    G8RTOS_AddThread(SendDataToClient, 1, "Tx2Client");
    G8RTOS_AddThread(ReceiveDataFromClient, 1, "RxFrmClient");

    // Kill self
    G8RTOS_KillSelf();
    while(1);
}

extern void ReadJoystickHost(void){
    int16_t displacement = 0;
    while(1){
        GetJoystickCoordinates(&joystick_host_x_coor, &joystick_host_y_coor);
        // do we need to bias the value?
        displacement = joystick_host_x_coor / -4096;

//        if(joystick_host_x_coor > 8000)
//        {
//            displacement = -1;
//        }
//        else if(joystick_host_x_coor > 100)
//        {
//            displacement = 0;
//        }
//        else if(joystick_host_x_coor > -1000)
//        {
//            displacement = 0;
//        }
//        else if(joystick_host_x_coor > -8000)
//        {
//            displacement = 1;
//        }
        game_state.players[0].currentCenter += displacement; // update player 0 who is the host players is part of game state struct
        game_state.players[1].currentCenter += client_displacement; // update player 1 simultaneously to guarantee paddle move speed

        sleep(10);
    }
}

void GenerateBall() {
    test = 69;
    while(1)
    {
        if(game_state.numberOfBalls < MAX_NUM_OF_BALLS)
        {
            game_state.numberOfBalls++;
            // add the thread to the scheduler
            test = G8RTOS_AddThread(MoveBall, 1, "MoveBall");
        }
        // Sleeps proportional to the number of ball currently in play
        sleep(1024 * game_state.numberOfBalls);
//        G8RTOS_KillSelf(); // remove to add more balls
    }
}

void MoveBall() {
    uint_fast8_t ball_index;
    for(ball_index = 0; ball_index < MAX_NUM_OF_BALLS; ball_index++)
    {
        if(game_state.balls[ball_index].alive == false)
            break;
    }
    // debug code to check for error, shouldn't get stuck here
    if(ball_index == MAX_NUM_OF_BALLS)
        while(1);

    // designate this ball will be alive
    game_state.balls[ball_index].alive = true;

    game_state.balls[ball_index].color = LCD_WHITE;

    game_state.balls[ball_index].currentCenterX = MAX_SCREEN_X/2;
//            (rand() % (HORIZ_CENTER_MAX_BALL - HORIZ_CENTER_MIN_BALL)) +
//            HORIZ_CENTER_MIN_BALL;
    game_state.balls[ball_index].currentCenterY = (VERT_CENTER_MAX_BALL - VERT_CENTER_MIN_BALL)/2;
//            (rand() % (VERT_CENTER_MAX_BALL - VERT_CENTER_MIN_BALL)) +
//            VERT_CENTER_MIN_BALL;

    // Start with random velocity
    int16_t velocity_x = ((rand() % MAX_BALL_SPEED * 2) - MAX_BALL_SPEED);
    int16_t velocity_y = ((rand() % MAX_BALL_SPEED * 2) - MAX_BALL_SPEED);

    if(velocity_x == 0){
        velocity_x++;
    }
    if(velocity_y == 0){
        velocity_y++;
    }

    while(1)
    {
        static int16_t predictedCenterX = 0;
        static int16_t predictedCenterY = 0;
        static bool collision_predicted = false;

        bool wall_collision_already_detected = false; // used for catching simultaneous paddle & wall collisions

        if(collision_predicted)
        {
            game_state.balls[ball_index].currentCenterX = predictedCenterX;
            game_state.balls[ball_index].currentCenterY = predictedCenterY;

            collision_predicted = false;
        }
        else
        {
//      Ball movement
        game_state.balls[ball_index].currentCenterX += velocity_x;
//        game_state.balls[ball_index].currentCenterX %= MAX_SCREEN_X;
        game_state.balls[ball_index].currentCenterY += velocity_y;
//        game_state.balls[ball_index].currentCenterY %= MAX_SCREEN_Y;
        }

        // Collision checking

        // left wall
        {
            int32_t w = (BALL_SIZE + BOUNDARY_WIDTH) / 2;
            int32_t h = (BALL_SIZE + (ARENA_MAX_Y - ARENA_MIN_Y));
            int32_t dx = game_state.balls[ball_index].currentCenterX + velocity_x -
                    (ARENA_MIN_X - 2);
            int32_t dy = game_state.balls[ball_index].currentCenterY + velocity_y -
                    (ARENA_MAX_Y - ARENA_MIN_Y)/2;
            if(abs(dx) <= w && abs(dy) <= h)
            {
                predictedCenterX = 2*ARENA_MIN_X - game_state.balls[ball_index].currentCenterX + velocity_x;
                predictedCenterY = game_state.balls[ball_index].currentCenterY;
                // bounce the ball off of the left wall
                velocity_x = -velocity_x;
                wall_collision_already_detected = true;
            }
        }

        // right wall
        {
            int32_t w = (BALL_SIZE + BOUNDARY_WIDTH) / 2;
            int32_t h = (BALL_SIZE + (ARENA_MAX_Y - ARENA_MIN_Y)) / 2;
            int32_t dx = game_state.balls[ball_index].currentCenterX + velocity_x -
                    (ARENA_MAX_X + 2);
            int32_t dy = game_state.balls[ball_index].currentCenterY + velocity_y -
                    (ARENA_MAX_Y - ARENA_MIN_Y)/2;
            if(abs(dx) <= w && abs(dy) <= h)
            {
                predictedCenterX = 2*ARENA_MAX_X - game_state.balls[ball_index].currentCenterX + velocity_x;
                predictedCenterY = game_state.balls[ball_index].currentCenterY + velocity_y;
                // bounce the ball off of the left wall
                velocity_x = -velocity_x;
                wall_collision_already_detected = true;
            }
        }

        // paddle 0
        {
            int32_t w = (BALL_SIZE + PADDLE_LEN) / 2;
            int32_t h = (BALL_SIZE + PADDLE_WID) / 2;
            int32_t dx = game_state.balls[ball_index].currentCenterX + velocity_x -
                    game_state.players[0].currentCenter;
            int32_t dy = game_state.balls[ball_index].currentCenterY + velocity_y -
                    BOTTOM_PADDLE_EDGE - WIGGLE_ROOM;
            if(abs(dx) <= w && abs(dy) <= h)
            {
                // bounce the ball off of paddle 0 (the bottom paddle)
                if(game_state.balls[ball_index].currentCenterY > BOTTOM_PADDLE_EDGE)
                    game_state.balls[ball_index].currentCenterY = BOTTOM_PADDLE_EDGE - BALL_SIZE;

                if(!wall_collision_already_detected)
                    predictedCenterX = game_state.balls[ball_index].currentCenterX + velocity_x;
                predictedCenterY = 2*BOTTOM_PADDLE_EDGE - game_state.balls[ball_index].currentCenterY + velocity_y;
                velocity_y = -velocity_y;
                game_state.balls[ball_index].color = game_state.players[0].color;
            }
        }

        // paddle 1
        {
            int32_t w = (BALL_SIZE + PADDLE_LEN) / 2;
            int32_t h = (BALL_SIZE + PADDLE_WID) / 2;
            int32_t dx = game_state.balls[ball_index].currentCenterX + velocity_x -
                    game_state.players[1].currentCenter;
            int32_t dy = game_state.balls[ball_index].currentCenterY + velocity_y -
                    TOP_PADDLE_EDGE + WIGGLE_ROOM;
            if(abs(dx) <= w && abs(dy) <= h)
            {
                // bounce the ball off of paddle 1 (the top paddle)
                if(game_state.balls[ball_index].currentCenterY < TOP_PADDLE_EDGE)
                    game_state.balls[ball_index].currentCenterY = TOP_PADDLE_EDGE + BALL_SIZE;
                if(!wall_collision_already_detected)
                    predictedCenterX = game_state.balls[ball_index].currentCenterX + velocity_x;
                predictedCenterY = 2*TOP_PADDLE_EDGE - game_state.balls[ball_index].currentCenterY + velocity_y;
                velocity_y = -velocity_y;
                game_state.balls[ball_index].color = game_state.players[1].color;
            }
        }
        // bottom wall
        {
            if(game_state.balls[ball_index].currentCenterY  > BOTTOM_PLAYER_CENTER_Y)// BOTTOM_PLAYER_CENTER_Y
            {
                // kill ball
                game_state.balls[ball_index].kill_me = true;
                if(game_state.balls[ball_index].color == game_state.players[1].color)
                {
                    // increment score for top player
                    game_state.LEDScores[1]++;
                    if(game_state.LEDScores[1] >= MAX_LED_SCORE)
                    {
                        winner = game_state.players[1].color;
                        game_state.overallScores[1]++;
                        game_state.winner = true;
                        update_game_score = true;
//                        G8RTOS_AddThread(EndOfGameHost, 1, "EndGameHost");
                    }
                    G8RTOS_SignalSemaphore(&led_mutex);
                }
                G8RTOS_KillSelf();
                while(1);
            }
        }

        // top wall
        {
            if(game_state.balls[ball_index].currentCenterY  < TOP_PLAYER_CENTER_Y) // TOP_PLAYER_CENTER_Y
            {
                // kill ball
                game_state.balls[ball_index].kill_me = true;
                if(game_state.balls[ball_index].color == game_state.players[0].color)
                {
                    // increment score for bottom player
                    game_state.LEDScores[0]++;
                    if(game_state.LEDScores[0] >= MAX_LED_SCORE)
                    {
                        winner = game_state.players[0].color;
                        game_state.overallScores[0]++;
                        game_state.winner = true;
                        update_game_score = true;
//                        G8RTOS_AddThread(EndOfGameHost, 1, "EndGameHost");
                    }
                    G8RTOS_SignalSemaphore(&led_mutex);
                }
                G8RTOS_KillSelf();
                while(1);
            }
        }

        sleep(35); //sleep for 35
    }
}

void EndOfGameHost() {
    while(1)
    {
        // wait for all semaphores to be released
        G8RTOS_WaitSemaphore(&WiFi_mutex);
//        G8RTOS_WaitSemaphore(&led_mutex);
        G8RTOS_WaitSemaphore(&lcd_SPI);

        // kill all other threads
        G8RTOS_KillAllOtherThreads();

        DrawScore(); // update the score bc a game was won

        // re-initialize semaphores
        G8RTOS_InitSemaphore(&WiFi_mutex, 1);
        G8RTOS_InitSemaphore(&led_mutex, 0);
        G8RTOS_InitSemaphore(&lcd_SPI, 1);




        // clear screen with winner's color Print some message that waits for the host's action to start a new game
        host_end_game_screen();

        //Create an aperiodic thread that waits for the host's button press (the client will just be waiting on the host to start a new game)
        check_end_game_buttons = true;


        /* Configure low true interrupt on P4.0 for TP */
        P4DIR &= ~(BIT4 | BIT5); // Set P4 as input
        P4IFG &= ~(BIT4 | BIT5); // P4.0 IFG cleared
        P4IE  |= BIT4 | BIT5;  // Enable interrupt on P4.0
        P4IES |= BIT4 | BIT5;  // high-to-low transition
        P4REN |= BIT4 | BIT5;  // Pull-up resistor
        P4OUT |= BIT4 | BIT5;  // Sets res to pull-up

        NVIC_EnableIRQ(PORT4_IRQn); // enable the interrupt

        G8RTOS_AddAPeriodicEvent(end_game_button_press, 1, PORT4_IRQn);

        while(end_game_action == NOTHING);

        NVIC_DisableIRQ(PORT4_IRQn);

        if(end_game_action == RESTART)
        {
            // reset game variables
            game_state.numberOfBalls = 0;
            for(uint16_t i=0; i<MAX_NUM_OF_BALLS; i++)
            {
                game_state.balls[i].currentCenterX = MAX_SCREEN_X/2;
                game_state.balls[i].currentCenterY = (VERT_CENTER_MAX_BALL - VERT_CENTER_MIN_BALL)/2;
                game_state.balls[i].color = LCD_WHITE;
                game_state.balls[i].alive = false;
                game_state.balls[i].kill_me = false;
            }

            game_state.winner = false;
            game_state.gameDone = false;
            for(uint16_t i=0; i<MAX_NUM_OF_PLAYERS; i++)
            {
                game_state.players[i].currentCenter = PADDLE_X_CENTER; // start with paddles in center of screen
                game_state.LEDScores[i] = 0;
            }
            game_state.LEDScores[0] = 0;
            game_state.LEDScores[1] = 0;
            game_state.players[0].color = PLAYER_BLUE;
            game_state.players[0].position = BOTTOM;
            game_state.players[1].color = PLAYER_RED;
            game_state.players[1].position = TOP;

            // draw init board (draw arena, players, and scores)


            color_screen(BACK_COLOR); // clear arena with back color
            DrawBoundary();
//            DrawScore();
            for(uint16_t i=0; i<MAX_NUM_OF_PLAYERS; i++)
            {
                DrawPlayer(&(game_state.players[i])); // put the player at the center of the screen upon starting game
            }


            G8RTOS_AddThread(MoveLEDs, 1, "MoveLed");
            G8RTOS_SignalSemaphore(&led_mutex);

            // add all threads back
            G8RTOS_AddThread(GenerateBall, 1, "GenBall");
            G8RTOS_AddThread(DrawObjects, 1, "DrawObj");
            G8RTOS_AddThread(ReadJoystickHost, 1, "JoyHost");
            //Once ready, send notification to client, with the reinit gamestate
            G8RTOS_AddThread(SendDataToClient, 1, "Tx2Client");
            G8RTOS_AddThread(ReceiveDataFromClient, 1, "RxFrmClient");
            G8RTOS_AddThread(idle_thread, 254, "idle");


        }
        else if(end_game_action == QUIT)
        {
            LP3943_LedModeSet(RED, 0);
            LP3943_LedModeSet(BLUE, 0);

            quit_screen_host(); // RUN QUIT GAME SCREEN
            game_state.gameDone = true;
            G8RTOS_AddThread(SendDataToClient, 1, "Tx2Client");
            G8RTOS_AddThread(idle_thread, 254, "idle");
        }
        // kill self
        end_game_action = NOTHING;
        G8RTOS_KillSelf();
        while(1);
    }
}

void quit_screen_host(void){



    uint8_t final_message_lose[] = "LOSER! :("; // 70, 10 for each char
    uint8_t final_message_win[] = "WINNER! :)"; // 70, 10 for each char



    G8RTOS_WaitSemaphore(&lcd_SPI);
    color_screen(BACK_COLOR);
    G8RTOS_SignalSemaphore(&lcd_SPI);

    if(winner == LCD_BLUE)
    {
        G8RTOS_WaitSemaphore(&lcd_SPI);
        LCD_Text((ARENA_MAX_X - ARENA_MIN_X - 50)/2, (ARENA_MAX_Y - ARENA_MIN_Y)/2, final_message_win, LCD_PINK);
        G8RTOS_SignalSemaphore(&lcd_SPI);    }
    else
    {
        G8RTOS_WaitSemaphore(&lcd_SPI);
        LCD_Text((ARENA_MAX_X - ARENA_MIN_X - 50)/2, (ARENA_MAX_Y - ARENA_MIN_Y)/2, final_message_lose, LCD_PINK);
        G8RTOS_SignalSemaphore(&lcd_SPI);
    }

}

void quit_screen_client(void){



//    uint8_t final_message[] = "Bye bye"; // 70, 10 for each char
//    uint8_t final_message[20];

    uint8_t final_message_lose[] = "LOSER! :("; // 70, 10 for each char
    uint8_t final_message_win[] = "WINNER! :)"; // 70, 10 for each char



    G8RTOS_WaitSemaphore(&lcd_SPI);
    color_screen(BACK_COLOR);
    G8RTOS_SignalSemaphore(&lcd_SPI);
    if(winner == LCD_BLUE)
    {
        G8RTOS_WaitSemaphore(&lcd_SPI);
        LCD_Text((ARENA_MAX_X - ARENA_MIN_X - 50)/2, (ARENA_MAX_Y - ARENA_MIN_Y)/2, final_message_lose, LCD_PINK);
        G8RTOS_SignalSemaphore(&lcd_SPI);    }
    else
    {
        G8RTOS_WaitSemaphore(&lcd_SPI);
        LCD_Text((ARENA_MAX_X - ARENA_MIN_X - 50)/2, (ARENA_MAX_Y - ARENA_MIN_Y)/2, final_message_win, LCD_PINK);
        G8RTOS_SignalSemaphore(&lcd_SPI);
    }
//    G8RTOS_WaitSemaphore(&lcd_SPI);
//    LCD_Text((ARENA_MAX_X - ARENA_MIN_X - 50)/2, (ARENA_MAX_Y - ARENA_MIN_Y)/2, final_message, LCD_PINK);
//    G8RTOS_SignalSemaphore(&lcd_SPI);
}

void color_screen(uint16_t winner_color){
    G8RTOS_WaitSemaphore(&lcd_SPI);
    LCD_DrawRectangle(ARENA_MIN_X, ARENA_MAX_X, ARENA_MIN_Y, ARENA_MAX_Y, winner_color); // make the game screen all one color
    G8RTOS_SignalSemaphore(&lcd_SPI);
}

void host_end_game_screen(void){
    uint8_t play_again_message[] = "Press B0 to play again!";// 230 10 for each char
    uint8_t quit_game_message[] = "Press B1 to quit!"; // 170 10 for each char

    color_screen(winner); // clear arena with color of winning player

    G8RTOS_WaitSemaphore(&lcd_SPI);
    LCD_Text((ARENA_MAX_X - ARENA_MIN_X - 90)/2, (ARENA_MAX_Y - ARENA_MIN_Y)/4, play_again_message, LCD_WHITE);
    G8RTOS_SignalSemaphore(&lcd_SPI);

    G8RTOS_WaitSemaphore(&lcd_SPI);
    LCD_Text((ARENA_MAX_X - ARENA_MIN_X - 50)/2, (ARENA_MAX_Y - ARENA_MIN_Y)/2 , quit_game_message, LCD_WHITE);
    G8RTOS_SignalSemaphore(&lcd_SPI);

}

void SendDataToClient(void) {
    while(1)
    {
        // package data for client
        G8RTOS_WaitSemaphore(&WiFi_mutex);
        {
            GameState_t game_state_to_send = game_state;

            SendData((uint8_t*)&game_state_to_send, GetClientIP(),
                     sizeof(game_state_to_send));

            extern uint_fast64_t num_packets_sent;
            num_packets_sent++;
        }
        G8RTOS_SignalSemaphore(&WiFi_mutex);

        if(game_state.winner == true && game_state.gameDone == false)
            G8RTOS_AddThread(EndOfGameHost, 1, "EndHost");

        if(game_state.gameDone == true)
        {
            G8RTOS_KillSelf();
        }

        sleep(5);
    }
}

void ReceiveDataFromClient(void) {
    while(1)
    {
        int32_t ret_val = NOTHING_RECEIVED;

        while(ret_val < 0)
        {
            sleep(1);

            G8RTOS_WaitSemaphore(&WiFi_mutex);
            {
                ret_val = ReceiveData((uint8_t *)&new_client_data,
                                      sizeof(new_client_data));

//                if(ret_val != NOTHING_RECEIVED)
//                {
//                    if(new_client_data.IP_address != GetClientIP())
//                    {
//                        ret_val = NOTHING_RECEIVED;
//                    }

//                    else
//                    {
//                        new_client_data_available = true;
//
//                        uint8_t confirmation[2] = { 3, 7 };
//                        SendData(confirmation, new_client_data.IP_address,
//                                 sizeof(confirmation));
//                    }
//                }
            }
            G8RTOS_SignalSemaphore(&WiFi_mutex);
        }
//        uint32_t PRIMASK_state = StartCriticalSection();
        {
            client_displacement = new_client_data.displacement; // update client player displacement received from the client

            extern uint_fast64_t num_packets_received;
            num_packets_received++;
        }
//        EndCriticalSection(PRIMASK_state);

        sleep(2);
    }
}



extern void JoinGame(void) {
    // Set initial SpecificPlayerInfo_t strict attributes (you can get the IP address by calling getLocalIP()
    client_player.IP_address = GetClientIP();
    client_player.displacement = 0;
    client_player.playerNumber = GetPlayerRole(); // TODO see if this works
    client_player.ready = true;
    client_player.joined = true;
    client_player.acknowledge = true;

    // init board state
    // should I init player? or nah since nothing has started
//    game_state.players = players;
    game_state.numberOfBalls = 0;
    // should I init ball array?
    for(uint16_t i=0; i<MAX_NUM_OF_BALLS; i++)
    {
        game_state.balls[i].currentCenterX = MAX_SCREEN_X/2;
        game_state.balls[i].currentCenterY = (VERT_CENTER_MAX_BALL - VERT_CENTER_MIN_BALL)/2;
        game_state.balls[i].color = LCD_WHITE;
        game_state.balls[i].alive = false;
        game_state.balls[i].kill_me = false;
    }

    game_state.winner = false;
    game_state.gameDone = false;
    for(uint16_t i=0; i<MAX_NUM_OF_PLAYERS; i++)
    {
        game_state.players[i].currentCenter = PADDLE_X_CENTER; // start with paddles in center of screen
        game_state.LEDScores[i] = 0;
    }
    game_state.overallScores[0] = 0;
    game_state.overallScores[1] = 0;
    game_state.LEDScores[0] = 0;
    game_state.LEDScores[1] = 0;
    game_state.players[0].color = PLAYER_BLUE;
    game_state.players[0].position = BOTTOM;
    game_state.players[1].color = PLAYER_RED;
    game_state.players[1].position = TOP;

    // draw init board (draw arena, players, and scores)
    DrawBoundary();
    DrawScore();

    for(uint16_t i=0; i<MAX_NUM_OF_PLAYERS; i++)
    {
        DrawPlayer(&(game_state.players[i])); // put the player at the center of the screen upon starting game
    }

    // init semaphores
    G8RTOS_InitSemaphore(&led_mutex, 0);
    // semaphore init shit
    // LED I2C mutex
    G8RTOS_InitSemaphore(&led_I2C, 0);

    // sensor I2C mutex
    G8RTOS_InitSemaphore(&sensor_I2C, 1);

    // LCD/Touchpad mutex
    G8RTOS_InitSemaphore(&lcd_SPI, 1);

    // Wifi mutex
    G8RTOS_InitSemaphore(&WiFi_mutex, 1); // can I move this to the other semaphore init area

    // Add threads: ReadJoystickClient, SendDataToHost, ReceiveDataFromHost, DrawObjects, MoveLEDs(lower priority), Idle
    G8RTOS_AddThread(ReadJoystickClient, 2, "JoyClient");
    G8RTOS_AddThread(SendDataToHost, 1, "Tx2Host");
    G8RTOS_AddThread(ReceiveDataFromHost, 1, "RxFrmHost");
    G8RTOS_AddThread(DrawObjects, 2, "DrawObj");
    G8RTOS_AddThread(MoveLEDs, 1, "MoveLed");
    G8RTOS_AddThread(idle_thread, 254, "idle");

    G8RTOS_KillSelf();
    while(1);
}

extern void ReadJoystickClient(void){
    int16_t displacement = 0;
    while(1)
    {
        GetJoystickCoordinates(&joystick_client_x_coor, &joystick_client_y_coor);
        // do we need to bias the value?

// too fast, create instability
//        displacement = joystick_client_x_coor / -4096;

        if(joystick_client_x_coor > 8000)
        {
            displacement = -1;
        }
        else if(joystick_client_x_coor > 100)
        {
            displacement = 0;
        }
        else if(joystick_client_x_coor > -1000)
        {
            displacement = 0;
        }
        else if(joystick_client_x_coor > -8000)
        {
            displacement = 1;
        }
        //should this go to self? or a pointer that decides what is client and host
//        game_state.players[1].currentCenter += displacement; // update the game state
        client_player.displacement = displacement; // also update the packet that will be sent over wifi
        sleep(10);
    }
}

void SendDataToHost(){
    while(1)
    {
        G8RTOS_WaitSemaphore(&WiFi_mutex);
        {
            // SpecificPlayerInfo_t client_player_to_send;
            //
            // uint32_t PRIMASK_state = StartCriticalSection();
            // {
            //     client_player_to_send = client_player;
            // }
            // EndCriticalSection(PRIMASK_state);

            SendData((uint8_t *)&client_player, HOST_IP_ADDR,
                     sizeof(client_player));

            extern uint_fast64_t num_packets_sent;
            num_packets_sent++;

        }
        G8RTOS_SignalSemaphore(&WiFi_mutex);
        sleep(2);
    }
}

void ReceiveDataFromHost() {
    GameState_t gamestate_from_host;
    while(1)
    {
        int32_t ret_val = NOTHING_RECEIVED;

        while(ret_val < 0)
        {

            G8RTOS_WaitSemaphore(&WiFi_mutex);
            {
                ret_val = ReceiveData((uint8_t *)&gamestate_from_host, // update game state
                                      sizeof(gamestate_from_host));
//                if(ret_val != NOTHING_RECEIVED)
//                {
//                    uint8_t confirmation[2] = { 3, 7 };
//                    SendData(confirmation, GetHostIP(),
//                             sizeof(confirmation));
//                }
            }
            G8RTOS_SignalSemaphore(&WiFi_mutex);

            //sleep(1);
        }

//        uint32_t PRIMASK_state = StartCriticalSection();
        {

            // check if game scores have changed
            if(game_state.LEDScores[0] != gamestate_from_host.LEDScores[0] || game_state.LEDScores[1] != gamestate_from_host.LEDScores[1])
            {
                // signal led mutex
                G8RTOS_SignalSemaphore(&led_mutex);
            }


//            if(game_state.overallScores[0] != gamestate_from_host.overallScores[0] || game_state.overallScores[1] != gamestate_from_host.overallScores[1])
//            {
//                // update game score global
//                DrawScore();
//            }

            game_state = gamestate_from_host;

            if(game_state.winner)
            {
                if(game_state.overallScores[0] > game_state.overallScores[1])
                {
                    // update game score global
                    winner = game_state.players[0].color;
                }
                else
                {
                    winner = game_state.players[1].color;
                }
                G8RTOS_AddThread(EndOfGameClient, 1, "EndGameClient");
            }

            extern uint_fast64_t num_packets_received;
            num_packets_received++;
        }
//        EndCriticalSection(PRIMASK_state);

        sleep(5);
    }
}

void EndOfGameClient() {
    while(1) // TODO: Actually write code for end state
    {
            DrawScore();


        // wait for all semaphores to be released
            G8RTOS_WaitSemaphore(&WiFi_mutex);
//            G8RTOS_WaitSemaphore(&led_mutex);
            G8RTOS_WaitSemaphore(&lcd_SPI);

        // kill all other threads
            G8RTOS_KillAllOtherThreads();

        // re-initialize semaphores
            G8RTOS_InitSemaphore(&WiFi_mutex, 1);
            G8RTOS_InitSemaphore(&led_mutex, 0);
            G8RTOS_InitSemaphore(&lcd_SPI, 1);

        // clear screen with winner's color
            client_end_game_screen();

            // TODO Should i turn on receive data thread?
            G8RTOS_AddThread(SendDataToHost, 1, "Tx2Host");
            G8RTOS_AddThread(ReceiveDataFromHost, 1, "RxFrmHost");

            // wait for host to restart game
            while(game_state.winner == true && game_state.gameDone == false);

            if(game_state.winner == false && game_state.gameDone == false)
            {
                // reset game variables
                client_player.displacement = 0;
                // draw init board (draw arena, players, and scores)
                color_screen(BACK_COLOR); // clear arena with back color
                DrawBoundary();
                G8RTOS_AddThread(MoveLEDs, 1, "MoveLed");
                G8RTOS_SignalSemaphore(&led_mutex);

                for(uint16_t i=0; i<MAX_NUM_OF_PLAYERS; i++)
                {
                    DrawPlayer(&(game_state.players[i])); // put the player at the center of the screen upon starting game
                }

                // add all threads back
                G8RTOS_AddThread(ReadJoystickClient, 1, "JoyClient");
                G8RTOS_AddThread(DrawObjects, 1, "DrawObj");
                G8RTOS_AddThread(idle_thread, 254, "idle");
            }
            else if(game_state.gameDone == true && game_state.winner == true)
            {

                LP3943_LedModeSet(RED, 0);
                LP3943_LedModeSet(BLUE, 0);

                // wait for all semaphores to be released
//                G8RTOS_WaitSemaphore(&WiFi_mutex);
//                G8RTOS_WaitSemaphore(&lcd_SPI);
                // kill all other threads
                G8RTOS_KillAllOtherThreads(); // kill the wifi communication
                quit_screen_client(); // RUN QUIT GAME SCREEN
                G8RTOS_AddThread(idle_thread, 254, "idle");
            }

        // kill self
        G8RTOS_KillSelf();
    }
}

void client_end_game_screen(){
    uint8_t wait_for_host_message[] = "Waiting for host...";// 230 10 for each char
//    uint8_t quit_game_message[] = "Press B1 to quit!"; // 170 10 for each char

    color_screen(winner); // clear arena with color of winning player

    G8RTOS_WaitSemaphore(&lcd_SPI);
    LCD_Text((ARENA_MAX_X - ARENA_MIN_X - 120)/2, (ARENA_MAX_Y - ARENA_MIN_Y)/4, wait_for_host_message, LCD_WHITE);
    G8RTOS_SignalSemaphore(&lcd_SPI);
}



void end_game_button_press(void) {
    if(check_end_game_buttons)
    {
        if(P4->IFG & BIT4)
        {
            end_game_action = RESTART;
        }
        else if(P4->IFG & BIT5)
        {
            end_game_action = QUIT;
        }
        check_end_game_buttons = false;
        P4->IE &= ~(BIT4 | BIT5);
        P4->IFG &= ~(BIT4 | BIT5); // clear flag
    }
}

extern void button_press(void) {
    if(waiting_on_player_type_designation)
    {
        if(P4->IFG & BIT4)
        {
            player_type = Host;
            waiting_on_player_type_designation = false;
        }
        else if(P4->IFG & BIT5)
        {
            player_type = Client;
            waiting_on_player_type_designation = false;
        }
    }

    P4->IE &= ~(BIT4 | BIT5);
    P4->IFG &= ~(BIT4 | BIT5); // clear flag
}

inline uint16_t random_color(void) {
    uint8_t color_index = rand()%10;

    switch(color_index)
    {
    case  0:
        return LCD_WHITE;
    case  1:
        return LCD_BLUE;
    case  2:
        return LCD_RED;
    case  3:
        return LCD_MAGENTA;
    case  4:
        return LCD_GREEN;
    case  5:
        return LCD_CYAN;
    case  6:
        return LCD_YELLOW;
    case  7:
        return LCD_PURPLE;
    case  8:
        return LCD_ORANGE;
    case  9:
        return LCD_PINK;
    case 10:
    default:
        return LCD_OLIVE;
    }
}
