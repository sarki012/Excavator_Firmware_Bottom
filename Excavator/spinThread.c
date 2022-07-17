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

volatile extern char rxval[60];
void spinThread( void *pvParameters )
{
    int pixelsX = 0, i = 0;
    int zeroVal = 0;
    int samples = 0;
    int stopSending = 0;
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
                if(rxval[i] == 'o')
                {
                    pixelsX = charToInt(rxval[i+1], rxval[i+2], rxval[i+3], rxval[i+4]);
                    if(pixelsX == 0)
                    {
                        zeroVal = 1;
                    }
                    stopSending = 0;
                    break;
                }
                if(rxval[i] == '#')
                {
                    stopSending = 1;
                    break;
                }
            }
            if(pixelsX > 0 && stopSending == 0)
            {
                LATAbits.LATA4 = 1;     //Forward
            }
            else if(pixelsX < 0 && stopSending == 0)
            {
                LATAbits.LATA4 = 0;     //Reverse
                pixelsX *= -1;        //We only want positive magnitudes
            }
            
            if(pixelsX < 75 && zeroVal != 1 && stopSending == 0)
            {
                PHASE3 = 1000;
                PDC3 = 0;
            }
            else if(zeroVal != 1 && stopSending == 0) 
            {
                PHASE3 = 6000 - (pixelsX*7);
                PDC3 = PHASE2/2;
                delay(500);
            }          
            samples = 0;
        }
        samples++;
    }    
}
