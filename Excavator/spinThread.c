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
    int spin = 0, i = 0;
    int spinZero = 0;
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
            spinZero = 0;
            for(i = 0; i < 95; i++)
            {
                if(rxval[i] == 'o')
                {
                    spin = charToInt(rxval[i+1], rxval[i+2], rxval[i+3], rxval[i+4]);
                    if(spin == 0)
                    {
                        spinZero = 1;
                    }
                    break;
                }
            }
            if(spin > 0)
            {
                LATAbits.LATA4 = 1;     //Forward
            }
            else if(spin < 0)
            {
                LATAbits.LATA4 = 0;     //Reverse
                spin *= -1;        //We only want positive magnitudes
            }

            if(spin < 40 && spinZero != 1)
            {
                PHASE3 = 1000;
                PDC3 = 0;
            }
 
            else if(spinZero != 1)
            {
                PHASE3 = 11000 - (spin*15);
                PDC3 = PHASE2/2;
                delay(5000);
            }          
            samples = 0;
        }
        samples++;
    }    
}