// ===========================================================================
//  (C) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:       videomode.cxx
//  TYPE:       c++ header
//  PROJECT:    Studierstube/VT
//  CONTENT:    commandline tool to switch the videomode on an SGI Impact board
//              between a default mode and 800x600 60Hz, 2 channels.
//              (the glasstron stereo mode!) see README for more information
//  VERSION:    1.31
// ===========================================================================
//  AUTHORS:    gh   Gerd Hesina
// ===========================================================================
//  HISTORY:
//  14-apr-00 00:05:00  gh      last modification (added commandline options)
//  07-apr-00 10:00:00  gh      created
// ===========================================================================

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stbapi/util/option.h>

void main(int argc, char **argv)
{
    OptionFlag optGlasstronMode("glasstron","g");
    OptionFlag optNormalMode("normal","n");
    if(!Option::parse(argc,argv)) { Option::printUsage(); exit(-1); }

    int glasstronFlag=optGlasstronMode.isSpecified();
    int normalFlag=optNormalMode.isSpecified();
    
    if(glasstronFlag&&normalFlag)
    {
        fprintf(stderr,"Please specify only "
                       "ONE videomode (glasstron OR normal)!");
        exit(-1);
    }
    if(glasstronFlag)  // switch to 800x600 60Hz, 2 channels
        system("cd /var/X11/Xvc/; cp 2@800x600_60_ext Impact0_TimingTable;/usr/gfx/stopgfx;/usr/gfx/startgfx");
    else    // switch to default mode
        system("cd /var/X11/Xvc/; cp default Impact0_TimingTable;/usr/gfx/stopgfx;/usr/gfx/startgfx");
}
