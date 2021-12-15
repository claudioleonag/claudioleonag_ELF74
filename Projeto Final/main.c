#include "include/uart.h"
#include "include/threads.h"
/* Define main entry point.  */
int main()
{
    
    /* Please refer to Chapter 6 of the ThreadX User Guide for a complete
       description of this demonstration.  */


    /* Enter the ThreadX kernel.  */
    UARTStartup();
    tx_kernel_enter();
}