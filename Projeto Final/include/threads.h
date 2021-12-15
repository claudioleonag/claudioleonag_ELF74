#ifndef SAMPLE_THREADX_H_   /* Include guard */
#define SAMPLE_THREADX_H_
#define public

#define BYTE_POOL_SIZE__ 9120
#define STACK_SIZE__ 1024
#define COEFFICIENT 0.15

#include "tx_api.h"
/* Define event buffer.  */

#ifdef TX_ENABLE_EVENT_TRACE
UCHAR   trace_buffer[0x10000];
#endif
/* Define thread prototypes.  */

void ThreadRead(ULONG);
void ThreadWrite(ULONG);
void ThreadControl(ULONG);


#endif // 