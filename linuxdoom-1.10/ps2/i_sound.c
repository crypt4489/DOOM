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
//	System interface for sound.
//
//-----------------------------------------------------------------------------

static const char
    rcsid[] = "$Id: i_unix.c,v 1.5 1997/02/03 22:45:10 b1 Exp $";

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include <math.h>

#include <sys/time.h>
#include <sys/types.h>

#include <fcntl.h>
#include <unistd.h>

// Timer stuff. Experimental.
#include <time.h>
#include <signal.h>

#include "z_zone.h"

#include "i_system.h"
#include "i_sound.h"
#include "m_argv.h"
#include "m_misc.h"
#include "w_wad.h"

#include "doomdef.h"

#include <audsrv.h>
#include "audio/ps_sound.h"
// UNIX hack, to be removed.


// Get the interrupt. Set duration in millisecs.
int I_SoundSetTimer(int duration_of_tick);
void I_SoundDelTimer(void);


// A quick hack to establish a protocol between
// synchronous mix buffer updates and asynchronous
// audio writes. Probably redundant with gametic.
static int flag = 0;

// The number of internal mixing channels,
//  the samples calculated for each mixing step,
//  the size of the 16bit, 2 hardware channel (stereo)
//  mixing buffer, and the samplerate of the raw data.

// Needed for calling the actual sound output.
#define NUM_CHANNELS 24


// The actual lengths of all sound effects.
int lengths[NUMSFX];

audsrv_adpcm_t samples[NUMSFX];
audsrv_adpcm_t *samplestoplay[NUM_CHANNELS];
static int volumes[NUM_CHANNELS];
VagFile *vagFiles[NUMSFX];
static int currentSampleIndex = 0;
static int sampleToPlayIndex = 0;
static int vagFileIndex = 0;

//
// Safe ioctl, convenience.
//
void myioctl(int fd,
             int command,
             int *arg)
{

}

//
// This function loads the sound data from the WAD lump,
//  for single sound.
//

#include "log/ps_log.h"

void *
getsfx(char *sfxname,
       int *len)
{
  unsigned char *sfx;

  int size;
  
  char name[20];
  int sfxlump;

  // static int count = 0;
  //  Get the sound data from the WAD, allocate lump
  //   in zone memory.
  sprintf(name, "ds%s", sfxname);

  // Now, there is a severe problem with the
  //  sound handling, in it is not (yet/anymore)
  //  gamemode aware. That means, sounds from
  //  DOOM II will be requested even with DOOM
  //  shareware.
  // The sound list is wired into sounds.c,
  //  which sets the external variable.
  // I do not do runtime patches to that
  //  variable. Instead, we will use a
  //  default sound for replacement.
  if (W_CheckNumForName(name) == -1)
    sfxlump = W_GetNumForName("dspistol");
  else
    sfxlump = W_GetNumForName(name);

  size = W_LumpLength(sfxlump);

  sfx = (unsigned char *)W_CacheLumpNum(sfxlump, PU_STATIC);

  u16 sampleRate = *((u16*)(sfx + 2));

  VagFile *file = ConvertRawPCMToVag(sfx + 0x18, size - 0x18, sampleRate, 1, 8);

  Z_Free(sfx);

  *len = file->header.dataLength + 16;

  u8 *buffer = file->samples;

  vagFiles[vagFileIndex++] = file;
  
  return (void *)(buffer);
}

//
// This function adds a sound to the
//  list of currently active sounds,
//  which is maintained as a given number
//  (eight, usually) of internal channels.
// Returns a handle.
//
int addsfx(int sfxid,
           int volume,
           int step,
           int seperation)
{

  //audsrv_load_adpcm(&samples[sfxid], S_sfx[sfxid].data, lengths[sfxid]);

  return 0;
}

//
// SFX API
// Note: this was called by S_Init.
// However, whatever they did in the
// old DPMS based DOS version, this
// were simply dummies in the Linux
// version.
// See soundserver initdata().
//
void I_SetChannels()
{

}

void I_SetSfxVolume(int volume)
{
  // Identical to DOS.
  // Basically, this should propagate
  //  the menu/config file setting
  //  to the state variable used in
  //  the mixing.
  snd_SfxVolume = volume;
}

// MUSIC API - dummy. Some code from DOS version.
void I_SetMusicVolume(int volume)
{
  // Internal state variable.
  snd_MusicVolume = volume;
  // Now set volume on output device.
  // Whatever( snd_MusciVolume );
}

//
// Retrieve the raw data lump index
//  for a given SFX name.
//
int I_GetSfxLumpNum(sfxinfo_t *sfx)
{
  char namebuf[9];
  sprintf(namebuf, "ds%s", sfx->name);
  return W_GetNumForName(namebuf);
}


static boolean I_CountSoundPlaying(int id, int count)
{
  audsrv_adpcm_t *sample = &samples[id];
  int countl = 0;
  for (int i = 0; i<NUM_CHANNELS; i++)
  {
    if (audsrv_is_adpcm_playing(i, sample))
    {
      countl++;
    }

    if (countl >= count)
    {
      return false;
    }
  }
  
  
  return true;
}

//
// Starting a sound means adding it
//  to the current list of active sounds
//  in the internal channels.
// As the SFX info struct contains
//  e.g. a pointer to the raw data,
//  it is ignored.
// As our sound handling does not handle
//  priority, it is ignored.
// Pitching (that is, increased speed of playback)
//  is set, but currently not used by mixing.
//
int I_StartSound(int id,
                 int vol,
                 int sep,
                 int pitch,
                 int priority,
                 int count)
{
  if (!I_CountSoundPlaying(id, count))
  {
    return -1;
  }
  if (sampleToPlayIndex >= NUM_CHANNELS)
    sampleToPlayIndex = 0;
  samplestoplay[sampleToPlayIndex] = &samples[id];
  volumes[sampleToPlayIndex++] = MAX_VOLUME * ((float)vol/15);
  return id;
}

void I_StopSound(int handle)
{
  audsrv_adpcm_t *sample = &samples[handle];
  for (int i = 0; i<NUM_CHANNELS; i++)
  {
    if (audsrv_is_adpcm_playing(i, sample))
    {
      audsrv_adpcm_set_volume(i, 0);
    }
  }
}

int I_SoundIsPlaying(int handle)
{
  // Ouch.
  return gametic < handle;
}

//
// This function loops all active (internal) sound
//  channels, retrieves a given number of samples
//  from the raw sound data, modifies it according
//  to the current (internal) channel parameters,
//  mixes the per channel samples into the global
//  mixbuffer, clamping it to the allowed range,
//  and sets up everything for transferring the
//  contents of the mixbuffer to the (two)
//  hardware channels (left and right, that is).
//
// This function currently supports only 16bit.
//
void I_UpdateSound(void)
{
  // if we just paused game, mute all game sounds, but play menu sounds
  static boolean justhappened = true;
  if (paused && justhappened)
  {
    justhappened = false;
    for (int i = NUM_CHANNELS; i>=0; i--)
      audsrv_adpcm_set_volume(i, 0);
    return;
  } else if (!(paused || justhappened)) {
    justhappened = true;
  }
  // find a channel to play a sample
  for (int i = currentSampleIndex; i < NUM_CHANNELS && samplestoplay[i]; i++)
  {
      int channel = audsrv_ch_play_adpcm(i, samplestoplay[i]);
      audsrv_adpcm_set_volume(channel, volumes[i]);
      samplestoplay[i] = NULL;
      currentSampleIndex++;
  }

  if (currentSampleIndex >= NUM_CHANNELS)
    currentSampleIndex = 0;
}



//
// This would be used to write out the mixbuffer
//  during each game loop update.
// Updates sound buffer and audio device at runtime.
// It is called during Timer interrupt with SNDINTR.
// Mixing now done synchronous, and
//  only output be done asynchronous?
//
void I_SubmitSound(void)
{
  
}

void I_UpdateSoundParams(int handle,
                         int vol,
                         int sep,
                         int pitch)
{
  // I fail too see that this is used.
  // Would be using the handle to identify
  //  on which channel the sound might be active,
  //  and resetting the channel parameters.

  // UNUSED.
  handle = vol = sep = pitch = 0;
}

void I_ShutdownSound(void)
{
  return;
}

void I_InitSound()
{
  // Secure and configure sound device first.
  printf("I_InitSound: ");

  for (int i = 0; i < NUM_CHANNELS; i++)
  {
    samplestoplay[i] = NULL;
  }

  for (int i = 1; i < NUMSFX; i++)
  {
    // Alias? Example is the chaingun sound linked to pistol.
    if (!S_sfx[i].link)
    {
      // Load data from WAD file.
      S_sfx[i].data = getsfx(S_sfx[i].name, &lengths[i]);
    }
    else
    {
      // Previously loaded already?
      S_sfx[i].data = S_sfx[i].link->data;
      lengths[i] = lengths[(S_sfx[i].link - S_sfx) / sizeof(sfxinfo_t)];
    }
    audsrv_load_adpcm(&samples[i], S_sfx[i].data, lengths[i]);
  }

  printf(" pre-cached all sound data\n");

  // Finished initialization.
  printf("I_InitSound: sound module ready\n");
}

//
// MUSIC API.
// Still no music done.
// Remains. Dummies.
//
void I_InitMusic(void) {}
void I_ShutdownMusic(void) {}

static int looping = 0;
static int musicdies = -1;

void I_PlaySong(int handle, int looping)
{
  // UNUSED.
  handle = looping = 0;
  musicdies = gametic + TICRATE * 30;
}

void I_PauseSong(int handle)
{
  // UNUSED.
  handle = 0;
}

void I_ResumeSong(int handle)
{
  // UNUSED.
  handle = 0;
}

void I_StopSong(int handle)
{
  // UNUSED.
  handle = 0;

  looping = 0;
  musicdies = 0;
}

void I_UnRegisterSong(int handle)
{
  // UNUSED.
  handle = 0;
}

int I_RegisterSong(void *data)
{
  // UNUSED.
  data = NULL;

  return 1;
}

// Is the song playing?
int I_QrySongPlaying(int handle)
{
  // UNUSED.
  handle = 0;
  return looping || musicdies > gametic;
}

//
// Experimental stuff.
// A Linux timer interrupt, for asynchronous
//  sound output.
// I ripped this out of the Timer class in
//  our Difference Engine, including a few
//  SUN remains...
//


// Interrupt handler.
void I_HandleSoundTimer(int ignore)
{
  return;
}

// Get the interrupt. Set duration in millisecs.
int I_SoundSetTimer(int duration_of_tick)
{
  return 0;
}

// Remove the interrupt. Set duration to zero.
void I_SoundDelTimer()
{

}
