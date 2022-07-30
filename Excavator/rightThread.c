/*
 * File:   rightThread.c
 * Author: Erik Sarkinen
 *
 * Created on April 29, 2022, 4:43 PM
 */

#include <xc.h>
#include <p33ep512mc502.h>
#include "FreeRTOS.h"
#include "task.h"
#include "FreeRTOSConfig.h"
#include "main.h"

volatile extern char rxval[100];    //The UART receive array which holds the data sent 
                                    //via radio from the top circuit board
void rightThread( void *pvParameters )
{
    int pixelsY = 0;
    int i = 0;
    int samples = 0;
    int stopSending = 0;
    while(1)
    {
        if(samples == SAMPLE_RATE)  //We're only going to update the motors every SAMPLE_RATE loops
        {
            for(i = 0; i < 95; i++)
            {
                if(rxval[i] == '@')     //The tablet sends a '@' when the thumb is lifted
                {
                    stopSending = 1;
                    break;
                }
                else if(rxval[i] == 'r')
                {
                    //This means the next character is a +/- followed by three characters
                    //which are passed to charToInt which returns an integer value
                    //pixelsY holds the pixels in the y-direction from the left track slider
                    pixelsY = charToInt(rxval[i+1], rxval[i+2], rxval[i+3], rxval[i+4]);
                    stopSending = 0;
                    break;
                }
            }
            if(pixelsY > 0  && stopSending == 0)     //Thumb press is in the positive direction
            {
                LATAbits.LATA0 = 1;     //Forward
            }
            else if(pixelsY < 0 && stopSending == 0)    //Thumb press is in the negative direction
            {
                LATAbits.LATA0 = 0;     //Reverse
                pixelsY *= -1;        //We only want positive magnitudes
            }
            if(pixelsY < 150 || stopSending == 1)       //Thumb is near the origin or thumb is lifted
            {
                PHASE1 = 1000;
                PDC1 = 0;           //Turn the motor off
            }
            else if(pixelsY >= 150 && stopSending == 0) 
            {
                PHASE1 = 10000 - (pixelsY*16);      //Set the PWM period
                PDC1 = PHASE1/2;                    //Duty cycle is arbitrary
                delay(500);
            }  
            samples = 0;
        }
        samples++;
    }    
}
