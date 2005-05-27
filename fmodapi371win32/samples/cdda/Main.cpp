//===============================================================================================
// CDDA.EXE
// Copyright (c), Firelight Technologies Pty, Ltd, 1999-2003.
//
// Use FMOD stream API to do digital CD playback
//===============================================================================================

#include <string.h>
#include <stdio.h>

#if defined(WIN32) || defined(__WATCOMC__)
    #include <windows.h>
    #include <conio.h>
#else
    #include "../../api/inc/wincompat.h"
    #include <string.h>
#endif

#include "../../api/inc/fmod.h"
#include "../../api/inc/fmod_errors.h"    // optional


/*
[
    [DESCRIPTION]

    [PARAMETERS]
 
    [RETURN_VALUE]

    [REMARKS]

    [SEE_ALSO]
]
*/
int main(int argc, char *argv[])
{
    unsigned char key;
    FSOUND_STREAM *stream;
    int channel = -1;
    int track = 0;
    char *cd_error;

    if (FSOUND_GetVersion() < FMOD_VERSION)
    {
        printf("Error : You are using the wrong DLL version!  You should be using FMOD %.02f\n", FMOD_VERSION);
        return 1;
    }

    if (!FSOUND_Init(44100, 32, 0))
    {
        printf("Error!\n");
        printf("%s\n", FMOD_ErrorString(FSOUND_GetError()));
        FSOUND_Close();
        return 1;
    }

    stream = FSOUND_Stream_Open("d:*?", FSOUND_NORMAL | FSOUND_NONBLOCKING, 0, 0);
    if (!stream)
    {
        printf("Error!\n");
        printf("%s\n", FMOD_ErrorString(FSOUND_GetError()));
        FSOUND_Close();
        return 1;
    }

    FSOUND_Stream_SetSubStream(stream, 0);

    printf("=========================================================================\n");
    printf("Press f        Skip forward 2 seconds\n");
    printf("      b        Skip back 2 seconds\n");
    printf("      n        Next track\n");
    printf("      SPACE    Pause/Unpause\n");
    printf("      ESC      Quit\n");
    printf("=========================================================================\n");

    key = 0;
    do
    {
        if (stream && (channel < 0))
        {
            int this_openstate = FSOUND_Stream_GetOpenState(stream);
            static int last_openstate = -1;

            if (this_openstate == -3)
            {
                if (FSOUND_Stream_FindTagField(stream, 0, "CD_ERROR", (void **)&cd_error, 0))
                {
                    printf("%s\n", cd_error);
                }
                else
                {
                    printf("ERROR: Couldn't open CDDA stream\n");
                }
                FSOUND_Stream_Close(stream);
                FSOUND_Close();
                return 1;
            }

            if ((last_openstate != 0) && (this_openstate == 0))
            {
                static int firsttime = TRUE;

                if (firsttime)
                {
                    char *cd_device_info;

                    if (!FSOUND_Stream_GetTagField(stream, 0, 0, 0, (void **)&cd_device_info, 0))
                    {
                        printf("ERROR: Couldn't get CD_DEVICE_INFO tag\n");
                        FSOUND_Stream_Close(stream);
                        FSOUND_Close();
                        return 1;
                    }

                    printf(cd_device_info);
                    printf("\n=========================================================================\n");
                    firsttime = FALSE;
                    
                    if (FSOUND_Stream_FindTagField(stream, 0, "CD_ERROR", (void **)&cd_error, 0))
                    {
                        printf("%s\n", cd_error);
                        FSOUND_Stream_Close(stream);
                        FSOUND_Close();
                        return 1;
                    }
                }

                channel = FSOUND_Stream_PlayEx(FSOUND_FREE, stream, 0, TRUE);
                FSOUND_SetPaused(channel, FALSE);
            }

            last_openstate = this_openstate;
        }

        if (kbhit())
        {
            key = getch();

            if (channel != -1)
            {
                if (key == ' ')
                {
                    FSOUND_SetPaused(channel, !FSOUND_GetPaused(channel));
                }

                if (key == 'f')
                {
                    FSOUND_Stream_SetTime(stream, FSOUND_Stream_GetTime(stream) + 2000);
                }

                if (key == 'b')
                {
                    FSOUND_Stream_SetTime(stream, FSOUND_Stream_GetTime(stream) - 2000);
                }

                if (key == 'n')
                {
                    track++;
                    if (track >= FSOUND_Stream_GetNumSubStreams(stream))
                    {
                        track = 0;
                    }
                    FSOUND_Stream_SetSubStream(stream, track);
                    channel = -1;
                }
            }
        }

        if (FSOUND_Stream_GetOpenState(stream) == 0)
        {
            printf("Track %d/%d  %02d:%02d/%02d:%02d  cpu %5.02f%%   \r", track + 1, 
                                                                          FSOUND_Stream_GetNumSubStreams(stream), 
                                                                          FSOUND_Stream_GetTime(stream) / 1000 / 60, 
                                                                          FSOUND_Stream_GetTime(stream) / 1000 % 60, 
                                                                          FSOUND_Stream_GetLengthMs(stream) / 1000 / 60, 
                                                                          FSOUND_Stream_GetLengthMs(stream) / 1000 % 60, 
                                                                          FSOUND_GetCPUUsage());
        }

        FSOUND_Update();
        Sleep(10);
    
    } while (key != 27);

    printf("\n");

    FSOUND_Stream_Close(stream);
    FSOUND_Close();

    return 0;
}
