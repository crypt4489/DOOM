#ifndef PS_VU1PIPELINE_H
#define PS_VU1PIPELINE_H

#include "ps_global.h"

void CreatePipelineSizes(u32 code, u32 *numberOfCbs, u32 *vu1_header_size);
u32 GetDoubleBufferOffset(u32 base);

VU1Pipeline* GetPipelineByName(const char *name, GameObject *obj);


PipelineCallback *CreatePipelineCBNode(pipeline_callback cb, qword_t *pipeline_loc, void *argument, u32 id);
VU1Pipeline *CreateVU1Pipeline(const char *name, int sizeOfCBS, u32 renderPasses);
void AddPipelineCallbackNode(VU1Pipeline *pipeline, PipelineCallback *node);
void AddVU1Pipeline(GameObject *obj, VU1Pipeline *pipeline);

void SetActivePipelineByName(GameObject *obj, const char *name);
void SetActivePipeline(GameObject *obj, VU1Pipeline *pipe);
void ExecutePipelineCBs(GameObject *obj, VU1Pipeline *pipe);
void DeletePipeline(VU1Pipeline *pipe);
qword_t* AddPipelineCallbackNodeQword(VU1Pipeline *pipeline, PipelineCallback *node, qword_t *q, qword_t *targ);
void ParsePipeline(GameObject *obj, VU1Pipeline *pipe);
void RenderPipeline(GameObject *obj, VU1Pipeline *active_pipe);

#endif