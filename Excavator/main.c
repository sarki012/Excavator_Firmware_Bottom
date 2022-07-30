/*
 * File:   main.c
 * Author: Erik Sarkinen
 *
 * Created on May 5, 2022, 1:46 PM
 */

// 'C' source line config statements

// FICD
#pragma config ICS = PGD1               // ICD Communication Channel Select bits (Communicate on PGEC1 and PGED1)
#pragma config JTAGEN = OFF             // JTAG Enable bit (JTAG is disabled)

// FPOR
#pragma config ALTI2C1 = OFF            // Alternate I2C1 pins (I2C1 mapped to SDA1/SCL1 pins)
#pragma config ALTI2C2 = OFF            // Alternate I2C2 pins (I2C2 mapped to SDA2/SCL2 pins)
#pragma config WDTWIN = WIN25           // Watchdog Window Select bits (WDT Window is 25% of WDT period)

// FWDT
#pragma config WDTPOST = PS32768        // Watchdog Timer Postscaler bits (1:32,768)
#pragma config WDTPRE = PR128           // Watchdog Timer Prescaler bit (1:128)
#pragma config PLLKEN = ON              // PLL Lock Enable bit (Clock switch to PLL source will wait until the PLL lock signal is valid.)
#pragma config WINDIS = OFF             // Watchdog Timer Window Enable bit (Watchdog Timer in Non-Window mode)
#pragma config FWDTEN = OFF             // Watchdog Timer Enable bit (Watchdog timer enabled/disabled by user software)

// FOSC
#pragma config POSCMD = NONE            // Primary Oscillator Mode Select bits (Primary Oscillator disabled)
#pragma config OSCIOFNC = ON            // OSC2 Pin Function bit (OSC2 is general purpose digital I/O pin)
#pragma config IOL1WAY = OFF            // Peripheral pin select configuration (Allow multiple reconfigurations)
#pragma config FCKSM = CSDCMD           // Clock Switching Mode bits (Both Clock switching and Fail-safe Clock Monitor are disabled)

// FOSCSEL
#pragma config FNOSC = FRC              // Oscillator Source Selection (Internal Fast RC (FRC))
#pragma config PWMLOCK = OFF            // PWM Lock Enable bit (PWM registers may be written without key sequence)
#pragma config IESO = ON                // Two-speed Oscillator Start-up Enable bit (Start up device with FRC, then switch to user-selected oscillator source)

// FGS
#pragma config GWRP = OFF               // General Segment Write-Protect bit (General Segment may be written)
#pragma config GCP = OFF                // General Segment Code-Protect bit (General Segment Code protect is Disabled)

#include <xc.h>
#include <p33ep512mc502.h>
/* Standard includes. */
#include <stdio.h>
/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "croutine.h"
#include "FreeRTOSConfig.h"
#include "main.h"

volatile char rxval[100];    //The UART receive array which holds the data sent 
                             //via radio from the top circuit board
int x = 0;
//Uart 1 is the radio
void __attribute__((__interrupt__, auto_psv)) _U1RXInterrupt(void)             
{
    IFS0bits.U1RXIF = 0;        //Clear the interrupt flag
    rxval[x] = U1RXREG;         //Read the receive register into the receive array
    x++;
    if(x == 100)
    {  
        x = 0;
    }
    return;
 }
void __attribute__((__interrupt__, auto_psv)) _DefaultInterrupt(void)
{
    return;
}

void main(void) {
    int i = 0;
    for(i = 0; i < 100; i++)
    {
        rxval[i] = 0;       //Initialize the receive array to 0
    }
    init();

	//Left track stepper motor thread
	xTaskCreate( leftThread, "LeftTrack", 512, NULL, 1, NULL );
    //Right track stepper motor thread
    xTaskCreate( rightThread, "RightTrack", 512, NULL, 1, NULL );
    //Spin stepper motor thread
    xTaskCreate( spinThread, "Spin", 512, NULL, 1, NULL );
	//Start the scheduler.
	vTaskStartScheduler();
	/* Will only reach here if there is insufficient heap available to start
	the scheduler. */
    return;
}
void vApplicationIdleHook( void )
{
	/* Schedule the co-routines from within the idle task hook. */
	vCoRoutineSchedule();
}