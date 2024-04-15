#ifndef VIFCOMM_H
#define VIFCOMM_H
#include <vif_codes.h>
#include "ps_global.h"
//#include "ps_vumanager.h"




void UploadProgramToVU1(u32 *cStart, u32 *cEnd, u32 dest, u32 packetSize, u32 programSize);
qword_t* add_unpack_data(qword_t* q, u32 dest_address, void *data, u32 qwSize, u8 use_top, u32 vif_pack);
qword_t * ReadUnpackData(qword_t *q, u32 dest_address, u32 qwSize, u8 use_top, u32 vif_pack);
qword_t* add_start_program_vu1(qword_t *q, u32 address);
qword_t* set_alpha_registers(qword_t *q, blend_t *blend, int context);
void LoadFrameBufferVIF(unsigned char *pixels, framebuffer_t *frame, int width, int height, int psm);
qword_t* load_texture_vif(qword_t* q, Texture *tex, void *pixels, unsigned char *clut_buffer);
qword_t* set_tex_address_mode(qword_t*q, u32 mode, u32 context);
qword_t *VIFSetupScaleVector(qword_t*b);
qword_t* vif_setup_tex(qword_t *b, Texture *tex, u32 context);
qword_t* vif_setup_rgbaq(qword_t *b, Color color);
qword_t* InitDoubleBufferingQWord(qword_t *q, u16 base, u16 offset);
qword_t *UploadVectorsVU0(qword_t *q, void *vectors, u32 offset, u32 *dest, u32 size);
qword_t *add_unpack_data2(qword_t *q, u32 dest_address, void *data, u32 qwSize, u8 use_top, u32 vif_pack);
u32 UploadStartProgram(u32 startCode, u32 startAddress, u32 inte);

u32 UploadFlushTag(u32 inte);

#endif
