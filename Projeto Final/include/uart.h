#ifndef UART_H_   /* Include guard */
#define UART_H_
#include <stdint.h>

void UARTSend(char *command);
void UARTStartup(void);
float GetValue(char *sensor, char *buf);

#endif // FOO_H_