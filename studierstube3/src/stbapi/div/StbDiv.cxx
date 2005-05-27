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
/** main header file for div
  *
  * @author  Gerd Hesina, Jan Prikryl
  *
  * $Id: StbDiv.cxx 3504 2004-07-20 11:34:20Z tomp $
  * @file                                                                   */
 /* ======================================================================= */

#include <cassert>

#include <stbapi/div/StbDiv.h>
#include <stbapi/widgets/So3DCheckBox.h>
#include <stbapi/widgets/So3DSlider.h>
#include <stbapi/widgets/SoBase3DButton.h>
#include <stbapi/widgets/SoBase3DDial.h>
#include <stbapi/widgets/So3DRadioButton.h>
#include <stbapi/interaction/SoDragSeparator.h>
#include <stbapi/interaction/SoDragKit.h>
#include <stbapi/util/buffer.h>

StbDiv::StbDiv(const char* address, unsigned int port, Mode theMode, bool initFromRemote, const char *nic ) :
	CDivMain( address, port, theMode, initFromRemote, nic )	
{  
}

StbDiv::~StbDiv()
{
  //nil
}

SbBool StbDiv::processMessage(StbBuffer *inBuffer, short divCommand)
{
    // first do the standard processing, if that fails we try special stb commands.
    if( CDivMain::processMessage( inBuffer, divCommand ) == TRUE )
        return TRUE;
    switch(divCommand)
    {        
		case DIV_SET_CHECKBOX_STATE:
		{
			char *nodeString=inBuffer->getString();
            So3DCheckBox *theCheckBox=(So3DCheckBox *)SoNode::getByName(nodeString);
            if( NULL == theCheckBox )
            {
                printf("StbDiv checkbox %s not present !\n", nodeString );
                break;
            }
		    SbBool pressedState=inBuffer->getInt32();
			int highlightState=inBuffer->getInt32();
			int insideState=inBuffer->getInt32();
            theCheckBox->updateFromNetwork(pressedState, highlightState, insideState);
			break;
		}
		case DIV_SET_SLIDER_STATE:
		{
			char *nodeString=inBuffer->getString();
            SoBase3DSlider *theSlider=(SoBase3DSlider *)SoNode::getByName(nodeString);
            if( NULL == theSlider )
            {
                printf("StbDiv slider %s not present !\n", nodeString );
                break;
            }
            float currentValue=inBuffer->getFloat();
            theSlider->updateFromNetwork(currentValue);
			break;
		}
		case DIV_SET_DIAL_STATE:
		{
			char *nodeString=inBuffer->getString();
            SoBase3DDial *theDial=(SoBase3DDial *)SoNode::getByName(nodeString);
            if( NULL == theDial )
            {
                printf("StbDiv dial %s not present !\n", nodeString );
                break;
            }
            float currentValue=inBuffer->getFloat();
            theDial->updateFromNetwork(currentValue);
			break;
		}
		case DIV_SET_BUTTON_STATE:
		{
			char *nodeString=inBuffer->getString();
            SoBase3DButton *theButton=(SoBase3DButton *)SoNode::getByName(nodeString);
            if( NULL == theButton )
            {
                printf("StbDiv button %s not present !\n", nodeString );
                break;
            }
            int pressedState=inBuffer->getInt32();
            theButton->updateFromNetwork(pressedState);
			break;
		}
		case DIV_SET_RADIOBUTTON_STATE:
		{
			char *nodeString=inBuffer->getString();
            So3DRadioButton *theRadioButton=(So3DRadioButton *)SoNode::getByName(nodeString);
            if( NULL == theRadioButton )
            {
                printf("StbDiv radiobutton %s not present !\n", nodeString );
                break;
            }
            int pressedState=inBuffer->getInt32();
            theRadioButton->updateFromNetwork(pressedState);
			break;
		}
		case DIV_SET_DRAGKIT_STATE:
		{
			char *nodeString=inBuffer->getString();
            SoDragKit *theDragKit=(SoDragKit *)SoNode::getByName(nodeString);
            if( NULL == theDragKit )
            {
                printf("StbDiv dragkit %s not present !\n", nodeString );
                break;
            }
            float f1=inBuffer->getFloat();
            float f2=inBuffer->getFloat();
            float f3=inBuffer->getFloat();
            SbVec3f trans(f1,f2,f3);
            f1=inBuffer->getFloat();
            f2=inBuffer->getFloat();
            f3=inBuffer->getFloat();
            float f4=inBuffer->getFloat();
            theDragKit->updateFromNetwork(trans, SbRotation(f1,f2,f3,f4));
			break;
		}
		case DIV_SET_DRAGSEPARATOR_STATE:
		{
			char *nodeString=inBuffer->getString();
            SoDragSeparator *theDragSeparator=(SoDragSeparator *)SoNode::getByName(nodeString);
            if( NULL == theDragSeparator )
            {
                printf("StbDiv dragseparator %s not present !\n", nodeString );
                break;
            }
            float f1=inBuffer->getFloat();
            float f2=inBuffer->getFloat();
            float f3=inBuffer->getFloat();
            SbVec3f trans(f1,f2,f3);
            f1=inBuffer->getFloat();
            f2=inBuffer->getFloat();
            f3=inBuffer->getFloat();
            float f4=inBuffer->getFloat();
            theDragSeparator->updateFromNetwork(trans, SbRotation(f1,f2,f3,f4));
			break;
		}
		default:
			return FALSE;
    }
    return TRUE;
}

void StbDiv::setCheckBoxState(So3DCheckBox *theCheckBox)
{
   SbName changedNodeName=theCheckBox->getName();
   assert((!changedNodeName)!=TRUE);
   CBuffer outBuffer;
   outBuffer.addShort(DIV_SET_CHECKBOX_STATE);
   outBuffer.addBytes(changedNodeName.getLength()+1, 
                      (void*)changedNodeName.getString());
   outBuffer.addInt32(theCheckBox->isPressed());
   outBuffer.addInt32(theCheckBox->isHighlighted());
   outBuffer.addInt32(theCheckBox->getInsideState());
   sendData((const char*)outBuffer.getBuffer(), outBuffer.getSize());    
}

void StbDiv::setSliderState(SoBase3DSlider *theSlider, float currentValue)
{
   SbName changedNodeName=theSlider->getName();
   assert((!changedNodeName)!=TRUE);
   CBuffer outBuffer;
   outBuffer.addShort(DIV_SET_SLIDER_STATE);
   outBuffer.addBytes(changedNodeName.getLength()+1, 
                      (void*)changedNodeName.getString());
   outBuffer.addFloat(currentValue);
   sendData((const char*)outBuffer.getBuffer(), outBuffer.getSize());    
}

void StbDiv::setDialState(SoBase3DDial *theDial, float currentValue)
{
   SbName changedNodeName=theDial->getName();
   assert((!changedNodeName)!=TRUE);
   CBuffer outBuffer;
   outBuffer.addShort(DIV_SET_DIAL_STATE);
   outBuffer.addBytes(changedNodeName.getLength()+1, 
                      (void*)changedNodeName.getString());
   outBuffer.addFloat(currentValue);
   sendData((const char*)outBuffer.getBuffer(), outBuffer.getSize());    
}

void StbDiv::setButtonState(SoBase3DButton *theButton)
{
   SbName changedNodeName=theButton->getName();
   assert((!changedNodeName)!=TRUE);
   CBuffer outBuffer;
   outBuffer.addShort(DIV_SET_BUTTON_STATE);
   outBuffer.addBytes(changedNodeName.getLength()+1, 
                      (void*)changedNodeName.getString());
   outBuffer.addInt32(theButton->isPressed());
   sendData((const char*)outBuffer.getBuffer(), outBuffer.getSize());    
}

void StbDiv::setRadioButtonState(So3DRadioButton *theRadioButton)
{
   SbName changedNodeName=theRadioButton->getName();
   assert((!changedNodeName)!=TRUE);
   CBuffer outBuffer;
   outBuffer.addShort(DIV_SET_RADIOBUTTON_STATE);
   outBuffer.addBytes(changedNodeName.getLength()+1, 
                      (void*)changedNodeName.getString());
   outBuffer.addInt32(theRadioButton->isPressed());
   sendData((const char*)outBuffer.getBuffer(), outBuffer.getSize());    
}

void StbDiv::setDragKitState(SoDragKit *theDragKit, SbVec3f trans, SbRotation rot)
{
   SbName changedNodeName=theDragKit->getName();
   assert((!changedNodeName)!=TRUE);
   CBuffer outBuffer;
   outBuffer.addShort(DIV_SET_DRAGKIT_STATE);
   outBuffer.addBytes(changedNodeName.getLength()+1, 
                      (void*)changedNodeName.getString());
   float f1, f2, f3, f4;
   trans.getValue(f1,f2,f3);
   outBuffer.addFloat(f1);
   outBuffer.addFloat(f2);
   outBuffer.addFloat(f3);
   rot.getValue(f1,f2,f3,f4);
   outBuffer.addFloat(f1);
   outBuffer.addFloat(f2);
   outBuffer.addFloat(f3);
   outBuffer.addFloat(f4);
   sendData((const char*)outBuffer.getBuffer(), outBuffer.getSize());    
}

void StbDiv::setDragSeparatorState(SoDragSeparator *theDragSeparator, SbVec3f trans, SbRotation rot)
{
   SbName changedNodeName=theDragSeparator->getName();
   assert((!changedNodeName)!=TRUE);
   CBuffer outBuffer;
   outBuffer.addShort(DIV_SET_DRAGSEPARATOR_STATE);
   outBuffer.addBytes(changedNodeName.getLength()+1, 
                      (void*)changedNodeName.getString());
   float f1, f2, f3, f4;
   trans.getValue(f1,f2,f3);
   outBuffer.addFloat(f1);
   outBuffer.addFloat(f2);
   outBuffer.addFloat(f3);
   rot.getValue(f1,f2,f3,f4);
   outBuffer.addFloat(f1);
   outBuffer.addFloat(f2);
   outBuffer.addFloat(f3);
   outBuffer.addFloat(f4);
   sendData((const char*)outBuffer.getBuffer(), outBuffer.getSize());    
}

/* ===========================================================================
   End of stbdiv.cxx
   ===========================================================================
   Automatic Emacs configuration follows.
   Local Variables:
   mode:c++
   c-basic-offset: 4
   eval: (c-set-offset 'substatement-open 0)
   eval: (c-set-offset 'case-label '+)
   eval: (c-set-offset 'statement 'c-lineup-runin-statements)
   eval: (setq indent-tabs-mode nil)
   End:
=========================================================================== */
