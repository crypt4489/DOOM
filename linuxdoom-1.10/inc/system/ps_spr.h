#ifndef PS_SPR_H
#define PS_SPR_H
#include "ps_global.h"
#include <dma_registers.h>
#include <dma.h>
void sendPacketChainSPR(void *data);
void sendPacketSPR(void *data, u32 qwc);
void receiveSPR( void *data, void *dest, u32 qwc);
void ultimate_memcpy(void *from_data, u32 qwc, void *to_data);
#endif
