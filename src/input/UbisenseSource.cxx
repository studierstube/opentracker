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
/** source file for Ubisense source node.
  *
  * @author Mathis Csisinko
  *
  * $Id$
  * @file                                                                   */
 /* ======================================================================= */

// this will remove the warning 4786
#include "../tool/disable4786.h"

#include "UbisenseSource.h"
#ifdef USE_UBISENSE

#ifdef WIN32
#pragma comment(lib,"UClientAPI.lib")
#endif

namespace ot {

UbisenseSource::UbisenseSource(const Object &object,const LocationClient &locationClient,DataClient &dataClient): Node(),button(0x0000),lastTime(0.),buttonTime(0.),state(),object(object),locationClient(locationClient),dataClient(dataClient)
{
#ifdef _DEBUG
	dataClient.notify(object,NotifyBeep);
	dataClient.notify(object,NotifyFlashGreen);
#endif
}

UbisenseSource::~UbisenseSource()
{
#ifdef _DEBUG
	dataClient.notify(object,NotifyBeep);
	dataClient.notify(object,NotifyFlashRed);
#endif
}

bool UbisenseSource::calcState()
{
	Location location;
	MyNameClient client;
    
	printf("\n");
	//std::cout << "Getting all named people:"; 
	UClientAPI::Map<Object,String> all_people = client.get_all_named(Type("Person")); 
	//for (UClientAPI::Map<Object,String>::const_iterator i = all_people.begin(); i != all_people.end(); ++i) 
	//{   printf("\n");
	    //std::cout << " " << (*i).first.to_string() << " has name " << (*i).second << std::endl; 
	//}

	String found_name; 
	if (!client.get_object_name(object,found_name)) 
	{ std::cout << "Cannot get name of " << object.to_string(); 
	  return false; 
	} 
	std::cout << "Ubitag name: " << found_name;

	if (locationClient.get_object_location(object,location) && location.time_ > lastTime)
	{
		state = State();
		state.position[0] = static_cast<float>(location.pos_.x_); 
		state.position[1] = static_cast<float>(location.pos_.y_); 
		state.position[2] = static_cast<float>(location.pos_.z_);

		
		printf("\n  location x: %f", state.position[0]);
		printf("\n  location y: %f", state.position[1]);
		printf("\n  location z: %f\n\n", state.position[2]);

		if (buttonTime > lastTime)
		{
			state.button = button;
			button = 0x0000;
		}
		state.time = (location.time_ > buttonTime ? location.time_: buttonTime) * 1000.;
		state.confidence = 1.f;
		lastTime = location.time_ > buttonTime ? location.time_: buttonTime;
		return true;
	}
	else
		if (buttonTime > lastTime)
		{
			state.button = button;
			state.time = buttonTime * 1000.;
			state.confidence = 1.f;
			button = 0x0000;
			lastTime = buttonTime;
    		return true;
		}
	return false;
}

void UbisenseSource::setButtonState(unsigned short button,double time)
{
	UbisenseSource::button |= button;
	buttonTime = time;
}

} // namespace ot

#endif  // USE_UBISENSE
