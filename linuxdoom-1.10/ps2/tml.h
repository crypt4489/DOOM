/* TinyMidiLoader - v0.7 - Minimalistic midi parsing library - https://github.com/schellingb/TinySoundFont
                                     no warranty implied; use at your own risk
   Do this:
      #define TML_IMPLEMENTATION
   before you include this file in *one* C or C++ file to create the implementation.
   // i.e. it should look like this:
   #include ...
   #include ...
   #define TML_IMPLEMENTATION
   #include "tml.h"

   [OPTIONAL] #define TML_NO_STDIO to remove stdio dependency
   [OPTIONAL] #define TML_MALLOC, TML_REALLOC, and TML_FREE to avoid stdlib.h
   [OPTIONAL] #define TML_MEMCPY to avoid string.h

   LICENSE (ZLIB)

   Copyright (C) 2017, 2018, 2020 Bernhard Schelling

   This software is provided 'as-is', without any express or implied
   warranty.  In no event will the authors be held liable for any damages
   arising from the use of this software.

   Permission is granted to anyone to use this software for any purpose,
   including commercial applications, and to alter it and redistribute it
   freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not
      claim that you wrote the original software. If you use this software
      in a product, an acknowledgment in the product documentation would be
      appreciated but is not required.
   2. Altered source versions must be plainly marked as such, and must not be
      misrepresented as being the original software.
   3. This notice may not be removed or altered from any source distribution.

*/

#ifndef TML_INCLUDE_TML_INL
#define TML_INCLUDE_TML_INL

#ifdef __cplusplus
extern "C" {
#endif

// Define this if you want the API functions to be static
#ifdef TML_STATIC
#define TMLDEF static
#else
#define TMLDEF extern
#endif

#define NUM_CHANNELS_TML 16

static unsigned char volumes_tml[NUM_CHANNELS_TML] = {127, 127, 127, 127,
							  127, 127, 127, 127,
							  127, 127, 127, 127,
							  127, 127, 127, 127,	}; 

static int channel_map_tml[NUM_CHANNELS_TML];

static const unsigned char controller_map_tml[NUM_CHANNELS_TML] =
{
    0x00, 0x20, 0x01, 0x07, 0x0A, 0x0B, 0x5B, 0x5D,
    0x40, 0x43, 0x78, 0x7B, 0x7E, 0x7F, 0x79
};

// Channel message type
enum TMLMessageType
{
	TML_NOTE_OFF = 0x80, TML_NOTE_ON = 0x90, TML_KEY_PRESSURE = 0xA0, TML_CONTROL_CHANGE = 0xB0, TML_PROGRAM_CHANGE = 0xC0, TML_CHANNEL_PRESSURE = 0xD0, TML_PITCH_BEND = 0xE0, TML_SET_TEMPO = 0x51
};

// Midi controller numbers
enum TMLController
{
	TML_BANK_SELECT_MSB,      TML_MODULATIONWHEEL_MSB, TML_BREATH_MSB,       TML_FOOT_MSB = 4,      TML_PORTAMENTO_TIME_MSB,   TML_DATA_ENTRY_MSB, TML_VOLUME_MSB,
	TML_BALANCE_MSB,          TML_PAN_MSB = 10,        TML_EXPRESSION_MSB,   TML_EFFECTS1_MSB,      TML_EFFECTS2_MSB,          TML_GPC1_MSB = 16, TML_GPC2_MSB, TML_GPC3_MSB, TML_GPC4_MSB,
	TML_BANK_SELECT_LSB = 32, TML_MODULATIONWHEEL_LSB, TML_BREATH_LSB,       TML_FOOT_LSB = 36,     TML_PORTAMENTO_TIME_LSB,   TML_DATA_ENTRY_LSB, TML_VOLUME_LSB,
	TML_BALANCE_LSB,          TML_PAN_LSB = 42,        TML_EXPRESSION_LSB,   TML_EFFECTS1_LSB,      TML_EFFECTS2_LSB,          TML_GPC1_LSB = 48, TML_GPC2_LSB, TML_GPC3_LSB, TML_GPC4_LSB,
	TML_SUSTAIN_SWITCH = 64,  TML_PORTAMENTO_SWITCH,   TML_SOSTENUTO_SWITCH, TML_SOFT_PEDAL_SWITCH, TML_LEGATO_SWITCH,         TML_HOLD2_SWITCH,
	TML_SOUND_CTRL1,          TML_SOUND_CTRL2,         TML_SOUND_CTRL3,      TML_SOUND_CTRL4,       TML_SOUND_CTRL5,           TML_SOUND_CTRL6,
	TML_SOUND_CTRL7,          TML_SOUND_CTRL8,         TML_SOUND_CTRL9,      TML_SOUND_CTRL10,      TML_GPC5, TML_GPC6,        TML_GPC7, TML_GPC8,
	TML_PORTAMENTO_CTRL,      TML_FX_REVERB = 91,      TML_FX_TREMOLO,       TML_FX_CHORUS,         TML_FX_CELESTE_DETUNE,     TML_FX_PHASER,
	TML_DATA_ENTRY_INCR,      TML_DATA_ENTRY_DECR,     TML_NRPN_LSB,         TML_NRPN_MSB,          TML_RPN_LSB,               TML_RPN_MSB,
	TML_ALL_SOUND_OFF = 120,  TML_ALL_CTRL_OFF,        TML_LOCAL_CONTROL,    TML_ALL_NOTES_OFF,     TML_OMNI_OFF, TML_OMNI_ON, TML_POLY_OFF, TML_POLY_ON
};

// A single MIDI message linked to the next message in time
typedef struct tml_message
{
	// Time of the message in milliseconds
	unsigned int time;

	// Type (see TMLMessageType) and channel number
	unsigned char type, channel;

	// 2 byte of parameter data based on the type:
	// - key, velocity for TML_NOTE_ON and TML_NOTE_OFF messages
	// - key, key_pressure for TML_KEY_PRESSURE messages
	// - control, control_value for TML_CONTROL_CHANGE messages (see TMLController)
	// - program for TML_PROGRAM_CHANGE messages
	// - channel_pressure for TML_CHANNEL_PRESSURE messages
	// - pitch_bend for TML_PITCH_BEND messages
	union
	{
		#ifdef _MSC_VER
		#pragma warning(push)
		#pragma warning(disable:4201) //nonstandard extension used: nameless struct/union
		#elif defined(__GNUC__)
		#pragma GCC diagnostic push
		#pragma GCC diagnostic ignored "-Wpedantic" //ISO C++ prohibits anonymous structs
		#endif

		struct { union { char key, control, program, channel_pressure; }; union { char velocity, key_pressure, control_value; }; };
		struct { unsigned short pitch_bend; };

		#ifdef _MSC_VER
		#pragma warning( pop )
		#elif defined(__GNUC__)
		#pragma GCC diagnostic pop
		#endif
	};

	// The pointer to the next message in time following this event
	struct tml_message* next;
} tml_message;

// The load functions will return a pointer to a struct tml_message.
// Normally the linked list gets traversed by following the next pointers.
// Make sure to keep the pointer to the first message to free the memory.
// On error the tml_load* functions will return NULL most likely due to an
// invalid MIDI stream (or if the file did not exist in tml_load_filename).

#ifndef TML_NO_STDIO
// Directly load a MIDI file from a .mid file path
TMLDEF tml_message* tml_load_filename(const char* filename);
#endif

// Load a MUS file from a block of memory
TMLDEF tml_message* tml_load_memory(const void* buffer, int size);

// Get infos about this loaded MIDI file, returns the note count
// NULL can be passed for any output value pointer if not needed.
//   used_channels:   Will be set to how many channels play notes
//                    (i.e. 1 if channel 15 is used but no other)
//   used_programs:   Will be set to how many different programs are used
//   total_notes:     Will be set to the total number of note on messages
//   time_first_note: Will be set to the time of the first note on message
//   time_length:     Will be set to the total time in milliseconds
TMLDEF int tml_get_info(tml_message* first_message, int* used_channels, int* used_programs, int* total_notes, unsigned int* time_first_note, unsigned int* time_length);

// Read the tempo (microseconds per quarter note) value from a message with the type TML_SET_TEMPO
TMLDEF int tml_get_tempo_value(tml_message* set_tempo_message);

// Free all the memory of the linked message list (can also call free() manually)
TMLDEF void tml_free(tml_message* f);

// Stream structure for the generic loading
struct tml_stream
{
	// Custom data given to the functions as the first parameter
	void* data;

	// Function pointer will be called to read 'size' bytes into ptr (returns number of read bytes)
	int (*read)(void* data, void* ptr, unsigned int size);
};

// Generic Midi loading method using the stream structure above
TMLDEF tml_message* tml_load(struct tml_stream* stream);

// If this library is used together with TinySoundFont, tsf_stream (equivalent to tml_stream) can also be used
struct tsf_stream;
TMLDEF tml_message* tml_load_tsf_stream(struct tsf_stream* stream);

#ifdef __cplusplus
}
#endif

// end header
// ---------------------------------------------------------------------------------------------------------
#endif //TML_INCLUDE_TML_INL

#ifdef TML_IMPLEMENTATION

#if !defined(TML_MALLOC) || !defined(TML_FREE) || !defined(TML_REALLOC)
#  include <stdlib.h>
#  define TML_MALLOC  malloc
#  define TML_FREE    free
#  define TML_REALLOC realloc
#endif

#if !defined(TML_MEMCPY)
#  include <string.h>
#  define TML_MEMCPY  memcpy
#endif

#ifndef TML_NO_STDIO
#  include <stdio.h>
#endif

#define TML_NULL 0

////crash on errors and warnings to find broken midi files while debugging
//#define TML_ERROR(msg) *(int*)0 = 0xbad;
//#define TML_WARN(msg)  *(int*)0 = 0xf00d;

////print errors and warnings
//#define TML_ERROR(msg) printf("ERROR: %s\n", msg);
//#define TML_WARN(msg)  printf("WARNING: %s\n", msg);

#ifndef TML_ERROR
#define TML_ERROR(msg)
#endif

#ifndef TML_WARN
#define TML_WARN(msg)
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifndef TML_NO_STDIO
static int tml_stream_stdio_read(FILE* f, void* ptr, unsigned int size) { return (int)fread(ptr, 1, size, f); }
TMLDEF tml_message* tml_load_filename(const char* filename)
{
	struct tml_message* res;
	struct tml_stream stream = { TML_NULL, (int(*)(void*,void*,unsigned int))&tml_stream_stdio_read };
	#if __STDC_WANT_SECURE_LIB__
	FILE* f = TML_NULL; fopen_s(&f, filename, "rb");
	#else
	FILE* f = fopen(filename, "rb");
	#endif
	if (!f) { TML_ERROR("File not found"); return 0; }
	stream.data = f;
	res = tml_load(&stream);
	fclose(f);
	return res;
}
#endif

struct tml_stream_memory { const char* buffer; unsigned int total, pos; };
static int tml_stream_memory_read(struct tml_stream_memory* m, void* ptr, unsigned int size) { if (size > m->total - m->pos) size = m->total - m->pos; TML_MEMCPY(ptr, m->buffer+m->pos, size); m->pos += size; return size; }
TMLDEF struct tml_message* tml_load_memory(const void* buffer, int size)
{
	struct tml_stream stream = { TML_NULL, (int(*)(void*,void*,unsigned int))&tml_stream_memory_read };
	struct tml_stream_memory f = { 0, 0, 0 };
	f.buffer = (const char*)buffer;
	f.total = size;
	stream.data = &f;
	return tml_load(&stream);
}

struct tml_track
{
	unsigned int Idx, End, Ticks;
};

struct tml_tempomsg
{
	unsigned int time;
	unsigned char type, Tempo[3];
	tml_message* next;
};

struct tml_parser
{
	unsigned char *buf, *buf_end; 
	int last_status, message_array_size, message_count;
};

enum TMLSystemType
{
	TML_TEXT  = 0x01, TML_COPYRIGHT    = 0x02, TML_TRACK_NAME     = 0x03, TML_INST_NAME     = 0x04, TML_LYRIC           = 0x05, TML_MARKER       = 0x06, TML_CUE_POINT = 0x07,
	TML_EOT   = 0x2f, TML_SMPTE_OFFSET = 0x54, TML_TIME_SIGNATURE = 0x58, TML_KEY_SIGNATURE = 0x59, TML_SEQUENCER_EVENT = 0x7f,
	TML_SYSEX = 0xf0, TML_TIME_CODE    = 0xf1, TML_SONG_POSITION  = 0xf2, TML_SONG_SELECT   = 0xf3, TML_TUNE_REQUEST    = 0xf6, TML_EOX          = 0xf7, TML_SYNC      = 0xf8,
	TML_TICK  = 0xf9, TML_START        = 0xfa, TML_CONTINUE       = 0xfb, TML_STOP          = 0xfc, TML_ACTIVE_SENSING  = 0xfe, TML_SYSTEM_RESET = 0xff
};

static int tml_readbyte(struct tml_parser* p)
{
	return (p->buf == p->buf_end ? -1 : *(p->buf++));
}

static int AllocateMIDIChannel_tml(void)
{
    int result;
    int max;
    int i;

    // Find the current highest-allocated channel.

    max = -1;

    for (i=0; i<NUM_CHANNELS_TML; ++i)
    {
        if (channel_map_tml[i] > max)
        {
            max = channel_map_tml[i];
        }
    }

    // max is now equal to the highest-allocated MIDI channel.  We can
    // now allocate the next available channel.  This also works if
    // no channels are currently allocated (max=-1)

    result = max + 1;

    // Don't allocate the MIDI percussion channel!

    if (result == 9)
    {
        ++result;
    }

    return result;
}


static int GetMIDIChannel_tml(int mus_channel, int *isnew)
{
    // Find the MIDI channel to use for this MUS channel.
    // MUS channel 15 is the percusssion channel.
	//printf("check %d\n", mus_channel);
    if (mus_channel == 15)
    {
        return 9;
    }
    {
        // If a MIDI channel hasn't been allocated for this MUS channel
        // yet, allocate the next free MIDI channel.

        if (channel_map_tml[mus_channel] == -1)
        {
            channel_map_tml[mus_channel] = AllocateMIDIChannel_tml();

            *isnew = 1;
        } 

        return channel_map_tml[mus_channel];
    }
}

static int tml_parsemessage(tml_message** f, struct tml_parser* p)
{
	static int deltatime = 0;
	int status = tml_readbyte(p), delay = 0, channel, isnew = 0; 
	tml_message* evt;
	delay = (status & 0x80);
	channel = GetMIDIChannel_tml((status & 0x0f), &isnew);

	if (p->message_array_size == p->message_count || (p->message_array_size == p->message_count-1 && isnew))
	{
		//start allocated memory size of message array at 64, double each time until 8192, then add 1024 entries until done
		p->message_array_size += (!p->message_array_size ? 64 : (p->message_array_size > 4096 ? 1024 : p->message_array_size));
		*f = (tml_message*)TML_REALLOC(*f, p->message_array_size * sizeof(tml_message));
		if (!*f) { TML_ERROR("Out of memory"); return -1; }
	}

	evt = *f + p->message_count;
	if (isnew)
	{
		evt->time = deltatime;
		evt->type = TML_CONTROL_CHANGE;
		evt->channel = channel;
		evt->control = 0x7b;
		evt->control_value = 0;
		evt++;
		p->message_count++;
	}
	
	int param, param2;
	param = tml_readbyte(p);
	evt->channel = channel;
	switch ((status & 0x70))
	{
		case 0:
			evt->type = TML_NOTE_OFF;
			evt->key = (param & 0x7f);
			evt->velocity = 0;
			break;
		case 16:
			evt->type = TML_NOTE_ON;
			evt->key = (param & 0x7f);
			if (param & 0x80)
			{
				evt->velocity = volumes_tml[channel] = (tml_readbyte(p) & 0x7f);
			}
			else 
			{
				evt->velocity = (volumes_tml[channel] & 0x7f);
			}
			break;
		case 64:
			evt->type = TML_CONTROL_CHANGE;
			evt->control = controller_map_tml[(param & 0xff)];
			param2 = (tml_readbyte(p));
			if (param2 & 0x80)
			{
				evt->control_value = 0x7f;
			}
			else 
			{
				evt->control_value = param2;
			}
			break;
		case 32:
			evt->pitch_bend = param * 64;
			evt->type = TML_PITCH_BEND;
			break;
		case 96:
			evt->type = TML_EOT;
			break;
		default: //ignore system/manufacture messages
			evt->type = 0;
			break;
	}
	
	if (deltatime || evt->type)
	{
		evt->time = deltatime;
		p->message_count++;
		deltatime = 0;
	}

	if (delay)
	{
		int timedelay = 0;
		while(1)
		{
				//printf("%d\n", *song);
			int read = 0;
			timedelay = (timedelay * 128) + ((read=tml_readbyte(p)) & 0x7F);
			if (!(read & 0x80))
			{
				break;
			}
		}
		deltatime+=timedelay;
	}

	return evt->type;
}

TMLDEF tml_message* tml_load(struct tml_stream* stream)
{
	int division = 70;
	unsigned char mus_header[16], *trackbuf = TML_NULL;
	struct tml_message* messages = TML_NULL;
	struct tml_track *track;
	struct tml_parser p = { TML_NULL, TML_NULL, 0, 0, 0 };

	// Parse MUS header
	if (stream->read(stream->data, mus_header, 16) != 16) { TML_ERROR("Unexpected end of file"); return messages; }
	if (mus_header[0] != 'M' || mus_header[1] != 'U' || mus_header[2] != 'S' || mus_header[3] != 0x1A) { TML_ERROR("Doesn't look like a MUS file: invalid MUS header"); return messages; }
	
	int lenSong = (int)(mus_header[5] << 8) | mus_header[4];
	int offSet = ((int)(mus_header[7] << 8) | mus_header[6])-16;
 
	track = (struct tml_track*)TML_MALLOC(sizeof(struct tml_track));
	track->Idx = track->End = track->Ticks = 0;

	for (int i = 0; i<16; i++)
		channel_map_tml[i] = -1;
	

	trackbuf = (unsigned char*)TML_MALLOC(lenSong + offSet);
	if (stream->read(stream->data, trackbuf, lenSong+offSet) != (lenSong+offSet)) { TML_WARN("Unexpected end of file"); return messages; }

	track->Idx = p.message_count;
	for (p.buf_end = (p.buf = (trackbuf + offSet)) + lenSong; p.buf != p.buf_end;)
	{
		int type = tml_parsemessage(&messages, &p);
		if (type == TML_EOT || type < 0) break;
	}
	if (p.buf != p.buf_end) { TML_WARN( "Track length did not match data length"); }
	track->End = p.message_count;
	
	TML_FREE(trackbuf);

	// Change message time signature from delta ticks to actual msec values and link messages ordered by time
	if (p.message_count)
	{
		tml_message *PrevMessage = TML_NULL, *Msg, *MsgEnd, Swap;
		unsigned int ticks = 0, tempo_ticks = 0; //tick counter and value at last tempo change
		int step_smallest, msec, tempo_msec = 0; //msec value at last tempo change
		float ticks2time = 500000 / (1000.0 * division); //milliseconds per tick

		// Loop through all messages over all track ordered by time
		for (step_smallest = 0; step_smallest != 0x7fffffff; ticks += step_smallest)
		{
			step_smallest = 0x7fffffff;
			msec = tempo_msec + (int)((ticks - tempo_ticks) * ticks2time);
			
			if (track->Idx == track->End) continue;
			for (Msg = &messages[track->Idx], MsgEnd = &messages[track->End]; Msg != MsgEnd && track->Ticks + Msg->time == ticks; Msg++, track->Idx++)
			{
				track->Ticks += Msg->time;
				if (Msg->type == TML_SET_TEMPO)
				{
					unsigned char* Tempo = ((struct tml_tempomsg*)Msg)->Tempo;
					ticks2time = ((Tempo[0]<<16)|(Tempo[1]<<8)|Tempo[2])/(1000.0 * division);
					tempo_msec = msec;
					tempo_ticks = ticks;
				}
				if (Msg->type)
				{
					Msg->time = msec;
					if (PrevMessage) { PrevMessage->next = Msg; PrevMessage = Msg; }
					else { Swap = *Msg; *Msg = *messages; *messages = Swap; PrevMessage = messages; }
				}
			}
			if (Msg != MsgEnd && track->Ticks + Msg->time > ticks)
			{
				int step = (int)(track->Ticks + Msg->time - ticks);
				if (step < step_smallest) step_smallest = step;
			}
		}
		
		if (PrevMessage) PrevMessage->next = TML_NULL;
		else p.message_count = 0;
	}
	TML_FREE(track);

	if (p.message_count == 0)
	{
		TML_FREE(messages);
		messages = TML_NULL;
	}

	return messages;
}

TMLDEF tml_message* tml_load_tsf_stream(struct tsf_stream* stream)
{
	return tml_load((struct tml_stream*)stream);
}

TMLDEF int tml_get_info(tml_message* Msg, int* out_used_channels, int* out_used_programs, int* out_total_notes, unsigned int* out_time_first_note, unsigned int* out_time_length)
{
	int used_programs = 0, used_channels = 0, total_notes = 0;
	unsigned int time_first_note = 0xffffffff, time_length = 0;
	unsigned char channels[16] = { 0 }, programs[128] = { 0 };
	for (;Msg; Msg = Msg->next)
	{
		time_length = Msg->time;
		if (Msg->type == TML_PROGRAM_CHANGE && !programs[(int)Msg->program]) { programs[(int)Msg->program] = 1; used_programs++; }
		if (Msg->type != TML_NOTE_ON) continue;
		if (time_first_note == 0xffffffff) time_first_note = time_length;
		if (!channels[Msg->channel]) { channels[Msg->channel] = 1; used_channels++; }
		total_notes++;
	}
	if (time_first_note == 0xffffffff) time_first_note = 0;
	if (out_used_channels  ) *out_used_channels   = used_channels;
	if (out_used_programs  ) *out_used_programs   = used_programs;
	if (out_total_notes    ) *out_total_notes     = total_notes;
	if (out_time_first_note) *out_time_first_note = time_first_note;
	if (out_time_length    ) *out_time_length     = time_length;
	return total_notes;
}

TMLDEF int tml_get_tempo_value(tml_message* msg)
{
	unsigned char* Tempo;
	if (!msg || msg->type != TML_SET_TEMPO) return 0;
	Tempo = ((struct tml_tempomsg*)msg)->Tempo;
	return ((Tempo[0]<<16)|(Tempo[1]<<8)|Tempo[2]);
}

TMLDEF void tml_free(tml_message* f)
{
	TML_FREE(f);
}

#ifdef __cplusplus
}
#endif

#endif //TML_IMPLEMENTATION
