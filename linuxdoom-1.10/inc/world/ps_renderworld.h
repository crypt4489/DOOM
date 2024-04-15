#ifndef RENDERWORLD_H
#define RENDERWORLD_H
#include "ps_global.h"


RenderWorld *CreateRenderWorld();
void DestoryRenderWorld(RenderWorld *world);
void AddLightToRenderWorld(RenderWorld *world, LightStruct *light);
void AddObjectToRenderWorld(RenderWorld *world, GameObject *obj);
RenderWorld* RemoveLightFromRenderWorld(RenderWorld *world, LightStruct *light);
RenderWorld* RemoveObjectFromRenderWorld(RenderWorld *world, GameObject *obj);
void DrawWorld(RenderWorld *world);
LinkedList* CleanWorldLightsList(RenderWorld *world);
LinkedList* CleanWorldObjList(RenderWorld *world);
void FrustumCullForWorldObjects(RenderWorld *world);
RenderWorld* AddWorldCallback(RenderWorld *world);
#endif