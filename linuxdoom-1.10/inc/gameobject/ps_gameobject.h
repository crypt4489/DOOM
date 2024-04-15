#ifndef PS_GAMEOBJECT_H
#define PS_GAMEOBJECT_H

#include "ps_global.h"

#define CREATE_PRIM_STRUCT(prim, primType, shade, map, fog, blend, antialias, map_type, fix) \
    prim.type = primType,                                                                    \
    prim.shading = shade,                                                                    \
    prim.mapping = map,                                                                      \
    prim.fogging = fog,                                                                      \
    prim.blending = blend,                                                                   \
    prim.antialiasing = antialias,                                                           \
    prim.mapping_type = map_type,                                                            \
    prim.colorfix = fix

#define CREATE_ALPHA_REGS(blender, c1, c2, c3, a, fixed) \
    blender.color1 = c1,                                 \
    blender.color2 = c2,                                 \
    blender.color3 = c3,                                 \
    blender.alpha = a,                                   \
    blender.fixed_alpha = fixed

#define CREATE_RGBAQ_STRUCT(c, red, green, blue, alpha, qv) \
    c.r = red,                                              \
    c.g = green,                                            \
    c.b = blue,                                             \
    c.a = alpha,                                            \
    c.q = qv

void CleanGameObject(GameObject *obj);
GameObject *InitializeGameObject();
qword_t *CreateVU1VertexUpload(qword_t *q, MeshBuffers *buffer, u32 start, u32 end, u32 drawSize, u8 code, qword_t *vu1_addrs);
qword_t *CreateVU1TargetUpload(qword_t *q, GameObject *obj, u32 start, u32 end, u32 drawSize, u8 code, qword_t *vu1_addrs);
qword_t *PackBuffersVU1(qword_t *q, MeshBuffers *buffer, u32 count, u32 *top, u32 offset, u8 code);
MeshBuffers *CreateMaterial(MeshBuffers *buff, u32 start, u32 end, u32 id);
LinkedList *AddMaterial(LinkedList *list, Material *mat);
qword_t * CreateMeshDMAUpload(qword_t *q, GameObject *obj, u32 drawSize, u16 drawCode, u32 matCount, qword_t *vu1_addr);

void SetupGameObjectPrimRegs(GameObject *obj, Color color, u32 renderState);


#endif