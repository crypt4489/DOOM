#ifndef PS_SOUND_H
#define PS_SOUND_H
#define	SOUND_IRX 0x14410998
#include "ps_global.h"

struct wavfile_header_t __attribute__((packed));
typedef struct wavfile_header_t WavFileHeader;
struct wavfile_holder_t;
typedef struct wavfile_holder_t WavFile;

struct vagfile_header_t __attribute__((packed));
typedef struct vagfile_header_t VagFileHeader;
struct vagfile_holder_t;
typedef struct vagfile_holder_t VagFile;

struct wavfile_header_t {
	char	riff_tag[4];
	int	riff_length;
	char	wav_tag[4];
	char	fmt_tag[4];
	int	fmt_length;
	short	audio_format;
	short	num_channels;
	int	sample_rate;
	int	byte_rate;
	short	block_align;
	short	bits_per_sample;
	char	data_tag[4];
	int	data_length;
};

struct wavfile_holder_t
{
	WavFileHeader header;
	u8 *samples;
};


struct vagfile_header_t {
	u8 magic[4];
	u32 version;
	u32 reserved4;
	u32 dataLength;
	u32 sampleRate;
	u8 reserved10[10];
	u8 channels;
	u8 reservedbyte;
	s8 filename[16];
};

struct vagfile_holder_t {
	struct vagfile_header_t header;
	u8 *samples;
};

WavFile *LoadWavFile(const char *name);

VagFile *LoadVagFile(const char *name);

void InitSound();

void LoadSample(unsigned char *wav, int size);

int WaitAudio(int buflen);

u8* CreateVagSamples(s16* samples, u32 len, u32* outSize, u32 loopStart, u32 loopEnd, u32 loopFlag);

s16 *Convert16PCMToShortSamples(u8 *buffer, u32 size);

s16* Convert8PCMToShortSamples(u8 *buffer, u32 size);

void CreateVagSamplesBuffer(VagFile *vagFile, u8 *buffer);

VagFile *ConvertRawPCMToVag(u8 *buffer, u32 size, u32 sampleRate, u32 channels, u32 bitDepth);

#endif