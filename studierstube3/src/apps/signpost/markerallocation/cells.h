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
  * $Id: cells.h 4222 2005-05-24 14:54:10Z schall $
  * @file                                                                   */
 /* ======================================================================= */


#ifndef __cells_h__
#define __cells_h__

#include <string>
#include <vector>
#include "box.h"
#include "cell.h"



class cells
{
private:
    float cellsize;
    float x0, y0, z0;
    int xd, yd, zd;
    box bbox;

    std::vector<cell *> list;
public:
    cells(box &b, float size)
    {
        cellsize = size;
        bbox = b;
        x0 = floor(bbox.x0 / cellsize) * cellsize;
        y0 = floor(bbox.y0 / cellsize) * cellsize;
        z0 = floor(bbox.z0 / cellsize) * cellsize;
        xd = (int)((bbox.x1 - x0) / cellsize + 1);
        yd = (int)((bbox.y1 - y0) / cellsize + 1);
        zd = (int)((bbox.z1 - z0) / cellsize + 1);

        printf(" x0 %f ,y0 %f ,z0 %f --- xd %d ,yd %d ,zd %d\n", x0, y0, z0, xd, yd, zd);

		xd = 1;
		yd = 1;
		zd = 1;

        int x,y,z;

        for (y=0; y<yd; y++)
        {
            for (z=0; z<zd; z++)
            {
                for (x=0; x<xd; x++)
                {
                    box cb;

                    //cb.x0 = x0 + x * cellsize;
                    //cb.y0 = y0 + y * cellsize;
                    //cb.z0 = z0 + z * cellsize;
                    //cb.x1 = cb.x0 + cellsize;
                    //cb.y1 = cb.y0 + cellsize;
                    //cb.z1 = cb.z0 + cellsize;
                    cell *c = new cell(bbox);  //cb);
                    list.push_back(c);
                }
            }
        }

      /*  for (y=0; y<yd; y++)
        {
            for (z=0; z<zd; z++)
            {
                for (x=0; x<xd; x++)
                {
                    cell *c = list[x+(z+y*zd)*xd];
                    for (int yy=-1; yy<=+1; yy++)
                    {
                        for (int zz=-1; zz<=+1; zz++)
                        {
                            for (int xx=-1; xx<=+1; xx++)
                            {
                                if ((y+yy >= 0) && (y+yy < yd) && (z+zz >= 0) && (z+zz < zd) && (x+xx >= 0) && (x+xx < xd) && ((yy != 0) || (zz != 0) || (xx != 0))) 
                                    c->neighborcells.push_back(list[(x+xx)+((z+zz)+(y+yy)*zd)*xd]);
                            }
                        }
                    }
                }
            }
        }
*/
    }

    ~cells()
    {
    }

    void addmarker(markerinfo &mi)
    {
/*
        int x = (int)((mi.marker->cenx - x0) / cellsize);
        int y = (int)((mi.marker->ceny - y0) / cellsize);
        int z = (int)((mi.marker->cenz - z0) / cellsize);
        if ((x >= 0) && (x < xd) && (z >= 0) && (z < zd)) list[(zd-1-z)*xd+x]->addmarker(mi);
*/
		printf("!!! add marker: %f %f %f list size: %d\n", mi.marker->cenx, mi.marker->ceny, mi.marker->cenz, list.size());

        cell *c = get((float)mi.marker->cenx,(float) mi.marker->ceny,(float) mi.marker->cenz);
		
        if (c != NULL) 
		{ c->addmarker(mi);
		  printf("marker added \n\n");
		}
		else
			printf("NULL\n");

    }

    int num()
    {
        return list.size();
    }

    cell *get(int i)
    {
        return list[i];
    }

    cell *get(float x, float y, float z)
    {
        int xx = (int)((x - x0) / cellsize);
        int yy = (int)((y - y0) / cellsize);
        int zz = (int)((z - z0) / cellsize);
        
		//if ((yy >= 0) && (yy < yd) && (xx >= 0) && (xx < xd) && (zz >= 0) && (zz < zd)) 
		return list[0];  //list[xx+(zz+yy*zd)*xd];

        //return NULL;
    }

    void update()
    {
        int num = list.size();
        for (int i=0; i<num; i++)
        {
            list[i]->update();
            char buf[32];
            sprintf(buf, "cell%i", i);
			//printf("cell%i", i);
            list[i]->name = buf;
        }
    }

};

#endif