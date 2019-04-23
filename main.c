#include "msp.h"
#include "BSP.h"
#include "LCDLib.h"
#include "cc3100_usage.h"
#include "Game.h"
#include <driverlib.h>
#include <BSP.h>
#include <core_cm4.h>
#include <stdint.h>
#include <stdbool.h>
#include "G8RTOS.h"


uint_fast64_t num_packets_sent = 0;
uint_fast64_t num_packets_received = 0;

void main(void) {


//	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

    G8RTOS_Init();

    AcquirePlayerRole();

    // Perform Client-Host Handshake
    {
        if(GetPlayerRole() == Host)
        {
            extern uint32_t client_IP;
            while(ReceiveData((uint8_t *)&client_IP, sizeof(client_IP)) == NOTHING_RECEIVED);
            // Use an LED on the Launchpad to indicate Wi-Fi connection
            P2->DIR |= BIT1; // init P2.1(green) as output
            P2->OUT = 0x00; //clear led
            BITBAND_PERI(P2->OUT,1) ^= 1;

            uint64_t ack = 0x37;
            SendData((uint8_t *)&ack, client_IP, sizeof(ack));

            Delay(2);
            uint64_t ack2 = 0;
            while(ReceiveData((uint8_t *)&ack2, sizeof(ack2)) == NOTHING_RECEIVED);
            if(ack2 == 37)
            {
                LCD_Text(2, 2, "Client-Host Handshake Success", LCD_WHITE);
            }
            else
            {
                char ack2_string[64];
                sprintf(ack2_string, "Received: 0x%x", ack2);
                LCD_Text(2,  2, "Client-Host Handshake Failure", LCD_WHITE);
                LCD_Text(2, 18, ack2_string, LCD_WHITE);

                while(1);
            }
        }
        else if(GetPlayerRole() == Client)
        {
            uint32_t client_IP = GetClientIP();
            SendData((uint8_t *)&client_IP, GetHostIP(), sizeof(client_IP));

            Delay(2);
            uint64_t ack = 0;
            while(ReceiveData((uint8_t *)&ack, sizeof(ack)) == NOTHING_RECEIVED);

            if(ack == 0x37)
            {
                LCD_Text(2, 2, "Client-Host Handshake Success", LCD_WHITE);

                uint64_t ack2 = 37; // non-hex 37 this time
                SendData((uint8_t *)&ack2, GetHostIP(), sizeof(ack2));
                // if you  have joined the game, acknowledge you've joined to the host and show connection with an LED
                P1->DIR |=  BIT0; // init P1.0 (RED) as output
                P1->OUT = 0x00; // clear led
                BITBAND_PERI(P1->OUT, 0) ^= 1;

            }
            else
            {
                char ack_string[64];
                sprintf(ack_string, "Received: 0x%x", ack);
                LCD_Text(2,  2, "Client-Host Handshake Failure", LCD_WHITE);
                LCD_Text(2, 18, ack_string, LCD_WHITE);

                while(1);
            }
        }

        Delay(1000);

        LCD_Clear(BACK_COLOR);
    }


    if(GetPlayerRole() == Host)
    {
        G8RTOS_AddThread(CreateGame, 1, "CreateGame");
    }
    else //if(GetPlayerRole() == Client)
    {
        G8RTOS_AddThread(JoinGame, 1, "JoinGame");
    }

    G8RTOS_Launch();
    PCM_gotoLPM0();


}
