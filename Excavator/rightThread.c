#include <xc.h>
#include <p33ep512mc502.h>
#include "FreeRTOS.h"
#include "task.h"
#include "FreeRTOSConfig.h"
#include "main.h"

volatile extern char rxval[40];
void rightThread( void *pvParameters )
{
    int rightTrack = 0, i = 0;
    //When rightTrack = 60, max speed, PHASE1 = 750
    //When rightTrack is around 0, PHASE1 = 5,000
    while(1)
    {
        for(i = 0; i < 35; i++)
        {
            if(rxval[i] == 'r')
            {
                rightTrack = charToInt(rxval[i+1], rxval[i+2], rxval[i+3]);
                break;
            }
        }
        if(rightTrack >= 0)
        {
            LATAbits.LATA1 = 1;     //Forward
        }
        else if(rightTrack < 0)
        {
            LATAbits.LATA1 = 0;     //Reverse
            rightTrack *= -1;        //We only want positive magnitudes
        }
        if(rightTrack < 15)
        {
            PHASE2 = 1000;
            PDC2 = 0;
        }
        else
        {
            PHASE2 = 5000 - (rightTrack*40);
            PDC2 = PHASE2/2;
        }
    }     
}