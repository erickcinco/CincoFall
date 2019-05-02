#include "msp.h"
#include "BSP.h"
#include "LCDLib.h"
#include "cc3100_usage.h"
#include "Game.h"
#include <driverlib.h>
#include <core_cm4.h>
#include "fighter_cat_gif_color_array_no_background.h"
//#include "fighter_cat_gif_color_array.h"
#include "G8RTOS.h"


//#include "cat_color_array.h"
//#include "parrot_color_array.h"
//#include "dog_color_array.h"

uint_fast64_t num_packets_sent = 0;
uint_fast64_t num_packets_received = 0;
#define REFRESH 40

void main(void) {
    G8RTOS_Init();
    LCD_Clear(BACK_COLOR);
//    LCD_Draw_Sprite(0, CAT_SPRITE_WIDTH, 0, CAT_SPRITE_HEIGHT, cat_color_array);
//    LCD_Draw_Sprite(100, 100 + PARROT_SPRITE_WIDTH, 100, 100 + PARROT_SPRITE_HEIGHT, parrot_color_array);
//    LCD_Draw_Sprite(MAX_SCREEN_X/2, MAX_SCREEN_X/2 + DOG_SPRITE_WIDTH, MAX_SCREEN_Y/2, MAX_SCREEN_Y/2 + DOG_SPRITE_HEIGHT, dog_color_array);

    while(1){
        LCD_Draw_Sprite(MAX_SCREEN_X/2, MAX_SCREEN_X/2 + FIGHTER_CAT_SPRITE_WIDTH, MAX_SCREEN_Y/2, MAX_SCREEN_Y/2 + FIGHTER_CAT_SPRITE_HEIGHT, fighter_cat_gif_color_array_frame_0);
        Delay(REFRESH);
        LCD_Draw_Sprite(MAX_SCREEN_X/2, MAX_SCREEN_X/2 + FIGHTER_CAT_SPRITE_WIDTH, MAX_SCREEN_Y/2, MAX_SCREEN_Y/2 + FIGHTER_CAT_SPRITE_HEIGHT, fighter_cat_gif_color_array_frame_1);
        Delay(REFRESH);
        LCD_Draw_Sprite(MAX_SCREEN_X/2, MAX_SCREEN_X/2 + FIGHTER_CAT_SPRITE_WIDTH, MAX_SCREEN_Y/2, MAX_SCREEN_Y/2 + FIGHTER_CAT_SPRITE_HEIGHT, fighter_cat_gif_color_array_frame_2);
        Delay(REFRESH);
        LCD_Draw_Sprite(MAX_SCREEN_X/2, MAX_SCREEN_X/2 + FIGHTER_CAT_SPRITE_WIDTH, MAX_SCREEN_Y/2, MAX_SCREEN_Y/2 + FIGHTER_CAT_SPRITE_HEIGHT, fighter_cat_gif_color_array_frame_5);
        Delay(REFRESH);
        LCD_Draw_Sprite(MAX_SCREEN_X/2, MAX_SCREEN_X/2 + FIGHTER_CAT_SPRITE_WIDTH, MAX_SCREEN_Y/2, MAX_SCREEN_Y/2 + FIGHTER_CAT_SPRITE_HEIGHT, fighter_cat_gif_color_array_frame_6);
        Delay(REFRESH);
//        LCD_Draw_Sprite(MAX_SCREEN_X/2, MAX_SCREEN_X/2 + FIGHTER_CAT_SPRITE_WIDTH, MAX_SCREEN_Y/2, MAX_SCREEN_Y/2 + FIGHTER_CAT_SPRITE_HEIGHT, fighter_cat_gif_color_array_frame_7);
//        Delay(REFRESH);
//        LCD_Draw_Sprite(MAX_SCREEN_X/2, MAX_SCREEN_X/2 + FIGHTER_CAT_SPRITE_WIDTH, MAX_SCREEN_Y/2, MAX_SCREEN_Y/2 + FIGHTER_CAT_SPRITE_HEIGHT, fighter_cat_gif_color_array_frame_8);
//        Delay(REFRESH);
    }

//
//    AcquirePlayerRole();
//
//    // Perform Client-Host Handshake
//    {
//        if(GetPlayerRole() == Host)
//        {
//            extern uint32_t client_IP;
//            while(ReceiveData((uint8_t *)&client_IP, sizeof(client_IP)) == NOTHING_RECEIVED);
//            // Use an LED on the Launchpad to indicate Wi-Fi connection
//            P2->DIR |= BIT1; // init P2.1(green) as output
//            P2->OUT = 0x00; //clear led
//            BITBAND_PERI(P2->OUT,1) ^= 1;
//
//            uint64_t ack = 0x37;
//            SendData((uint8_t *)&ack, client_IP, sizeof(ack));
//
//            Delay(2);
//            uint64_t ack2 = 0;
//            while(ReceiveData((uint8_t *)&ack2, sizeof(ack2)) == NOTHING_RECEIVED);
//            if(ack2 == 37)
//            {
//                LCD_Text(2, 2, "Client-Host Handshake Success", LCD_WHITE);
//            }
//            else
//            {
//                char ack2_string[64];
//                sprintf(ack2_string, "Received: 0x%x", ack2);
//                LCD_Text(2,  2, "Client-Host Handshake Failure", LCD_WHITE);
//                LCD_Text(2, 18, ack2_string, LCD_WHITE);
//
//                while(1);
//            }
//        }
//        else if(GetPlayerRole() == Client)
//        {
//            uint32_t client_IP = GetClientIP();
//            SendData((uint8_t *)&client_IP, GetHostIP(), sizeof(client_IP));
//
//            Delay(2);
//            uint64_t ack = 0;
//            while(ReceiveData((uint8_t *)&ack, sizeof(ack)) == NOTHING_RECEIVED);
//
//            if(ack == 0x37)
//            {
//                LCD_Text(2, 2, "Client-Host Handshake Success", LCD_WHITE);
//
//                uint64_t ack2 = 37; // non-hex 37 this time
//                SendData((uint8_t *)&ack2, GetHostIP(), sizeof(ack2));
//                // if you  have joined the game, acknowledge you've joined to the host and show connection with an LED
//                P1->DIR |=  BIT0; // init P1.0 (RED) as output
//                P1->OUT = 0x00; // clear led
//                BITBAND_PERI(P1->OUT, 0) ^= 1;
//
//            }
//            else
//            {
//                char ack_string[64];
//                sprintf(ack_string, "Received: 0x%x", ack);
//                LCD_Text(2,  2, "Client-Host Handshake Failure", LCD_WHITE);
//                LCD_Text(2, 18, ack_string, LCD_WHITE);
//
//                while(1);
//            }
//        }
//
//        Delay(1000);
//
//        LCD_Clear(BACK_COLOR);
//    }
//
//
//    if(GetPlayerRole() == Host)
//    {
//        G8RTOS_AddThread(CreateGame, 1, "CreateGame");
//    }
//    else //if(GetPlayerRole() == Client)
//    {
//        G8RTOS_AddThread(JoinGame, 1, "JoinGame");
//    }
//
//    G8RTOS_Launch();
    PCM_gotoLPM0();


}
