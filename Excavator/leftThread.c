/*
 * File:   boom_task.c
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
void leftThread( void *pvParameters )
{
    int leftTrack = 0, i = 0;
    //When leftTrack = 60, max speed, PHASE1 = 500
    //When leftTrack is around 0, PHASE1 = 10,000
    LATAbits.LATA0 = 1;         //Left direction
    PHASE1 = 1000;
    PDC1 = PHASE1/2;
    while(1)
    {

        /*
        for(i = 0; i < 35; i++)
        {
            if(rxval[i] == 'l')
            {
                leftTrack = charToInt(rxval[i+1], rxval[i+2], rxval[i+3]);
                break;
            }
        }
        if(leftTrack >= 0)
        {
            LATAbits.LATA0 = 1;     //Forward
        }
        else
        {
            LATAbits.LATA0 = 0;     //Reverse
            leftTrack *= -1;        //We only want positive magnitudes
        }
         * */
       // PHASE1 = 2000;
       // PHASE1 = 10000 - (leftTrack*126);
       // PDC1 = PHASE1/2;
    }    
}
