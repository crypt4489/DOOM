// Emacs style mode select   -*- C++ -*-
//-----------------------------------------------------------------------------
//
// $Id:$
//
// Copyright (C) 1993-1996 by id Software, Inc.
//
// This source is available for distribution and/or modification
// only under the terms of the DOOM Source Code License as
// published by id Software. All rights reserved.
//
// The source is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// FITNESS FOR A PARTICULAR PURPOSE. See the DOOM Source Code License
// for more details.
//
// $Log:$
//
// DESCRIPTION:
//	DOOM graphics stuff for X11, UNIX.
//
//-----------------------------------------------------------------------------

static const char
	rcsid[] = "$Id: i_x.c,v 1.6 1997/02/03 22:45:10 b1 Exp $";

#include <stdlib.h>
#include <unistd.h>

#include <stdarg.h>
#include <sys/time.h>
#include <sys/types.h>

#include <signal.h>

#include "doomstat.h"
#include "i_system.h"
#include "v_video.h"
#include "m_argv.h"
#include "d_main.h"
#include "z_zone.h"
#include "doomdef.h"
#include "i_video.h"
// #include <draw2d.h>
// #include <draw3d.h>
#include <draw_primitives.h>
// #include <draw_types.h>
#include <gif_tags.h>
#include "pad/ps_pad.h"
#include "pipelines/ps_pipelineinternal.h"
#include "gameobject/ps_gameobject.h"
#include "pipelines/ps_vu1pipeline.h"
#include "dma/ps_dma.h"
#include "gs/ps_gs.h"
#include "system/ps_vumanager.h"
#include "textures/ps_texture.h"
#include "system/ps_vif.h"
#include "pipelines/ps_pipelinecbs.h"
#include "ps_global.h"
#include "log/ps_log.h"
#include "textures/ps_texture.h"
#include "gs/ps_gs.h"
#include "gamemanager/ps_manager.h"
#include "system/ps_timer.h"

extern u32 SKYDOOM_HEIGHT;
extern u32 SKYDOOM_WIDTH;

u32 SKYDOOM_HEIGHT_HALF;
u32 SKYDOOM_WIDTH_HALF;

// Fake mouse handling.
// This cannot work properly w/o DGA.
// Needs an invisible mouse cursor at least.

Texture *image;

float timestart, timeend;

extern u32 port;
extern u32 slot;
extern char padBuf[256];
static u32 old_pad = 0;
static u32 new_pad;
static u32 currData;
struct padButtonStatus buttons;
static u32 events_id[16] = {KEY_ESCAPE, KEY_SPEED, 0, KEY_PAUSE, KEY_UPARROW, KEY_RIGHTARROW, KEY_DOWNARROW, KEY_LEFTARROW, 0, KEY_FIRE, KEY_CYCLE_LEFT, KEY_CYCLE_RIGHT, KEY_BACKSPACE, KEY_SELECT, KEY_ENTER, KEY_TAB};
static u8 JoyRHPv = 127;
static u8 JoyLVPv = 127;
static u8 JoyLHPv = 127;
static u8 JoyRVPv = 127;
// static u8 framebuffer[320*240*4];
static u32 lower = 50;
static u32 upper = 200;
void UpdatePad()
{
	s32 state = padGetState(port, 0);
	event_t events[5];
	if (state == PAD_STATE_DISCONN)
	{
		ERRORLOG("Pad(%d, %d) is disconnected", port, slot);
		return;
	}

	state = padRead(port, 0, &buttons);

	if (state != 0)
	{
		currData = 0xffff ^ buttons.btns;

		new_pad = currData & ~old_pad;

		if (buttons.rjoy_h <= lower && JoyRHPv > lower)
		{
			DEBUGLOG("LOOK LEFT PRESSED %d %d",
					 buttons.rjoy_h, JoyRHPv);
			events[0].type = ev_keydown;
			events[0].data1 = KEY_LOOK_LEFT;
			D_PostEvent(&events[0]);
		}
		if (buttons.rjoy_h > lower && JoyRHPv <= lower)
		{
			DEBUGLOG("LOOK LEFT RELEASED %d %d",
					 buttons.rjoy_h, JoyRHPv);
			events[0].type = ev_keyup;
			events[0].data1 = KEY_LOOK_LEFT;
			D_PostEvent(&events[0]);
		}
		if (buttons.rjoy_h >= upper && JoyRHPv < upper)
		{
			DEBUGLOG("LOOK RIGHT PRESSED %d %d",
					 buttons.rjoy_h, JoyRHPv);
			events[0].type = ev_keydown;
			events[0].data1 = KEY_LOOK_RIGHT;
			D_PostEvent(&events[0]);
		}
		if (buttons.rjoy_h < upper && JoyRHPv >= upper)
		{
			DEBUGLOG("LOOK RIGHT RELEASED %d %d",
					 buttons.rjoy_h, JoyRHPv);
			events[1].type = ev_keyup;
			events[1].data1 = KEY_LOOK_RIGHT;
			D_PostEvent(&events[1]);
		}

		if (buttons.rjoy_v <= lower && JoyRVPv > lower)
		{
			DEBUGLOG("LOOK UP PRESSED %d %d",
					 buttons.rjoy_v, JoyRVPv);
			events[0].type = ev_keydown;
			events[0].data1 = KEY_LOOK_UP;
			D_PostEvent(&events[0]);
		}
		if (buttons.rjoy_v > lower && JoyRVPv <= lower)
		{
			DEBUGLOG("LOOK UP RELEASED %d %d",
					 buttons.rjoy_h, JoyRVPv);
			events[0].type = ev_keyup;
			events[0].data1 = KEY_LOOK_UP;
			D_PostEvent(&events[0]);
		}
		if (buttons.rjoy_v >= upper && JoyRVPv < upper)
		{
			DEBUGLOG("LOOK DOWN PRESSED %d %d",
					 buttons.rjoy_v, JoyRVPv);
			events[0].type = ev_keydown;
			events[0].data1 = KEY_LOOK_DOWN;
			D_PostEvent(&events[0]);
		}
		if (buttons.rjoy_v < upper && JoyRVPv >= upper)
		{
			DEBUGLOG("LOOK DOWN RELEASED %d %d",
					 buttons.rjoy_v, JoyRVPv);
			events[1].type = ev_keyup;
			events[1].data1 = KEY_LOOK_DOWN;
			D_PostEvent(&events[1]);
		}

		if (buttons.ljoy_h <= lower && JoyLHPv > lower)
		{
			DEBUGLOG("LEFT PRESSED %d %d", buttons.ljoy_h, JoyLHPv);
			events[1].type = ev_keydown;
			events[1].data1 = KEY_MOVE_LEFT;
			D_PostEvent(&events[1]);
		}
		if (buttons.ljoy_h > lower && JoyLHPv <= lower)
		{
			DEBUGLOG("LEFT RELEASED %d %d", buttons.ljoy_h, JoyLHPv);
			events[1].type = ev_keyup;
			events[1].data1 = KEY_MOVE_LEFT;
			D_PostEvent(&events[1]);
		}
		if (buttons.ljoy_h >= upper && JoyLHPv < upper)
		{
			DEBUGLOG("RIGHT PRESSED %d %d", buttons.ljoy_h, JoyLHPv);
			events[1].type = ev_keydown;
			events[1].data1 = KEY_MOVE_RIGHT;
			D_PostEvent(&events[1]);
		}
		if (buttons.ljoy_h < upper && JoyLHPv >= upper)
		{
			DEBUGLOG("RIGHT RELEASED %d %d", buttons.ljoy_h, JoyLHPv);
			events[1].type = ev_keyup;
			events[1].data1 = KEY_MOVE_RIGHT;
			D_PostEvent(&events[1]);
		}

		if (buttons.ljoy_v <= lower && JoyLVPv > lower)
		{

			DEBUGLOG("UP PRESSED %d %d", buttons.ljoy_v, JoyLVPv);
			events[2].type = ev_keydown;
			events[2].data1 = KEY_UPARROW;
			D_PostEvent(&events[2]);
		}
		if (buttons.ljoy_v > lower && JoyLVPv <= lower)
		{
			DEBUGLOG("UP RELEASED %d %d", buttons.ljoy_v, JoyLVPv);
			events[2].type = ev_keyup;
			events[2].data1 = KEY_UPARROW;
			D_PostEvent(&events[2]);
		}
		if (buttons.ljoy_v >= upper && JoyLVPv < upper)
		{
			DEBUGLOG("DOWN PRESSED %d %d", buttons.ljoy_v, JoyLVPv);
			events[2].type = ev_keydown;
			events[2].data1 = KEY_DOWNARROW;
			D_PostEvent(&events[2]);
		}
		if (buttons.ljoy_v < upper && JoyLVPv >= upper)
		{
			DEBUGLOG("DOWN RELEASED %d %d", buttons.ljoy_v, JoyLVPv);
			events[2].type = ev_keyup;
			events[2].data1 = KEY_DOWNARROW;
			D_PostEvent(&events[2]);
		}

		JoyRVPv = buttons.rjoy_v;
		JoyRHPv = buttons.rjoy_h;
		JoyLHPv = buttons.ljoy_h;
		JoyLVPv = buttons.ljoy_v;
		//	DEBUGLOG("%d %d", buttons.ljoy_v, JoyLVPv);
		//	DEBUGLOG("%d %d", buttons.ljoy_h, JoyLHPv);
		//	DEBUGLOG("%d %d", buttons.rjoy_h, JoyRHPv);

		int padType = 0x0001;
		for (int i = 0; padType != 0; i++)
		{
			if (new_pad & padType)
			{
				events[3].type = ev_keydown;
				events[3].data1 = events_id[i];
				D_PostEvent(&events[3]);
			}
			// release
			if (!(currData & padType) && (old_pad & padType))
			{
				events[3].type = ev_keyup;
				events[3].data1 = events_id[i];
				D_PostEvent(&events[3]);
			}

			padType <<= 1;
		}

		old_pad = currData;
	}
}

//
// I_StartFrame
//
void I_StartFrame(void)
{
	// er?
}
void I_ShutdownGraphics(void)
{
}

// I_StartTic
//
void I_StartTic(void)
{
	UpdatePad();
}

//
// I_UpdateNoBlit
//
void I_UpdateNoBlit(void)
{
	// what is this?
}
static Color colors[256];
//
// I_FinishUpdate
//
void I_FinishUpdate(void)
{

	byte *in = screens[0];
	for (int i = 0; i < SCREENHEIGHT * SCREENWIDTH; i++)
	{
		int inColor = in[i];
		int index = i * 4;
		image->pixels[index + 0] = colors[inColor].r;
		image->pixels[index + 1] = colors[inColor].g;
		image->pixels[index + 2] = colors[inColor].b;
	}

	timeend = getTicks(g_Manager.timer);

	if (timeend - timestart > 16)
	{
		ClearScreen(g_Manager.targetBack, g_Manager.gs_context, 0x00, 0xFF, 0x00, 0x00);
		DrawFullScreenQuad(SKYDOOM_HEIGHT_HALF, SKYDOOM_WIDTH_HALF, image);
		EndFrame();
		timestart = timeend;
	}
}

//
// I_ReadScreen
//
void I_ReadScreen(byte *scr)
{
	memcpy(scr, screens[0], SCREENWIDTH * SCREENHEIGHT);
}

//
// Palette stuff.
//

void UploadNewPalette(byte *palette)
{
}

//
// I_SetPalette
//
void I_SetPalette(byte *palette)
{
	int i;

	// set the X colormap entries

	byte *clut_palette = palette;

	for (i = 0; i < 256; i++)
	{
		/*
		DEBUGLOG("%d %d %d", gammatable[usegamma][*clut_palette],
		gammatable[usegamma][*clut_palette+1], gammatable[usegamma][*clut_palette+2]);
		*/
		int c = gammatable[usegamma][*clut_palette++];
		colors[i].r = c;
		c = gammatable[usegamma][*clut_palette++];
		colors[i].g = c;
		c = gammatable[usegamma][*clut_palette++];
		colors[i].b = c;
	}
}

void DrawFullScreenQuad(int height, int width, Texture *_image)
{
	UploadTextureToVRAM(_image);
	qword_t *ret = InitializeDMAObject();

	// u64 reglist = ((u64)DRAW_UV_REGLIST) << 8 | DRAW_UV_REGLIST;

	qword_t *dcode_tag_vif1 = ret;
	ret++;

	u8 red, green, blue, alpha;

	red = green = blue = 0xFF;

	alpha = 0x80;

	ret = CreateDMATag(ret, DMA_CNT, 3, 0, 0, 0);

	ret = CreateDirectTag(ret, 2, 0);

	ret = CreateGSSetTag(ret, 1, 1, GIF_FLG_PACKED, 1, GIF_REG_AD);

	ret = SetupZTestGS(ret, 1, 0, 0x80, ATEST_METHOD_NOTEQUAL, ATEST_KEEP_FRAMEBUFFER, 0, 0, g_Manager.gs_context);

	qword_t *dmatag = ret;
	ret++;
	qword_t *direct = ret;

	ret++;
	PACK_GIFTAG(ret, GIF_SET_TAG(1, 1, 0, 0, GIF_FLG_PACKED, 1), GIF_REG_AD);
	ret++;

	PACK_GIFTAG(ret, GS_SET_PRIM(PRIM_TRIANGLE_STRIP, PRIM_SHADE_GOURAUD, DRAW_ENABLE, DRAW_DISABLE, DRAW_DISABLE, DRAW_DISABLE, PRIM_MAP_UV, g_Manager.gs_context, PRIM_UNFIXED), GS_REG_PRIM);
	ret++;

	u32 regCount = 3;

	u64 regFlag = ((u64)GIF_REG_RGBAQ) << 0 | ((u64)GIF_REG_UV) << 4 | ((u64)GIF_REG_XYZ2) << 8;

	PACK_GIFTAG(ret, GIF_SET_TAG(4, 1, 0, 0, GIF_FLG_REGLIST, regCount), regFlag);
	ret++;

	int u0 = 0;
	int v0 = 0;

	int u1 = ((_image->width) << 4);
	int v1 = ((_image->height) << 4);

	PACK_GIFTAG(ret, GIF_SET_RGBAQ(red, green, blue, alpha, 1), GIF_SET_UV(u0, v0));
	ret++;

	PACK_GIFTAG(ret, GIF_SET_XYZ(CreateGSScreenCoordinates(width, -), CreateGSScreenCoordinates(height, -), 0xFFFFFF), GIF_SET_RGBAQ(red, green, blue, alpha, 1));
	ret++;
	PACK_GIFTAG(ret, GIF_SET_UV(u0, v1), GIF_SET_XYZ(CreateGSScreenCoordinates(width, -), CreateGSScreenCoordinates(height, +), 0xFFFFFF));
	ret++;
	PACK_GIFTAG(ret, GIF_SET_RGBAQ(red, green, blue, alpha, 1), GIF_SET_UV(u1, v0));

	ret++;
	PACK_GIFTAG(ret, GIF_SET_XYZ(CreateGSScreenCoordinates(width, +), CreateGSScreenCoordinates(height, -), 0xFFFFFF), GIF_SET_RGBAQ(red, green, blue, alpha, 1));
	ret++;
	PACK_GIFTAG(ret, GIF_SET_UV(u1, v1), GIF_SET_XYZ(CreateGSScreenCoordinates(width, +), CreateGSScreenCoordinates(height, +), 0xFFFFFF));

	ret++;

	CreateDMATag(dmatag, DMA_END, ret - dmatag - 1, 0, 0, 0);

	CreateDirectTag(direct, ret - direct - 1, 1);

	u32 sizeOfPipeline = ret - dcode_tag_vif1 - 1;

	CreateDCODEDmaTransferTag(dcode_tag_vif1, DMA_CHANNEL_VIF1, 0, 1, sizeOfPipeline);

	CreateDCODETag(ret, DMA_DCODE_END);

	SubmitDMABuffersAsPipeline(ret, NULL);
}

void I_InitGraphics(void)
{
	SKYDOOM_HEIGHT_HALF = SKYDOOM_HEIGHT >> 1;
	SKYDOOM_WIDTH_HALF = SKYDOOM_WIDTH >> 1;

	image = (Texture *)malloc(sizeof(Texture));

	image->width = SCREENWIDTH;
	image->height = SCREENHEIGHT;
	image->psm = GS_PSM_32;

	InitTextureResources(image, 0);

	image->lod.mag_filter = LOD_MAG_LINEAR;
	image->lod.min_filter = LOD_MIN_LINEAR;

	image->lod.l = 0;
	image->lod.k = 0.0f;
	image->lod.calculation = LOD_USE_K;
	image->lod.max_level = 0;

	image->pixels = (u8 *)malloc(320 * 240 * 4);

	// AddToManagerTexList(&g_Manager, image);

	timestart = timeend = getTicks(g_Manager.timer);
}
