#ifndef CAMERA_H
#define CAMERA_H
#include "ps_global.h"


void CameraLookAt(Camera *cam, VECTOR pos, VECTOR target, VECTOR up);

void DumpCameraFrustum(Camera *cam);

void CreateCameraQuat(Camera *cam, VECTOR out);

inline void SetGlobalDrawingCamera(Camera *cam)
{
    g_DrawCamera = cam;
}

int HandleCamMovement(Camera* cam, u32 type);

Camera* EndRendering(Camera *cam);


void UpdateViewMatrix();

void UpdateCameraMatrix(Camera *cam);

void InitCameraObb(Camera *cam, float x, float y, float z, u32 type);

void CreateCameraWorldMatrix(Camera *cam, MATRIX output);

void CreateCameraFrustum(Camera *cam);

Camera* InitCamera(int width, int height, float near, float far, float aspect, float angle);

int TestObjectInCameraFrustum(Camera *cam, GameObject *obj);

void CleanCameraObject(Camera *cam);

#endif
