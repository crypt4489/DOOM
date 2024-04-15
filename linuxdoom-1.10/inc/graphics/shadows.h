#ifndef PS_SHADOWMAP_H
#define PS_SHADOWMAP_H
#include "ps_global.h"
void CreateShadowMapVU1Pipeline(GameObject *obj, u32 programNumber, u32 qwSize);
void SetupPerObjDrawShadowRegisters(VU1Pipeline *pipe, GameObject* obj, void *arg, qword_t *pipeline_loc);
void SetupPerObjDrawShadowVU1Header(VU1Pipeline *pipe, GameObject* obj, void *arg, qword_t *pipeline_loc);
void DrawQuad(int height, int width, int xOffset, int yOffset, u8 blend, Texture *shadowTex);
#endif