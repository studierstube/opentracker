 /* ========================================================================
  * Copyright (C) 2003  Vienna University of Technology
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
  * PROJECT: PUCServer
  * ======================================================================== */
/** header file for PUCServer.
  *
  * @author Thomas Psik
  *
  * $Id: PUCInterface.h 3426 2004-07-14 12:11:27Z tamer $
  * @file                                                                   */
 /* ======================================================================= */


#ifndef _PUCINTERFACE_H
#define _PUCINTERFACE_H


/**
 * This module supports PUC clients
 * @ingroup pucbasicLib
 */

class PUCInterface
{
public:
	void setDataPtr(void * data) { deviceDataPtr = data; }
	
	// pure virtual functions
	virtual std::string getSpecificationMethod()  = 0;
	
	virtual void stateChangeReqMethod(std::string, std::string) = 0;
	
	virtual void fullStateMethod() = 0;

	virtual void invokeCommandMethod(std::string) = 0;

protected:
	void *deviceDataPtr;
};


#endif // !defined(_PUCINTERFACE_H)
