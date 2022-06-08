#include <xc.h>
#include <p33ep512mc502.h>
#include "FreeRTOS.h"
#include "task.h"
#include "FreeRTOSConfig.h"
#include "main.h"

volatile extern char rxval[40];
void rightThread( void *pvParameters )
{
    LATAbits.LATA1 = 1;         //Right direction
    PHASE2 = 1000;
    PDC2 = PHASE2/2;
    while(1)
    {

        /*
        for(i = 0; i < 35; i++)
        {
            if(rxval[i] == 'b')
            {
                //boom = charToInt(rxval[i+1], rxval[i+2], rxval[i+3]);
            }
        }
         */ 
    }    
}