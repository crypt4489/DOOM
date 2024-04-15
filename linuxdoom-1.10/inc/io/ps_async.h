#ifndef PS_ASYNC_H
#define PS_ASYNC_H
#include "ps_global.h"
#include "io/ps_file_io.h"

typedef void (*finish_async_callback)(void*);
typedef void (*handle_file_loaded)(void *, void*, u8*, u32);

void LoadASync(const char *name,
                void* object,
                void *params,
                handle_file_loaded loaderCB,
                finish_async_callback finish);

void InitASyncIO(int queueCount, float time);
void SuspendIOThread();
void WakeupIOThread();
void DeinitASyncIO();
void HandleASyncIO();
void DebugPrintIO();
#endif
