#ifndef PS_GS_H
#define PS_GS_H

#include "ps_global.h"
#include <gs_psm.h>
#include <gs_privileged.h>
#include <gs_gp.h>
#include <draw_tests.h>


#define CreateGSScreenCoordinates(input, operand)   ((2048 operand input) << 4)


void InitGS(GameManager *manager, framebuffer_t *frame, zbuffer_t *z, int context, u32 psm);
void InitFramebuffer(framebuffer_t *frame, int width, int height, int psm);
void LoadFrameBuffer(framebuffer_t *frame, unsigned char *pixels, int width, int height, int psm);
void SetGraph(GameManager *manager, framebuffer_t *frame);
void CreateTexBuf(Texture *texture, int width, int psm);

qword_t* CreateGSSetTag(qword_t *q, u32 count, u32 eop, u32 type, u32 nreg, u32 regaddr);
qword_t *SetFrameBufferMask(qword_t *q, framebuffer_t *frame, u32 mask, u32 context);
qword_t *SetZBufferMask(qword_t *q, zbuffer_t *z, u32 mask, u32 context);
void InitDrawingEnvironment(framebuffer_t *frame, zbuffer_t *z, int hheight, int hwidth, int context, int waitFinish);
void CopyVRAMToVRAM(int srcAd, int srcH, int srcW, int dstAd, int dstH, int dstW, int psm);
void CopyVRAMToMemory(int address, int width, int height, int x, int y, int psm, u32* buffer);

void CreateClutBuf(clutbuffer_t *clut, int width, int psm);
void InitZBuffer(zbuffer_t *z, int width, int height, int zsm, int method);
void CreateClutStructs(Texture *tex, int psm);
void CreateTexStructs(Texture *tex, int width, int psm, u32 components, u32 function, u32 texfilter);
void SetupRenderTarget(RenderTarget *target, int context, int wait);
RenderTarget *CreateRenderTarget(int height, int width, int zsm, int zmethod, int psm);
Texture *CreateTextureFromRenderTarget(RenderTarget *target, u32 filter, u32 function);
RenderTarget *AllocRenderTarget();
void DestroyRenderTarget(RenderTarget *target);


void ClearScreen(RenderTarget *target, int context, int r, int g, int b, int a);

qword_t *draw_clear_alpha(qword_t *q, int context, float x, float y, float width, float height, int r, int g, int b, int a);

qword_t *draw_enable_tests_alpha(qword_t *q, int context, int alpha, u32 method);
qword_t *draw_disable_tests_alpha(qword_t *q, int context, int alpha);
qword_t *SetupZTestGS(qword_t* q, int z_test_method, int z_test_enable, char alphaValue, char alpha_test_method, char frameBufferTest, char alpha_test_enable, char alpha_test, int context);
qword_t* SetupRGBAQGS(qword_t *b, Color color);
qword_t* SetupAlphaGS(qword_t *q, blend_t *blend, int context);
#endif
//
