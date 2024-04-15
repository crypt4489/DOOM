#ifndef PS_SOUNDTYPES_H
#define PS_SOUNDTYPES_H

#include "ps_global.h"

static float vaglut[5][2] = { { 0.0, 0.0 },
							{  -60.0 / 64.0, 0.0 },
							{ -115.0 / 64.0, 52.0 / 64.0 },
							{  -98.0 / 64.0, 55.0 / 64.0 },
							{ -122.0 / 64.0, 60.0 / 64.0 } };

enum VAGFlag
{
	VAGF_NOTHING = 0,         /* Nothing*/
	VAGF_LOOP_LAST_BLOCK = 1, /* Last block to loop */
	VAGF_LOOP_REGION = 2,     /* Loop region*/
	VAGF_LOOP_END = 3,        /* Ending block of the loop */
	VAGF_LOOP_FIRST_BLOCK = 4,/* First block of looped data */
	VAGF_UNK = 5,             /* ?*/
	VAGF_LOOP_START = 6,      /* Starting block of the loop*/
	VAGF_PLAYBACK_END = 7     /* Playback ending position */
};

typedef struct enc_block_t
{
	s8 shift;
	s8 predict;
	u8 flags;
	u8 sample[14];
} EncBlock;

#endif