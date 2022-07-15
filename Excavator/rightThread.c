#include <xc.h>
#include <p33ep512mc502.h>
#include "FreeRTOS.h"
#include "task.h"
#include "FreeRTOSConfig.h"
#include "main.h"

volatile extern char rxval[100];
void rightThread( void *pvParameters )
{
    int rightTrack = 0, i = 0;
    int rightTrackZero = 0;
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
            rightTrackZero = 0;
            for(i = 0; i < 95; i++)
            {
                if(rxval[i] == 'r')
                {

                    rightTrack = charToInt(rxval[i+1], rxval[i+2], rxval[i+3], rxval[i+4]);
                    if(rightTrack == 0)
                    {
                        rightTrackZero = 1;
                    }
                    break;
                }
            }
            if(rightTrack > 0)
            {
                LATAbits.LATA0 = 1;     //Forward
            }
            else if(rightTrack < 0)
            {
                LATAbits.LATA0 = 0;     //Reverse
                rightTrack *= -1;        //We only want positive magnitudes
            }

            if(rightTrack < 40 && rightTrackZero != 1)
            {
                PHASE1 = 1000;
                PDC1 = 0;
            }
 
            else if(rightTrackZero != 1) 
            {
                PHASE1 = 11000 - (rightTrack*45);
                PDC1 = PHASE1/2;
                delay(5000);
            }          
            samples = 0;
        }
        samples++;
    }   
}