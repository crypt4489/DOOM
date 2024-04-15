#ifndef PS_QUAT_H
#define PS_QUAT_H
#include "ps_global.h"

void CreateQuatRotationAxes(const VECTOR right, const VECTOR up, const VECTOR forward, VECTOR out);
void CreateRotationMatFromQuat(const VECTOR quat, MATRIX m);
void Slerp(VECTOR q1, VECTOR q2, float delta, VECTOR out);
void QuaternionNormalize(VECTOR in, VECTOR out);
#endif