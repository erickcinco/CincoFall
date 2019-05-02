/*
 * Game.c
 *
 *  Created on: Apr 14, 2019
 *      Author: Damien
 */
#include "Game.h"
#include <driverlib.h>
#include <stdio.h>

static playerType player_role;
uint32_t client_IP = 0; // 0-initialized

void AcquirePlayerRole() {
    LCD_Text(2,  2, "B0: Become Host",   LCD_RED);
    LCD_Text(2, 18, "B1: Become Client", LCD_RED);

    P4->OUT &= ~(BIT4 + BIT5);
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P4, GPIO_PIN4);
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P4, GPIO_PIN5);

    while(1)
    {
        static bool B0_debouce = false;
        static bool B1_debouce = false;

        Delay(10);

        if(!(P4->IN & BIT4))
        {
            if(B0_debouce)
            {
                player_role = Host;
                break;
            }
            else
            {
                B0_debouce = true;
            }
        }
        else
        {
            B0_debouce = false;
        }

        if(!(P4->IN & BIT5))
        {
            if(B1_debouce)
            {
                player_role = Client;
                break;
            }
            else
            {
                B1_debouce = true;
            }
        }
        else
        {
            B1_debouce = false;
        }
    }
    LCD_Clear(BACK_COLOR);

//    initCC3100(player_role);
    // Need to send data immediately afterwards to get the board working
    // properly any time after this.
    uint8_t junk = 0xff;
//    SendData((uint8_t *)&junk, 0, sizeof(junk));

    if(player_role == Host)
        LCD_Text(2, 2, "Host", LCD_WHITE);
    else if(player_role == Client)
    {
        LCD_Text(2, 2, "Client", LCD_WHITE);
        client_IP = getLocalIP();
    }

//    uint32_t local_IP = getLocalIP();
//    char ip_addr[16];
//    sprintf(ip_addr, "%d.%d.%d.%d", local_IP>>24 & 0xff, local_IP>>16 & 0xff,
//            local_IP>>8 & 0xff, local_IP & 0xff);
//
//    LCD_Text(2, 18, ip_addr, LCD_WHITE);
    Delay(1500);

    LCD_Clear(BACK_COLOR);
}

playerType GetPlayerRole() {
    return player_role;
}

uint32_t GetClientIP() {
    return client_IP;
}

uint32_t GetHostIP() {
    return HOST_IP_ADDR;
}
