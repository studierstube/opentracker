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
  * $Id: cell.h 3801 2004-09-30 14:02:02Z reitmayr $
  * @file                                                                   */
 /* ======================================================================= */


#ifndef __cell_h__
#define __cell_h__

#include <vector>
#include <string>
#include "box.h"
#include "types.h"

class cell
{
private:

public:
    std::vector<markerinfo *> markers;
    std::vector<markerinfo *> visiblemarkers;
    std::vector<cell *> neighborcells;
    box bbox;
    std::string name;


    cell(box &b)
    {
        bbox = b;
    }

    ~cell()
    {
    }

    void addmarker(markerinfo &mi)
    {
        markers.push_back(&mi);
    }

    int num()
    {
        return markers.size();
    }

    int numnb()
    {
        int n = num();
        int nc_num = neighborcells.size();
        for (int i=0; i<nc_num; i++) if (neighborcells[i] != 0) n += neighborcells[i]->num();
        return n;
    }

    void update()
    {
        visiblemarkers.clear();
        int nc_num = neighborcells.size();
        for (int k=-1; k<nc_num; k++)
        {
            cell *c = (k < 0) ? (this) : (neighborcells[k]);
            if (c != 0)
            {
                for (unsigned int l=0; l<c->markers.size(); l++)
                {
                    markerinfo *mi = c->markers[l];
                    visiblemarkers.push_back(mi);
                }
            }
        }
    }
};



#endif