 /* ========================================================================
  * Copyright (C) 2000,2001  Vienna University of Technology
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
  * PROJECT: Studierstube
  * ======================================================================== */ 
/** c++ header for AssemblyStep
  *
  * @author Manuel Linsmayer <manuel.linsmayer@expersite.com>, Reza Mostafaie
  *
  * $Id: AssemblyStep.h 3426 2004-07-14 12:11:27Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _ASSEMBLYSTEP_H_
#define _ASSEMBLYSTEP_H_

#ifdef WIN32
#pragma warning (disable: 4786)
#pragma warning (disable: 4788)
#endif

#include <string>
#include <vector>

/**
 * Class representing an assembly step
 * @author Manuel Linsmayer , Reza Mostafaie
 * @ingroup aeki
 */
class AssemblyStep {

public:
	virtual ~AssemblyStep() { };

};

/**
 * Struct representing a connection
 * @author Manuel Linsmayer , Reza Mostafaie
 * @ingroup aeki
 */
struct Connection {
	std::string connectorName1;
	std::string componentInstanceName1;
	std::string connectorName2;
	std::string componentInstanceName2;
};

/**
 * Class representing a component addition
 * @author Manuel Linsmayer , Reza Mostafaie
 * @ingroup aeki
 */
class ComponentInstanceAddition : public virtual AssemblyStep {

private:
	std::string componentInstanceName;
	std::vector<Connection> connections;

public:
	std::string getComponentInstanceName() { return (this->componentInstanceName); }
	void setComponentInstanceName(std::string componentInstanceName) { this->componentInstanceName = componentInstanceName; }
	std::vector<Connection>& getConnections() { return (this->connections); }

};

//----------------------------------------------------------------------------
// Axis
//----------------------------------------------------------------------------

enum enum_axis { XPOS, XNEG, ZPOS, ZNEG };

inline enum_axis str2axis(const std::string& val) {
	if (val=="xpos") return enum_axis(XPOS);
	if (val=="xneg") return enum_axis(XNEG);
	if (val=="zpos") return enum_axis(ZPOS);
	if (val=="zneg") return enum_axis(ZNEG);
	return enum_axis(XPOS);   // TODO: throw exception
};

//----------------------------------------------------------------------------
// Angle
//----------------------------------------------------------------------------

enum enum_angle { DEG90 = 90, DEG180 = 180, DEG270 = 270 };

inline enum_angle str2angle(const std::string& val) {
	if (val=="90") return enum_angle(DEG90);
	if (val=="180") return enum_angle(DEG180);
	if (val=="270") return enum_angle(DEG270);
	return enum_angle(DEG90);   // TODO: throw exception
};

/**
 * Class representing a rotation
 * @author Manuel Linsmayer , Reza Mostafaie
 * @ingroup aeki
 */
class Rotation : public virtual AssemblyStep {
	
private:
	enum_axis axis;
	enum_angle angle;

public:
	Rotation() : axis(enum_axis(XPOS)), angle(DEG90) { }
	Rotation(enum_axis axis) : axis(axis), angle(DEG90) { }
	Rotation(enum_axis axis, enum_angle angle) : axis(axis), angle(angle) { }
	enum_axis getAxis() { return (this->axis); }
	void setAxis(enum_axis axis) { this->axis = axis; }
	enum_angle getAngle() { return (this->angle); }
	void setAngle(enum_angle angle) { this->angle = angle; }

};

#endif

/* ===========================================================================
    End of AssemblyStep.h
=========================================================================== */
