#ifndef PSTEXTURE_H
#define PSTEXTURE_H

#include "ps_global.h"

#define PS_FILTER_NNEIGHBOR 0
#define PS_FILTER_BILINEAR 1

#define SetFilters(tex, filter) \
        do { \
            (tex)->lod.min_filter = filter; \
            (tex)->lod.mag_filter = filter; \
        } while(0)

#define SetMagFilter(tex, filter) \
        do { \
            (tex)->lod.mag_filter = filter; \
        } while(0)

#define SetMinFilter(tex, filter) \
        do { \
            (tex)->lod.min_filter = filter; \
        } while(0)

void CleanTextureStruct(Texture *tex);
void AddStringNameToTexture(Texture *tex, const char *buffer);
Texture *AddAndCreateTexture(const char *filePath, u32 readType, u8 useProgrammedAlpha, u8 alphaVal, u32 mode, u8 texFiltering);
Texture *AddAndCreateAlphaMap(const char *filePath, u32 readType, u32 mode);
void InitTextureResources(Texture *tex, u32 mode);
//u32 CompareTextureNames(Texture *tex1, Texture *tex2);
qword_t* CreateTexChain(qword_t *input, Texture *tex);
qword_t *CreateTexChainWOTAGS(qword_t *input, Texture *tex);
void UploadTextureViaManagerToVRAM(Texture *tex);
void UploadTextureToVRAM(Texture *tex);
Texture *AddAndCreateTextureFromBuffer(u8 *fileData, u32 size, 
                                       const char *nameOfTex, u32 readType, 
                                       u8 useAlpha, u8 alpha, 
                                       u32 mode, u8 texFiltering);
//void UploadTextureViaManagerToVRAM(Texture *tex);

u32 GetTextureIDByName(const char *name, TexManager *TexManager);
Texture *GetTextureByID(u32 id, TexManager *texManager);
void AddMipMapTexture(Texture *tex, Texture *toAdd);
Texture *RemoveMipLevelFromTexture(Texture *tex, Texture *toRemove);
void SetupTexLODStruct(Texture *tex, float _k, char _l, int max, int filter_min, int filter_mag);
void SampleTextureByPixel(Texture *tex, Color *rgba, int x, int y);
void SampleTextureByUV(Texture *tex, Color *rgba, float x, float y);
#endif
