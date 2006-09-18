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

#include <cmath>

#include "../OpenTracker.h"

/**
 * The abstract base class for representing results of the GPSParser class.
 * The type field tells the user about the actual data represented by the
 * instance. Don't mess with the type field :). Instances of this base class
 * with type = INVALID will be used to denote errors during parsing.
 *
 * @ingroup input
 * @author Gerhard Reitmayr
 */


#ifndef OT_NO_GPS_SUPPORT

// defines the model used for ECEF transformation

#include "../misc/GeoidModel.h"

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

        // add additional XYZ coordinate (ECEF format)
        // see
        // http://www.colorado.edu/geography/gcraft/notes/datum/gif/llhxyz.gif
        float xECEF;
        float yECEF;
        float zECEF;

        static const GPResult * parse( const char * );

        /** Convert to Earth-centered, Earth-fixed XYZ coordinates. */
        // see
        // http://www.colorado.edu/geography/gcraft/notes/datum/gif/llhxyz.gif

        void convert2ECEF(GeoidModel *geoid)
        {
            double rad_cur, gdlat, gdlon;
            double gdalt = altitude;
            
            // convert angles to radians
            gdlat = MathUtils::GradToRad * lat;
            gdlon = MathUtils::GradToRad * lon;
            
            // radius of curvature in the prime vertical
            rad_cur  = geoid->a() /
                sqrt(1.0-geoid->eccSquared()*pow((sin(gdlat)),2.0));

//             printf("lat = %2.24f\n", lat);
//             printf("lon = %2.24f\n", lon);
            
            xECEF = static_cast<float>((rad_cur + gdalt) * ::cos(gdlat) * ::cos(gdlon));
            yECEF = static_cast<float>((rad_cur + gdalt) * ::cos(gdlat) * ::sin(gdlon));
            zECEF = static_cast<float>(((1.0 - geoid->eccSquared()) * rad_cur + gdalt) * ::sin(gdlat));
//              printf("xECEF = %08.2f\n", xECEF);
//              printf("yECEF = %f08.2\n", yECEF);
//              printf("zECEF = %f08.2\n", zECEF);
            
        }

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


#endif // OT_NO_GPS_SUPPORT


#endif // !defined(_GPSPARSER_H_)

/* 
 * ------------------------------------------------------------
 *   End of GPSParser.h
 * ------------------------------------------------------------
 *   Automatic Emacs configuration follows.
 *   Local Variables:
 *   mode:c++
 *   c-basic-offset: 4
 *   eval: (c-set-offset 'substatement-open 0)
 *   eval: (c-set-offset 'case-label '+)
 *   eval: (c-set-offset 'statement 'c-lineup-runin-statements)
 *   eval: (setq indent-tabs-mode nil)
 *   End:
 * ------------------------------------------------------------ 
 */
