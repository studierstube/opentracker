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
/** header file for GPSModule
 *
 * @author Gerhard Reitmayr
 * 
 * $Id$
 *
 * @file                                                                   */
/* ======================================================================= */
/**
 * @page module_ref Module Reference
 * @section gpsmodule GPSModule 
 *
 * This module provides and drives @ref gpssource, @ref gpsdirectionsource, @ref gpsinfosource, 
 * @ref gpsgarmincompass and @ref gpsgarminaltitude nodes that 
 * generate various GPS data events. The configuration element is called @c GPSConfig.
 * It reads GPS position data from a GPS receiver connected on the serial port 
 * and transmiting its data in NMEA format as Latitude, Longitude and Height in
 * the x, y and z components of the position. The angles are given in radiants and
 * the height in meter. The data is in WGS84 coordinate system. 
 * The hdop parameter is given as 1 / hdop in the confidence value.
 *
 * It allows to connect an optional DGPSIP server for DGPS correction data. 
 * Be sure to set your GPS receiver to NMEA output mode and RTCM input mode 
 * for this work. It also allows you to log the NMEA strings received from
 * the gps receiver into a dedicated log file for further processing.
 *
 * It has the following attributes :
 * @li @c dev serial port where the GPS receiver is attached.
 * @li @c baudrate default value is 9600 other values are standard baud rates
 * @li @c DGPSserver an optional DGPSIP server for DGPS correction data
 * @li @c DGPSport port of the DGPSIP port, default is 2101
 * @li @c debug on|off prints out RTCM and NMEA data for debugging
 * @li @c DGPSmirror a port number to run a server on that mirrors the RTCM correction
 *        data for other computers. If not specified, no server will be started.
 * @li @c logfile path and name of a file to log all received strings into (optional)
 * @li @c rtcmdev another serial port to output rtcm data, if it is not the device serial port
 *
 * An example configuration element looks like this :
 * @verbatim
 <GPSConfig dev="COM1" baudrate="9600" DGPSserver="something" DGPSport="2101" debug="off"/>@endverbatim
 *
 */

#ifndef _GPSMODULE_H
#define _GPSMODULE_H

#include "../OpenTracker.h"
#include "GPSDriver.h"

#include <string>

class ACE_FILE_IO;

namespace ot {


class GPSSource;
class GPSDirectionSource;
class GPSGarminCompass;
class GPSGarminAltitude;
class GPSInfoSource;

/**
 * This module supports a GPS receiver with optional DGPS correction data
 * from an DGPSIP server. It drives all the different GPS source nodes associated with it, but only
 * one instance per type. Currently it only supports one device.
 *
 * It uses the GPSDriver class to operate the GPS device and any DGPSIP servers. Any required
 * transformations are implemented in the individual GPS source nodes. 
 * 
 * @see GPSSource, GPSDirectionSource, GPSGarminCompass, GPSGarminAltitude for more information
 * on the individual source nodes.
 *
 * @ingroup input
 * @author Gerhard Reitmayr
 */
class OPENTRACKER_API GPSModule : public ThreadModule , public NodeFactory, public GPSListener
{
public:
    GPSModule();
    virtual ~GPSModule();
    
    virtual void init(StringTable& attributes,  ConfigNode * localTree);
    
    virtual Node * createNode( const std::string & name, StringTable & attributes );
    
    virtual void pushState();
    
    virtual void start();
    
    virtual void close();
    
    virtual void newData( const GPResult * point, const char * line, void * userData );
    
protected:
    
    virtual void run();
    
    template<class T> void updateSource(T * source)
    {
        if( source != NULL )
        {
            lock();
            if( source->state.time < source->buffer.time )
            {
                source->state = source->buffer;
                unlock();
                source->updateObservers( source->state );
            }
            else
                unlock();
        }
    };
    
    bool debug;
    std::string device;
    int baudRate;
    std::string dgpsServer;
    int dgpsPort;
    int dgpsmirror;
    std::string rtcmdev;
    
    GPSSource * source;
    GPSDirectionSource * dirSource;
    GPSGarminCompass * compassSource;
    GPSGarminAltitude * altitudeSource;
    GPSInfoSource * infoSource;
    GPSDriver * driver;
    
    ACE_FILE_IO * logFile;
    
    friend class GPSSource;
    friend class GPSDirectionSource;
    friend class GPSGarminCompass;
    friend class GPSGarminAltitude;
    friend class GPSInfoSource;
};


}  // namespace ot


#endif // !defined(_GPSMODULE_H)
