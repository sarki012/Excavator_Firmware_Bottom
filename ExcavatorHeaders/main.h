// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef MAIN_H
#define	MAIN_H

#include <xc.h> // include processor files - each processor file is guarded. 
#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "croutine.h"
#include "FreeRTOSConfig.h"

#define SAMPLE_RATE 100      //Was 20
#define LOOPS 2000      //Tried 2000. Too fast. Last try: 2500. Ok but still too fast. Last try 3000
#define LOOPS_CURL 1500     //Want the curl to be faster
void init(void);
void leftThread(void *pvParameters);
void rightThread(void *pvParameters);
void spinThread(void *pvParameters);
int charToInt(char digit3, char digit2, char digit1, char digit0);

#endif	/* XC_HEADER_TEMPLATE_H */

