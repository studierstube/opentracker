// ===========================================================================
//  (C) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:       voAppearance.h
//  TYPE:       c++ header
//  PROJECT:    Studierstube MediDesk
//  CONTENT:    Demo application for volume rendering
//  VERSION:    1.3
// ===========================================================================
//  AUTHORS:    ww      Werner Wohlfahrter
// ===========================================================================
//  HISTORY:    2000 09 26  ww :) 
// ===========================================================================

#ifndef _voAppearance_h_
#define _voAppearance_h_

#include "voStatus.h"
#include "stbapi/misc/ivmacros.h"
#include <Inventor/nodes/SoNode.h>
#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/fields/SoSFString.h>
#include <Inventor/fields/SoSFEnum.h>
#include <Inventor/fields/SoSFInt32.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/nodes/SoEventCallback.h>
#include <vo/Volumizer.h>
#include <vo/Appearance.h>
#include <vo/AppearanceActions.h>
#include <iostream.h>
#include <GL/gl.h>
#include <Inventor/SbBox.h>
#include <Inventor/misc/SoState.h>
#include <Inventor/SoPickedPoint.h>
#include <Inventor/actions/SoAction.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoAnnotation.h>
#include <Inventor/nodes/SoText3.h>
#include <Inventor/SbString.h>
#include <Inventor/nodes/SoScale.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <vo/LookupTable.h>


class voAppearance : public SoNode {

   SO_NODE_HEADER(voAppearance);

public:

   // Initializes this class
   static void                 initClass(voStatus  *s);

   // Constructor
   voAppearance();

   voBrickSetCollection        *aVolume;
   virtual void                readVolume(void);
   virtual void                readVolumeRAW(void);
   char **                     createRawFileNames(
                                  char * fileName, 
                                  int sliceCount);

   //-----------------------------------------------------------------------------
   // set methods
   // volume data settings
   void                        setUseLutStatus();
   void                        setUseMipStatus();
   void                        setUseOverStatus();
   void                        setVoScale(float value);
   void                        setVolumeFormatField(char * format);
   void                        setDataFormatField(char * format);
   void                        setVolumeSizeXField(int x);
   void                        setVolumeSizeYField(int y);
   void                        setVolumeSizeZField(int z);
   void                        setHeaderLengthField(int hl);
   void                        setSuffixLengthField(int sl);
   bool                        useLut();
   void                        lut(voIndexedTetraSet *,  
                                   voBrickSetCollection * aVolume, 
                                   voRenderingMode renderingMode, 
                                   int lutLength,
                                   int lutcenter,
                                   int lutwidth);
   inline voBrickSetCollection *getVolume(void);
   inline const char*          getVolumeFormatField(void);
   inline const char*          getDataFormatField(void);
   inline int                  getVolumeSizeXField(void);
   inline int                  getVolumeSizeYField(void);
   inline int                  getVolumeSizeZField(void);
   inline int                  getHeaderLengthField(void);
   inline int                  getSuffixLengthField(void);
   SoSFInt32                   lutCenter;    
   SoSFInt32                   lutWidth;    
   SoSFFloat                   voScale;

protected:
   virtual void                GLRender(SoGLRenderAction *action);
   virtual void                callback(SoCallbackAction *action);
   virtual void                doAction(SoAction *action);

private:
   // Fields
   SoSFString                  fileNameField;
   SoSFString                  volumeFormatField;
   SoSFString                  dataFormatField;
   SoSFInt32                   volumeSizeXField;
   SoSFInt32                   volumeSizeYField;
   SoSFInt32                   volumeSizeZField;
   SoSFInt32                   headerLengthField;
   SoSFInt32                   suffixLengthField;
   SoSFEnum                    interpolationTypeField;
   SoSFEnum                    renderingModeField;

   SoSFBool                    useLutStatus;    
   SoSFBool                    useMipStatus;    
   SoSFBool                    useOverStatus;    

   // Destructor
   virtual                     ~voAppearance();

};

inline voBrickSetCollection *
voAppearance::getVolume(void) {
  return aVolume;
}

inline const char*
voAppearance::getVolumeFormatField(void) {
   return volumeFormatField.getValue().getString();
}

inline const char*
voAppearance::getDataFormatField(void) {
   return dataFormatField.getValue().getString();
}

inline int
voAppearance::getVolumeSizeXField(void) {
  return volumeSizeXField.getValue();
}

inline int
voAppearance::getVolumeSizeYField(void) {
  return volumeSizeYField.getValue();
}

inline int
voAppearance::getVolumeSizeZField(void) {
  return volumeSizeZField.getValue();
}

inline int
voAppearance::getHeaderLengthField(void) {
  return headerLengthField.getValue();
}

inline int
voAppearance::getSuffixLengthField(void) {
  return suffixLengthField.getValue();
}


#endif // _voAppearance_h_
