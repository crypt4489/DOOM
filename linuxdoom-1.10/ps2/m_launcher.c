#include "m_launcher.h"
#include "doomtype.h"
#include "doomdef.h"
#include "i_video.h"
#include "ps_global.h"
#include "gs/ps_gs.h"
#include "textures/ps_texture.h"
#include "textures/ps_font.h"
#include "gamemanager/ps_manager.h"
#include "log/ps_log.h"
#include "pad/ps_pad.h"

#include <stdlib.h>
#include <ctype.h>
#include <string.h>

extern u32 SKYDOOM_HEIGHT;
extern u32 SKYDOOM_WIDTH;

boolean runninglauncher = false;

Texture *background = NULL;
Font *fontimage = NULL;

u32 wadlistsize = 0;
u32 wadselect = 0;
u32 halfw = 0, halfh = 0;

char **wadlist;

GameMode_t *gamemodes;

extern GameMode_t gamemode;
extern char mainwad[25];

//pad stuff
extern u32 port;
extern u32 slot;
extern char padBuf[256];
static u32 old_pad = 0;
static u32 new_pad;
static u32 currData;

void M_LauncherInit(void)
{
    runninglauncher = true;

    background = AddAndCreateTexture("BACKGROUND.PNG", READ_PNG, 0, 0, TEX_ADDRESS_CLAMP, 1);

    fontimage = CreateFontStruct("DEFAULTFONT.BMP", "DEFAULTFONTDATA.DAT", READ_BMP);
    fontimage->color.r = 0x00;
    fontimage->color.g = 0x00;
    fontimage->color.b = 0x00;

    halfh = SKYDOOM_HEIGHT >> 1;
    halfw = SKYDOOM_WIDTH >> 1;

    wadlist = malloc(sizeof(char *) * 10);
    gamemodes = (GameMode_t *)malloc(sizeof(GameMode_t) * 10);
}

static void UpdatePad()
{
    struct padButtonStatus buttons;

    s32 state = padGetState(port, 0);

    if (state == PAD_STATE_DISCONN)
    {
        ERRORLOG("Pad(%d, %d) is disconnected", port, slot);
        return;
    }

    state = padRead(port, 0, &buttons);

    currData = 0xffff ^ buttons.btns;

    new_pad = currData & ~old_pad;

    if (new_pad & PAD_DOWN)
    {
        //DEBUGLOG("%d %d", wadlistsize, wadselect);
        if (wadselect > 0)
        {
            wadselect--;
        }
    }

    if (new_pad & PAD_UP)
    {
        //DEBUGLOG("%d %d", wadlistsize, wadselect);
        if (wadselect < wadlistsize - 1)
        {
            wadselect++;
        }
    }

    if (new_pad & PAD_CROSS)
    {
        runninglauncher = false;
        //mainwad = wadlist[wadselect];
        memcpy(mainwad, wadlist[wadselect], strlen(wadlist[wadselect]));
        gamemode = gamemodes[wadselect];
    }

    old_pad = currData;
}

static void CopyWADAndUpper(char *output, char *input)
{
    int len = strlen(input);
    for (int i = 0; i < len; i++)
    {
        output[i] = toupper(input[i]);
    }
    output[len + 1] = '\0';
}

void M_LauncherRun(void)
{
    for (int i = 0; i<wadlistsize; i++)
        CopyWADAndUpper(wadlist[i], wadlist[i]);
    while (runninglauncher)
    {
        UpdatePad();
        ClearScreen(g_Manager.targetBack, g_Manager.gs_context, 0, 0, 0, 255);
        DrawFullScreenQuad(halfh, halfw, background);
        PrintText(fontimage, wadlist[wadselect], 50, 200);
        EndFrame();
    }

    ClearScreen(g_Manager.targetBack, g_Manager.gs_context, 0, 0, 0, 255);
    EndFrame();

}

void M_LauncherDeinit(void)
{
    CleanFontStruct(fontimage);
    CleanTextureStruct(background);
    //ClearManagerTexList(&g_Manager);
    background = NULL;
    fontimage = NULL;
    for (int i = 0; i<wadlistsize; i++)
    {
        free(wadlist[i]);
    }
    free(wadlist);
    free(gamemodes);
}
