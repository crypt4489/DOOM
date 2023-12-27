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
//	Cheat sequence checking.
//
//-----------------------------------------------------------------------------

static const char
    rcsid[] = "$Id: m_cheat.c,v 1.1 1997/02/03 21:24:34 b1 Exp $";

#include "m_cheat.h"
#include "doomdef.h"

//
// CHEAT SEQUENCE PACKAGE
//

static int firsttime = 1;
static unsigned char cheat_xlate_table[256];

#include "log/ps_log.h"
boolean cht_compareCheat(cheatseq_t *cheat, char *string)
{
    int i;
    if (firsttime)
    {
        firsttime = 0;
        for (i = 0; i < 256; i++)
            cheat_xlate_table[i] = SCRAMBLE(i);
    }
    int len = strlen(string);
    unsigned char *cheat_sequence = cheat->sequence;
    
    for (i = 0; i < len && (cheat_sequence[i] != 0xff && cheat_sequence[i] != 1) ; i++)
    {
        if (cheat_xlate_table[string[i]] != cheat_sequence[i])
        {
            return false;
        }
    }
    return true;
}

//
// Called in st_stuff module, which handles the input.
// Returns a 1 if the cheat was successful, 0 if failed.
//
int cht_CheckCheat(cheatseq_t *cht,
                   char key)
{
    int i;
    int rc = 0;

    if (!cht->p)
        cht->p = cht->sequence; // initialize if first time

    if (*cht->p == 0)
        *(cht->p++) = key;
    else if (cheat_xlate_table[(unsigned char)key] == *cht->p)
        cht->p++;
    else
        cht->p = cht->sequence;

    if (*cht->p == 1)
        cht->p++;
    else if (*cht->p == 0xff) // end of sequence character
    {
        cht->p = cht->sequence;
        rc = 1;
    }

    return rc;
}

void cht_GetParam(cheatseq_t *cht,
                  char *buffer)
{

    unsigned char *p, c;

    p = cht->sequence;
    while (*(p++) != 1)
        ;

    do
    {
        c = *p;
        *(buffer++) = c;
        *(p++) = 0;
    } while (c && *p != 0xff);

    if (*p == 0xff)
        *buffer = 0;
}

void cht_GetParamString(cheatseq_t *cht, char *string,
                  char *buffer)
{

    char *p, *cs, c;

    p = string;
    cs = cht->sequence;
    while (*(cs++) != 1)
    {
        p++;
    }
        

    do
    {
        c = *p;
        *(buffer++) = c;
        *(p++) = 0;

    } while (c && *(cs++) != 0xff);

    if (*p == 0xff)
        *buffer = 0;
}