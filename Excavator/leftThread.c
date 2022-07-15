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
    int leftTrack = 0, i = 0;
    int leftTrackPrev = 0, leftTrackZero = 0;
    int samples = 0;
 //   PHASE2 = 2000;
  //  PDC2 = PHASE2/2;
    //When leftTrack = 200 or -200, max speed, PHASEx = 1,000
    //When -50 < leftTrack < 50 is around 0, PHASEx = 0;
    //When motor first turns on, PHASEx = 10,000
    //
    while(1)
    {
        if(samples == SAMPLE_RATE)
        {
            leftTrackZero = 0;
            for(i = 0; i < 95; i++)
            {
                if(rxval[i] == 'l')
                {

                    leftTrack = charToInt(rxval[i+1], rxval[i+2], rxval[i+3], rxval[i+4]);
                    if(leftTrack == 0)
                    {
                        leftTrackZero = 1;
                    }
                    break;
                }
            }
            if(leftTrack > 0)
            {
                LATAbits.LATA1 = 1;     //Forward
            }
            else if(leftTrack < 0)
            {
                LATAbits.LATA1 = 0;     //Reverse
                leftTrack *= -1;        //We only want positive magnitudes
            }
            /*

            if(leftTrack < 40 && leftTrackZero != 1)
            {
                PHASE2 = 1000;
                PDC2 = 0;
            }
 */
            if(leftTrack > 200)
            {
                leftTrack = 200;
            }
            if(leftTrackZero != 1 && abs(leftTrack - leftTrackPrev) > 20) 
            {
                PHASE2 = 11000 - (leftTrack*40);
                PDC2 = PHASE2/2;
                delay(50000);
            }          
            leftTrackPrev = leftTrack;
            samples = 0;
        }
        samples++;
    }    
}
