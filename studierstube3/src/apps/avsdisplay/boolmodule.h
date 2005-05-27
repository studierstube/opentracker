// ===========================================================================
//  (C) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:       boolmodule
//  TYPE:       c++ code
//  PROJECT:    Studierstube
//  CONTENT:    Logical True-False Input Object 
//  VERSION:    2.0
// ===========================================================================
//  AUTHORS:    peci      Clemens Pecinovsky
// ===========================================================================
//  HISTORY:
//
//  11-jul-99 17:00:00  peci      created
// ===========================================================================
#ifndef _BOOLMODULE_H_
#define _BOOLMODULE_H_

#include "pipmodule.h"

#include <stbapi/widgets/So3DIntSlider.h>
#include <stbapi/widgets/So3DIntDial.h>
#include <stbapi/widgets/So3DCheckBox.h>

class BoolModule : public PipModule
{
public:
  //Constructor
  //needs the Pointer to its Client, its Id, and its Parameter string
  BoolModule(ClientModule *client,int moduleId,char *para);

  //Destructor
  virtual ~BoolModule();

  //does the initialization of the OIV Objects within the rendering loop.
  virtual void initOIVObjects() {}

  //gets the Module Type, in this case it returns STB_MODULE_BOOL
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
  int val;	//current Bool Value of the Module
  char visType; //visualization Type, 
                //'S' means Slider, 'D' means Dial, 'B' means Button....

  So3DIntSlider *mySlider;        //Slider Widget
  So3DIntDial *myDial;            //Dial Widger
  So3DCheckBox *myCheckBox;       //Button Widget

  SoSeparator   *helpShowSlider;  //Separator for Help-Text-Geometry for Slider
  SoSeparator   *helpShowDial;    //Separator for Help-Text-Geometry for Dial
  SoSeparator   *helpShowCheckBox;//Separator for Help-Text-Geometry for Button

  //Callbacks for Slider,Dail and Button. 
  //they call the update() Method....
  static void mySliderCB(void *userData, So3DIntSlider *slider);
  static void myDialCB(void *userData, So3DIntDial *dial);
  static void myCheckBoxCB(void *userData, So3DCheckBox *checkBox);

}; //class BoolModule



#endif//_BoolModule_H_
