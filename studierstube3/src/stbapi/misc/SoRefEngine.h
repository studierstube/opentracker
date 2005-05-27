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
  * ======================================================================== */

#ifndef _SOREFENGINE_H_
#define _SOREFENGINE_H_

#include <Inventor/engines/SoSubEngine.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/fields/SoSFVec3f.h>

#include <stbapi/stbapi.h>

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

/** 
@ingroup util

This engine exposes the value of  a field its output is connected to. You can use it
to get the value of a field that is changing internally, but that you cannot DEF / USE
for some reason. Unfortunately, this doesn't work with engineoutputs, and is therefore of limited use.

File format/defaults:
@verbatim
SoRefEngine {
    type (type) [mandatory]
    (type) in NULL
}
@endverbatim

@todo Should we get rid of this class?
*/
class STBAPI_API SoRefEngine : public SoEngine {

   SO_ENGINE_HEADER(SoRefEngine);


 public:

    /// value input
	SoField * in;
	/// input from our connected field. This is connected automatically.
	SoField * fieldIn;

    /// value output
	SoEngineOutput * out;
	/// Output to our connected field. Connect this to set up the engine.
	SoEngineOutput * fieldOut;

  private:
      /// We have to store these per instance, because of changing type.
    SoFieldData * instanceFieldData;
    SoEngineOutputData * instanceEngineOutputData;


  public:

   // Initializes this class for use in scene graphs. This
   // should be called after database initialization and before
   // any instance of this node is constructed.
   static void initClass();

   // Constructor
   SoRefEngine(SoType inputType);

 protected:

    // i don't know if we need this.. this is present in e.g. SoGate in OSOIV, but not in COIN
   // virtual SoFieldContainer * copyThroughConnection() const;

     virtual void copyContents(const SoFieldContainer * from, SbBool copyconnections);

 
 private:

   /// Private constructor. Only used when reading from file.
   SoRefEngine();
   /// Destructor. Private to keep people from trying to delete nodes, rather than using the reference count mechanism.
   virtual ~SoRefEngine();

   virtual void inputChanged(SoField * whichField);
   virtual void evaluate();


    // these engines must write out their input type before writing
    // their fields, so these are overridden:
    virtual SbBool	readInstance(SoInput *in, unsigned short flags);
    virtual void	writeInstance(SoOutput *out);

    void setup(SoType inputType);

    void setOutput(SoField * input, SoEngineOutput * output);

    SbBool theFieldChanged;
    int conversionCase;




};

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif // _SOREFENGINE_H_
