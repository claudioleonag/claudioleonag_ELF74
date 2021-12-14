#ifndef UART_H_   /* Include guard */
#define UART_H_
#include <stdint.h>

void UARTIntHandler(void);
void UARTSend(const uint8_t *pui8Buffer, uint32_t ui32Count);
void UARTStartup(void);

#endif // FOO_H_