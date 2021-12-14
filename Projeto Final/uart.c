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
#include "include/sample_threadx.h"


uint32_t g_ui32SysClock;

#ifdef DEBUG
void
__error__(char *pcFilename, uint32_t ui32Line)
{
}
#endif

void UARTIntHandler(void){
  UINT status;
  uint32_t ui32Status;
  char buffer[16] = {0};
  int i = 0;
  ui32Status = UARTIntStatus(UART0_BASE, true); // Get the interrrupt status.
  UARTIntClear(UART0_BASE, ui32Status); // Clear the asserted interrupts.
  ULONG letter;
  
  while(UARTCharsAvail(UART0_BASE) && i < 16 ){ // Loop while there are characters in the receive FIFO.
    letter = UARTCharGet(UART0_BASE);
    buffer[i] = letter;
    i += 1;
    UARTCharPut(UART0_BASE, letter); // Read the next character from the UART and write it back to the UART. 

    //feedback visual
    GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0, GPIO_PIN_0); //Pisca o Led 
    SysCtlDelay(g_ui32SysClock / (1000 * 3));
    GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0, 0);
    
    // Create end word
    if (i <= 15)
      {
        buffer[i] = '\0';
      }    

  }

    status =  tx_queue_send(&queue_uart, buffer, TX_NO_WAIT);

    /* Check completion status.  */
 }
void UARTSend(const uint8_t *pui8Buffer, uint32_t ui32Count){
    //
    // Loop while there are more characters to send.
    //
    while(ui32Count--)
    {
        //
        // Write the next character to the UART.
        //
        UARTCharPutNonBlocking(UART0_BASE, *pui8Buffer++);
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
  IntEnable(INT_UART0);
  UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT);
  
  //UARTSend((uint8_t *)"\033[H\033[JUART Pronta para uso!!\n\r",45);
  
}


