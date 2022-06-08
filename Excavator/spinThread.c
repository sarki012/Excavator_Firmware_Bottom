/*
 * File:   stickTask.c
 * Author: Erik
 *
 * Created on April 29, 2022, 4:43 PM
 */
#include <xc.h>
#include <p33ep512mc502.h>
#include "FreeRTOS.h"
#include "task.h"
#include "FreeRTOSConfig.h"
#include "main.h"

volatile extern char rxval[40];
void spinThread( void *pvParameters )
{
    LATAbits.LATA4 = 1;         //Spin direction
    PHASE3 = 1000;
    PDC3 = PHASE3/2;
    while(1)
    {
        /*
        for(i = 0; i < 35; i++)
        {
            if(rxval[i] == 'b')
            {
                //boom = charToInt(rxval[i+1], rxval[i+2], rxval[i+3]);
            }
        }
         */ 
    }    
}