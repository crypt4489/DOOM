#ifndef PS_PLANE_H
#define PS_PLANE_H
#include "ps_global.h"
void ComputePlane(VECTOR vec, VECTOR normal, VECTOR plane);
void PointInPlane(VECTOR plane, VECTOR p, VECTOR pointInPlane, VECTOR planePoint);

void SetupPlane(VECTOR planeNormal, VECTOR planePoint, Plane *plane);
float DistanceFromPlane(VECTOR planeEquation, VECTOR point);

void NormalizePlane(VECTOR in, VECTOR out);

#endif