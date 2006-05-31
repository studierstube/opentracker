 /* ========================================================================
  * Copyright (c) 2006,
  * Institute for Computer Graphics and Vision
  * Graz University of Technology
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without
  * modification, are permitted provided that the following conditions are
  * met:
  *
  * Redistributions of source code must retain the above copyright notice,
  * this list of conditions and the following disclaimer.
  *
  * Redistributions in binary form must reproduce the above copyright
  * notice, this list of conditions and the following disclaimer in the
  * documentation and/or other materials provided with the distribution.
  *
  * Neither the name of the Graz University of Technology nor the names of
  * its contributors may be used to endorse or promote products derived from
  * this software without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
  * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
  * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
  * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER
  * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
  * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
  * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
  * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
  * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
