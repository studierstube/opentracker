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
  * $Id$
  * @file                                                                   */
 /* ======================================================================= */

// this will remove the warning 4786
#include "../tool/disable4786.h"

#include "TimeModule.h"
#include <stdio.h>
#include <iostream>

using namespace std;

#include <ace/Log_Msg.h>

namespace ot {

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
    if( attributes.get("display").compare("true") == 0 )
    {
        display = 1;
    }
}
  
void TimeModule::start()
{
    if( isInitialized() == 1 )
    {
        count = 0;
        startTime = OSUtils::currentTime();
    }    
} 
    
int TimeModule::stop()
{    
    count++;
    if( rate != 0 )
    {        
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

void TimeModule::close()
{
    if( display == 1 )
    {
        //cout << "Framerate " << count * 1000 / ( OSUtils::currentTime() - startTime ) << endl;
		ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:Framerate %f\n"), count * 1000 / ( OSUtils::currentTime() - startTime )));
    }
}

} // namespace ot
