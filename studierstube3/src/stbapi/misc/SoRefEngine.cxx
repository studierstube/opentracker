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
  * ======================================================================== 
  * PROJECT: Studierstube
  * ======================================================================== */ 

#ifdef WIN32
#include <SoWinEnterScope.h>
#include <windows.h>
#include <SoWinLeaveScope.h>
#endif

#ifdef __COIN__
#include <Inventor/engines/SoSubEngineP.h>
#endif

#include <Inventor/fields/SoFields.h>
#include <Inventor/errors/SoReadError.h>

#include "SoRefEngine.h"

//cannot use this because of special handling of inputs/outputs...
//SO_ENGINE_SOURCE(SoRefEngine);
#ifdef __COIN__
unsigned int SoRefEngine::classinstances = 0;
SoFieldData * SoRefEngine::inputdata = NULL;
const SoFieldData ** SoRefEngine::parentinputdata = NULL;
SoEngineOutputData * SoRefEngine::outputdata = NULL;
const SoEngineOutputData ** SoRefEngine::parentoutputdata = NULL;

SoType SoRefEngine::classTypeId;

#else 
SO__ENGINE_VARS(SoRefEngine);
#endif

// Constants for all of the multi-value fields; gate's input can be
// any of the SoMF types, and its output will be the corresponding
// SoSF type.

enum TypeConst {
    SFBitMask,
    MFBitMask,
    SFBool,
    MFBool,
    SFColor,
    MFColor,
    SFEnum,
    MFEnum,
    SFFloat,
    MFFloat,
    SFInt32,
    MFInt32,
    SFMatrix,
    MFMatrix,
    SFName,
    MFName,
    SFNode,
    MFNode,
    SFPath,
    MFPath,
    SFPlane,
    MFPlane,
    SFRotation,
    MFRotation,
    SFShort,
    MFShort,
    SFString,
    MFString,
    SFTime,
    MFTime,
    SFUInt32,
    MFUInt32,
    SFUShort,
    MFUShort,
    SFVec2f,
    MFVec2f,
    SFVec3f,
    MFVec3f,
    SFVec4f,
    MFVec4f,

    BAD_TYPE
};    

void
SoRefEngine::initClass()
{
    SO_ENGINE_INIT_CLASS(SoRefEngine, SoEngine, "Engine");
}

SoRefEngine::SoRefEngine(SoType inputType)
{
    SO_ENGINE_CONSTRUCTOR(SoRefEngine);

    in = NULL;
    fieldIn = NULL;
    out = NULL;
    fieldOut = NULL;

    setup(inputType);

}

/// Only used when reading from file
SoRefEngine::SoRefEngine()
{
    SO_ENGINE_CONSTRUCTOR(SoRefEngine);

    in = NULL;
    fieldIn = NULL;
    out = NULL;
    fieldOut = NULL;
}

SoRefEngine::~SoRefEngine()
{
    delete instanceFieldData;
    delete instanceEngineOutputData;

    if (in != NULL) {
	    delete in;
	    delete fieldIn;
	    delete out;
	    delete fieldOut;
    }
}

void SoRefEngine::setup(SoType inputType)
{
    //set up our local FieldData and EngineOutputData instances
#ifdef __COIN__
    instanceFieldData = new SoFieldData(inputdata);
    instanceEngineOutputData = new SoEngineOutputData(outputdata);
#else //OSOIV
    instanceFieldData = new SoFieldData(inputData);
    instanceEngineOutputData = new SoEngineOutputData(outputData);
#endif

    // Instead of SO_ENGINE_ADD_INPUT().
    this->in = (SoField *)inputType.createInstance();
    //this->in->setNum(0);
    this->in->setContainer(this);
    this->instanceFieldData->addField(this, "in", this->in);

    // Instead of SO_ENGINE_ADD_INPUT().
    this->fieldIn = (SoField *)inputType.createInstance();
    //this->fieldIn->setNum(0);
    this->fieldIn->setContainer(this);
    this->instanceFieldData->addField(this, "fieldIn", this->fieldIn);

    // Instead of SO_ENGINE_ADD_OUTPUT().
    this->out = new SoEngineOutput;
    this->instanceEngineOutputData->addOutput(this, "out", this->out, inputType);
    this->out->setContainer(this);

    // Instead of SO_ENGINE_ADD_OUTPUT().
    this->fieldOut = new SoEngineOutput;
    this->instanceEngineOutputData->addOutput(this, "fieldOut", this->fieldOut, inputType);
    this->fieldOut->setContainer(this);


// This handy macro sets up conversionCase, which is used to quickly
// decide what type we're hooked up to at evaluate() time:

#define DECIDE(_class_) \
	(inputType == So##_class_::getClassTypeId()) { conversionCase = _class_; }			     

	// Set up for which switch to use in evaluate() routine:
	if DECIDE(SFBitMask) 
	else if DECIDE(MFBitMask) 
	else if DECIDE(SFBool)    
	else if DECIDE(MFBool)    
	else if DECIDE(SFColor)   
	else if DECIDE(MFColor)   
	else if DECIDE(SFEnum)    
	else if DECIDE(MFEnum)    
	else if DECIDE(SFFloat)   
	else if DECIDE(MFFloat)   
	else if DECIDE(SFInt32)   
	else if DECIDE(MFInt32)   
	else if DECIDE(SFMatrix)  
	else if DECIDE(MFMatrix)  
	else if DECIDE(SFName)    
	else if DECIDE(MFName)    
	else if DECIDE(SFNode)    
	else if DECIDE(MFNode)    
	else if DECIDE(SFPath)    
	else if DECIDE(MFPath)    
	else if DECIDE(SFPlane)   
	else if DECIDE(MFPlane)   
	else if DECIDE(SFRotation)
	else if DECIDE(MFRotation)
	else if DECIDE(SFShort)   
	else if DECIDE(MFShort)   
	else if DECIDE(SFString)  
	else if DECIDE(MFString)  
	else if DECIDE(SFTime)    
	else if DECIDE(MFTime)    
	else if DECIDE(SFUInt32)  
	else if DECIDE(MFUInt32)  
	else if DECIDE(SFUShort)  
	else if DECIDE(MFUShort)  
	else if DECIDE(SFVec2f)   
	else if DECIDE(MFVec2f)   
	else if DECIDE(SFVec3f)   
	else if DECIDE(MFVec3f)   
    else if DECIDE(SFVec4f)   
    else if DECIDE(MFVec4f)   
#undef DECIDE
	else {
#ifdef _DEBUG
	    SoDebugError::post("SoGate::setup",
			"Can't gate field of type %s",
			inputType.getName().getString());
#endif
	    conversionCase = BAD_TYPE;
	}

    theFieldChanged = false;

}  
  
void SoRefEngine::inputChanged(SoField * whichField)
{
#ifdef _DEBUG
    printf("SoRefEngine::inputChanged() called! \n");
#endif

    if (whichField == fieldIn) theFieldChanged = true;
    else theFieldChanged = false;
}

void SoRefEngine::evaluate()
{
#ifdef _DEBUG
    printf("SoRefEngine::evaluate() called! \n");
#endif

    SoFieldList outlist;
    SoField * inconn = NULL;
    fieldOut->getForwardConnections(outlist);
    fieldIn->getConnectedField(inconn);

    // we have a new connection to fieldOut -> connect the connected field to fieldIn
    if (outlist.getLength() > 0 && outlist[0] != inconn){
        fieldIn->connectFrom(outlist[0]);
        theFieldChanged = true;
    }

    if (theFieldChanged) {
#ifdef _DEBUG
    printf("SoRefEngine::field input changed, setting out... \n");
#endif
        setOutput(fieldIn, out);
    }
    else {
#ifdef _DEBUG
    printf("SoRefEngine::value input changed, setting out + fieldOut... \n");
#endif
        setOutput(in, out);
        setOutput(in, fieldOut);
    }

    theFieldChanged = false;
}

void SoRefEngine::setOutput(SoField * input, SoEngineOutput * output) {

    if (!output->isEnabled()) return;

    for (int i = 0; i < output->getNumConnections(); i++) {
	    SoField *outField = (SoField *)(*output)[i];
	    if (outField->isReadOnly()) continue;
	    
    // Handy macro for doing type-correct setValues(getValues())
#define DO_MF(_class_) \
((_class_ *)outField)-> setValues(0, ((_class_ *)input)->getNum(), ((_class_ *)input)->getValues(0)); \
((_class_ *)outField)->setNum(((_class_ *)input)->getNum()); \
break
#define DO_SF(_class_) \
((_class_ *)outField)-> setValue(((_class_ *)input)->getValue()); \
break
				     
	    switch(conversionCase) {
            case SFBitMask  : DO_SF(SoSFBitMask);
            case MFBitMask  : DO_MF(SoMFBitMask);
            case SFBool     : DO_SF(SoSFBool);
            case MFBool     : DO_MF(SoMFBool);
            case SFColor    : DO_SF(SoSFColor);
            case MFColor    : DO_MF(SoMFColor);
            case SFEnum     : DO_SF(SoSFEnum);
            case MFEnum     : DO_MF(SoMFEnum);
            case SFFloat    : DO_SF(SoSFFloat);
            case MFFloat    : DO_MF(SoMFFloat);
            case SFInt32    : DO_SF(SoSFInt32);
            case MFInt32    : DO_MF(SoMFInt32);
            case SFMatrix   : DO_SF(SoSFMatrix);
            case MFMatrix   : DO_MF(SoMFMatrix);
            case SFName     : DO_SF(SoSFName);
            case MFName     : DO_MF(SoMFName);
            case SFNode     : DO_SF(SoSFNode);
            case MFNode     : DO_MF(SoMFNode);
            case SFPath     : DO_SF(SoSFPath);
            case MFPath     : DO_MF(SoMFPath);
            case SFPlane    : DO_SF(SoSFPlane);
            case MFPlane    : DO_MF(SoMFPlane);
            case SFRotation : DO_SF(SoSFRotation);
            case MFRotation : DO_MF(SoMFRotation);
            case SFShort    : DO_SF(SoSFShort);
            case MFShort    : DO_MF(SoMFShort);
            case SFString   : DO_SF(SoSFString);
            case MFString   : DO_MF(SoMFString);
            case SFTime     : DO_SF(SoSFTime);
            case MFTime     : DO_MF(SoMFTime);
            case SFUInt32   : DO_SF(SoSFUInt32);
            case MFUInt32   : DO_MF(SoMFUInt32);
            case SFUShort   : DO_SF(SoSFUShort);
            case MFUShort   : DO_MF(SoMFUShort);
            case SFVec2f    : DO_SF(SoSFVec2f);
            case MFVec2f    : DO_MF(SoMFVec2f);
            case SFVec3f    : DO_SF(SoSFVec3f);
            case MFVec3f    : DO_MF(SoMFVec3f);
            case SFVec4f    : DO_SF(SoSFVec4f);
            case MFVec4f    : DO_MF(SoMFVec4f);
#undef DO_MF
#undef DO_SF
	      case BAD_TYPE:
	        ; // Do nothing, already complained
	        break;
	      default:
	        // Something is seriously wrong:
    #ifdef _DEBUG
	        SoDebugError::post("SoRefEngine::evaluate",
			       "conversionCase is %d!", conversionCase);
    #endif
	        break;
	    }
	    
    }
}


SbBool
SoRefEngine::readInstance(SoInput * in, unsigned short flags)
{
    SbName typeName;
    if (!in->read(typeName) || typeName != "type") {
        SoReadError::post(in,
                          "\"type\" keyword is missing, erroneous format for "
                          "engine class '%s'.",
                          this->getTypeId().getName().getString());
        return FALSE;
    }

    SbName typeVal;
    if (!in->read(typeVal)) {
        SoReadError::post(in, "Couldn't read input type for engine.");
        return FALSE;
    }

    SoType inputType = SoType::fromName(typeVal);

    this->setup(inputType);

    return SoEngine::readInstance(in, flags);
}


void
SoRefEngine::writeInstance(SoOutput * out)
{
  if (this->writeHeader(out, FALSE, TRUE)) return;

  SbBool binarywrite = out->isBinary();

  if (!binarywrite) out->indent();
  out->write("type");
  if (!binarywrite) out->write(' ');
  out->write(this->in->getTypeId().getName());
  if (binarywrite) out->write((unsigned int)0);
  else out->write('\n');

  this->getFieldData()->write(out, this);
  this->writeFooter(out);
}

void
SoRefEngine::copyContents(const SoFieldContainer * from, SbBool copyconnections)
{
  SoRefEngine * src = (SoRefEngine *)from;
  if (src->in) {
      this->setup(src->in->getTypeId());
  }
  SoEngine::copyContents(from, copyconnections);
}

const SoFieldData *
SoRefEngine::getFieldData() const 
{
    return instanceFieldData;
}


const SoEngineOutputData *
SoRefEngine::getOutputData() const
{
    return instanceEngineOutputData;
}

void *
SoRefEngine::createInstance()
{
    return new SoRefEngine;
}

#ifdef __COIN__
SoType SoRefEngine::getClassTypeId(void) { return SoRefEngine::classTypeId; }
#endif
SoType SoRefEngine::getTypeId(void) const { return SoRefEngine::classTypeId; } 


