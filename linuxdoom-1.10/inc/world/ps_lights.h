#ifndef PS_LIGHTS_H
#define PS_LIGHTS_H
#include "ps_global.h"


LightStruct *CreateLightStruct(u32 type);
void SetLightColor(LightStruct *light, VECTOR color);
void SetLightTheta(LightStruct *light, float ang);
qword_t *PackLightIntoQWord(qword_t *q, LightStruct *light);

inline void SetLightRadius(LightStruct *light, float rad)
{
    light->radius = rad;
};

inline qword_t *InitVU1LightHeader(qword_t *q, u32 count)
{
    q->sw[0] = count;
    q->sw[1] = count;
    q->sw[2] = count;
    q->sw[3] = count;
    q++;
    return q;
};

#endif