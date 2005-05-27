// ===========================================================================

//  (C) 1999, 2000 Vienna University of Technology

// ===========================================================================

//  NAME:       videomodeWindows.cxx

//  TYPE:       c++ code

//  PROJECT:    Studierstube tools

//  CONTENT:    Change current videomode under Windows

//  VERSION:    1.1

// ===========================================================================

//  AUTHORS:    gh   Gerd Hesina

// ===========================================================================
//  HISTORY:
//
//  1999-Aug-02 00:00:00 gh initial version
// ===========================================================================
//
//  Purpose: set display settings. If no parameters are specified reset to 
//  default (does not sometimes work for some reason?)
//
//  Usage: win32normal.exe width height frequency

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

void errorMessage(LONG ret)

{
    fputs("ERROR: ", stderr);

    switch (ret) {

    case DISP_CHANGE_RESTART:

        fputs("The computer must be restarted", stderr);

        break;

    case DISP_CHANGE_BADFLAGS:

        fputs("Invalid set of flags", stderr);

        break;

    case DISP_CHANGE_BADPARAM:

        fputs("Invalid parameter", stderr);

        break;

    case DISP_CHANGE_FAILED:

        fputs("Driver failed", stderr);

        break;

    case DISP_CHANGE_BADMODE:

        fputs("Mode not supported", stderr);

        break;

    case DISP_CHANGE_NOTUPDATED:

        fputs("Unable to write registry", stderr);

        break;

    default:

        fputs("Unknown error", stderr);

    }

}



void main(int argc, char **argv)
{

    LONG ret;
    //no arguments: reset to registry default display settings
    if(argc==1)
	{
        ret = ChangeDisplaySettings(NULL,0);

        if (ret != DISP_CHANGE_SUCCESSFUL)

        {

            errorMessage(ret);

            exit(-1);

        }

		exit(0);
	}

    if(argc!=4)
	{
		fprintf(stderr,"usage: %s width height frequency\n", argv[0]);
		exit(-1);
	}


    DEVMODE devMode;


    devMode.dmSize             = sizeof(DEVMODE);
	devMode.dmDisplayFrequency = atoi(argv[3]);
	devMode.dmPelsWidth        = atoi(argv[1]);
	devMode.dmPelsHeight       = atoi(argv[2]);
	devMode.dmFields           = DM_PELSWIDTH|DM_PELSHEIGHT|DM_DISPLAYFREQUENCY;


    ret = ChangeDisplaySettings(&devMode, 0);


    if (ret != DISP_CHANGE_SUCCESSFUL)

    {

        errorMessage(ret);

        exit(-1);

    }

}
