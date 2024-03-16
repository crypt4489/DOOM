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
//	Main program, simply calls D_DoomMain high level loop.
//
//-----------------------------------------------------------------------------

static const char
rcsid[] = "$Id: i_main.c,v 1.4 1997/02/03 22:45:10 b1 Exp $";

#include "audsrvx.h"
#include <loadfile.h>
#include <iopheap.h>

#include "doomdef.h"
#include "i_video.h"

#include "m_argv.h"
#include "d_main.h"
#include "gamemanager/ps_manager.h"
#include "gs/ps_gs.h"
#include "log/ps_log.h"
#include "io/ps_file_io.h"
#include "math/ps_misc.h"

#include "tsf.h"
u32 SKYDOOM_HEIGHT = 480;
u32 SKYDOOM_WIDTH = 640;

tsf* gTsfInstance = NULL;

u32 *audioBuffer1 = NULL;
u32 *audioBuffer2 = NULL;
s32 sifTransferID = -1;
boolean bufferToWrite, bufferToRead;
int
main
( int		argc,
  char**	argv ) 
{ 
    InitializeSystem(0, SKYDOOM_WIDTH, SKYDOOM_HEIGHT, GS_PSM_24);
    I_InitGraphics();
    int ret;
    printf("kicking IRXs\n");
    ret = SifLoadModule("cdrom0:\\LIBSD.IRX", 0, NULL);
    printf("libsd loadmodule %d\n", ret);

    printf("loading audsrv\n");
    ret = SifLoadModule("cdrom0:\\AUDSRV.IRX", 0, NULL);
    printf("audsrv loadmodule %d\n", ret);

    ret = audsrv_init();

    audsrv_adpcm_init();

    struct audsrv_fmt_t audio;
    audio.freq = 22050;
    audio.bits = 16;
    audio.channels = 1;
    
    audsrv_set_format(&audio);

    audsrv_set_volume(MAX_VOLUME);

    audioBuffer1 = (u32*)SifAllocIopHeap(44100 * 5);
    audioBuffer2 = (u32*)SifAllocIopHeap(44100 * 5);

    if (!audioBuffer1 || !audioBuffer2)
    {
        ERRORLOG("Cannot allocate audio buffers");
    }

    audsrv_set_buffers(audioBuffer1, audioBuffer2, 44100*5, 44100*5);

    u32 sf2Size = 0;

    char buffer[25];

    Pathify("gzdoom.sf2", buffer);

    unsigned char *gzsf2 = ReadFileInFull(buffer, &sf2Size);
    
    DEBUGLOG("size of gzdoom %d", sf2Size);

    gTsfInstance = tsf_load_memory(gzsf2, sf2Size);

    tsf_set_output(gTsfInstance, TSF_MONO, 22050, 0.0f);




    myargc = argc; 
    myargv = argv; 
 
    D_DoomMain (); 

    return 0;
} 
