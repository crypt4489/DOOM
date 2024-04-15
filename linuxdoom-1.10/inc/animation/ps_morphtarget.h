#ifndef PS_MORPH_TARGET
#define PS_MORPH_TARGET
#include "ps_global.h"

#define GetMorphInterpolatorNodesCount(_interp)            \
    ((_interp)->interpCount)

#define GetMorphInterpolatorNodesCapacity(_interp)            \
    ((_interp)->interpCap)

#define GetMorphCurrentInterpolatorNode(_interp)            \
    ((_interp)->currInterpNode)

#define GetMorphMeshCount(_interp)            \
    ((_interp)->meshCount)

#define GetMorphMeshCapacity(_interp)            \
    ((_interp)->meshCap)

void CreateMorphTargetBuffersFromFile(const char *targetFile, MeshBuffers *buffer);
MorphTargetBuffer *CreateMorphTargetBuffer(u32 bufferSize);
MorphTargetBuffer *CreateInterpolatorNodes(MorphTargetBuffer *buffer, u32 interpolatorSize);
u32 AddMeshToTargetBuffer(MorphTargetBuffer *buffer, MeshBuffers *mesh);
void AddInterpolatorNode(MorphTargetBuffer *buffer, u16 _start, u16 _end, float _scale);
GameObject *CreateObjectMorphBuffer(GameObject *obj, u32 bufferSize);
MorphTargetBuffer *SetInterpolatorNode(MorphTargetBuffer *buffer, u32 num);
Interpolator *GetInterpolatorNode(MorphTargetBuffer *buffer, u32 num);
Interpolator *GetCurrentInterpolatorNode(MorphTargetBuffer *buffer);
MeshBuffers *GetMorphMeshBuffer(MorphTargetBuffer *buffer, u32 num);
void SetInterpolatorCallback(MorphTargetBuffer *buffer, interpolator_callback cb);
void SetMorphTargetCallback(MorphTargetBuffer *buffer, morph_target_callback cb);
void ExecuteMorphTargetCBFuncs(MorphTargetBuffer *buffer);
u32 GenericUpdateInterpolatorNode(Interpolator *node);
void GenericUpdateMorphBuffer(MorphTargetBuffer *buffer);

#endif