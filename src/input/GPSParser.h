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

#ifndef _GPSPARSER_H_
#define _GPSPARSER_H_

/**
 * The abstract base class for representing results of the GPSParser class.
 * The type field tells the user about the actual data represented by the
 * instance. Don't mess with the type field :). Instances of this base class
 * with type = INVALID will be used to denote errors during parsing.
 *
 * @ingroup input
 * @author Gerhard Reitmayr
 */

namespace ot {

class GPResult {
public :
    enum Type {
        GPGGA,
        GPVTG, 
        HCHDG, 
        PGRMZ,
        INVALID
    } type;
       
    GPResult() {
        type = GPResult::INVALID;
    }
};

class GPGGA : public GPResult {
public :
    double  time;
    double  lat;
    double  lon;
    int     fix;
    int     numsats;
    double  hdop;
    double  altitude;    
    double  height;
    double  diffdelay;
    int     statid;

    static const GPResult * parse( const char * );

protected:
    GPGGA(){
        type = GPResult::GPGGA;
    }
};

class GPVTG : public GPResult {
public :
    double  trueCourse;
    double  magneticCourse;
    double  speedKnots;
    double  speedKlm;

    static const GPResult * parse( const char * );

protected:
    GPVTG(){
        type = GPResult::GPVTG;
    }
};

class HCHDG : public GPResult {
public :
    double  heading;
    double  variation;
    
    static const GPResult * parse( const char * );
    
protected:
    HCHDG(){
        type = GPResult::HCHDG;
    }
};

class PGRMZ: public GPResult {
public :
    double  altitude;
    
    static const GPResult * parse( const char * );
    
protected:
    PGRMZ(){
        type = GPResult::PGRMZ;
    }
};

/**
 * This is a GPS helper class for parsing the different output strings returned
 * by the GPS receiver module. It will output a class that represents the result
 * and needs to be tested for the correct type. See GPResult. It 
 * implements the following NMEA and extension messages: GPGGA, GPVTG. PGRMZ, HCHDG.
 *
 * @ingroup input
 * @author Gerhard Reitmayr
 */
class GPSParser  
{
public :
    static const GPResult * parse( const char * line );
    static bool checkSum( const char * line );
private:
    GPSParser(){};
    static const GPResult * (*parsers[])(const char *);
};

} // namespace ot

#endif // !defined(_GPSPARSER_H_)
