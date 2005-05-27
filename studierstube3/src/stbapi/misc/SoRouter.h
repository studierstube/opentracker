/* ========================================================================
 * Copyright (C) 2000-2003  Vienna University of Technology
 *
 * This framework is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This framework is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this framework; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * For further information please contact Dieter Schmalstieg under
 * <schmalstieg@ims.tuwien.ac.at> or write to Dieter Schmalstieg,
 * Vienna University of Technology, Favoritenstr. 9-11/188, A1040 Vienna,
 * Austria.
 * ========================================================================
 * PROJECT: Studierstube
 * ======================================================================== */
/**
 *
 * @author Thomas Psik, Flo Ledermann
 *
 * $Id: SoRouter.h 3716 2004-09-16 12:49:44Z tomp $
 * @file                                                                   */
/* ======================================================================= */


#ifndef _SOROUTER_H_
#define _SOROUTER_H_

#include <Inventor/nodes/SoSubNode.h>

#include <Inventor/fields/SoSFNode.h>
#include <Inventor/fields/SoSFEngine.h>
#include <Inventor/fields/SoSFString.h>
#include <Inventor/fields/SoSFInt32.h>  
#include <Inventor/fields/SoSFBool.h>

#include <Inventor/sensors/SoFieldSensor.h>

#include <Inventor/engines/SoEngine.h>

#include <stbapi/stbapi.h>
#include <stbapi/misc/SoSFFieldContainer.h>
#include <stbapi/misc/SoRoute.h>

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

/**
@ingroup util

This node allows the explicit construction of routes between two fields (or an engineoutput and a field, to be
exact) in the scenegraph. Source and target fields can either be specified by a string in the format "<defname>.<fieldname>",
or by referencing the source or target node or engine with an SFNode/SFEngine field and specifying the fieldname as a string.
The route can be activated and deactivated by setting the route field.

Note that if this node is removed from the scenegraph, the route will always be removed.

If either the from or to field is changed a new route will be established, the old route will be removed. Specifing non existing fields or 
engine outputs will only be an error at read in time. At runtime only warnings will be displayed in the console.

If you need no control over the route you should use @see SoRoute.

File Format / defaults:

@verbatim

SoRouter {
   SoSFNode		sourceNode		NULL	# A reference to a node containing the source output
   SoSFEngine	sourceEngine	NULL	# A reference to an engine containing the source output
   SoSFString	sourceField		""		# The name of the source field
   SoSFString	sourceRef		""      # The full reference to the source field in "<defname>.<fieldname>" syntax 
   SoSFNode		destNode		NULL	# A reference to a node containing the destination field.
   SoSFEngine	destEngine		NULL	# A reference to an engine containing the destination field.
   SoSFString	destField		""		# The name of the destination field. 
   SoSFString	destRef			""		# The full reference to the destination field in "<defname>.<fieldname>" syntax
   SoSFBool		route			FALSE	# Toggles the connection on/off.
   SoSFBool		setValue		FALSE	# if TRUE and the connection is established, the toField will be set (else will only be updated if fromField is set !!)
   SoSFBool		evaluateAtStart	TRUE    # if TRUE the source/dest fields will be checked at startup, else the existents will only be check if route is set to TRUE
  
# from SoRoute
   SoSFString	from	""      # The full reference to the source field in "<defname>.<fieldname>" syntax 
   SoSFString	to		""		# The full reference to the destination field in "<defname>.<fieldname>" syntax 
}
  
@endverbatim

Examples

  DEF CUBE   SoCube		{}
  DEF CYLINDER   SoCylinder {}

  SoRouter {
	  sourceNode	USE CUBE
	  sourceField	"height"
	  destNode		USE CYLINDER
	  destField		"height"
	  route			TRUE
  }

  SoRouter {
	  sourceRef		"CUBE.height"
	  destRef		"CYLINDER.height"
  }

  SoRouter {
	  from	"CUBE.height" to "CYLINDER.height"
	 route TRUE
	}

@ see SoRoute
*/
#define SUPPORT_OLD_SOROUTER
//#undef SUPPORT_OLD_SOROUTER

// -------------------------------
// -------------------------------
#ifdef SUPPORT_OLD_SOROUTER
// -------------------------------
// -------------------------------

class STBAPI_API SoRouter : public SoRoute {

   SO_NODE_HEADER(SoRouter);

 public:
   // Fields:
   SoSFFieldContainer sourceFieldContainer; ///< a reference to a FieldContainer (engine or node) containing the "from" field.
   SoSFFieldContainer destFieldContainer;   ///< a reference to a FieldContainer (engine or node) containing the "to" field.
  	
   /// A reference to a node containing the source field.
   SoSFNode       sourceNode;       
   /// A reference to an engine containing the source output.
   SoSFEngine     sourceEngine;
   /// The name of the source field. 
   SoSFString     sourceField;
   
   /** The full reference to the source field in "<defname>.<fieldname>" syntax. Use this as an alternative to REFing the
   source into the sourceNode/sourceEngine field @todo depreciated should switch to 'from' */
   SoSFString     sourceRef;

   /// A reference to a node containing the destination field.
   SoSFNode       destNode;       
   /// A reference to an engine containing the destination field.
   SoSFEngine     destEngine;
   /// The name of the destination field. 
   SoSFString     destField;
   
   /** The full reference to the destination field in "<defname>.<fieldname>" syntax. Use this as an alternative to REFing the
   source node into the destNode/destEngine field @todo depreciated should switch to 'to' */
   SoSFString     destRef;

   /// Toggles the connection on/off.
   SoSFBool		  route;     

   /// if connection is established the value of the toField will be set 
   SoSFBool	  setValue;

   /// flag wether to check source/dest at startup or only at connection 
   SoSFBool	  evaluateAtStart;

    /// if connection is established the value of the toField will be set 
   SoSFInt32   pri;

   // Initializes this class for use in scene graphs. This
   // should be called after database initialization and before
   // any instance of this node is constructed.
   static void    initClass();

   // Constructor
   SoRouter();

   static void routeChangedCB(void* data, SoSensor* which);

   static void destRefChangedCB (void* data, SoSensor* which);
   static void sourceRefChangedCB (void* data, SoSensor* which);
   static void toDeletedCB(void* data, SoSensor* which);
 protected:

   	SoFieldSensor routeSensor;
   	SoFieldSensor sourceSensor;
   	SoFieldSensor sourceRefSensor;
   	SoFieldSensor destSensor;
    SoFieldSensor destRefSensor;
	
   	SoFieldSensor fromFieldNameSensor;
   	SoFieldSensor toFieldNameSensor;

	SoFieldSensor fromSensor;
	SoFieldSensor toSensor;

	SoFieldSensor toDeletedSensor;

	void updateRoute();
	SbBool updateAllFromPtr(SoInput *in = NULL);
	SbBool updateAllToPtr(SoInput *in = NULL);

    /// Reads stuff into instance. Returns FALSE on error, a reimplementation of SoRoute::readInstance
    virtual SbBool readInstance(SoInput *in, unsigned short flags);
   	
 private:
   // Destructor. Private to keep people from trying to delete
   // nodes, rather than using the reference count mechanism.
   // Makes newer GCC complaining about destructor not being
   // avaliable as public function.
   virtual ~SoRouter();

};

// -------------------------------
// -------------------------------
#else 
// -------------------------------
// NEUE IMPLEMENTIERUNG nur from und to bleiben

class STBAPI_API SoRouter : public SoRoute {

   SO_NODE_HEADER(SoRouter);

 public:

   /// Toggles the connection on/off.
   SoSFBool		  route;     
	
   // Initializes this class for use in scene graphs. This
   // should be called after database initialization and before
   // any instance of this node is constructed.
   static void    initClass();

   // Constructor
   SoRouter();

   static void routeChangedCB(void* data, SoSensor* which);
   static void toDeletedCB(void* data, SoSensor* which);

 protected:

   	SoFieldSensor routeSensor;
	SoFieldSensor fromSensor;
	SoFieldSensor toSensor;
	SoFieldSensor toDeletedSensor;

	void updateRoute();

    /// Reads stuff into instance. Returns FALSE on error, a reimplementation of SoRoute::readInstance
    virtual SbBool readInstance(SoInput *in, unsigned short flags);
   	
 private:
   // Destructor. Private to keep people from trying to delete
   // nodes, rather than using the reference count mechanism.
   // Makes newer GCC complaining about destructor not being
   // avaliable as public function.
   virtual ~SoRouter();
};

#endif 


#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif // _SOROUTER_H_
