/*
 * File:   delay.c
 * Author: Erik Sarkinen
 *
 * Created on July 11th, 2022, 11:48 PM
 */
#include <xc.h>
#include <p33ep512mc502.h>
#include "FreeRTOS.h"
#include "task.h"
#include "FreeRTOSConfig.h"
#include "main.h"

void delay(int numLoops)
{
    int i = 0, j = 0;
    for(i = 0; i < numLoops; i++)
    {
        for(j = 0; j < 100; j++);
    }
    return;
}
