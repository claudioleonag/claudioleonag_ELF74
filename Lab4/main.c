#include <stdint.h>
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


uint32_t g_ui32SysClock;

#ifdef DEBUG
void
__error__(char *pcFilename, uint32_t ui32Line)
{
}
#endif

void UARTIntHandler(void){
  uint32_t ui32Status;
  ui32Status = MAP_UARTIntStatus(UART0_BASE, true); // Get the interrrupt status.
  MAP_UARTIntClear(UART0_BASE, ui32Status); // Clear the asserted interrupts.
  unsigned char letter = 0;
  
  while(MAP_UARTCharsAvail(UART0_BASE)){ // Loop while there are characters in the receive FIFO.
    letter = MAP_UARTCharGet(UART0_BASE);
    MAP_UARTCharPut(UART0_BASE, letter); // Read the next character from the UART and write it back to the UART.
    
    if(isalpha(letter)){    //Verifica se o dígito é alfabético
      if(isupper(letter)){ //Verifica se o dígito está em uppercase
      MAP_UARTCharPut(UART0_BASE, tolower(letter)); // Read the next character from the UART and write it back to the UART.
      MAP_UARTCharPut(UART0_BASE, '\n'); // Jump line
      }
      else{
        MAP_UARTCharPut(UART0_BASE, toupper(letter)); //Se o dígito está em lowercase
        MAP_UARTCharPut(UART0_BASE, '\n'); // Jump line
      }
    }     
    
    
    MAP_GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0, GPIO_PIN_0); //Pisca o Led 
    SysCtlDelay(g_ui32SysClock / (1000 * 3));
    MAP_GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0, 0); 
    }  
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
        MAP_UARTCharPutNonBlocking(UART0_BASE, *pui8Buffer++);
    }
}


int main()
{
  // configura o clock para 120MHz
  g_ui32SysClock = MAP_SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ |
                                             SYSCTL_OSC_MAIN |
                                             SYSCTL_USE_PLL |
                                             SYSCTL_CFG_VCO_240), 120000000);
 


  
  //Habilita a porta GPIO do LED
  MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);
  //Saída pino 0
  MAP_GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, GPIO_PIN_0);
  
  //Habilita UART
  MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
  MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
  
  //Habilita interrupção
  MAP_IntMasterEnable();
  
  //Configura as portas como portas UART
  MAP_GPIOPinConfigure(GPIO_PA0_U0RX);
  MAP_GPIOPinConfigure(GPIO_PA1_U0TX);
  MAP_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
  
  
  //Configura a UART para 115.200 e operação 8N1
  
  MAP_UARTConfigSetExpClk(UART0_BASE, g_ui32SysClock, 115200,
                            (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                             UART_CONFIG_PAR_NONE));
  //Habilita interrupção pelo UART
  MAP_IntEnable(INT_UART0);
  MAP_UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT);
  
  //Texto de entrada
  UARTSend((uint8_t *)"\033[H\033[JDigite aqui: ",40);
  while(1)
  {
  }
}
