 /* ========================================================================
  * Copyright (C) 2001  Vienna University of Technology
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the GNU Lesser General Public
  * License as published by the Free Software Foundation; either
  * version 2.1 of the License, or (at your option) any later version.
  *
  * This library is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  * Lesser General Public License for more details.
  *
  * You should have received a copy of the GNU Lesser General Public
  * License along with this library; if not, write to the Free Software
  * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
  *
  * For further information please contact Gerhard Reitmayr under
  * <reitmayr@ims.tuwien.ac.at> or write to Gerhard Reitmayr,
  * Vienna University of Technology, Favoritenstr. 9-11/188, A1040 Vienna,
  * Austria.
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** header file for GPSParser class and helper data types
  *
  * @author Gerhard Reitmayr
  * 
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/input/GPSParser.cxx,v 1.1 2003/06/16 13:17:01 reitmayr Exp $
  *
  * @file                                                                   */
 /* ======================================================================= */

#include <cstring>
#include <cstdlib>

const int NMEABUFSZ = 1024;

#include "GPSParser.h"

const GPResult * GPSParser::parse( const char * line )
{
    if (strncmp("$GPGGA,", line, 7) == 0) {
        return parseGPGGA( line );
    } 
    else if (strncmp("$GPVTG,", line, 7) == 0) {
        return parseGPVTG( line );
    }
    return new GPResult;
}

bool GPSParser::checkSum( const char * line )
{
    const char * cp;
    unsigned int csum, checksum;

    cp = strchr(line, '*');
    if (cp) {
        cp++;
        csum = strtol(cp, NULL, 16);
        
        checksum = 0;
        for (cp = line + 1; *cp; cp++) {
            if (*cp == '*')	/* delimiter */
                break;
            checksum ^= *cp;
        }
        return (checksum == csum);
    } 
    return false;
}

const GPResult * GPSParser::parseGPGGA( const char * line )
{
    char         *cp,
                 *ep;
    char         buffer[NMEABUFSZ];
    int          fix,
                 statid,
                 numsats;
    double       time, 
                 lat,
                 lon,
                 hdop,
                 alt,
                 height,
                 diffdelay;
    
    // copy string into work buffer
    strncpy( buffer, line, NMEABUFSZ);
    /*
    * $GPGGA,011243,3743.000,N,12214.000,W,2,07,0.4,2.8,M,-27.9,M,0,0269*4
    * 5
    *
    * NOTE time is in hours minutes fractional_minutes
    *
    * time, lat, ns, lon, ew, fix, numsats, hdop, alt, M, height, M,
    * diff-delay, statid, csum
    */    
    if( checkSum( buffer) )
    {        
        cp = ep = (char *) buffer;
        ep = strchr(cp, ',');
        if (!ep)
            return new GPResult;
        *ep++ = '\0';
        
        /* time */
        cp = ep;
        ep = strchr(cp, ',');
        if (!ep)
            return new GPResult;
        *ep++ = '\0';
        
        /* 012345 --> 01:23:45 */
        time = atoi(cp + 4);
        cp[4] = 0;
        time += 60 * atoi(cp + 2);
        cp[2] = 0;
        time += 3600 * atoi(cp);
        
        /* latitude */
        cp = ep;
        ep = strchr(cp, ',');
        if (!ep)
            return new GPResult;
        *ep++ = '\0';
        lat = atof(cp + 2) / 60.0;
        cp[2] = 0;
        lat += atof(cp);
        
        /* ns */
        cp = ep;
        ep = strchr(cp, ',');
        if (!ep)
            return new GPResult;
        *ep++ = '\0';
        if (*cp == 'S') {
            lat = -lat;
        }
        /* longitude */
        cp = ep;
        ep = strchr(cp, ',');
        if (!ep)
            return new GPResult;
        *ep++ = '\0';
        lon = atof(cp + 3) / 60.0;
        cp[3] = 0;
        lon += atof(cp);
        
        /* ew */
        cp = ep;
        ep = strchr(cp, ',');
        if (!ep)
            return new GPResult;
        *ep++ = '\0';
        if (*cp == 'W') {
            lon = -lon;
        }
        /* fix 0,1,2 */
        cp = ep;
        ep = strchr(cp, ',');
        if (!ep)
            return new GPResult;
        *ep++ = '\0';
        fix = atoi(cp);
        
        /* number of sats */
        cp = ep;
        ep = strchr(cp, ',');
        if (!ep)
            return new GPResult;
        *ep++ = '\0';
        numsats = atoi(cp);
        
        /* HDOP */
        cp = ep;
        ep = strchr(cp, ',');
        if (!ep)
            return new GPResult;
        *ep++ = '\0';
        hdop = atof(cp);
        
        /* altitude */
        cp = ep;
        ep = strchr(cp, ',');
        if (!ep)
            return new GPResult;
        *ep++ = '\0';
        alt = atof(cp);
        
        /* M - discard  */
        cp = ep;
        ep = strchr(cp, ',');
        if (!ep)
            return new GPResult;
        *ep++ = '\0';
        
        /* height above geode */
        cp = ep;
        ep = strchr(cp, ',');
        if (!ep)
            return new GPResult;
        *ep++ = '\0';
        height = atof(cp);
        
        /* M - discard */
        cp = ep;
        ep = strchr(cp, ',');
        if (!ep)
            return new GPResult;
        *ep++ = '\0';
        
        /* differential delay -- optional value */
        cp = ep;
        ep = strchr(cp, ',');
        if (!ep) {			/* the delim might be "*" */
            ep = strchr(cp, '*');
        }
        if (!ep)
            return new GPResult;
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

        GPGGA * result = new GPGGA;

        result->lat = lat;
        result->lon = lon;
        result->altitude = alt; 
        result->height = height;
        result->time = time;
        result->hdop = hdop;
        result->statid = statid;
        result->numsats = numsats;
        result->diffdelay = diffdelay;
        result->fix = fix;

        return result;
    }
    return new GPResult;
}

const GPResult * GPSParser::parseGPVTG( const char * line )
{
    char    *cp,
            *ep;
    char    buffer[NMEABUFSZ];
    double  trueCourse,
            magneticCourse,
            speedKnots,
            speedKlm;
    
    // copy string into work buffer
    strncpy( buffer, line, NMEABUFSZ);
    
    /*
    * $GPVTG,360.0,T,348.7,M,000.0,N,000.0,K*43
    */    
    if( checkSum(buffer) )
    {        
        /* header */
        cp = ep = (char *)buffer;
        ep = strchr(cp, ',');
        if (!ep)
            return new GPResult;
        *ep++ = '\0';
        
        /* true course */
        cp = ep;
        ep = strchr(cp, ',');
        if (!ep)
            return new GPResult;
        *ep++ = '\0';
        trueCourse = atof( cp );
        
        /* T - discard  */
        cp = ep;
        ep = strchr(cp, ',');
        if (!ep)
            return new GPResult;
        *ep++ = '\0';
        
        /* magnetic course */
        cp = ep;
        ep = strchr(cp, ',');
        if (!ep)
            return new GPResult;
        *ep++ = '\0';
        magneticCourse = atof( cp );

        /* M - discard  */
        cp = ep;
        ep = strchr(cp, ',');
        if (!ep)
            return new GPResult;
        *ep++ = '\0';

        /* speed in knots */
        cp = ep;
        ep = strchr(cp, ',');
        if (!ep)
            return new GPResult;
        *ep++ = '\0';
        speedKnots = atof( cp );
        
        /* N - discard  */
        cp = ep;
        ep = strchr(cp, ',');
        if (!ep)
            return new GPResult;
        *ep++ = '\0';

        /* speed in klms */
        cp = ep;
        ep = strchr(cp, ',');
        if (!ep)
            return new GPResult;
        *ep++ = '\0';
        speedKlm = atof( cp );
    
        GPVTG * result = new GPVTG;
        result->trueCourse = trueCourse;
        result->magneticCourse = magneticCourse;
        result->speedKnots = speedKnots;
        result->speedKlm = speedKlm;
        return result;
    }
    return new GPResult;
}