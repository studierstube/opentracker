/******************************************************************************
*									      *
*	File:     nmea.c						      *
*	Author:   Wolfgang Rupprecht <wolfgang@wsrcc.com>		      *
*	Created:  Thu Aug  1 19:12:42 PDT 1996				      *
*	Contents: read nmea position sentences from the gps		      *
*									      *
*       $Id: nmea.cxx,v 1.1 2003/03/27 18:26:02 reitmayr Exp $
******************************************************************************/

/*
 *  Copyright (c) 1996 - 1999 Wolfgang Rupprecht
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  			    NO WARRANTY
 *
 *    BECAUSE THE PROGRAM IS LICENSED FREE OF CHARGE, THERE IS NO
 *  WARRANTY FOR THE PROGRAM, TO THE EXTENT PERMITTED BY APPLICABLE
 *  LAW.  EXCEPT WHEN OTHERWISE STATED IN WRITING THE COPYRIGHT
 *  HOLDERS AND/OR OTHER PARTIES PROVIDE THE PROGRAM "AS IS" WITHOUT
 *  WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING, BUT
 *  NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 *  FITNESS FOR A PARTICULAR PURPOSE.  THE ENTIRE RISK AS TO THE
 *  QUALITY AND PERFORMANCE OF THE PROGRAM IS WITH YOU.  SHOULD THE
 *  PROGRAM PROVE DEFECTIVE, YOU ASSUME THE COST OF ALL NECESSARY
 *  SERVICING, REPAIR OR CORRECTION.
 *
 *    IN NO EVENT UNLESS REQUIRED BY APPLICABLE LAW OR AGREED TO IN
 *  WRITING WILL ANY COPYRIGHT HOLDER, OR ANY OTHER PARTY WHO MAY
 *  MODIFY AND/OR REDISTRIBUTE THE PROGRAM AS PERMITTED ABOVE, BE
 *  LIABLE TO YOU FOR DAMAGES, INCLUDING ANY GENERAL, SPECIAL,
 *  INCIDENTAL OR CONSEQUENTIAL DAMAGES ARISING OUT OF THE USE OR
 *  INABILITY TO USE THE PROGRAM (INCLUDING BUT NOT LIMITED TO LOSS OF
 *  DATA OR DATA BEING RENDERED INACCURATE OR LOSSES SUSTAINED BY YOU
 *  OR THIRD PARTIES OR A FAILURE OF THE PROGRAM TO OPERATE WITH ANY
 *  OTHER PROGRAMS), EVEN IF SUCH HOLDER OR OTHER PARTY HAS BEEN
 *  ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */

#include "nmea.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

extern int      verbose;

/*
 *
 * see: /v/src/gps/doc2/nmeatype.txt
 *
 * data: /v/src/gps/data/track-wsrcc.nmea
 *
 * first sentence:
 *
 * $PSLIB,,,K*23
 * $PSLIB,,,J*22
 * $GPRMC,011243,A,3743.000,N,12214.000,W,000.0,267.6,310796,,*03
 * $GPRMB,A,0.00,R,010,ASCEND,3743.000,N,12214.000,W,000.0,000.0,,A*28
 * $GPGGA,011243,3743.000,N,12214.000,W,2,07,0.4,2.8,M,-27.9,M,0,0269*45
 * $GPGSA,A,3,01,03,,,15,21,23,25,31,,,,,,,,0.7,0.4,0.6*36
 * $GPGSV,3,1,09,01,70,041,49,03,17,209,46,09,01,031,00,14,05,281,34*76
 * $GPGSV,3,2,09,15,30,315,50,21,46,069,50,23,11,086,39,25,31,159,49*7F
 * $GPGSV,3,3,09,31,28,249,48,,,,,,,,,,,,*4B
 * $PGRME,5.9,M,10.1,M,11.7,M*25
 * $GPGLL,3743.000,N,12214.000,W,011243,A*3B
 * $PGRMZ,9,f,3*12
 * $PGRMM,WGS 84*06
 * $GPWPL,3741.000,N,12155.000,W,002*60
 * (pause for a fractional second, then continue)
 *
 * excerpted from Bennett: ftp://sundae.triumf.ca/pub/peter/index.html
 *
 * $GPGGA,011243,3743.000,N,12214.000,W,2,07,0.4,2.8,M,-27.9,M,0,0269*45
 *      GGA - Global Positioning System Fix Data
 *      GGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M, , *42
 *         123519       Fix taken at 12:35:19 UTC
 *         4807.038,N   Latitude 48 deg 07.038' N
 *         01131.000,E  Longitude 11 deg 31.000' E
 *         1            Fix quality: 0 = invalid
 *                                   1 = GPS fix
 *                                   2 = DGPS fix
 *         08           Number of satellites being tracked
 *         0.9          Horizontal dilution of position
 *         545.4,M      Altitude, Metres, above mean sea level
 *         46.9,M       Height of geoid (mean sea level) above WGS84
 *                      ellipsoid
 *         (empty field) time in seconds since last DGPS update
 *         (empty field) DGPS station ID number
 *
 * $GPGLL,3743.000,N,12214.000,W,011243,A*3B
 *      GLL - Geographic position, Latitude and Longitude
 *      GLL,4916.45,N,12311.12,W,225444,A
 *         4916.46,N    Latitude 49 deg. 16.45 min. North
 *         12311.12,W   Longitude 123 deg. 11.12 min. West
 *         225444       Fix taken at 22:54:44 UTC
 *         A            Data valid
 *           (Garmin 65 does not include time and status)
 *
 * $GPGSA,A,3,01,03,,,15,21,23,25,31,,,,,,,,0.7,0.4,0.6*36
 *      GSA - GPS DOP and active satellites
 *      GSA,A,3,04,05,,09,12,,,24,,,,,2.5,1.3,2.1*39
 *         A            Auto selection of 2D or 3D fix (M = manual)
 *         3            3D fix
 *         04,05...     PRNs of satellites used for fix (space for 12)
 *         2.5          PDOP (dilution of precision)
 *         1.3          Horizontal dilution of precision (HDOP)
 *         2.1          Vertical dilution of precision (VDOP)
 *           DOP is an indication of the effect of satellite geometry on
 *           the accuracy of the fix.
 *
 * $GPGSV,3,1,09,01,70,041,49,03,17,209,46,09,01,031,00,14,05,281,34*76
 *      GSV - Satellites in view
 *      GSV,2,1,08,01,40,083,46,02,17,308,41,12,07,344,39,14,22,228,45*75
 *         2            Number of sentences for full data
 *         1            sentence 1 of 2
 *         08           Number of satellites in view
 *         01           Satellite PRN number
 *         40           Elevation, degrees
 *         083          Azimuth, degrees
 *         46           Signal strength - higher is better
 *         <repeat for up to 4 satellites per sentence>
 *              There my be up to three GSV sentences in a data packet
 *
 * $GPRMB,A,0.00,R,010,ASCEND,3743.000,N,12214.071,W,000.0,000.0,,A*28
 *      RMB - Recommended minimum navigation information (sent by nav.
 *              receiver when a destination waypoint is active)
 *      RMB,A,0.66,L,003,004,4917.24,N,12309.57,W,001.3,052.5,000.5,V*0B
 *         A            Data status A = OK, V = warning
 *         0.66,L       Cross-track error (nautical miles, 9.9 max.),
 *                              steer Left to correct (or R = right)
 *         003          Origin waypoint ID
 *         004          Destination waypoint ID
 *         4917.24,N    Destination waypoint latitude 49 deg. 17.24 min. N
 *         12309.57,W   Destination waypoint longitude 123 deg. 09.57 min. W
 *         001.3        Range to destination, nautical miles
 *         052.5        True bearing to destination
 *         000.5        Velocity towards destination, knots
 *         V            Arrival alarm  A = arrived, V = not arrived
 *         *0B          mandatory checksum
 *
 * $GPRMC,011243,A,3743.000,N,12214.000,W,000.0,267.6,310796,,*03
 *      RMC - Recommended minimum specific GPS/Transit data
 *      RMC,225446,A,4916.45,N,12311.12,W,000.5,054.7,191194,020.3,E*68
 *         225446       Time of fix 22:54:46 UTC
 *         A            Navigation receiver warning A = OK, V = warning
 *         4916.45,N    Latitude 49 deg. 16.45 min North
 *         12311.12,W   Longitude 123 deg. 11.12 min West
 *         000.5        Speed over ground, Knots
 *         054.7        Course Made Good, True
 *         191194       Date of fix  19 November 1994
 *         020.3,E      Magnetic variation 20.3 deg East
 *         *68          mandatory checksum
 *
 * $GPWPL,3741.000,N,12155.000,W,002*60
 *      WPL - waypoint location
 *      WPL,4917.16,N,12310.64,W,003*65
 *         4917.16,N    Latitude of waypoint
 *         12310.64,W   Longitude of waypoint
 *         003          Waypoint ID
 *           When a route is active, this sentence is sent once for each
 *           waypoint in the route, in sequence. When all waypoints have
 *           been reported, GPR00 is sent in the next data set. In any
 *           group of sentences, only one WPL sentence, or an R00
 *           sentence, will be sent.
 *
 ***************************************************************************
 *      Proprietary sentences to control a garmin differential beacon
 *      receiver.
 ***************************************************************************
 *
 * $PGRME,5.9,M,10.1,M,11.7,M*25
 *      $PGRME,15.0,M,45.0,M,25.0,M*22
 *         15.0,M       Estimated horizontal position error in metres (HPE)
 *         45.0,M       Estimated vertical error (VPE) in metres
 *         25.0,M       Overall spherical equivalent position error
 *
 * $PGRMZ,9,f,3*12
 *      $PGRMZ,93,f,3*21
 *         93,f         Altitude in feet
 *         3            Position fix dimensions 2 = user altitude
 *                                              3 = GPS altitude
 *      This sentence shows in feet, regardless of units shown on the display.
 *
 * $PGRMM,WGS 84*06
 *      $PGRMM,NAD27 Canada*2F
 *         Currently active horizontal datum
 *
 *      Proprietary sentences to control a Starlink differential beacon
 *      receiver. (I assume Garmin's DBR is made by Starlink)
 * $PSLIB,,,K*23
 * $PSLIB,,,J*22
 *      $PSLIB,,,J*22
 *      $PSLIB,,,K*23
 *         These two sentences are normally sent together in each group
 *         of sentences from the GPS.
 *         The three fields are: Frequency, bit Rate, Request Type.  The
 *         value in the third field may be:
 *              J = status request
 *              K = configuration request
 *              blank = tuning message
 *
 *         When the GPS receiver is set to change the DBR frequency or
 *         baud rate, the "J" sentence is replaced (just once) by (for
 *         example): $PSLIB,320.0,200*59 to set the DBR to 320 KHz, 200
 *         baud.
 *
 */

void            do_gga(char * nmeabuf, struct llpoint * point, struct nmeastate * n);
void            do_gsv(char * nmeabuf, struct nmeastate * n);

struct nmeastate n;

int
nmeaRead(char * nmeabuf,
	 int incnt,
	 struct llpoint * point)
{
    char         *cp,
                   *crp,
                   *onmeabuf;
    int             oincnt;

    point->fix = 0;
    onmeabuf = nmeabuf;
    oincnt = incnt;

    while (incnt > 0) {
	if ((cp = (char *)memchr(nmeabuf, '$', incnt)) == NULL) {
	    /* no nmea delims - junk it all */
	    return oincnt;
	}
	/* else align buff and adjust count */
	incnt -= cp - nmeabuf;
	nmeabuf = cp;

	if ((crp = (char *)memchr(nmeabuf, '\n', incnt)) == NULL) {
	    /*
	     * No cr - return till more data trickles in.
	     * Let 'em know how much of buff we looked at and discarded.
	     */
	    return nmeabuf - onmeabuf;
	}
	*crp = '\0';		/* turn current buff fragment into a string */

	if (strncmp("$GPGGA,", (const char *)nmeabuf, 7) == 0) {
	    do_gga(nmeabuf, point, &n);
	} else if (strncmp("$GPGSV,", (const char *)nmeabuf, 7) == 0) {
	    do_gsv(nmeabuf, &n);
	}
	incnt -= crp - nmeabuf;
	nmeabuf = crp;
    }
    return oincnt;
}

void
do_gga(char * nmeabuf,
       struct llpoint * point,
       struct nmeastate * n)
{
    char         *cp,
                   *ep;
    u_int           csum,
                    checksum,
                    s,
                    fix,
                    statid;
    double          lat,
                    lon,
                    hdop,
                    alt,
                    height,
                    numsats,
                    diffdelay;

    if (verbose > 10)
	fprintf(stderr, "%s\n", nmeabuf);

    /*
     * $GPGGA,011243,3743.000,N,12214.000,W,2,07,0.4,2.8,M,-27.9,M,0,0269*4
     * 5
     *
     * NOTE time is in hours minutes fractional_minutes
     *
     * time, lat, ns, lon, ew, fix, numsats, hdop, alt, M, height, M,
     * diff-delay, statid, csum
     */

    cp = strchr(nmeabuf, '*');
    if (cp) {
	cp++;
	csum = strtol(cp, NULL, 16);

	checksum = 0;
	for (cp = nmeabuf + 1; *cp; cp++) {
	    if (*cp == '*')	/* delimiter */
		break;
	    checksum ^= *cp;
	}
	if (checksum != csum) {
	    if (verbose > 10)
		fprintf(stderr,
			"nmea: bad checksum exp 0x%x got 0x%x\n",
			csum, checksum);
	    return;
	}
    } else {
	if (verbose > 10)
	    fprintf(stderr, "GGA -- csum\n");
	/* return; */
    }

    cp = ep = nmeabuf;
    ep = strchr(cp, ',');
    if (!ep) {
	if (verbose > 10)
	    fprintf(stderr, "GGA -- gga\n");
	return;
    }
    *ep++ = '\0';

    /* time */
    cp = ep;
    ep = strchr(cp, ',');
    if (!ep) {
	if (verbose > 10)
	    fprintf(stderr, "GGA -- time\n");
	return;
    }
    *ep++ = '\0';

    /* 012345 --> 01:23:45 */
    s = atoi(cp + 4);
    cp[4] = 0;
    s += 60 * atoi(cp + 2);
    cp[2] = 0;
    s += 3600 * atoi(cp);

    /* latitude */
    cp = ep;
    ep = strchr(cp, ',');
    if (!ep) {
	if (verbose > 10)
	    fprintf(stderr, "GGA -- lat\n");
	return;
    }
    *ep++ = '\0';
    lat = atof(cp + 2) / 60.0;
    cp[2] = 0;
    lat += atof(cp);

    /* ns */
    cp = ep;
    ep = strchr(cp, ',');
    if (!ep) {
	if (verbose > 10)
	    fprintf(stderr, "GGA -- NS\n");
	return;
    }
    *ep++ = '\0';
    if (*cp == 'S') {
	lat = -lat;
    }
    /* longitude */
    cp = ep;
    ep = strchr(cp, ',');
    if (!ep) {
	if (verbose > 10)
	    fprintf(stderr, "GGA -- lon\n");
	return;
    }
    *ep++ = '\0';
    lon = atof(cp + 3) / 60.0;
    cp[3] = 0;
    lon += atof(cp);

    /* ew */
    cp = ep;
    ep = strchr(cp, ',');
    if (!ep) {
	if (verbose > 10)
	    fprintf(stderr, "GGA -- EW\n");
	return;
    }
    *ep++ = '\0';
    if (*cp == 'W') {
	lon = -lon;
    }
    /* fix 0,1,2 */
    cp = ep;
    ep = strchr(cp, ',');
    if (!ep) {
	if (verbose > 10)
	    fprintf(stderr, "GGA -- fix\n");
	return;
    }
    *ep++ = '\0';
    fix = atoi(cp);

    /* number of sats */
    cp = ep;
    ep = strchr(cp, ',');
    if (!ep) {
	if (verbose > 10)
	    fprintf(stderr, "GGA -- numsats\n");
	return;
    }
    *ep++ = '\0';
    numsats = atof(cp);

    /* HDOP */
    cp = ep;
    ep = strchr(cp, ',');
    if (!ep) {
	if (verbose > 10)
	    fprintf(stderr, "GGA -- hdop\n");
	return;
    }
    *ep++ = '\0';
    hdop = atof(cp);

    /* altitude */
    cp = ep;
    ep = strchr(cp, ',');
    if (!ep) {
	if (verbose > 10)
	    fprintf(stderr, "GGA -- alt\n");
	return;
    }
    *ep++ = '\0';
    alt = atof(cp);

    /* M - discard  */
    cp = ep;
    ep = strchr(cp, ',');
    if (!ep) {
	if (verbose > 10)
	    fprintf(stderr, "GGA -- m1\n");
	return;
    }
    *ep++ = '\0';

    /* height above geode */
    cp = ep;
    ep = strchr(cp, ',');
    if (!ep) {
	if (verbose > 10)
	    fprintf(stderr, "GGA -- height\n");
	return;
    }
    *ep++ = '\0';
    height = atof(cp);

    /* M - discard */
    cp = ep;
    ep = strchr(cp, ',');
    if (!ep) {
	if (verbose > 10)
	    fprintf(stderr, "GGA -- m2\n");
	return;
    }
    *ep++ = '\0';

    /* differential delay -- optional value */
    cp = ep;
    ep = strchr(cp, ',');
    if (!ep) {			/* the delim might be "*" */
	ep = strchr(cp, '*');
    }
    if (!ep) {
	if (verbose > 10)
	    fprintf(stderr, "GGA -- delay\n");
	return;
    }
    *ep++ = '\0';
    diffdelay = atof(cp);

    /* diff station ID - optional */
    cp = ep;
    ep = strchr(cp, '*');
    if (!ep) {
	statid = 0;
    } else {
	*ep++ = '\0';
	statid = atoi(cp);
    }

    /* ----------- */

    if (verbose > 10)
	fprintf(stderr,
		"nmea: %f %f %0.1f tm: %u fx: %d hdop: %0.1f num: %0.1f stat: %d delay: %0.1f\n",
		lat, lon, alt, s, fix, hdop, numsats, statid, diffdelay);

    point->lat = lat;
    point->lon = lon;
    point->height = alt;
    point->time = s;
    point->hdop = hdop;
    point->statid = statid;
    point->numsats = numsats;
    point->diffdelay = diffdelay;
    point->fix = fix;

    n->time = s;
    n->lat = lat;
    n->lon = lon;
    n->hdop = hdop;
    n->height = alt;
    n->fix = fix;
    point->numsats = numsats;
}

void
do_gsv(char * nmeabuf,
       struct nmeastate * n)
{
    char         *cp,
                   *ep;
    u_int           csum,
                    checksum,
                    totnum,
                    numsent,
                    numsat,
                    prn,
                    elev,
                    azi,
                    sig;

    if (verbose > 10)
	fprintf(stderr, "%s\n", nmeabuf);

    cp = strchr(nmeabuf, '*');
    if (cp) {
	cp++;
	csum = strtol(cp, NULL, 16);

	checksum = 0;
	for (cp = nmeabuf + 1; *cp; cp++) {
	    if (*cp == '*')	/* delimiter */
		break;
	    checksum ^= *cp;
	}
	if (checksum != csum) {
	    if (verbose > 10)
		fprintf(stderr,
			"nmea: bad checksum exp 0x%x got 0x%x\n",
			csum, checksum);
	    return;
	}
    } else {
	if (verbose > 10)
	    fprintf(stderr, "GSV -- csum\n");
	/* return; */
    }

    cp = ep = nmeabuf;
    ep = strchr(cp, ',');
    if (!ep) {
	if (verbose > 10)
	    fprintf(stderr, "GSV -- gsv\n");
	return;
    }
    *ep++ = '\0';

    /* totnum */
    cp = ep;
    ep = strchr(cp, ',');
    if (!ep) {
	if (verbose > 10)
	    fprintf(stderr, "GSV -- totnum\n");
	return;
    }
    *ep++ = '\0';
    totnum = atoi(cp);

    /* numsent */
    cp = ep;
    ep = strchr(cp, ',');
    if (!ep) {
	if (verbose > 10)
	    fprintf(stderr, "GSV -- numsent\n");
	return;
    }
    *ep++ = '\0';
    numsent = atoi(cp);

    /* numsat */
    cp = ep;
    ep = strchr(cp, ',');
    if (!ep) {
	if (verbose > 10)
	    fprintf(stderr, "GSV -- numsat\n");
	return;
    }
    *ep++ = '\0';
    numsat = atoi(cp);

    /* ----------- */

    if (verbose > 10)
	fprintf(stderr, "totnum: %d totsent: %d numsat: %d\n",
		totnum, numsent, numsat);

    /* ----------- */

    /* prn0 */
    cp = ep;
    ep = strchr(cp, ',');
    if (!ep) {
	if (verbose > 10)
	    fprintf(stderr, "GSV -- prn0\n");
	return;
    }
    *ep++ = '\0';
    if (*cp == '\0') {
	return;
    }
    prn = atoi(cp);
    if ((prn >= NUMSV) || (prn < 0)) {
	if (verbose > 10)
	    fprintf(stderr, "GSV -- prn0 range\n");
	return;
    }
    /* elev0 */
    cp = ep;
    ep = strchr(cp, ',');
    if (!ep) {
	if (verbose > 10)
	    fprintf(stderr, "GSV -- elev0\n");
	return;
    }
    *ep++ = '\0';
    elev = atoi(cp);

    /* azi0 */
    cp = ep;
    ep = strchr(cp, ',');
    if (!ep) {
	if (verbose > 10)
	    fprintf(stderr, "GSV -- azi0\n");
	return;
    }
    *ep++ = '\0';
    azi = atoi(cp);

    /* sig0 */
    cp = ep;
    ep = strchr(cp, ',');
    if (!ep) {
	if (verbose > 10)
	    fprintf(stderr, "GSV -- sig0\n");
	return;
    }
    *ep++ = '\0';
    sig = atoi(cp);


    n->sv[prn].azi = azi;
    n->sv[prn].elev = elev;
    n->sv[prn].sig = sig;
    n->sv[prn].time = n->time;


    /* ------------- */

    /* prn1 */
    cp = ep;
    ep = strchr(cp, ',');
    if (!ep) {
	if (verbose > 10)
	    fprintf(stderr, "GSV -- prn1\n");
	return;
    }
    *ep++ = '\0';
    if (*cp == '\0')
	return;
    prn = atoi(cp);
    if ((prn >= NUMSV) || (prn < 0)) {
	if (verbose > 10)
	    fprintf(stderr, "GSV -- prn1 range\n");
	return;
    }
    /* elev1 */
    cp = ep;
    ep = strchr(cp, ',');
    if (!ep) {
	if (verbose > 10)
	    fprintf(stderr, "GSV -- elev1\n");
	return;
    }
    *ep++ = '\0';
    elev = atoi(cp);

    /* azi1 */
    cp = ep;
    ep = strchr(cp, ',');
    if (!ep) {
	if (verbose > 10)
	    fprintf(stderr, "GSV -- azi1\n");
	return;
    }
    *ep++ = '\0';
    azi = atoi(cp);

    /* sig1 */
    cp = ep;
    ep = strchr(cp, ',');
    if (!ep) {
	if (verbose > 10)
	    fprintf(stderr, "GSV -- sig1\n");
	return;
    }
    *ep++ = '\0';
    sig = atoi(cp);

    n->sv[prn].azi = azi;
    n->sv[prn].elev = elev;
    n->sv[prn].sig = sig;
    n->sv[prn].time = n->time;

    /* ------------- */

    /* prn2 */
    cp = ep;
    ep = strchr(cp, ',');
    if (!ep) {
	if (verbose > 10)
	    fprintf(stderr, "GSV -- prn2\n");
	return;
    }
    *ep++ = '\0';
    if (*cp == '\0')
	return;
    prn = atoi(cp);
    if ((prn >= NUMSV) || (prn < 0)) {
	if (verbose > 10)
	    fprintf(stderr, "GSV -- prn2 range\n");
	return;
    }
    /* elev2 */
    cp = ep;
    ep = strchr(cp, ',');
    if (!ep) {
	if (verbose > 10)
	    fprintf(stderr, "GSV -- elev2\n");
	return;
    }
    *ep++ = '\0';
    elev = atoi(cp);

    /* azi2 */
    cp = ep;
    ep = strchr(cp, ',');
    if (!ep) {
	if (verbose > 10)
	    fprintf(stderr, "GSV -- azi2\n");
	return;
    }
    *ep++ = '\0';
    azi = atoi(cp);

    /* sig2 */
    cp = ep;
    ep = strchr(cp, ',');
    if (!ep) {
	if (verbose > 10)
	    fprintf(stderr, "GSV -- sig2\n");
	return;
    }
    *ep++ = '\0';
    sig = atoi(cp);

    n->sv[prn].azi = azi;
    n->sv[prn].elev = elev;
    n->sv[prn].sig = sig;
    n->sv[prn].time = n->time;

    /* ------------- */

    /* prn3 */
    cp = ep;
    ep = strchr(cp, ',');
    if (!ep) {
	if (verbose > 10)
	    fprintf(stderr, "GSV -- prn3\n");
	return;
    }
    *ep++ = '\0';
    if (*cp == '\0')
	return;
    prn = atoi(cp);
    if ((prn >= NUMSV) || (prn < 0)) {
	if (verbose > 10)
	    fprintf(stderr, "GSV -- prn3 range\n");
	return;
    }
    /* elev3 */
    cp = ep;
    ep = strchr(cp, ',');
    if (!ep) {
	if (verbose > 10)
	    fprintf(stderr, "GSV -- elev3\n");
	return;
    }
    *ep++ = '\0';
    elev = atoi(cp);

    /* azi3 */
    cp = ep;
    ep = strchr(cp, ',');
    if (!ep) {
	if (verbose > 10)
	    fprintf(stderr, "GSV -- azi3\n");
	return;
    }
    *ep++ = '\0';
    azi = atoi(cp);

    /* sig3 */
    cp = ep;
    ep = strchr(cp, '*');
    if (!ep) {
	if (verbose > 10)
	    fprintf(stderr, "GSV -- sig3\n");
	return;
    }
    *ep++ = '\0';
    sig = atoi(cp);

    n->sv[prn].azi = azi;
    n->sv[prn].elev = elev;
    n->sv[prn].sig = sig;
    n->sv[prn].time = n->time;
}

/*
 * end
 */
