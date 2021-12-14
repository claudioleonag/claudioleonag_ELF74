#ifndef SAMPLE_THREADX_H_   /* Include guard */
#define SAMPLE_THREADX_H_
#define public

#define DEMO_STACK_SIZE         1024
#define DEMO_BYTE_POOL_SIZE     9120
#define DEMO_BLOCK_POOL_SIZE    100
#define DEMO_QUEUE_SIZE         100

#define BYTE_POOL_SIZE__ 9120
#define STACK_SIZE__ 1024
#define QUEUE_MSG_SIZE__ TX_4_ULONG
#define QUEUE_MSG_COUNT__ 4
#define QUEUE_SIZE__ (QUEUE_MSG_SIZE__ * QUEUE_MSG_COUNT__)

#include "tx_api.h"
/* Define event buffer.  */

#ifdef TX_ENABLE_EVENT_TRACE
UCHAR   trace_buffer[0x10000];
#endif
/* Define thread prototypes.  */

extern TX_QUEUE queue_uart;

void    thread_0_entry(ULONG thread_input);
void    thread_1_entry(ULONG thread_input);
void    thread_2_entry(ULONG thread_input);
void    thread_3_and_4_entry(ULONG thread_input);
void    thread_5_entry(ULONG thread_input);
void    thread_6_and_7_entry(ULONG thread_input);
void    tx_application_define(void *first_unused_memory);


#endif // 