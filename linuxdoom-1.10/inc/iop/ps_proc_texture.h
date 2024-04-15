#ifndef PROC_TEXTURE_H
#define PROC_TEXTURE_H

#include "ps_global.h"


#define INIT_PROC_TEX 0
#define CHECKER_BOARD_FUNCTION 5
#define PERLIN_NOISE_FUNCTION 4
#define READ_FROM_IOP 3
#define WRITE_TO_IOP 2

#define	PROC_TEXTURE_IRX 0x9811789

typedef struct proc_texture_t
{
    int ee_addr;
    int iop_addr;
    int height;
    int bpp;
    int texSize;
} proc_texture;


void *allocIopSpace(int bytes);
void copy_data_to_iop(void* buffer, int bytes, void* iop_addr);
void init_rpc();
void generate_iop_checker_tex(proc_texture *proc, void* iop_addr, int size, void* ee_addr, int context, int angle);
void generate_iop_perlin_tex(proc_texture *proc, void* iop_addr, int size, void* ee_addr, int context);
void copy_data_from_iop(void* buffer, int bytes, void* iop_addr);
#endif
