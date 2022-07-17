#include <xc.h>
#include <p33ep512mc502.h>
#include "FreeRTOS.h"
#include "task.h"
#include "FreeRTOSConfig.h"
#include "main.h"

volatile extern char rxval[100];
void rightThread( void *pvParameters )
{
    int pixelsY = 0, i = 0;
    int zeroVal = 0;
    int samples = 0;
    int stopSending = 0;
    //When leftTrack = 300 or -300, max speed, PHASEx = 1,000
    //When -75 < leftTrack < 75 is around 0, PHASEx = 0;
    //When motor first turns on, PHASEx = 6,000
    while(1)
    {
        if(samples == SAMPLE_RATE)
        {
            zeroVal= 0;
            for(i = 0; i < 95; i++)
            {
                if(rxval[i] == 'r')
                {
                    pixelsY = charToInt(rxval[i+1], rxval[i+2], rxval[i+3], rxval[i+4]);
                    if(pixelsY == 0)
                    {
                        zeroVal = 1;
                    }
                    stopSending = 0;
                    break;
                }
                if(rxval[i] == '@')
                {
                    stopSending = 1;
                    break;
                }
            }
            if(pixelsY > 0 && stopSending == 0)
            {
                LATAbits.LATA0 = 1;     //Forward
            }
            else if(pixelsY < 0 && stopSending == 0)
            {
                LATAbits.LATA0 = 0;     //Reverse
                pixelsY *= -1;        //We only want positive magnitudes
            }
            
            if(pixelsY < 75 && zeroVal != 1 && stopSending == 0)
            {
                PHASE1 = 1000;
                PDC1 = 0;
            }
            else if(zeroVal != 1 && stopSending == 0) 
            {
                PHASE1 = 6000 - (pixelsY*16);
                PDC1 = PHASE1/2;
                delay(500);
            }          
            samples = 0;
        }
        samples++;
    }    
}
