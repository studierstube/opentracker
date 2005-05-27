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
/** This file contains the SoAlphabetKit.
  * See the class description for details.
  *
  * @author Benjamin A. Schmit
  *
  * $Id: SoAlphabetKit.cxx 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#include "SoAlphabetKit.h"

#include <Inventor/SoInput.h>
#include <Inventor/SbPList.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/nodes/SoText3.h>
#include <Inventor/nodes/SoTexture2.h>
#include <Inventor/nodes/SoTransform.h>
#include <stbapi/misc/GRT_defs.h>

#include <math.h>

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

/** The texture graphics of the SoAlphabetKit.
  * It should separate the different input areas:
  */
static const char* _grt_alphatex= "graphix/SoAlphabetKit.jpg";


/** The files used for the SoAlphabetKit gesture recognition.
  * The order is: numbers, characters, extended, special:
  */
static const char* _grt_alphagst[4]=
    {
        "graphix/numbers.gst",
        "graphix/characters.gst",
        "graphix/special.gst",
        "graphix/extended.gst"
    };

SO_KIT_SOURCE(SoAlphabetKit);


/// Adds a string to the history.

void SoAlphabetKit::memorize(SbString string)
{
    // search for current text:
    bool found= false;

    // empty string will be discarded (i.e. always found):
    if (string.getLength() == 0)
        found= true;

    // not found -> search in history:
    if (!found)
        for (int i= 0; i < history.getNum(); i++)
            if (history[i] == string)
	        found= true;

    // not found -> continue in completion:
    if (!found)
        for (int i= 0; i < completion.getNum(); i++)
            if (completion[i] == string)
	        found= true;

    // still not found -> add to history:
    if (!found)
        history.set1Value(history.getNum(), string);
}


/// Invokes string completion.

void SoAlphabetKit::complete()
{
    // initialize some variables:
    int hist= 0;
    if (historyEnabled.getValue()) hist= history.getNum();
    int comp= completion.getNum();
    int max= comp + hist;
    SbString txt= text.getValue().getSubString(0, completionChar);;
    if (completionChar == -1) completionChar= txt.getLength();

    // loop through the possibilities
    SbString str;
    int j;
    for (int i= 0; i < max; i++)
    {
        // calculate the "real" i:
        j= i + completionCount + 1;
        if (j >= max) j-= max;

        // get the string:
        if (j < hist) str= history[j];
            // history is used before completion:
        else str= completion[j - hist];

        // judge if string is valid completion:
        if ((completionChar == 0) || (str.getSubString(0, completionChar - 1)
            == txt.getSubString(0, completionChar - 1)))
        {
            // yes, set text and rescue state:
            text.setValue(str);
            completionCount= j;
            i= max;  // leave loop
        }
    }
}


/// Macros used for initialization.

void SoAlphabetKit::constSoAlphabetKit()
{
    // create this SoAlphabetKit:
    SO_KIT_CONSTRUCTOR(SoAlphabetKit);

    // add the catalog parts:
    SO_KIT_ADD_CATALOG_ENTRY(surfaceTexture, SoTexture2, FALSE,
        surfaceSeparator, surface, FALSE);
    SO_KIT_ADD_CATALOG_ENTRY(textSwitch, SoSwitch, FALSE,
        topSeparator, \x0, FALSE);
    SO_KIT_ADD_CATALOG_ENTRY(textMaterial, SoMaterial, FALSE,
        textSwitch, \x0, FALSE);
    SO_KIT_ADD_CATALOG_ENTRY(textTransform, SoTransform, FALSE,
        textSwitch, \x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY(text3, SoText3, FALSE,
        textSwitch, \x0, FALSE);

    // add enum values:
    SO_KIT_DEFINE_ENUM_VALUE(Visible, HIDDEN);
    SO_KIT_DEFINE_ENUM_VALUE(Visible, VISIBLE);
    SO_KIT_SET_SF_ENUM_TYPE(textVisible, Visible);

    // add the nodekit's fields:
    SO_NODE_ADD_FIELD(text, (""));
    SO_NODE_ADD_FIELD(textColor, (_grt_text));
    SO_NODE_ADD_FIELD(textVisible, (VISIBLE));
    SO_NODE_ADD_FIELD(completion, (""));
    SO_NODE_ADD_FIELD(history, (""));
    SO_NODE_ADD_FIELD(historyEnabled, (TRUE));

    // initialize this SoAlphabetKit:
    SO_KIT_INIT_INSTANCE();
}


/// This initializes the SoAlphabetKit.

void SoAlphabetKit::initSoAlphabetKit(
    float myXSize, float myYSize, float myHeight)
{
    // surfaceTexture:
    {
        SoTexture2* tx= (SoTexture2*) getAnyPart("surfaceTexture", true);
        tx->filename.setValue(_grt_alphatex);
    }

    // textSwitch:
    {
        SoSwitch* sw= (SoSwitch*) getAnyPart("textSwitch", true);
        textEng.in.connectFrom(&textVisible);
        sw->whichChild.connectFrom(&textEng.out);
    }

    // textMaterial:
    {
        SoMaterial* mat= (SoMaterial*) getAnyPart("textMaterial", true);
        mat->diffuseColor.connectFrom(&textColor);
    }

    // textTransform:
    {
        SoTransform* trn= (SoTransform*) getAnyPart("textTransform", true);
        trn->translation.setValue(0.0f, 131*height.getValue()/256, 0.0f);
        trn->rotation.setValue(SbVec3f(1.0f, 0.0f, 0.0f), -M_PI/2);
    }

    // text3:
    {
        SoText3* txt= (SoText3*) getAnyPart("text3", true);
        txt->justification= SoText3::CENTER;
        txt->string.connectFrom(&text);
        text.connectFrom(&(txt->string));
    }

    // fields:
    completion.deleteValues(0, -1);
    history.deleteValues(0, -1);

    // initialize internal data:
    alphabetCallback= NULL;
    alphabetData= NULL;
    mode= 0;
    completionCount= -1;
    completionChar= -1;
}


/// Extension mechanism for subclassing - string entry.

void SoAlphabetKit::alphabetImplementation()
{
    // intentionally empty - override to extend
}


/// Extension of the SoGestureKit.

void SoAlphabetKit::gestureImplementation(int gesture)
{
    // should we switch to special characters?
    // if we recognize a switch, skip everything else here...
    if ((mode < 2) && rcgDot)  // dot has another meaning in mode 2 and 3
    {
        // yes, switch!
        mode= 2;
        return;  // we do not want to identify this further
    }

    char maj= (char) (gesture/100);
    char min= (char) (gesture - 100*((int) maj));
    SbString str= text.getValue();
    char chr[2]= {' ', 0};
    switch (maj)
    {
    case 10:
        // numbers:
        chr[0]= min + '0';
        str+= chr;
        text.setValue(str);
        mode= 0;
        completionCount= -1;
        completionChar= -1;
        break;
    case 20:
        // characters:
        if (mode == 1) chr[0]= min + 'A';
        else chr[0]= min + 'a';
        str+= chr;
        text.setValue(str);
        mode= 0;
        completionCount= -1;
        completionChar= -1;
        break;
    case 30:
        // special characters:
        switch (min)
        {
        case 00:
            // space:
            chr[0]= ' ';
            str+= chr;
            text.setValue(str);
            mode= 0;
            completionCount= -1;
            completionChar= -1;
            break;
        case 01:
            // backspace:
            str.deleteSubString(str.getLength() - 1);
            text.setValue(str);
            mode= 0;
            completionCount= -1;
            completionChar= -1;
            break;
        case 02:
            // return:
            if (historyEnabled.getValue())
                memorize(str);
            if (alphabetCallback != NULL) alphabetCallback(alphabetData, this);
            alphabetImplementation();
            mode= 0;
            completionCount= -1;
            completionChar= -1;
            break;
        case 03:
            // shift:
            mode= 1 - mode;
            break;
        case 04:
            // extended follows:
            mode= 3;
            break;
        }
        break;
    case 40:
        // if (mode == 2)  // special characters
        {
          chr[0]= _grt_spc[(int)min];
            if (chr[0] == '\t') complete();
            else
            {
                str+= chr;
                text.setValue(str);
                completionCount= -1;
                completionChar= -1;
            }
            mode= 0;
        }
        break;
    case 50:
        // extended characters:
#ifdef DEF_EXT  // else: ignore extended characters
        chr[0]= _grt_ext[min];
        if (chr[0] != ' ') str+= chr;  // else: not defined here!
        text.setValue(str);
        completionCount= -1;
        completionChar= -1;
#endif
        mode= 0;
        break;
    }
}


/// Extension of the SoGestureKit.

void SoAlphabetKit::startAtCenter(bool center)
{
    // first, find out where the gesture files are.
    // we do this via the class SoInput:
    SbStringList dirs = SoInput::getDirectories();
    int lng= dirs.getLength();
    for (int i= 0; i < lng; i++)
    {
        SbString file= *(dirs[i]);
        file+= "/";
        if ((mode == 0) && center) file+= _grt_alphagst[1];
        else file+= _grt_alphagst[(int)mode];
        setGestures(file);
        if (getGestures() != "")
            i= lng;  // leave loop
            // OK, the gesture file is here:
    }
}


/// Static class initialization.

void SoAlphabetKit::initClass()
{
    SoGestureKit::initClass();
    SO_KIT_INIT_CLASS(SoAlphabetKit, SoGestureKit, "SoGestureKit");
}


/// Default Constructor.

SoAlphabetKit::SoAlphabetKit()
{
    constSoAlphabetKit();
    initSoAlphabetKit(xSize.getValue(), ySize.getValue(), height.getValue());
}


/// Parameterized Constructor.

SoAlphabetKit::SoAlphabetKit(float myXSize, float myYSize, float myHeight)
    : SoGestureKit(myXSize, myYSize, myHeight)
{
    constSoAlphabetKit();
    initSoAlphabetKit(myXSize, myYSize, myHeight);
}


/// Destructor.

SoAlphabetKit::~SoAlphabetKit() {}


/// Sets a callback on string entry.

void SoAlphabetKit::setAlphabetCallback(
    SoAlphabetKitCB* cb, void* userData= NULL)
{
    alphabetCallback= cb;
    alphabetData= userData;
}


/// Removes a callback on string entry.

void SoAlphabetKit::removeAlphabetCallback()
{
    alphabetCallback= NULL;
    alphabetData= NULL;
}


/// Returns the current state of the SoAlphabetKit.

char SoAlphabetKit::getMode() const
{
    return mode;
}
