#ifndef PS_LTM_H
#define PS_LTM_H

#include "ps_global.h"

inline void GetPositionVectorCopyLTM(MATRIX ltm, VECTOR out)
{
    out[0] = ltm[12];
    out[1] = ltm[13];
    out[2] = ltm[14];
    out[3] = ltm[15];
};

inline void SetPositionVectorLTM(MATRIX ltm, VECTOR pos)
{
    ltm[12] = pos[0];
    ltm[13] = pos[1];
    ltm[14] = pos[2];
    //ltm[15] = 1.0f;
};

inline void GetRightVectorCopyLTM(MATRIX ltm, VECTOR out)
{
    out[0] = ltm[0];
    out[1] = ltm[1];
    out[2] = ltm[2];
    out[3] = 0.0f;
};

inline void GetUpVectorCopyLTM(MATRIX ltm, VECTOR out)
{
    out[0] = ltm[4];
    out[1] = ltm[5];
    out[2] = ltm[6];
    out[3] = 0.0f;
};

inline void GetForwardVectorCopyLTM(MATRIX ltm, VECTOR out)
{
    out[0] = ltm[8];
    out[1] = ltm[9];
    out[2] = ltm[10];
    out[3] = 0.0f;
};

inline void SetRightVectorLTM(MATRIX ltm, VECTOR right)
{
    ltm[0] = right[0];
    ltm[1] = right[1];
    ltm[2] = right[2];
};

inline void SetUpVectorLTM(MATRIX ltm, VECTOR up)
{
    ltm[4] = up[0];
    ltm[5] = up[1];
    ltm[6] = up[2];
};

inline void SetForwardVectorLTM(MATRIX ltm, VECTOR forward)
{
    ltm[8] = forward[0];
    ltm[9] = forward[1];
    ltm[10] = forward[2];
};

inline void SetScaleVectorLTM(MATRIX ltm, VECTOR scale)
{
    ltm[3] = scale[0];
    ltm[7] = scale[1];
    ltm[11] = scale[2];
};

inline void GetScaleVectorLTM(MATRIX ltm, VECTOR out)
{
    out[0] = ltm[3];
    out[1] = ltm[7];
    out[2] = ltm[11];
    out[3] = 1.0f;
};

void CreateWorldMatrixLTM(MATRIX ltm, MATRIX m);

inline void SetRotationVectorsLTM(MATRIX ltm, VECTOR up, VECTOR right, VECTOR forward)
{
    ltm[0] = right[0];
    ltm[1] = right[1];
    ltm[2] = right[2];

    ltm[4] = up[0];
    ltm[5] = up[1];
    ltm[6] = up[2];

    ltm[8] = forward[0];
    ltm[9] = forward[1];
    ltm[10] = forward[2];
};

inline void GetRotationVectorsLTM(MATRIX ltm, VECTOR up, VECTOR right, VECTOR forward)
{
    right[0] = ltm[0];
    right[1] = ltm[1];
    right[2] = ltm[2];

    up[0] = ltm[4];
    up[1] = ltm[5];
    up[2] = ltm[6];

    forward[0] = ltm[8];
    forward[1] = ltm[9];
    forward[2] = ltm[10];

};

inline VECTOR* GetRightVectorLTM(MATRIX m)
{
    return (VECTOR*)&m[0];
};

inline VECTOR* GetUpVectorLTM(MATRIX m)
{
    return (VECTOR*)&m[4];
};

inline VECTOR* GetForwardVectorLTM(MATRIX m)
{
    return (VECTOR*)&m[8];
};

inline VECTOR* GetPositionVectorLTM(MATRIX m)
{
    return (VECTOR*)&m[12];
};

inline u32 GetDirtyLTM(MATRIX m)
{
    u32 *ptr = (u32*)&m[15];
    return *ptr & 0x00000001;
};

inline void SetDirtyLTM(MATRIX m)
{
    u32 *ptr = (u32*)&m[15];
    *ptr = *ptr | 0x00000001U;
};

inline void ClearDirtyLTM(MATRIX m)
{
    u32 *ptr = (u32*)&m[15];
    *ptr = *ptr & 0xFFFFFF00U;
};

inline float GetLastLTM(MATRIX m)
{
    u32 *ptr = (u32*)&m[15];
    u32 fixed = *ptr & 0xFFFFFF00U;
    //DEBUGLOG("%d %x", fixed, *ptr);
    fixed = fixed >> 8;
    //DEBUGLOG("%d", fixed);
    return (float)fixed/(float)(1U<<16);
};

inline void SetLastLTM(MATRIX m, float val)
{
    u32 fixed = val * (1U<<16);
   // DEBUGLOG("fixed: %d", fixed);
    u32 *ptr = (u32*)&m[15];
    *ptr = *ptr & 0x00000001;
    *ptr = *ptr | (fixed<<8U);
   // DEBUGLOG("fixed: %x", *ptr);
};
void SetLastAndDirtyLTM(MATRIX m, float w);

void SetupLTM(VECTOR pos, VECTOR up, VECTOR right,
         VECTOR forward, VECTOR scales,
          float q, MATRIX ltm);

#endif