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
/** BAUML Library
  *
  * @author Michael Knapp <knapp@cg.tuwien.ac.at>
  *
  * $Id: arroom.h 3801 2004-09-30 14:02:02Z reitmayr $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef __arroom_h__
#define __arroom_h__

#include <iostream>
#include <vector>
#include <string>
#include <map>

class ARRoom
{
public:
    std::string roomid;

    enum PolygonType { floor = 0, wall = 1, ceiling = 2, portal = 3, special = 4, num_types = 5 };

    class ARVertex
    {
    public:
        union { double comp[3]; struct { double x, y, z; }; };
        int index;
        ARVertex(); 
        void setPosition(const char *positionstr);
    };

    class ARPolygon
    {
    public:
        int type;
        std::vector<int> indexlist;
        std::string name;
        ARPolygon(); 
        ~ARPolygon(); 
        void setVertices(const char *verticesstr);
        void setType(const char *typestr);
        void setName(const char *namestr);
    };

    class ARMarker
    {
    public:
        int number;
        bool override;
        std::string name;
        std::string group;
        double size;
        double cenx, ceny, cenz;
        double dirx, diry, dirz;
        double horx, hory, horz;
        double verx, very, verz;
        ARMarker(); 
        ~ARMarker(); 
        void setNumber(const char *namestr);
        void setName(const char *namestr);
        void setGroup(const char *groupstr);
        void setSize(const char *sizestr);
        void setVertices(const char *verticesstr, ARRoom *owner, int offset);
        void setVertices(ARVertex *pointtl, ARVertex *pointtr, ARVertex *pointbl, ARVertex *pointbr);
        void setOverride(bool or);
        bool isOverrider();
        double getDeterminant();
    };

    class ARPortal
    {
    public:
        std::string room;
        std::string polygon;
        std::string polygonnb;
        ARPortal(); 
        ~ARPortal(); 
    };

    ARRoom();
    virtual ~ARRoom();

    void setRoomId(const char *id);
    void setOrigin(const char *originstr);
    void setOrigin(double x, double y, double z);

    const char *getRoomId();
    void getOrigin(double &x, double &y, double &z);
//    getPolygons(int type, bool absolute);

    int getVertexNum();
    int getPolygonNum();
    int getMarkerNum();
    int getPortalNum();

    void addVertex(ARVertex *vertex);
    void addPolygon(ARPolygon *polygon);
    void addMarker(ARMarker *marker);
    void addPortal(ARPortal *portal);

    ARVertex *getVertex(int i);
    ARPolygon *getPolygon(int i);
    ARPolygon *getPolygon(const char *name);
    ARMarker *getMarker(int i);
    ARPortal *getPortal(int i);

    void getPolygonCenter(ARPolygon *polygon, double &x, double &y, double &z);
    void getPolygonCenterAbsolute(ARPolygon *polygon, double &x, double &y, double &z);
    ARPolygon *getPortalPolygon(ARPortal *portal);

    void cleanUp();

    std::string cell;

private:

protected:
    ARVertex origin;
    std::vector<ARVertex *> vertexlist;
    std::vector<ARPolygon *> polygonlist;
    std::vector<ARMarker *> markerlist;
    std::vector<ARPortal *> portallist;
};

typedef std::vector<ARRoom *> ARRoomVec;
typedef std::map<std::string, ARRoom *> ARRoomMap;

#endif
