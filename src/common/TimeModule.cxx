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
/** source file for Time module.
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/common/TimeModule.cxx,v 1.1 2001/06/11 03:22:37 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

#include "TimeModule.h"

void TimeModule::init( StringTable & attributes,  ConfigNode * localTree)
{
    Module::init( attributes, localTree );
    
    int num;   
    if( attributes.get("sleep").compare("") != 0 )
    {
        num = sscanf(attributes.get("sleep").c_str(), " %i", &sleep );
        if( num == 0 )
        {
            sleep = 1;
        }
    } else if( attributes.get("rate").compare("") != 0 )
    {
        num = sscanf( attributes.get("rate").c_str(), " %lf", &rate );
        if( num == 0 )
        {
            rate = 0.1;
        } else 
        {
            rate = rate / 1000;
        }
    }            
}
  
void TimeModule::start()
{
    if( isInitialized() == 1 && rate != 0 )
    {
        count = 0;
        startTime = OSUtils::currentTime();
    }    
} 
    
int TimeModule::stop()
{    
    if( rate != 0 )
    {
        count++;
        double s = count/rate - ( OSUtils::currentTime() - startTime );
        if( s >= 10 )
        {
            OSUtils::sleep( s );
        }
    }
    else if( sleep != 0 )
    {
        OSUtils::sleep( sleep );
    }
    return 0;
}
