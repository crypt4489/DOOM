#ifndef PS_TEXTURE_IO_H
#define PS_TEXTURE_IO_H

#include "ps_global.h"

#define BITMAP_ID      0x4D42

typedef struct __attribute__((packed)) BitmapFileHeader
{
	u16  bfType;
    u32  bfSize;
    u16  bfReserved1;
    u16  bfReserved2;
    u32  bfOffBits;
} BitmapFileHeader;

typedef struct __attribute__((packed)) BitmapInfoHeader
{
	u32	biSize ;
	int	biWidth;
	int	biHeight;
	u16	biPlanes;
	u16	biBitCount;
	u32	biCompression;
	u32	biSizeImage;
	int	biXPelsPerMeter;
	int	biYPelsPerMeter;
	u32	biClrUsed;
	u32	biClrImportant;
} BitmapInfoHeader;

typedef struct create_tex_params_t
{
	char* name;
	u32 readType;
	u32 alpha;
	u32 useAlpha;
} CreateTextureParams;

void LoadBitmap(u8 *buffer, Texture *tex, unsigned char useAlpha, unsigned char alpha);

void LoadPng(u8 *data, Texture *tex, u32 size, u8 useAlpha, u8 alphaVal);

void CreateTextureFromFile(void* object, void* arg, u8 *buffer, u32 bufferLen);

Texture *ReadTexFile(const char *fileName, char *name, u32 readType, u8 alpha, u8 useAlpha);

#endif
