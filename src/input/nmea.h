/******************************************************************************
*									      *
*	File:     dgpsip.h						      *
*	Author:   Wolfgang Rupprecht <wolfgang@capsicum.wsrcc.com>	      *
*	Created:  Sat Feb  6 14:08:47 PST 1999				      *
*	Contents: data structs that are shared between files                  *
*									      *
*	Copyright (c) 1999 Wolfgang Rupprecht.				      *
*	All rights reserved.						      *
*									      *
*	$Id: nmea.h,v 1.1 2003/03/27 18:26:02 reitmayr Exp $
******************************************************************************/

#include <ace/OS.h>

struct llpoint {
    double          lat;
    double          lon;
    double          height;
    double          time;
    double          hdop;
    double          diffdelay;
    double          numsats;
    int             fix;
    int             statid;
};

struct sv {
    u_short         azi;
    u_short         elev;
    u_short         sig;
    u_long          time;
};

#define NUMSV 32

struct nmeastate {
    u_long          time;
    double          lat,
                    lon,
                    height,
                    hdop;
    u_int           fix,
                    numsats;	/* SV's in fix */
    struct sv       sv[NUMSV];
};

extern struct nmeastate n;

int nmeaRead(char * buf, int cnt, struct llpoint * point);

/* end */
