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
 * header file for DIVAddressFactory
 *
 * @author Gerhard Reitmayr
 *
 * $Id: DIVAddressFactory.h 3427 2004-07-14 12:20:49Z tamer $
 * @file                                                                   */
/* ======================================================================= */


#ifndef _DIVADDRESSFACTORY_H_
#define _DIVADDRESSFACTORY_H_

#include <string>

class Application;

/**
 * This class generates a new DIV multicast group address for every call. It 
 * only implements an interface of static methods. The Application class uses
 * it to define a DIV multicast group for every application registered with
 * the system. All addresses must be unique. The current implementation starts
 * with a fixed IP address and port number and increases both. A possible
 * future more will use several port numbers for a single IP address.
 * 
 * Design : It encapsulates the address generation and abstracts it away 
 * from the Application class. If extended with a singleton pattern, it would
 * be possible to install different implementations at startup time instead
 * of compile time.
 *
 * @author Gerhard Reitmayr
 * @ingroup sman2
 */
class DIVAddressFactory  
{
public:
    /**
     * generates a new DIV multicast address and returns it in the passed 
     * parameters.
     * @param app reference to the Application object, can be usefull, if 
     *      address should depend on it
     * @param address pointer to string containing IP address. The string is
     *      modified to return the new address.
     * @param port pointer to int containing the port number. The int is
     *     modified to return the new port.
     */
    static void getDIVAddress( Application & app, std::string * address, int * port );

protected:
    /// variable to count up the addresses
    static int addressCounter;
    /// counter for the port numbers
    static int portCounter;

private:
    /// private constructor to disallow instance of this class.
	DIVAddressFactory();
	virtual ~DIVAddressFactory();

};

#endif // _DIVADDRESSFACTORY_H_
