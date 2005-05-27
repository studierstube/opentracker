// ===========================================================================
//  (C) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:       intmodule
//  TYPE:       c++ code
//  PROJECT:    Studierstube
//  CONTENT:    Integer-Slider Input Object 
//  VERSION:    2.0
// ===========================================================================
//  AUTHORS:    peci      Clemens Pecinovsky
// ===========================================================================
//  HISTORY:
//
//  1-jun-99 08:00:00  peci      created
// ===========================================================================
#ifndef _INTMODULE_H_
#define _INTMODULE_H_

#include "pipmodule.h"

#include <stbapi/widgets/So3DIntSlider.h>
#include <stbapi/widgets/So3DIntDial.h>

class IntModule : public PipModule
{
public:
  //Constructor
  //needs the Pointer to its Client, its Id, and its Parameter string
  IntModule(ClientModule *client,int moduleId,char *para);

  //Destructor
  virtual ~IntModule();

  //does the initialization of the OIV Objects within the rendering loop.
  virtual void initOIVObjects() {}

  //gets the Module Type, in this case it returns STB_MODULE_INT
  virtual int getType();

  //sets the properties to the values of the parameterstring.
  virtual void setUpdate(char *data);

  //is this Module a PipModule? in this case it returns always TRUE.
  virtual int isPipModule();

  //virtual method used for generating the Pip-widgets....
  virtual void updatePip(int user,SoSeparator *sep,float &left);

  //send an update parameterstring direct to the stbWin Module...
  void update();

private:
  int min,max,val;    //current Value and its min and max value
  char visType;       //visualization Type, 
                      //'S' means Slider, 'D' means Dial


  So3DIntSlider *mySlider;  //Slider Widget
  So3DIntDial   *myDial;    //Dial Widget

  SoSeparator   *helpShowSlider;  //Separator for Help-Text-Geometry for Slider
  SoSeparator   *helpShowDial;    //Separator for Help-Text-Geometry for Dial

  //Callbacks for Slider and Dail
  //they call the update() Method....
  static void mySliderCB(void *userData, So3DIntSlider *slider);
  static void myDialCB(void *userData, So3DIntDial *dial);

}; //class IntModule



#endif//_INTMODULE_H_
