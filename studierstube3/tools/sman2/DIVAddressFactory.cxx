/* ========================================================================
 * Copyright (C) 2000,2001,2002  Vienna University of Technology
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
 * For further information please contact Dieter Schmalstieg under
 * <schmalstieg@ims.tuwien.ac.at> or write to Dieter Schmalstieg,
 * Vienna University of Technology, Favoritenstr. 9-11/188, A1040 Vienna,
 * Austria.
 * ========================================================================
 * PROJECT: sman2
 * ======================================================================== */
/**
 * implementation of DIVAddressFactory
 *
 * @author Gerhard Reitmayr
 *
 * $Id: DIVAddressFactory.cxx 3427 2004-07-14 12:20:49Z tamer $
 * @file                                                                   */
/* ======================================================================= */

#include <ace/Log_Msg.h>
#include "DIVAddressFactory.h"
#include "Application.h"

DIVAddressFactory::DIVAddressFactory()
{}

DIVAddressFactory::~DIVAddressFactory()
{}

int DIVAddressFactory::addressCounter = 2;
int DIVAddressFactory::portCounter = 12345;

void DIVAddressFactory::getDIVAddress( Application & app, std::string * address, int * port )
{
    char buffer[20];
    sprintf(buffer,"224.100.200.%d",addressCounter);
    *address = buffer;
    *port = portCounter;

    addressCounter++;
    portCounter++;

    ACE_DEBUG((LM_DEBUG,"DIVAddress for %s is %s:%d\n", app.appId.c_str(), address->c_str(), *port ));
}
