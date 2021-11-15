//*****************************************************************************
//
// blinky.c - Simple example to blink the on-board LED.
//
// Copyright (c) 2013-2020 Texas Instruments Incorporated.  All rights reserved.
// Software License Agreement
// 
// Texas Instruments (TI) is supplying this software for use solely and
// exclusively on TI's microcontroller products. The software is owned by
// TI and/or its suppliers, and is protected under applicable copyright
// laws. You may not combine this software with "viral" open-source
// software in order to form a larger program.
// 
// THIS SOFTWARE IS PROVIDED "AS IS" AND WITH ALL FAULTS.
// NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT
// NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. TI SHALL NOT, UNDER ANY
// CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
// DAMAGES, FOR ANY REASON WHATSOEVER.
// 
// This is part of revision 2.2.0.295 of the EK-TM4C1294XL Firmware Package.
//
//*****************************************************************************

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "inc/hw_memmap.h"
#include "inc/hw_nvic.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"



//*****************************************************************************
//
//! \addtogroup example_list
//! <h1>Blinky (blinky)</h1>
//!
//! A very simple example that blinks the on-board LED using direct register
//! access.
//
//*****************************************************************************

//*****************************************************************************
//
// The error routine that is called if the driver library encounters an error.
//
//*****************************************************************************
#ifdef DEBUG
void
__error__(char *pcFilename, uint32_t ui32Line)
{
    while(1);
}
#endif

//*****************************************************************************
//
// Blink the on-board LED.
//
//*****************************************************************************

volatile uint32_t msTicks = 0; 
uint32_t periodCounter = 3000; // 3s

void turnLedOn(void)
{
  GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_1, GPIO_PIN_1);
  GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0, GPIO_PIN_0);
  //printf("Led On \n");
}  

void turnLedOff(void)
{
  GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_1, 0x0);
  GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0, 0x0);
    //printf("Led Off \n");
}

void timerCounter(void)  {                              
  msTicks++;                                               
}

void piscaLed(void)
{
  uint32_t periodCounter = 100;
  msTicks = 0;
  uint32_t piscaCounter = 0; //quantidade de vezes que ele irá piscar
  while(piscaCounter != 20)
  {
  if (msTicks < periodCounter) 
        {
          turnLedOn();                  
        }
          if (msTicks >= periodCounter)
        {
          turnLedOff();
          if (msTicks > 2*periodCounter) 
          {
            piscaCounter++;
            msTicks = 0;
          }
  
         }
  }
}  

void handlerSWButton(void)
{
    turnLedOff();
    printf("Interrupcao!!\n"); 
    printf(" Passaram %d ciclos de clock. \n", 100*msTicks);
    if (msTicks < periodCounter)
    {
    //printf("Parabens voce ganhou!\n");     
      msTicks = 0;
    }
  
}

int main(void){
  
    SysCtlClockFreqSet(SYSCTL_XTAL_25MHZ, 2*SYSCTL_XTAL_25MHZ); //50MHz
    SysTickPeriodSet(SysCtlClockGet()/100);    // interrupção cada 1ms
    
    SysTickIntEnable();
    SysTickIntRegister(timerCounter); 
    printf("Bem vindo! O timer inicia apos a luz piscante acabar! \n");
    
   

    //
    // Enable the GPIO port that is used for the on-board LED.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOJ);
    

    //
    // Check if the peripheral access is enabled.
    //
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPION))
    {
    }
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOJ))
    {
    }

    //
    // Enable the GPIO pin for the LED (PN0).  Set the direction as output, and
    // enable the GPIO pin for digital function.
    //
    GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, GPIO_PIN_0); //Configura led 0
    GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, GPIO_PIN_0); // Configura led 1
    GPIOPinTypeGPIOInput(GPIO_PORTJ_BASE, GPIO_PIN_0); // Configura botão 0 para ler
    SysTickEnable();    
    GPIOIntTypeSet(GPIO_PORTJ_BASE, GPIO_PIN_0, GPIO_HIGH_LEVEL); // Interrupção ativa quando o botão é pressionado
    GPIOIntRegister(GPIO_PORTJ_BASE, handlerSWButton); // Handler para interrupção    
    GPIOIntEnable(GPIO_PORTJ_BASE, GPIO_PIN_0); // Ativa interrupção botão 0
   
    
    while(1)
    {

       if (msTicks < periodCounter) // 3 segundos on
        {
          turnLedOn();
        }
        if (msTicks >= periodCounter) // 3 segundos off
        {
          turnLedOff();
          
          if (msTicks > 2*periodCounter) // período de 6 segundos
          {   
            //printf("Tempo acabado! \n");
            piscaLed();
            msTicks = 0;      //reseta contador      
          }
        }     
    }
}
