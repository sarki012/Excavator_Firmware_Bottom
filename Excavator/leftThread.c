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
    //When leftTrack = 60, max speed, PHASE1 = 750
    //When leftTrack is around 0, PHASE1 = 5,000
    while(1)
    {
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
        else if(leftTrack < 0)
        {
            LATAbits.LATA0 = 0;     //Reverse
            leftTrack *= -1;        //We only want positive magnitudes
        }
        if(leftTrack < 15)
        {
            PHASE1 = 1000;
            PDC1 = 0;
        }
        else
        {
            PHASE1 = 5000 - (leftTrack*40);
            PDC1 = PHASE1/2;
        }
    }    
}
