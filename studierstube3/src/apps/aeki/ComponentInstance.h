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
/** c++ header for ComponentInstance
  *
  * @author Manuel Linsmayer <manuel.linsmayer@expersite.com>, Reza Mostafaie
  *
  * $Id: ComponentInstance.h 3426 2004-07-14 12:11:27Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _COMPONENTINSTANCE_H_
#define _COMPONENTINSTANCE_H_

#ifdef WIN32
#pragma warning (disable: 4786)
#pragma warning (disable: 4788)
#endif

#include <map>
#include <string>
#include <iostream>  // TODO: Delete
#include <Inventor/nodes/SoMatrixTransform.h>

//----------------------------------------------------------------------------
// Units
//----------------------------------------------------------------------------

enum enum_unit { MM = 1, CM = 10, DM = 100, M = 1000 };

inline enum_unit str2unit(const std::string& val) {
	if (val=="mm") return enum_unit(MM);
	if (val=="cm") return enum_unit(CM);
	if (val=="dm") return enum_unit(DM);
	if (val=="m") return enum_unit(M);
	return enum_unit(MM);   // TODO: throw exception
}

/**
 * Abstract class representing a shape
 * @author Manuel Linsmayer , Reza Mostafaie
 * @ingroup aeki
 */
class Shape {

public:
	virtual ~Shape() { };

};

/**
 * Class representing a cube
 * @author Manuel Linsmayer , Reza Mostafaie
 * @ingroup aeki
 */
class Cube : public Shape {

private:
	float radius;

public:
	float getRadius() { return (this->radius); }
	void setRadius(float radius) { this->radius = radius; }

};

/**
 * Class representing a connector
 * @author Manuel Linsmayer , Reza Mostafaie
 * @ingroup aeki
 */
class Connector {

private:
	float x,y,z;

public:
	Connector(float x = 0, float y = 0, float z = 0, enum_unit unit = enum_unit(MM)) : x(x/unit), y(y/unit), z(z/unit) { }
	float getX(enum_unit unit) { return (this->x / unit); }
	float getY(enum_unit unit) { return (this->y / unit); }
	float getZ(enum_unit unit) { return (this->z / unit); }
	void setX(float x, enum_unit unit) { this->x = x * unit; }
	void setY(float y, enum_unit unit) { this->y = y * unit; }
	void setZ(float z, enum_unit unit) { this->z = z * unit; }

};

/**
 * Class representing a component instance
 * @author Manuel Linsmayer , Reza Mostafaie
 * @ingroup aeki
 */
class ComponentInstance {

private:
	Shape* shape;
	float dimension_width;
	float dimension_height;
	float dimension_depth;
	std::map<std::string, Connector> connectors;
	unsigned int tracker;
	SoMatrixTransform* positioning;

public:
	Shape* getShape() { return (this->shape); }
	void setShape(Shape* shape) { this->shape = shape; }
	float getWidth(enum_unit unit) const { return (this->dimension_width / unit); }
	float getHeight(enum_unit unit) const { return (this->dimension_height / unit); }
	float getDepth(enum_unit unit) const { return (this->dimension_depth / unit); }
	void setWidth(float width, enum_unit unit) { this->dimension_width = width * unit; }
	void setHeight(float height, enum_unit unit) { this->dimension_height = height * unit; }
	void setDepth(float depth, enum_unit unit) { this->dimension_depth = depth * unit; }
	std::map<std::string, Connector>& getConnectors() { return (this->connectors); }
	unsigned int getTracker() { return (this->tracker); }
	void setTracker(unsigned int tracker) { this->tracker = tracker; }
	SoMatrixTransform& getPositioning() { return (*this->positioning); }
	void setPositioning(SoMatrixTransform& positioning) { this->positioning = &positioning; }

};

#endif

/* ===========================================================================
    End of ComponentInstance.h
=========================================================================== */
