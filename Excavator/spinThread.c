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
int spin = 0, i = 0;
    //When spin = 60, max speed, PHASE1 = 750
    //When spin is around 0, PHASE1 = 5,000
    while(1)
    {
        for(i = 0; i < 35; i++)
        {
            if(rxval[i] == 'o')
            {
                spin = charToInt(rxval[i+1], rxval[i+2], rxval[i+3]);
                break;
            }
        }
        if(spin >= 0)
        {
            LATAbits.LATA4 = 1;     //Forward
        }
        else if(spin < 0)
        {
            LATAbits.LATA4 = 0;     //Reverse
            spin *= -1;        //We only want positive magnitudes
        }
        if(spin < 20)
        {
            PHASE3 = 1000;
            PDC3 = 0;
        }
        else
        {
            PHASE3 = 5000 - (spin*40);
            PDC3 = PHASE3/2;
        }
    } 
}