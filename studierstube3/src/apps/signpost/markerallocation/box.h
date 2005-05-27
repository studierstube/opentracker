/* ========================================================================
  * Copyright (C) 2000  Vienna University of Technology
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

 /* ======================================================================= */
/** Marker Allocator
  *
  * @author Michael Knapp <knapp@cg.tuwien.ac.at>
  *
  * $Id: box.h 3426 2004-07-14 12:11:27Z tamer $
  * @file                                                                   */
 /* ======================================================================= */


#ifndef __box_h__
#define __box_h__

class box
{
public:
    float x0, y0, z0;
    float x1, y1, z1;

    void init()
    {
        x0 = +FLT_MAX;
        y0 = +FLT_MAX;
        z0 = +FLT_MAX;
        x1 = -FLT_MAX;
        y1 = -FLT_MAX;
        z1 = -FLT_MAX;
    }

    float width()
    {
        return (x1 - x0);
    }

    float height()
    {
        return (y1 - y0);
    }

    float depth()
    {
        return (z1 - z0);
    }

    bool in(float x, float y, float z)
    {
        return ((x >= x0) && (x < x1) && (y >= y0) && (y < y1) && (z >= z0) && (z < z1));
    }

    void center(float &x, float &y, float &z)
    {
        x = (x0 + x1) * 0.5f;
        y = (y0 + y1) * 0.5f;
        z = (z0 + z1) * 0.5f;
    }

};

#endif;