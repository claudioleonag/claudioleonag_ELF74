/* This is a small demo of the high-pereadormance ThreadX kernel.  It includes examples of eight
   threads of different priorities, using a message queue, semaphore, mutex, event flags group, 
   byte pool, and block pool. Please refer to Chapter 6 of the ThreadX User Guide for a complete
   description of this demonstration.  */

#include "include/threads.h"
#include <stdio.h>
#include <stdint.h>
#include "include/uart.h"


/* Define the ThreadX object control blocks...  */

TX_BYTE_POOL byte_pool;
UCHAR byte_pool_memory[BYTE_POOL_SIZE__];
TX_THREAD thread_read;
TX_THREAD thread_write;
TX_THREAD thread_control;
TX_QUEUE queue_uart_read;
TX_MUTEX mutex_read;
TX_MUTEX mutex_turn;

float read;
float turn;



/* Define what the initial system looks like.  */

void tx_application_define(void *first_unused_memory)
{
    void *alloc_bump_ptr = TX_NULL;

    tx_byte_pool_create(&byte_pool, "Byte Pool", byte_pool_memory, BYTE_POOL_SIZE__);

    tx_byte_allocate(&byte_pool, (VOID **) &alloc_bump_ptr, STACK_SIZE__, TX_NO_WAIT);
    tx_thread_create(&thread_read, "Thread Read", ThreadRead, 0, alloc_bump_ptr, STACK_SIZE__, 1, 1, TX_NO_TIME_SLICE, TX_AUTO_START);

    tx_byte_allocate(&byte_pool, (VOID **) &alloc_bump_ptr, STACK_SIZE__, TX_NO_WAIT);
    tx_thread_create(&thread_write, "Thread Write", ThreadWrite, 1, alloc_bump_ptr, STACK_SIZE__, 2, 2, TX_NO_TIME_SLICE, TX_AUTO_START);

    tx_byte_allocate(&byte_pool, (VOID **) &alloc_bump_ptr, STACK_SIZE__, TX_NO_WAIT);
    tx_thread_create(&thread_control, "Thread Control", ThreadControl, 1, alloc_bump_ptr, STACK_SIZE__, 2, 2, TX_NO_TIME_SLICE, TX_AUTO_START);

    tx_mutex_create(&mutex_read, "Mutex read", TX_NO_INHERIT);

    tx_mutex_create(&mutex_turn, "Mutex Turn", TX_NO_INHERIT);
}


/* Define the test threads.  */

void ThreadRead(ULONG thread_input) //essa thread lê um valor da uart, o mutex é para não disputar a UART
{
    char buffer[16] = {0};

    while (1)
    {
        float value_read = GetValue("read", buffer);

        if (tx_mutex_get(&mutex_read, TX_WAIT_FOREVER) != TX_SUCCESS)
            break;

        read = value_read; //armazena o valor lido em read
 
        if (tx_mutex_put(&mutex_read) != TX_SUCCESS)
            break;

        tx_thread_sleep(10);
    }
}

void ThreadWrite(ULONG thread_input)
{
    char buffer[16] = {0};

    UARTSend("A3;"); //inicia o carro
    tx_thread_sleep(200);
    UARTSend("A0;");

    while (1)
    {
        if (tx_mutex_get(&mutex_turn, TX_WAIT_FOREVER) != TX_SUCCESS) break;

        float turn_ = turn;

        if (tx_mutex_put(&mutex_turn) != TX_SUCCESS)
            break;
        
        int count = sprintf(buffer, "V%f;", turn_);

        buffer[count] = '\0';

        UARTSend(buffer);

        tx_thread_sleep(10);
    }
}

void ThreadControl(ULONG thread_input)
{
    while (1)
    {
        if (tx_mutex_get(&mutex_read, TX_WAIT_FOREVER) != TX_SUCCESS)
            break;

        float turn_ = -COEFFICIENT * read;

        if (tx_mutex_put(&mutex_read) != TX_SUCCESS)
            break;

        if (tx_mutex_get(&mutex_turn, TX_WAIT_FOREVER) != TX_SUCCESS)
            break;

        turn = turn_;

        if (tx_mutex_put(&mutex_turn) != TX_SUCCESS)
            break;

        tx_thread_sleep(10);
    }
}






