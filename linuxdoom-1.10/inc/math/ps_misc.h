#ifndef PS_MISC_H
#define PS_MISC_H
#include "ps_global.h"

MeshBuffers* CreateGrid(int N, int M, float depth, float width, MeshBuffers *buffer);
void CreateGridVectors(int N, int M, float depth, float width, MeshBuffers *buffer);
void CreateGridUVS(int N, int M, float depth, float width, MeshBuffers *buffer);
void CreateGridIndices(int N, int M, float depth, float width, MeshBuffers *buffer);
void ZSort(GameObject *obj, Camera *cam);

void Pathify(const char *name, char *file);
void AppendString(const char *input1, const char *input2, char *output, u32 max);

void dump_packet(qword_t *q, int max, int usefloat);

extern VECTOR forward;
extern VECTOR up;
extern VECTOR right;

#endif
