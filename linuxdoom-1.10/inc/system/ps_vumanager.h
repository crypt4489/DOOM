#ifndef PS_VUMANAGER
#define PS_VUMANAGER
#include <kernel.h>
#include <ee_regs.h>
#include "ps_global.h"



void AddVuProgram(u32 size);
u32 GetProgramAddress(int index);
u32 SizeOfProgramInstructions(u32 *codeStart, u32 *codeEnd);
u32 SizeOfProgramPacket(u32 *codeStart, u32 *codeEnd);
u32 GetCurrBasePointer();
void ReadFromVU(volatile u32* start, int printOutSize, u32 usefloatornot);
void SetupVU1INTEHandler();

/* VU1Manager Functions */

VU1Manager* CreateVU1Manager(u32 size);
void DestroyVU1Manager(VU1Manager *manager);
void AddProgramToManager(VU1Manager *manager, VU1Program *program);
u32 GetProgramAddressVU1Manager(VU1Manager *manager, u32 index);

/* VU1 Program Functions */

VU1Program* CreateVU1Program(u32 *codeStart, u32 *codeEnd, u32 stage);
void DestroyVU1Program(VU1Program *prog);
u32 GetProgramAddressVU1Program(VU1Program *prog);

#endif
