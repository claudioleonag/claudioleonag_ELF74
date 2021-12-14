/* This is a small demo of the high-performance ThreadX kernel.  It includes examples of eight
   threads of different priorities, using a message queue, semaphore, mutex, event flags group, 
   byte pool, and block pool. Please refer to Chapter 6 of the ThreadX User Guide for a complete
   description of this demonstration.  */

#include "include/sample_threadx.h"
#include <stdio.h>
#include "include/uart.h"

/* Define the ThreadX object control blocks...  */

TX_THREAD thread_read;
TX_THREAD thread_write;
TX_THREAD thread_control;
TX_QUEUE queue_uart;
TX_BYTE_POOL byte_pool;
UCHAR byte_pool_memory[BYTE_POOL_SIZE__];

void ThreadReadEntry(ULONG);
void ThreadWriteEntry(ULONG);
void ThreadControlEntry(ULONG);

/* Define what the initial system looks like.  */

void tx_application_define(void *first_unused_memory)
{
    CHAR *alloc_bump_ptr = TX_NULL;

    tx_byte_pool_create(&byte_pool, "Byte Pool", byte_pool_memory, BYTE_POOL_SIZE__);

    tx_byte_allocate(&byte_pool, (VOID **) &alloc_bump_ptr, STACK_SIZE__, TX_NO_WAIT);
    tx_thread_create(&thread_read, "Thread Read", ThreadReadEntry, 0, alloc_bump_ptr, STACK_SIZE__, 1, 1, TX_NO_TIME_SLICE, TX_AUTO_START);

    tx_byte_allocate(&byte_pool, (VOID **) &alloc_bump_ptr, STACK_SIZE__, TX_NO_WAIT);
    tx_thread_create(&thread_write, "Thread Write", ThreadWriteEntry, 1, alloc_bump_ptr, STACK_SIZE__, 2, 2, TX_NO_TIME_SLICE, TX_AUTO_START);

    tx_byte_allocate(&byte_pool, (VOID **) &alloc_bump_ptr, STACK_SIZE__, TX_NO_WAIT);
    tx_thread_create(&thread_control, "Thread Control", ThreadControlEntry, 1, alloc_bump_ptr, STACK_SIZE__, 2, 2, TX_NO_TIME_SLICE, TX_AUTO_START);

    tx_byte_allocate(&byte_pool, (VOID **) &alloc_bump_ptr, QUEUE_SIZE__, TX_NO_WAIT);
    tx_queue_create(&queue_uart, "Queue UART", QUEUE_MSG_SIZE__, alloc_bump_ptr, QUEUE_SIZE__);
}


/* Define the test threads.  */

void ThreadReadEntry(ULONG thread_input)
{
    char buffer[128] = {0};
    //int message_received;
    int received = 0;
    while (1)
    {
        if (tx_queue_receive(&queue_uart, buffer, TX_WAIT_FOREVER) != TX_SUCCESS)
        {
            break;
        }
        puts(buffer);
        received += 1;
        tx_thread_sleep(100);
    }
}

void ThreadWriteEntry(ULONG thread_input)
{
    while (1)
    {
        tx_thread_sleep(1000);
    }
}

void ThreadControlEntry(ULONG thread_input)
{
    while (1)
    {
        tx_thread_sleep(1000);
    }
}






