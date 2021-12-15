#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include <ctype.h>
#include <string.h>
#include "include/uart.h"
#include "include/threads.h"


uint32_t g_ui32SysClock;

#ifdef DEBUG
void
__error__(char *pcFilename, uint32_t ui32Line)
{
}
#endif

void UARTSend(char *command){
    //
    // Loop while there are more characters to send.
    //
    for (uint8_t i = 0; i < strlen(command); i++)
    {
        //
        // Write the next character to the UART.
        //
        UARTCharPut(UART0_BASE, command[i]);
    }
}

void UARTStartup(void){

  // configura o clock para 120MHz
  g_ui32SysClock = SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ |
                                             SYSCTL_OSC_MAIN |
                                             SYSCTL_USE_PLL |
                                             SYSCTL_CFG_VCO_240), 120000000);

  
  //Habilita a porta GPIO do LED
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);
  //Saída pino 0
  GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, GPIO_PIN_0);
  
  //Habilita UART
  SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
  
  //Habilita interrupção
  IntMasterEnable();
  
  //Configura as portas como portas UART
  GPIOPinConfigure(GPIO_PA0_U0RX);
  GPIOPinConfigure(GPIO_PA1_U0TX);
  GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
  
  
  //Configura a UART para 115.200 e operação 8N1
  
  UARTConfigSetExpClk(UART0_BASE, g_ui32SysClock, 115200,
                            (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                             UART_CONFIG_PAR_NONE));
  //Habilita interrupção pelo UART
  IntMasterEnable();
  
  //UARTSend(");
  
}

float GetValue(char *sensor, char *buf)
{
    UARTCharPut(UART0_BASE, 'P');
    for (uint8_t i = 0; i < strlen(sensor); i++)
    {
        UARTCharPut(UART0_BASE, sensor[i]);
    }
    UARTCharPut(UART0_BASE, ';');

    // Wait for buffer to fill with response.
    tx_thread_sleep(10);

    uint8_t i = 0;
    while (UARTCharsAvail(UART0_BASE))
    {
        buf[i] = UARTCharGet(UART0_BASE);
        i += 1;
    }

    return atof(buf + 1 + strlen(sensor));
}


