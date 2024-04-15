#ifndef PS_VECTOR_H
#define PS_VECTOR_H
#include "ps_global.h"
void VectorCopy(VECTOR out, VECTOR in);
void VectorIntCopy(VectorInt out, VectorInt in);
void VectorVoidCopy(void* out, void* in);
float DotProduct(VECTOR in1, VECTOR in2);
void RandomVectorsInit(VECTOR in);
void GetRandomVectors(VECTOR in);
float dist(VECTOR in);
void Normalize(VECTOR in, VECTOR out);
float distCOP2(VECTOR in);
void DumpVector(VECTOR elem);
void DumpVectorInt(VectorInt elem);
void VectorSubtractXYZ(VECTOR in, VECTOR in2, VECTOR out);
void VectorAddXYZ(VECTOR in, VECTOR in2, VECTOR out);
qword_t *VectorToQWord(qword_t *q, VECTOR v);
void ComputeNormal(VECTOR v0, VECTOR v1, VECTOR v2, VECTOR out);
float DotProductFour(VECTOR in1, VECTOR in2);
void CreateVector(float x, float y, float z, float w, VECTOR out);
void ScaleVectorXYZ(VECTOR vec, VECTOR input, float scale);
void ZeroVector(VECTOR out);
void VectorCopyXYZ(VECTOR in, VECTOR out);
void LerpNum(VECTOR in1, VECTOR in2, VECTOR output, float delta, u32 components);
void CrossProduct(VECTOR m, VECTOR n, VECTOR out);
#endif
