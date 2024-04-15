#ifndef PS_OBB_H
#define PS_OBB_H

#include "ps_global.h"


void ReadOBBFromVU1(GameObject *obj);
void InitOBB(GameObject *obj, int type);
void DestroyOBB(ObjectBounds *bound);


void FindRadiusMinMax(VECTOR top, VECTOR bottom, BoundingSphere *sphere);
void FindCenterForSphere(VECTOR top, VECTOR bottom, BoundingSphere *sphere);

int AABBCollision(VECTOR top1, VECTOR bottom1, VECTOR top2, VECTOR bottom2);

int CheckCollision(GameObject *obj1, GameObject *obj2, ...);

void FindCenterOfOBB(void *collisionData, int type, VECTOR center);


//find center and half pos from trans, scale, and axes of a OBB
void FindCenterAndHalfOBB(BoundingBox *box, VECTOR pos, VECTOR scale, VECTOR xAxis, VECTOR yAxis, VECTOR zAxis, VECTOR outCenter, VECTOR outHalf);
void FindCenterAndHalfAABB(BoundingBox *box, VECTOR outCenter, VECTOR outHalf);

int CheckSeparatingPlane(VECTOR pos, VECTOR plane, VECTOR half1, VECTOR half2, VECTOR xAxis1, VECTOR yAxis1, VECTOR zAxis1, VECTOR xAxis2, VECTOR yAxis2, VECTOR zAxis2);

int PerformSAT(VECTOR pos, VECTOR half1, VECTOR half2, VECTOR xAxis1, VECTOR yAxis1, VECTOR zAxis1, VECTOR xAxis2, VECTOR yAxis2, VECTOR zAxis2);

void FindOBBMaxAndMinVerticesVU0(GameObject *obj);

#endif
