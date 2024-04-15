#ifndef PS_MOVEMENT_H
#define PS_MOVEMENT_H
#include "ps_global.h"


#define PS_MV_CAMERA 0xbeef
#define PS_MV_GAMEOBJECT 0x0bad

void PitchLTMMove(MATRIX ltm, float angle, VECTOR lookOut, VECTOR upOut);
void RotateYLTMMove(MATRIX ltm,  float angle, VECTOR right_out, VECTOR up_out, VECTOR lookOut);

void RotateYLTM(MATRIX ltm, float angle);
void PitchLTM(MATRIX ltm, float angle);

void WalkLTM(MATRIX ltm, float d);
void StrafeLTM(MATRIX ltm, float d);
void WalkLTMMove(MATRIX ltm, float d, VECTOR newPos);
void StrafeLTMMove(MATRIX ltm, float d, VECTOR newPos);
#endif
