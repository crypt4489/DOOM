#ifndef PS_MATRIX_H
#define PS_MATRIX_H
#include "ps_global.h"

#include "gameobject/ps_ltm.h"

void MatrixIdentity(MATRIX m);
void MatrixCopy(MATRIX dest, MATRIX in);
void MatrixVoidCopy(void *dest, void *in);
void MatrixMultiply(MATRIX in1, MATRIX in2, MATRIX out);

void MatrixVectorMultiply(VECTOR out, MATRIX m, VECTOR in);
void MatrixVectorTransform(VECTOR out, MATRIX m, VECTOR in);

void CreateProjectionMatrix(MATRIX output, float width, float height, float aspect, float near, float far, float angle);
void CreateRotationMatrix(VECTOR axis, float angle, MATRIX output);
void CreateTranslationMatrix(VECTOR pos, MATRIX output);
void CreateScaleMatrix(VECTOR scales, MATRIX output);
void CreateWorldMatrix(MATRIX output, MATRIX scales, MATRIX rot, MATRIX trans);
void DumpMatrix(MATRIX elem);

void MatrixInverse(MATRIX src, MATRIX out);
void MatrixTranspose(MATRIX src);
void CreateNormalizedTextureCoordinateMatrix(MATRIX src);
void CreateOrthoGraphicMatrix(float xLow, float xHigh, float yLow, float yHigh, float near, float far, MATRIX out);

void Matrix3VectorMultiply(VECTOR out, MATRIX m, VECTOR in);
void ComputeReflectionMatrix(VECTOR normal, MATRIX res);

void CreateWorldMatrixFromVectors(VECTOR pos, VECTOR up, VECTOR forward, VECTOR right, VECTOR scales, MATRIX m);
void CreateRotationAndCopyMatFromObjAxes(MATRIX out, VECTOR up, VECTOR forward, VECTOR right);

void MatrixMultiply(MATRIX output, MATRIX input, MATRIX input1);

void CreateWorldMatrixFromQuatScalesTrans(VECTOR trans, VECTOR rot, VECTOR scale, MATRIX m);

void ExtractVectorFromMatrix(VECTOR trans, VECTOR rot, VECTOR scale, MATRIX m);
#endif
