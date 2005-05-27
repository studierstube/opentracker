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
  * $Id: main.cpp 4222 2005-05-24 14:54:10Z schall $
  * @file                                                                   */
 /* ======================================================================= */

//for windows dialogs
#include <windows.h>
#include <commctrl.h> 
#include "resource.h"
//--

#include <gl/glut.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <math.h>
#include <float.h>
#include <iostream>
#include <list>
#include <vector>

#include "types.h"
#include "box.h"
#include "cells.h"

using namespace std;

//---------------------------------------------------------

ARBuilding *building;

list<markerinfo> markers;
list<usedmarkerinfo> usedmarkers;
cells *cellmanager = 0;

box boundingbox;

bool showmarkernum = true;
bool shownumber = false;
bool showsinglenumber = false;
int singlenumber = 0;


float rasterscale = 1.0f;

//---------------------------------------------------------

void getmarkers()
{
    int rn = building->getRoomNum();
    for (int i=0; i<rn; i++)
    {
        ARRoom *room = building->getRoom(i);
        int mn = room->getMarkerNum();
        for (int j=0; j<mn; j++)
        {
            ARRoom::ARMarker *marker = room->getMarker(j);
            markerinfo mi;
            mi.num = -1;
            mi.marker = marker;
            mi.room = room;
            mi.assigned = false;
            markers.push_back(mi);
        }
    }
    markers.sort();
}

void showmarkers()
{
    cout << "==================================================" << endl;
    cout << "List of all markers" << endl;
    cout << "--------------------------------------------------" << endl;
    for (list<markerinfo>::iterator it = markers.begin(); it != markers.end(); it++)
    {
        markerinfo &mi = (*it);
        cout.setf(ios::right);   
        cout.width(5);
        cout << mi.marker->number;
        cout.setf(ios::left, 0);   
        cout << " - marker: " << mi.marker->name.c_str();
        cout << " - room: " << mi.room->getRoomId();
//        cout << " - num: " << mi.num;
        cout << endl;
    }
    cout <<  markers.size() << " Markers" << endl;
}

void extractmarkers()
{
  /*  for (list<markerinfo>::iterator it = markers.begin(); it != markers.end(); it++)
    {
        markerinfo &mi = (*it);
        if (mi.marker->name != "UNDEFINED")
        {
            mi.assigned = true;
            if (mi.marker->name.substr(0, 7) == "pattern")
            {
                char *end = NULL;
                char buf[4];
                strcpy(buf, mi.marker->name.substr(7, 2).c_str());
                int num = strtol(buf, &end, 10);
                if (*end == '\0')
                {
                    usedmarkerinfo umi;
                    umi.name = mi.marker->name;
                    umi.count = 0;
                    umi.num = num;
                    usedmarkers.remove(umi);
                    usedmarkers.push_back(umi);
                }
            }
        }
    }

    cout << "==================================================" << endl;
    cout << "List of used markers" << endl;
    cout << "--------------------------------------------------" << endl;
    int i=0;
    for (list<usedmarkerinfo>::iterator jt = usedmarkers.begin(); jt != usedmarkers.end(); jt++)
    {
        usedmarkerinfo &umi = (*jt);
        umi.num = i++;
    }

*/

	 for (list<markerinfo>::iterator it = markers.begin(); it != markers.end(); it++)
    {
        markerinfo &mi = (*it);
        
        mi.assigned = true;
		if (mi.marker->name != "UNDEFINED")
        {
            if (mi.marker->name.substr(0, 7) == "pattern")
            {
                char *end = NULL;
                char buf[4];
                strcpy(buf, mi.marker->name.substr(7, 3).c_str());
                int num = strtol(buf, &end, 10);
                if (*end == '\0')
                {
                    usedmarkerinfo umi;
                    umi.name = mi.marker->name;
                    umi.count = 0;
                    umi.num = num;
                    usedmarkers.remove(umi);
                    usedmarkers.push_back(umi);
				}
            }
		}
       
    }

    cout << "==================================================" << endl;
    cout << "List of used markers" << endl;
    cout << "--------------------------------------------------" << endl;
    int i=0;
    for (list<usedmarkerinfo>::iterator jt = usedmarkers.begin(); jt != usedmarkers.end(); jt++)
    {
        usedmarkerinfo &umi = (*jt);
        umi.num = i++;
    }

    cout << usedmarkers.size() << " Used Markers" << endl;
}

void getboundingbox()
{
    int rn = building->getRoomNum();
    for (int i=0; i<rn; i++)
    {
        ARRoom *room = building->getRoom(i);
        int pn = room->getPolygonNum();
        for (int j=0; j<pn; j++)
        {
            ARRoom::ARPolygon *polygon = room->getPolygon(j);
            int pon = polygon->indexlist.size();
            for (int k=0; k<pon; k++)
            {
                ARRoom::ARVertex *vertex = room->getVertex(polygon->indexlist[k]);
                if (vertex->x < boundingbox.x0) boundingbox.x0 = vertex->x;
                if (vertex->y < boundingbox.y0) boundingbox.y0 = vertex->y;
                if (vertex->z < boundingbox.z0) boundingbox.z0 = vertex->z;
                if (vertex->x > boundingbox.x1) boundingbox.x1 = vertex->x;
                if (vertex->y > boundingbox.y1) boundingbox.y1 = vertex->y;
                if (vertex->z > boundingbox.z1) boundingbox.z1 = vertex->z;
            }
        }
    }

    cout << "--------------------------------------------------" << endl;
    cout << "Bounding Box: (" << boundingbox.x0 << "," << boundingbox.y0 << "," << boundingbox.z0 << ")-(" << boundingbox.x1 << "," << boundingbox.y1 << "," << boundingbox.z1 << ")" << endl;
}

void makecells()
{
    cellmanager = new cells(boundingbox, building->getMarkerGridSize());

    // put markers into cells
    for (list<markerinfo>::iterator it = markers.begin(); it != markers.end(); it++)
    {
        markerinfo &mi = (*it);
        cellmanager->addmarker(mi);
    }
}

usedmarkerinfo *findmarker(string &name)
{
    for (list<usedmarkerinfo>::iterator jt = usedmarkers.begin(); jt != usedmarkers.end(); jt++)
    {
        usedmarkerinfo &umi = (*jt);
        if (umi.name == name) return &umi;
    }
    return 0;
}

usedmarkerinfo usemarker(markerinfo &mmi)
{
    usedmarkerinfo *uumi = 0;

    // least used marker 
    for (list<usedmarkerinfo>::iterator jt = usedmarkers.begin(); jt != usedmarkers.end(); jt++)
    {
        usedmarkerinfo &umi = (*jt);
        if (!umi.used) 
        {
            if (uumi == 0)
            {
                uumi = &umi;
            }
            else
            {
                if (umi.count < uumi->count) uumi = &umi;
            }
        }
    }


    // fartest marker
/*
    float maxdist = 0;
    for (list<markerinfo>::iterator it = markers.begin(); it != markers.end(); it++)
    {
        markerinfo &mi = (*it);
        if (mi.assigned) 
        {
            usedmarkerinfo *fumi = findmarker(mi.marker->name);
            if (!fumi->used)
            {
                float xd = mmi.marker->cenx - mi.marker->cenx;
                float yd = mmi.marker->ceny - mi.marker->ceny;
                float dist = sqrt(xd*xd + yd*yd);
                if (dist > maxdist) 
                {
                    maxdist = dist;
                    uumi = fumi;
                }
            }
        }
    }
*/
    if (uumi != 0)
    {
        uumi->used = true;
        uumi->count++;
        return *uumi;
    }

    usedmarkerinfo umi;
    char name[128];
    int n = usedmarkers.size();
    sprintf(name, "pattern%i", n);
    cout << "New Marker (" << n << "): " << name << endl;
    umi.name = name;
    umi.used = true;
    umi.count = 1;
    umi.num = n;
    usedmarkers.push_back(umi);
    return (*(--(usedmarkers.end())));
}

void assignmarkers()
{
    cellmanager->update();

	cout << "cellmanager->num:" << cellmanager->num() << endl;

    int i,j,k,l;
    for (i=0; i<cellmanager->num(); i++)
    {
        cell *c = cellmanager->get(i);

        for (list<usedmarkerinfo>::iterator jt = usedmarkers.begin(); jt != usedmarkers.end(); jt++)
        {
            usedmarkerinfo &umi = (*jt);
            umi.used = false;
        }

        int c_num = c->neighborcells.size();
        for (j=-1; j<c_num; j++)
        {
            cell *nc = (j < 0) ? (c) : (c->neighborcells[j]);
            if (nc != 0)
            {
                int nc_num = nc->visiblemarkers.size();
                for (l=0; l<nc_num; l++)
                {
                    markerinfo *mi = nc->visiblemarkers[l];
                    if (mi->assigned)
                    {
                        usedmarkerinfo *umi = findmarker(mi->marker->name);
                        if (umi != 0) 
                        {
                            umi->used = true;
                            mi->num = umi->num;
                        }
                    }
                }
            }
        }

        for (k=0; k<c->markers.size(); k++)
        {
            markerinfo *mi = c->markers[k];
            if (!mi->assigned)
            {
                usedmarkerinfo &umi = usemarker(*mi);
                mi->num = umi.num;
                mi->marker->name = umi.name;
                mi->assigned = true;

				

            }
        }
    }
}

void showusedmarkers()
{
    cout << "--------------------------------------------------" << endl;
    int i = 0;
    for (list<usedmarkerinfo>::iterator jt = usedmarkers.begin(); jt != usedmarkers.end(); jt++)
    {
        usedmarkerinfo &umi = (*jt);
        cout << "  Marker (" << i << ") - name: " << umi.name << " - count: " << umi.count << endl;
        i++;
    }
    cout << usedmarkers.size() << " Used Markers" << endl;
    cout << "--------------------------------------------------" << endl;
    for (list<markerinfo>::iterator it = markers.begin(); it != markers.end(); it++)
    {
        markerinfo &mi = (*it);
        cout.setf(ios::right);   
        cout.width(5);
        cout << mi.marker->number;
        cout.setf(ios::left, 0);   
        cout << " - marker: " << mi.marker->name.c_str();
        cout << " - room: " << mi.room->getRoomId();
        cout << endl;
    }
    cout << markers.size() << " Markers" << endl;

}

//---------------------------------------------------------

void disptext(float x, float y, const char *text, bool large = false)
{
    void *font = (large) ? GLUT_BITMAP_HELVETICA_18 : GLUT_BITMAP_HELVETICA_12;
    int len = strlen(text);
    for (int i=0; i<len; i++)
    {
        char ch = text[i];
        glRasterPos2f(x, y);
        x += (glutBitmapWidth(font, ch) * rasterscale);
        glutBitmapCharacter(font, ch);
    }
}

void drawrooms()
{
    glColor3f(1, 1, 1);
    int rn = building->getRoomNum();

	cout << rn << " rooms" <<endl;

    for (int i=0; i<rn; i++)
    {
//        glColor3ub(0, rand() % 256, rand() % 256);
        ARRoom *room = building->getRoom(i);
        int pn = room->getPolygonNum();
        for (int j=0; j<pn; j++)
        {
            ARRoom::ARPolygon *polygon = room->getPolygon(j);
            if (polygon->type == ARRoom::floor)
            {

				cout << room->getRoomId() << "  polygons: " << polygon->indexlist.size() << endl;
                glBegin(GL_LINE_LOOP);
                int pon = polygon->indexlist.size();
                for (int k=0; k<pon; k++)
                {
                    ARRoom::ARVertex *vertex = room->getVertex(polygon->indexlist[k]);
                    glVertex3f(vertex->x, -(vertex->y), 0);//(vertex->x, vertex->z, 0);
                }
                glEnd();
            }
        }
    }
}

void drawmarkers()
{
    const float scale = 0.33f;
    for (list<markerinfo>::iterator it = markers.begin(); it != markers.end(); it++)
    {
        markerinfo &mi = (*it);
        ARRoom::ARMarker *marker = mi.marker;

        glColor3f(1, 0, 0);
        float x, y;
        glBegin(GL_TRIANGLES);
        x = marker->cenx - marker->diry * scale * 0.5;
        y = marker->ceny + marker->dirx * scale * 0.5; //changed z into y
        glVertex3f(x, -y, 0);
        x = marker->cenx + marker->diry * scale * 0.5;
        y = marker->ceny - marker->dirx * scale * 0.5;
        glVertex3f(x, -y, 0);
        x = marker->cenx + marker->dirx * scale;
        y = marker->ceny + marker->diry * scale;
        glVertex3f(x, -y, 0);
        glEnd();

        if (showmarkernum)
        {
            if ((!showsinglenumber) || (showsinglenumber && ((singlenumber % usedmarkers.size()) == mi.num)))
            {
                glColor3f(0, 0, 0);
                char buf[16];
                sprintf(buf, "%i", (shownumber) ? (marker->number) : (mi.num));
                disptext(marker->cenx, -(marker->ceny), buf, showsinglenumber); //cenx, cenz
//                disptext(marker->cenx, marker->cenz, mi.marker->name.c_str(), showsinglenumber);
            }
        }

    }
}

void drawzero()
{
    glColor3f(0, 0, 1);
    glBegin(GL_QUADS);
    glVertex3f(-2*rasterscale, -2*rasterscale, 0);
    glVertex3f(+2*rasterscale, -2*rasterscale, 0);
    glVertex3f(+2*rasterscale, +2*rasterscale, 0);
    glVertex3f(-2*rasterscale, +2*rasterscale, 0);
    glEnd();
}

void drawcells()
{
    glColor3f(0, 0.5, 0);
    for (int i=0; i<cellmanager->num(); i++)
    {
        cell *c = cellmanager->get(i);
        glBegin(GL_LINE_LOOP);
        glVertex3f(c->bbox.x0, -(c->bbox.y0), 0); //changed z into y
        glVertex3f(c->bbox.x1, -(c->bbox.y0), 0);
        glVertex3f(c->bbox.x1, -(c->bbox.y1), 0);
        glVertex3f(c->bbox.x0, -(c->bbox.y1), 0);
        glEnd();

        float x, y, z;
        c->bbox.center(x, y, z);
        char buf[16];
        sprintf(buf, "%i,%i", c->num(), c->numnb());
        disptext(x, -y, buf); //changed z into y


    }
}

//---------------------------------------------------------

void idle()
{
//    glutPostRedisplay();
}

void display()
{
    glClearColor(0.7f, 0.7f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    drawrooms();
    drawcells(); 
    drawzero();
    drawmarkers();

    glFlush();
    glutSwapBuffers();
}

void mouse(int button, int state, int x, int y)
{
    glutPostRedisplay();
}

void keyboard(unsigned char button, int x, int y)
{
    switch (button)
    {
        case 'm':
        {
            showmarkernum = !showmarkernum;
            break;
        }
        case 'n':
        {
            shownumber = !shownumber;
            break;
        }
        case 's':
        {
            showsinglenumber = !showsinglenumber;
            break;
        }
        case '+':
        {
            singlenumber++;
            break;
        }
    }
    glutPostRedisplay();
} 

void reshape(int w, int h)
{
    float x0, x1, y0, y1;
    float ar0 = ((float)w) / ((float)h);
    float ar1 = boundingbox.width() / boundingbox.depth();

    if (ar0 < ar1)
    {
        x0 = boundingbox.x0;
        x1 = boundingbox.x1;
        float d = ((boundingbox.width() * h) / w) - boundingbox.depth();
        y0 = boundingbox.y0 - d * 0.5f;
        y1 = boundingbox.y1 + d * 0.5f;
        rasterscale = boundingbox.width() / w;
    }
    else
    {
        y0 = boundingbox.z0;
        y1 = boundingbox.z1;
        float d = ((boundingbox.depth() * w) / h) - boundingbox.width();
        x0 = boundingbox.y0 - d * 0.5f; // changed z into y
        x1 = boundingbox.y1 + d * 0.5f; // changed z into y
        rasterscale = boundingbox.depth() / h;
    }

    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(x0, x1, y1, y0, -1, +1);  
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glFlush();
    glutPostRedisplay();
}

int main(int argc, char **argv)
{
    if (argc != 5)
    {
        cout << "Usage: markerallocator input-type input-filename output-type output-filename\n";
        printf("\n");
        printf("input-type = 'o' for ocar, 'b' for bauml\n");
        printf("output-type = 'o' for ocar, 'b' for bauml\n");
        return 1;
    }

    BAUML bauml;
    cout << "Loading BAUML-File: " << argv[2] << endl;
    if (argv[1][0] == 'b') bauml.read_bauml(argv[2]);
    if (argv[1][0] == 'o') bauml.read_ocar(argv[2]);
    building = bauml.getBuilding();
    building->cleanUp();

    getmarkers();
    showmarkers();
    extractmarkers();
    getboundingbox();
    makecells();
    assignmarkers();
    showusedmarkers();

    cout << "Writing ... " << argv[4] << endl;
    if (argv[3][0] == 'b') bauml.write_bauml(argv[4]);
    if (argv[3][0] == 'o') bauml.write_ocar(argv[4]);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
    glutInitWindowSize(640, 480);
    glutCreateWindow("Marker Allocator");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutIdleFunc(idle);
    glutMainLoop();


    return 0;
}
