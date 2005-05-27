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
/** BAUML Viewer
  *
  * @author Michael Knapp <knapp@cg.tuwien.ac.at>
  *
  * $Id: main.cxx 3801 2004-09-30 14:02:02Z reitmayr $
  * @file                                                                   */
 /* ======================================================================= */

//for windows dialogs
//#include <windows.h>
//#include <commctrl.h> 
//#include "resource.h"
//--

#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoSphere.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoMaterialBinding.h>
#include <Inventor/nodes/SoOrthographicCamera.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoDirectionalLight.h>
#include <Inventor/nodes/SoLightModel.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoIndexedFaceSet.h>
#include <Inventor/nodes/SoIndexedLineSet.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoAsciiText.h>
#include <Inventor/nodes/SoText2.h>
#include <Inventor/nodes/SoText3.h>
#include <Inventor/nodes/SoFont.h>
#include <Inventor/nodes/SoEventCallback.h>
#include <Inventor/nodes/SoCone.h>
#include <Inventor/nodes/SoShapeHints.h>
#include <Inventor/events/SoKeyboardEvent.h>
#include <Inventor/SoDB.h>
#include <Inventor/Win/SoWin.h>
#include <Inventor/Win/SoWinRenderArea.h>

#include <Inventor/Win/viewers/SoWinExaminerViewer.h>
#include <Inventor/Win/viewers/SoWinPlaneViewer.h>  

// #include <qwidget.h>

#include "bauml.h"

#include <sstream>

using namespace std;

char VisiblePolygons[ARRoom::num_types];
bool VisibleText;
bool VisibleMarker;
bool VisibleVertices;

#define POLY_LINE 0x01
#define POLY_FILL 0x02
#define ALL_ROOMS "ALL_ROOMS" 

float VertexScaleFactor = 1.0f;
const char *RoomVertices = ALL_ROOMS;

BAUML *bauml;
char filename[512];
char filetype;


SoSeparator *createText(double x, double y, double z, const char *str)
{
    SoSeparator *text_sep = new SoSeparator();
    text_sep->ref();
/*
    SoDrawStyle *ds = new SoDrawStyle();
    text_sep->addChild(ds); 
    ds->style.setValue(SoDrawStyle::FILLED);
*/
    SoTransform *trans = new SoTransform();
    text_sep->addChild(trans);
    trans->translation.setValue(x, y, z + 0.1);

    SoAsciiText *text = new SoAsciiText();
    text_sep->addChild(text);
    text->string.setValue(str);
    text->justification.setValue(SoAsciiText::CENTER);

    return text_sep;
}

SoSeparator *createBuilding(BAUML *bauml)
{
    cout << "Create Building\n";

    int i,j,k;

    SoSeparator *sep = new SoSeparator();
    sep->ref();
  

    SoMaterialBinding *mb = new SoMaterialBinding();
    sep->addChild(mb);
    mb->value.setValue(SoMaterialBinding::PER_FACE_INDEXED);

    SoMaterial *mat = new SoMaterial();
    sep->addChild(mat);

    mat->diffuseColor.set1Value(ARRoom::wall,    1,1,0);
    mat->diffuseColor.set1Value(ARRoom::floor,   0,1,0);
    mat->diffuseColor.set1Value(ARRoom::ceiling, 0,1,1);
    mat->diffuseColor.set1Value(ARRoom::portal,  1,0,1);
    mat->diffuseColor.set1Value(ARRoom::special, 1,1,1);

    mat->transparency.set1Value(ARRoom::wall,    0.7f);
    mat->transparency.set1Value(ARRoom::floor,   0.7f);
    mat->transparency.set1Value(ARRoom::ceiling, 0.7f);
    mat->transparency.set1Value(ARRoom::portal,  0.7f);
    mat->transparency.set1Value(ARRoom::special, 0.7f);

    SoFont *f = new SoFont();
    sep->addChild(f); 
    f->name.setValue("Helvetica");
    f->size.setValue(0.5);

    ARBuilding *building = bauml->getBuilding();
    for (i=0; i<building->getRoomNum(); i++)
    {
        ARRoom *room = building->getRoom(i);
        double ox, oy, oz;
        room->getOrigin(ox, oy, oz);

        cout << "Room: " << room->getRoomId() << "\n";

        SoSeparator *room_sep = new SoSeparator();
        sep->addChild(room_sep);

        // vertices

        SoCoordinate3 *polygon_coords = new SoCoordinate3();
        room_sep->addChild(polygon_coords);

        int cidx = 0;
        for (j=0; j<room->getVertexNum(); j++)
        {
            ARRoom::ARVertex *vertex = room->getVertex(j);
            polygon_coords->point.set1Value(cidx++, ox+vertex->x, oy+vertex->y, oz+vertex->z);
        }

        // polygons

        SoDrawStyle *ds = new SoDrawStyle();
        room_sep->addChild(ds); 
        ds->style.setValue(SoDrawStyle::LINES);
        ds->lineWidth.setValue(2);

        SoShapeHints *sh1 = new SoShapeHints();
        sh1->faceType.setValue(SoShapeHints::CONVEX);
        room_sep->addChild(sh1);

        SoIndexedFaceSet *faceset_line = new SoIndexedFaceSet();
        room_sep->addChild(faceset_line);

        SoDrawStyle *ds2 = new SoDrawStyle();
        room_sep->addChild(ds2); 
        ds2->style.setValue(SoDrawStyle::FILLED);
        ds2->lineWidth.setValue(3);

        SoShapeHints *sh2 = new SoShapeHints();
        sh2->faceType.setValue(SoShapeHints::UNKNOWN_FACE_TYPE);
        room_sep->addChild(sh2);

        SoIndexedFaceSet *faceset_fill = new SoIndexedFaceSet();
        room_sep->addChild(faceset_fill);

        int vidx_line = 0;
        int midx_line = 0;
        int vidx_fill = 0;
        int midx_fill = 0;
        for (j=0; j<room->getPolygonNum(); j++)
        {
            ARRoom::ARPolygon *polygon = room->getPolygon(j);
            if (VisiblePolygons[polygon->type] & POLY_LINE)
            {
                for (k=0; k<polygon->indexlist.size(); k++) faceset_line->coordIndex.set1Value(vidx_line++, polygon->indexlist[k]);
                faceset_line->coordIndex.set1Value(vidx_line++, SO_END_FACE_INDEX);
                faceset_line->materialIndex.set1Value(midx_line++, polygon->type);
            }

            if (VisiblePolygons[polygon->type] & POLY_FILL)
            {
                for (k=0; k<polygon->indexlist.size(); k++) faceset_fill->coordIndex.set1Value(vidx_fill++, polygon->indexlist[k]);
                faceset_fill->coordIndex.set1Value(vidx_fill++, SO_END_FACE_INDEX);
                faceset_fill->materialIndex.set1Value(midx_fill++, polygon->type);
            }

            if ((polygon->type == ARRoom::floor) && VisibleText)
            {
                double x, y, z;
                room->getPolygonCenterAbsolute(polygon, x, y, z);

                SoMaterial *mat = new SoMaterial();
                room_sep->addChild(mat);
                mat->diffuseColor.setValue(1, 1, 1);

                SoFont *f = new SoFont();
                room_sep->addChild(f); 
                f->name.setValue("Helvetica");
                f->size.setValue(0.25);

                room_sep->addChild(createText(x, y, z, room->getRoomId()));
            }

            if ((polygon->type == ARRoom::portal) && VisibleText)
            {
                double x, y, z;
                room->getPolygonCenterAbsolute(polygon, x, y, z);

                SoMaterial *mat = new SoMaterial();
                room_sep->addChild(mat);
                mat->diffuseColor.setValue(1, 1, 1);

                SoFont *f = new SoFont();
                room_sep->addChild(f); 
                f->name.setValue("Helvetica");
                f->size.setValue(0.10f);

                room_sep->addChild(createText(x, y, z+0.5, polygon->name.c_str()));
            }

        }
/*
        SoDrawStyle *ds3 = new SoDrawStyle();
        room_sep->addChild(ds3); 
        ds2->style.setValue(SoDrawStyle::FILLED);
        ds2->lineWidth.setValue(2);
*/
        // ----- portals -----

        if ((VisiblePolygons[ARRoom::portal] & (POLY_LINE | POLY_FILL)) != 0)
        {
            int cidx_line = 0;
            int lidx_line = 0;
            int midx_line = 0;

            SoMaterial *mat = new SoMaterial();
            room_sep->addChild(mat);
            mat->diffuseColor.setValue(1,0,1);

            SoCoordinate3 *portal_coords = new SoCoordinate3();
            room_sep->addChild(portal_coords);

            SoIndexedLineSet *portal_lineset = new SoIndexedLineSet();
            room_sep->addChild(portal_lineset);

            for (j=0; j<room->getPortalNum(); j++)
            {

//                ARRoom::ARPortal *portal = room->getPortal(j);
//                cout << "  Portal: " << portal->room << " - " << portal->polygon << ", " << portal->polygonnb << "\n";
                ARRoom::ARPolygon *polygon = 0;
                ARRoom::ARPolygon *polygonnb = 0;
                ARRoom *roomnb = building->getPortalPolygons(room, room->getPortal(j), polygon, polygonnb);

                if ((polygon != 0) && (polygonnb != 0))
                {
                    double x0, y0, z0;
                    room->getPolygonCenterAbsolute(polygon, x0, y0, z0);
                    double x1, y1, z1;
                    roomnb->getPolygonCenterAbsolute(polygonnb, x1, y1, z1);

                    portal_lineset->coordIndex.set1Value(lidx_line++, cidx_line);
                    portal_coords->point.set1Value(cidx_line++, x0, y0, z0);
                    portal_lineset->coordIndex.set1Value(lidx_line++, cidx_line);
                    portal_coords->point.set1Value(cidx_line++, x1, y1, z1);
                    portal_lineset->coordIndex.set1Value(lidx_line++, SO_END_FACE_INDEX);
                    portal_lineset->materialIndex.set1Value(midx_line++, 0);

                    double xd = x1 - x0;
                    double yd = y1 - y0;
                    double zd = z1 - z0;

                    SoSeparator *portal_sep = new SoSeparator();
                    room_sep->addChild(portal_sep);

                    SoTransform *trans4 = new SoTransform();
                    portal_sep->addChild(trans4);
                    trans4->translation.setValue(x1, y1, z1);

                    SoTransform *trans3 = new SoTransform();
                    portal_sep->addChild(trans3);
                    SbRotation rot (SbVec3f(0.0f, 1.0f, 0.0f), SbVec3f((float)xd, (float)yd, (float)zd));
                    trans3->rotation = rot;

                    SoTransform *trans2 = new SoTransform();
                    portal_sep->addChild(trans2);
                    trans2->translation.setValue(0, 0.25, 0);

                    SoCone *cone = new SoCone();
                    portal_sep->addChild(cone);
                    cone->bottomRadius.setValue(0.25);
                    cone->height.setValue(0.5);
                }
            }
        }

        // ----- marker -----

        if (VisibleMarker)
        {
            SoFont *f = new SoFont();
            room_sep->addChild(f); 
            f->name.setValue("Helvetica");
            f->size.setValue(0.2f);

            for (j=0; j<room->getMarkerNum(); j++)
            {
                ARRoom::ARMarker *marker = room->getMarker(j);

                SoSeparator *marker_sep = new SoSeparator();
                room_sep->addChild(marker_sep);

                SoMaterial *mat = new SoMaterial();
                marker_sep->addChild(mat);
                mat->diffuseColor.setValue(1,0,0);

                SoTransform *trans1 = new SoTransform();
                marker_sep->addChild(trans1);
                trans1->translation.setValue(ox+marker->cenx, oy+marker->ceny, oz+marker->cenz);

                SoTransform *trans3 = new SoTransform();
                marker_sep->addChild(trans3);
                SbRotation rot (SbVec3f(0.0f, 1.0f, 0.0f), SbVec3f((float)marker->dirx, (float)marker->diry, (float)marker->dirz));
                trans3->rotation = rot;

                SoTransform *trans4 = new SoTransform();
                marker_sep->addChild(trans4);
                trans4->translation.setValue(0, marker->size * 0.5 * 2, 0);

                SoCone *cone = new SoCone();
                marker_sep->addChild(cone);
                cone->bottomRadius.setValue(marker->size * 0.5 * 2);
                cone->height.setValue(marker->size *2);

                SoTransform *trans2 = new SoTransform();
                marker_sep->addChild(trans2);
                trans2->translation.setValue(0, marker->size, 0);

                char str[20];
                sprintf(str, "%i", marker->number);
                // marker_sep->addChild(createText(0, 0, 0, str));
            }
        }

        // ----- vertices -----

        if (VisibleVertices && ((strcmp(RoomVertices, ALL_ROOMS) == 0) || (strcmp(RoomVertices, room->getRoomId()) == 0)))
        {
            float size = 0.1f;

            SoFont *f = new SoFont();
            room_sep->addChild(f); 
            f->name.setValue("Helvetica");
            f->size.setValue(size);

            SoSphere *sph = new SoSphere();
            sph->radius.setValue(size * 0.5);

            float rnd = (rand() % 100) * 0.01f;
            SoMaterial *sph_mat = new SoMaterial();
            sph_mat->diffuseColor.setValue(rnd,1-rnd,1);

            SoTransform *text_trans = new SoTransform();
            text_trans->translation.setValue(0, -size * 0.5, size * 0.5);

            SoMaterial *text_mat = new SoMaterial();
            text_mat->diffuseColor.setValue(1,1,0);

            for (j=0; j<room->getVertexNum(); j++)
            {
                ARRoom::ARVertex *vertex = room->getVertex(j);

                SoSeparator *vertex_sep = new SoSeparator();
                room_sep->addChild(vertex_sep);

                SoTransform *trans2 = new SoTransform();
                vertex_sep->addChild(trans2);
                trans2->translation.setValue(ox+vertex->x, oy+vertex->y, oz+vertex->z);

                SoTransform *trans3 = new SoTransform();
                vertex_sep->addChild(trans3);
                trans3->scaleFactor.setValue(VertexScaleFactor, VertexScaleFactor, VertexScaleFactor);

                vertex_sep->addChild(sph_mat);
                vertex_sep->addChild(sph);
                vertex_sep->addChild(text_trans);
                vertex_sep->addChild(text_mat);

                SoAsciiText *text = new SoAsciiText();
                vertex_sep->addChild(text);
                char str[20];
                sprintf(str, "%i", j);
                text->string.setValue(str);
                text->justification.setValue(SoAsciiText::CENTER);
            }

        }

    }

    return sep;
}




// overwrite examiner viewer class to allow direct keyboard interaction
// in camera mode
class BaumlExaminerViewer : public SoWinExaminerViewer
{
   public:
      BaumlExaminerViewer(HWND parent) : SoWinExaminerViewer(parent)
      {
         building_sep = NULL;
         help_switch = NULL;
      }
      
      SbBool processSoEvent(const SoEvent* const event)
      {
         const SoEvent* soEvent = event;
         SbBool evHandled = TRUE;
         
//         if ((soEvent != 0) && (SO_KEY_PRESS_EVENT(soEvent, SoKeyboardEvent::ANY))) {
         if (soEvent && (SO_KEY_PRESS_EVENT(soEvent, ANY))) {
            // keyboard key multiplexer
            switch (((SoKeyboardEvent *)soEvent)->getKey()) {
               case SoKeyboardEvent::W: VisiblePolygons[ARRoom::wall]++; break;
               case SoKeyboardEvent::F: VisiblePolygons[ARRoom::floor]++; break;
               case SoKeyboardEvent::C: VisiblePolygons[ARRoom::ceiling]++; break;
               case SoKeyboardEvent::P: VisiblePolygons[ARRoom::portal]++; break;
               case SoKeyboardEvent::S: VisiblePolygons[ARRoom::special]++; break;
               case SoKeyboardEvent::T: VisibleText = !VisibleText; break;
               case SoKeyboardEvent::H: help_switch->whichChild.setValue(SO_SWITCH_NONE - help_switch->whichChild.getValue()); break;
               case SoKeyboardEvent::M: VisibleMarker = !VisibleMarker; break;
               case SoKeyboardEvent::V: VisibleVertices = !VisibleVertices; break;
               case SoKeyboardEvent::I: VertexScaleFactor += 0.1f; break;
               case SoKeyboardEvent::O: VertexScaleFactor -= 0.1f; break;
               case SoKeyboardEvent::R: delete bauml; bauml = new BAUML(); 
                                        if (filetype == 'b') bauml->read_bauml(filename);
                                        if (filetype == 'o') bauml->read_ocar(filename); break;
               case SoKeyboardEvent::B: 
               {
                  ARBuilding *building = bauml->getBuilding();
                  if (building != 0)
                  {
                     string name = RoomVertices;
                     if (name == ALL_ROOMS)
                     {
                        cout << "enter room name: ";
                        cin >> name;
                     }
                     ARRoom *room = building->getRoom(name.c_str());
                     if (room != 0)
                     {
                        int i;
                        cout << "enter vertex number: ";
                        cin >> i;
                        ARRoom::ARVertex *vertex = room->getVertex(i-1);
                        if (vertex != 0)
                        {
                           cout << vertex->x << " " << vertex->y << " " << vertex->z << "\n";
                        }
                     }
                  }
               }
               break;
               
               case SoKeyboardEvent::D:
               {
                  string name;
                  cout << "enter room name: ";
                  cin >> name;
                  ARBuilding *building = bauml->getBuilding();
                  if (building != 0)
                  {
                     RoomVertices = ALL_ROOMS;
                     ARRoom *room = building->getRoom(name.c_str());
                     if (room != 0) RoomVertices = room->getRoomId();
                     if (RoomVertices != 0) cout << "Vertices of room " << RoomVertices << " are displayed\n";
                  }
               }
               break;
               
               case SoKeyboardEvent::ESCAPE:
               case SoKeyboardEvent::Q:
               case SoKeyboardEvent::X:
               {
                  SoWin::exitMainLoop();
                  break;
               }

               default:
                  evHandled = FALSE;
                  break;
            }           


            building_sep->removeAllChildren();
            building_sep->addChild(createBuilding(bauml));
         }
         else {
            evHandled = FALSE;
         }

         // return FALSE if handled by us
         return evHandled ? FALSE : SoWinExaminerViewer::processSoEvent(soEvent);
      }

      void setBuildingSeparator(SoSeparator *sep)
      {
         building_sep = sep;
      }

      void setHelpSwitch(SoSwitch *sw)
      {
         help_switch = sw;
      }

   private:
      SoSeparator *building_sep;
      SoSwitch *help_switch;
};


// ------------------------------

int main(int argc, char **argv)
{
    VisiblePolygons[ARRoom::wall]    = POLY_LINE | POLY_FILL;
    VisiblePolygons[ARRoom::floor]   = POLY_LINE | POLY_FILL;
    VisiblePolygons[ARRoom::ceiling] = POLY_LINE | POLY_FILL;
    VisiblePolygons[ARRoom::portal]  = POLY_LINE | POLY_FILL;
    VisiblePolygons[ARRoom::special] = POLY_LINE | POLY_FILL;
    VisibleText = true;
    VisibleMarker = true;
    VisibleVertices = false;

    if (argc != 3) 
    {
        cout << "Usage: input-type baumlviewer filename\n";
        printf("\n");
        printf("input-type = 'o' for ocar, 'b' for bauml\n");
        return 1;
    }

    stringstream helptext;

    helptext << "Press key for switching 'off' --> 'lines' --> 'filled'--> 'lines+filled' \n";
    helptext << "   W ... wall\n";
    helptext << "   F ... floor\n";
    helptext << "   C ... ceiling\n";
    helptext << "   P ... portal\n";
    helptext << "   S ... special\n";
    helptext << "Press key for switching 'off' --> 'on'\n";
    helptext << "   H ... HELP TEXT\n";
    helptext << "   T ... text\n";
    helptext << "   M ... marker\n";
    helptext << "   V ... vertices\n";
    helptext << "Press key for action:\n";
    helptext << "   R ... reload BAUML file\n";
    helptext << "   I ... increase vertex size\n";
    helptext << "   O ... decrease vertex size\n";
    helptext << "   B ... display vertex coord\n";
    helptext << "   D ... display vertices of single room\n";

    cout << "Loading " << argv[2] << "\n";
    strncpy(filename, argv[2], 511);
    filetype = argv[1][0];
    bauml = new BAUML();
    if (filetype == 'b') bauml->read_bauml(filename);
    if (filetype == 'o') bauml->read_ocar(filename);
    // setup Qt and SoQT
    // This initialises inventor and qt
    HWND window = SoWin::init("Fullscreen");
    if (window == NULL) exit(1);
    //window->setMinimumSize(700,300);

    // the common root node
    SoSeparator *root = new SoSeparator;
    root->ref();    
         
    SoPerspectiveCamera *myCamera = new SoPerspectiveCamera;
    myCamera->nearDistance = -25;
    myCamera->farDistance = 100;
    root->addChild(myCamera);

    SoLightModel *lm = new SoLightModel();
    lm->model = SoLightModel::BASE_COLOR;
    root->addChild(lm); 

    // Examiner Viewer
    BaumlExaminerViewer *myViewer = new BaumlExaminerViewer(window);
//  SoQtPlaneViewer *myViewer = new SoQtPlaneViewer(window);

    // setting up objects
    SoSeparator *building_sep = new SoSeparator();
    root->addChild(building_sep);
    myViewer->setBuildingSeparator(building_sep);
    building_sep->addChild(createBuilding(bauml));

    myViewer->setSceneGraph(root);
    myCamera->viewAll(root, myViewer->getViewportRegion());
    myViewer->setTitle("BAUML Viewer");
    myViewer->setHeadlight(FALSE);
    myViewer->setTransparencyType(SoGLRenderAction::SORTED_OBJECT_BLEND);
    myViewer->setBackgroundColor( SbColor(0,0,0));
    myViewer->show();

    // setting up help text
    SoSwitch *help_switch = new SoSwitch();
    root->addChild(help_switch);
    myViewer->setHelpSwitch(help_switch);

    SoSeparator *help_sep = new SoSeparator();
    help_switch->addChild(help_sep);
    help_switch->whichChild = 0;

    SoOrthographicCamera *oc = new SoOrthographicCamera;
    help_sep->addChild(oc);

    SoMaterial *m = new SoMaterial;
    help_sep->addChild(m);
    m->ambientColor.setValue(1,1,1);
    m->diffuseColor.setValue(1,1,1);
    m->transparency.setValue(0.3f);

    SoTransform *t = new SoTransform;
    help_sep->addChild(t);
    t->translation.setValue(-1.3f, 0.90f, 0.0f);

    SoFont *f = new SoFont;
    help_sep->addChild(f);
    f->size = 0.05f;

    SoAsciiText *at = new SoAsciiText;
    help_sep->addChild(at);
    at->spacing = 1.3f;
    
    int i=0;
    while (!helptext.eof())
    {
        char buf[1024];
        helptext.getline(buf, 1023);
        at->string.set1Value(i, buf);
        i++;
    }

    // show and set to full screen
    SoWin::show(window);

    // comment this out for debugging
    //window->showFullScreen();
    //window->showMaximized();

    // Start mainLoop
    SoWin::mainLoop();
    root->unref();

    delete bauml;

    return 0;
}
