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
#include <stdio.h>
#include <stdlib.h>
volatile extern char rxval[100];
void leftThread( void *pvParameters )
{
    int pixelsY = 0, i = 0;
    int zeroVal = 0;
    int samples = 0;
    int stopSending = 0;
 //   PHASE2 = 2000;
  //  PDC2 = PHASE2/2;
    //When leftTrack = 300 or -300, max speed, PHASEx = 1,000
    //When -75 < leftTrack < 75 is around 0, PHASEx = 0;
    //When motor first turns on, PHASEx = 6,000
    //
    while(1)
    {
        if(samples == SAMPLE_RATE)
        {
            zeroVal= 0;
            for(i = 0; i < 95; i++)
            {
                if(rxval[i] == 'l')
                {
                    pixelsY = charToInt(rxval[i+1], rxval[i+2], rxval[i+3], rxval[i+4]);
                    if(pixelsY == 0)
                    {
                        zeroVal = 1;
                    }
                    stopSending = 0;
                    break;
                }
                if(rxval[i] == '$')
                {
                    stopSending = 1;
                    break;
                }
            }
            if(pixelsY > 0 && stopSending == 0)
            {
                LATAbits.LATA1 = 0;     //Forward
            }
            else if(pixelsY < 0 && stopSending == 0)
            {
                LATAbits.LATA1 = 1;     //Reverse
                pixelsY *= -1;        //We only want positive magnitudes
            }
            
            if(pixelsY < 75 && zeroVal != 1 && stopSending == 0)
            {
                PHASE2 = 1000;
                PDC2 = 0;
            }
            else if(zeroVal != 1 && stopSending == 0) 
            {
                PHASE2 = 6000 - (pixelsY*16);
                PDC2 = PHASE2/2;
                delay(500);
            }          
            samples = 0;
        }
        samples++;
    }    
}
