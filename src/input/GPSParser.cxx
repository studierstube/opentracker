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
  * $Id$
  *
  * @file                                                                   */
 /* ======================================================================= */

#include <ace/SString.h>

const int NMEABUFSZ = 1024;

#include "GPSParser.h"

const GPResult * (* GPSParser::parsers[])(const char *) = {
    GPGGA::parse,
    GPVTG::parse,
    HCHDG::parse,
    PGRMZ::parse
};

const GPResult * GPSParser::parse( const char * line )
{
    if( checkSum( line ) )
    {
        const GPResult * result = NULL;
        for( int i = 0; i < 4; i++ )
        {
            result = parsers[i](line);
            if( result != NULL )
                return result;
        }
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
        csum = ACE_OS::strtol(cp, NULL, 16);
        
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

const GPResult * GPGGA::parse( const char * line )
{
    char         buffer[NMEABUFSZ];
    int          fix = 0,
                 statid = 0,
                 numsats = 0;
    double       time = 0, 
                 lat = 0,
                 lon = 0,
                 hdop = 0,
                 alt = 0,
                 height = 0,
                 diffdelay = 0;
    
    // copy string into work buffer
    ACE_OS::strncpy( buffer, line, NMEABUFSZ);
    
    /*
    * $GPGGA,011243,3743.000,N,12214.000,W,2,07,0.4,2.8,M,-27.9,M,0,0269*4
    * 5
    *
    * NOTE time is in hours minutes fractional_minutes
    *
    * time, lat, ns, lon, ew, fix, numsats, hdop, alt, M, height, M,
    * diff-delay, statid, csum
    */   
    if ( ACE_OS::strncmp("$GPGGA,", buffer, 7) == 0) {

        ACE_Tokenizer tok( buffer );
        tok.delimiter_replace(',', 0);
        char * token, * oldtoken;

        token = tok.next();
        if (!token)
            return new GPResult;

        /* time */
        token = tok.next();
        if (!token)
            return new GPResult;

        // not all receivers output a time, if there is no fix !
        if( token == buffer + 7 )
        {            
            oldtoken = token + ACE_OS::strlen( token ) + 1;
            
            /* 012345.0 --> 01:23:45.0 */
            time = atof(token + 4);
            token[4] = 0;
            time += 60 * atoi(token + 2);
            token[2] = 0;
            time += 3600 * atoi(token);
            
            token = tok.next();
            if( !token )
                return new GPResult;
            
            // test for fix, 
            if( token == oldtoken  )
            {            
                // we have a fix and parse the position
                /* latitude */
                lat = atof(token + 2) / 60.0;
                token[2] = 0;
                lat += atof(token);
                
                /* ns */
                token = tok.next();
                if (!token)
                    return new GPResult;
                if (*token == 'S') {
                    lat = -lat;
                }
                
                /* longitude */
                token = tok.next();
                if (!token)
                    return new GPResult;
                lon = atof(token + 3) / 60.0;
                token[3] = 0;
                lon += atof(token);
                
                /* ew */
                token = tok.next();
                if (!token)
                    return new GPResult;
                if (*token == 'W') {
                    lon = -lon;
                }
                // go to the next token
                token = tok.next();
                if (!token)
                    return new GPResult;
            }
        }

        /* fix 0,1,2 */
        fix = atoi(token);
        
        /* number of sats */
        token = tok.next();
        if (!token)
            return new GPResult;
        numsats = atoi(token);

        // the following will be only valid, if we have a fix
        if( fix > 0 )
        {
            /* HDOP */
            token = tok.next();
            if (!token)
                return new GPResult;
            hdop = atof(token);
            
            /* altitude */
            token = tok.next();
            if (!token)
                return new GPResult;
            alt = atof(token);
            
            /* M - discard  */
            token = tok.next();
            if (!token)
                return new GPResult;
            
            /* height above geode */
            token = tok.next();
            if (!token)
                return new GPResult;
            height = atof(token);
            
            /* M - discard */
            token = tok.next();
            if (!token)
                return new GPResult;
            
            /* differential delay -- optional value */
            token = tok.next();
            if (!token)
                return new GPResult;
            if( *token != '*' )
            {
                diffdelay = atof(token);
                token = tok.next();
            }
            
            /* diff station ID - optional */
            if (!token)
                return new GPResult;
            if( *token != '*' )
            {
                statid = atoi(token);
                token = tok.next();
            }
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
    // only return null, if we this is not for us.
    return NULL;
}

const GPResult * GPVTG::parse( const char * line )
{
    char    *cp,
            *ep;
    char    buffer[NMEABUFSZ];
    double  trueCourse,
            magneticCourse,
            speedKnots,
            speedKlm;
    
    // copy string into work buffer
    ACE_OS::strncpy( buffer, line, NMEABUFSZ);
    
    /*
    * $GPVTG,360.0,T,348.7,M,000.0,N,000.0,K*43
    */    
    if (ACE_OS::strncmp("$GPVTG,", buffer, 7) == 0)
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
    return NULL;
}

const GPResult * HCHDG::parse( const char * line )
{
    char    buffer[NMEABUFSZ];
    
    // copy string into work buffer
    ACE_OS::strncpy( buffer, line, NMEABUFSZ);
    
   /*
    * $HCHDG,101.1,,,7.1,W*3C
    */    
    if (ACE_OS::strncmp("$HCHDG,", buffer, 7) == 0)
    {        
        ACE_Tokenizer tok( buffer );
        tok.delimiter_replace(',', 0);
        char * token;
        double  heading,
            variation;
        
        /* skip header */
        token = tok.next();
        if( !token )
            return new GPResult;

        /* heading */
        token = tok.next();
        if (!token)
            return new GPResult;
        heading = atof( token );

        /* next two ',' are skipped by tokenizing */

        /* variation */
        token = tok.next();
        if (!token)
            return new GPResult;
        variation = atof( token );

        /* direction */
        token = tok.next();
        if (!token)
            return new GPResult;
        
        if( ACE_OS::strcmp(token, "E" ))
        {
            variation = - variation;
        }

        HCHDG * result = new HCHDG;
        result->heading = heading;
        result->variation = variation;
        return result;
    }
    return NULL;
}

const GPResult * PGRMZ::parse( const char * line )
{
    char buffer[NMEABUFSZ];
    
    // copy string into work buffer
    ACE_OS::strncpy( buffer, line, NMEABUFSZ);
    
   /*
    * $PGRMZ,246,f,3*1B
    */    
    if (ACE_OS::strncmp("$PGRMZ,", buffer, 7) == 0)
    {        
        ACE_Tokenizer tok( buffer );
        tok.delimiter_replace(',', 0);
        char * token;
        double altitude;
        
        /* skip header */
        token = tok.next();
        if( !token )
            return new GPResult;

        /* altitude in feet */
        token = tok.next();
        if (!token)
            return new GPResult;
        altitude = atof( token );        
        
        PGRMZ * result = new PGRMZ;
        result->altitude = altitude; 
        return result;
    }
    return NULL;
}
