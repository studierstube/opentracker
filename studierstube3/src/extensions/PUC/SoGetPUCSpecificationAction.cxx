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
/** Implementation of class SoGetPucSpecificationAction
  *
  * @author Thomas Psik
  *
  * $Id: SoGetPUCSpecificationAction.cxx 3823 2004-10-05 12:27:08Z tomp $
  * @file                                                                   */
 /* ======================================================================= */


#include <Inventor/nodes/SoGroup.h>
#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/misc/SoChildList.h>


#include "SoPucDevice.h"
#include "SoPucElementBase.h"
#include "SoPucStateBase.h"
#include "SoPucStateInt.h"
#include "SoPucStateFloat.h"
#include "SoPucStateFixed.h"
#include "SoPucStateEnumerated.h"
#include "SoPucStateBool.h"
#include "SoPucGroup.h"
#include "SoPucCommand.h"


#include "SoPucActiveIfNode.h"
#include "SoPucActiveIfClause.h"
#include "SoPucActiveIfOperator.h"

#include "SoGetPucSpecificationAction.h"

#include <iostream>

using namespace std;

#if defined(_DEBUG_PUC) && defined(_DEBUG)
	#define DEBUG_CODE(a) a;
#else
	#define DEBUG_CODE(a) ;
#endif

const int SoGetPucSpecificationAction::errorReturnCode = 1024;

SO_ACTION_SOURCE(SoGetPucSpecificationAction);

//
// initialize the class
// (must be called before any instance of the class is created)
//

void SoGetPucSpecificationAction::initClass()
{
    SO_ACTION_INIT_CLASS(SoGetPucSpecificationAction, SoAction);

    // enable elements

  //  SO_ENABLE(SoGetPucSpecificationAction, SoModelMatrixElement);
  //  SO_ENABLE(SoGetPucSpecificationAction, SoSwitchElement);

    // register static methods to be called for a specific node

	SO_ACTION_ADD_METHOD(SoNode,			callDoAction);
	SO_ACTION_ADD_METHOD(SoNodeKitListPart,	callDoAction);
	SO_ACTION_ADD_METHOD(SoGroup,			callDoAction);
	
	SO_ACTION_ADD_METHOD(SoPucDevice,		deviceSpecification);
	SO_ACTION_ADD_METHOD(SoPucElementBase,  stdElementBaseSpecification);
	SO_ACTION_ADD_METHOD(SoPucCommand,      commandSpecification);

	SO_ACTION_ADD_METHOD(SoPucStateBase,    stdStateBaseSpecification);

	SO_ACTION_ADD_METHOD(SoPucStateInt,			stateIntSpecification);
	SO_ACTION_ADD_METHOD(SoPucStateFloat,		stateFloatSpecification);
	SO_ACTION_ADD_METHOD(SoPucStateFixed,		stateFixedSpecification);
	SO_ACTION_ADD_METHOD(SoPucStateEnumerated,   stateEnumeratedSpecification);

	SO_ACTION_ADD_METHOD(SoPucActiveIfClause,	  activeIfClause);
	SO_ACTION_ADD_METHOD(SoPucActiveIfOperator,   activeIfOperator);
    
	  // init EventClass
}

//----------------------------------------------------------------------------

SoGetPucSpecificationAction::SoGetPucSpecificationAction()
{
    SO_ACTION_CONSTRUCTOR(SoGetPucSpecificationAction);
	pucSpec = "";
}

//----------------------------------------------------------------------------

SoGetPucSpecificationAction::~SoGetPucSpecificationAction()
{	
}

//----------------------------------------------------------------------------

void SoGetPucSpecificationAction::beginTraversal(SoNode *node)
{
	withInActiveIf = false;
	pucPath = "";
    traverse(node);		
}

//----------------------------------------------------------------------------

static const std::string endlString() { return "\n"; }
#define ENDL endlString()

void SoGetPucSpecificationAction::callDoAction(SoAction *action, SoNode *node)
{
    DEBUG_CODE(printf("SoGetPucSpecificationAction::callDoAction %x,%s\n",node,node->getTypeId().getName().getString()));
	node->doAction(action);
}

//----------------------------------------------------------------------------

void SoGetPucSpecificationAction::stdStateBaseSpecificationStart(SoAction *action, SoNode *node)
{
    DEBUG_CODE(printf("SoGetPucSpecificationAction::stdStateBaseSpecificationStart %x,%s\n",node,node->getTypeId().getName().getString()));
	SoPucStateBase *elm = (SoPucStateBase *)node;

	std::string ret;
	std::string fullName;
	fullName = ""+ ((SoGetPucSpecificationAction *)action)->pucPath+"."+elm->getStateName();
	elm->pucName = std::string(fullName.c_str());
	cout << "pucStateName set to ["<< elm->pucName << "] " << ENDL;
	// start tag <state ....>
	ret = "<" + elm->getElementName();

	// name is part of getBaseAttributesStr since version PUC/2.0
/*	ret += " name=\"";
	ret += elm->getStateName();
	ret += "\"";
*/
    ret += getBaseAttributesStr(elm);
	
	// write access 
	// access = "ReadOnly" | "WriteOnly"
	if (elm->access.getValue() != SoPucStateBase::ANY)
	{
		ret += " access=\"";
		if (elm->access.getValue() == SoPucStateBase::READ_ONLY)
			ret += "ReadOnly";
		else 
			ret += "WriteOnly";
		ret += "\"";
	}
	
	ret += ">"+ENDL;
	
	//
	ret += "<type>";
	// since PUC/2.0 "valueSpace" is not needed any more 
//	ret += "<valueSpace>" +ENDL;
	ret += "<" + elm->getType() +">"+ENDL;
	((SoGetPucSpecificationAction *)action)->pucSpec += ret.c_str();
}

//----------------------------------------------------------------------------
	
void SoGetPucSpecificationAction::stdStateBaseSpecificationEnd(SoAction *action, SoNode *node)
{
    DEBUG_CODE(printf("SoGetPucSpecificationAction::stdStateBaseSpecificationEnd %x,%s\n",node,node->getTypeId().getName().getString()));
	SoPucStateBase *elm = (SoPucStateBase *)node;

	std::string ret;
	ret += "</" + elm->getType() +">"+ENDL;
	// since PUC/2.0 "valueSpace" is not needed any more 
//	ret += "</valueSpace>";
	((SoGetPucSpecificationAction *)action)->pucSpec += ret.c_str();
	ret = "";
	// valueLabels
	stdStateBaseValueLabels(action, node);
	ret += "</type>"+ENDL;

	((SoGetPucSpecificationAction *)action)->pucSpec += ret.c_str();

	stdElementBaseInformation(action, node);
	
	((SoGetPucSpecificationAction *)action)->pucSpec += "</" + elm->getElementName()+ ">"+ENDL;	
}

//----------------------------------------------------------------------------

void SoGetPucSpecificationAction::stdStateBaseValueLabels(SoAction *action, SoNode *node)
{
	std::string ret;
	SoPucStateBase *elm = (SoPucStateBase *)node;
	SbString value;
	char buffer[40];

	if (elm->isValueLabelsSpecified())
	{
		ret += "<valueLabels>"+ENDL;
		if (elm->isOfType(SoPucStateBool::getClassTypeId()))
		{
			// special treatment of StateBool
			if (elm->valueLabels.getNum() > 2) 
			{
			    DEBUG_CODE(printf("SoGetPucSpecificationAction::stdStateBaseValueLabels more than 2 valueLabels for Bool State ! %x,%s [%s]\n",node,node->getTypeId().getName().getString(),node->getName());)
			}
			ret += "<map index=\"false\">";
			elm->valueLabels.get1(0, value);
			std::string t(value.getString());
			t[0] = ' ';
			t[t.length()-1] =  ' ';
			ret += "<label>" + t + "</label>";
			ret += "</map>"+ENDL;
			
			if (elm->valueLabels.getNum() > 1) 
			{
				ret += "<map index=\"true\">";
				elm->valueLabels.get1(1, value);
				std::string t(value.getString());
				t[0] = ' ';
				t[t.length()-1] =  ' ';
				ret += "<label>" + t + "</label>";
				ret += "</map>"+ENDL;
			}
		}
		else 
		{
			// for all other types	
			for (int i=0; i< elm->valueLabels.getNum(); i++)
			{
				itoa(i+1,buffer,10);
				ret += "<map index=\"";
				ret += buffer;
				ret += "\">";
				elm->valueLabels.get1(i, value);
				std::string t(value.getString());
				t[0] = ' ';
				t[t.length()-1] =  ' ';
				ret += "<label>" + t + "</label>";
				ret += "</map>"+ENDL;
			}
		}
		ret += "</valueLabels>"+ENDL;
	}

	((SoGetPucSpecificationAction *)action)->pucSpec += ret.c_str();	
}

//----------------------------------------------------------------------------

void SoGetPucSpecificationAction::stdStateBaseSpecification(SoAction *action, SoNode *node)
{
	stdStateBaseSpecificationStart(action, node);
	// nothing in between
	stdStateBaseSpecificationEnd(action, node);
}


//----------------------------------------------------------------------------
// < [ElementName] [baseAttributes >
// sub-nodes 
// <[ElementName]/>

void SoGetPucSpecificationAction::stdElementBaseSpecificationStart(SoGetPucSpecificationAction *action, SoPucElementBase *elm)
{
	std::string ret;
	
	ret += "<" + elm->getElementName();
	ret += getBaseAttributesStr(elm);

	action->pucSpec += ret.c_str();
}

//----------------------------------------------------------------------------

void SoGetPucSpecificationAction::stdElementBaseSpecificationEnd(SoGetPucSpecificationAction *action, SoPucElementBase *elm)
{	
	std::string ret;
	ret += ">"+ENDL;

	action->pucSpec += ret.c_str();

	stdElementBaseInformation(action, elm);
	
	action->pucSpec += "</" + elm->getElementName()+ ">"+ENDL;	
}

//----------------------------------------------------------------------------

void SoGetPucSpecificationAction::stdElementBaseSpecification(SoAction *action, SoNode *node)
{
    DEBUG_CODE(printf("SoGetPucSpecificationAction::stdElementBaseSpecification %x,%s\n",node,node->getTypeId().getName().getString()));
	SoPucElementBase *elm = (SoPucElementBase *)node;

	stdElementBaseSpecificationStart((SoGetPucSpecificationAction *)action, elm);
	// nothing in between
	stdElementBaseSpecificationEnd((SoGetPucSpecificationAction *)action, elm);
}

//----------------------------------------------------------------------------

void SoGetPucSpecificationAction::commandSpecification(SoAction *action, SoNode *node)
{
    DEBUG_CODE(printf("SoGetPucSpecificationAction::commandSpecification %x,%s\n",node,node->getTypeId().getName().getString()));
	SoPucCommand *elm = (SoPucCommand *)node;
	std::string ret;

	stdElementBaseSpecificationStart((SoGetPucSpecificationAction *)action, elm);
	// name is part of getBaseAttributesStr since version PUC/2.0
	/*
	// add attribute name
	ret += " name=\"";
	ret += elm->getCommandName();
	ret += "\"";
	*/

	((SoGetPucSpecificationAction *)action)->pucSpec += ret.c_str();

	stdElementBaseSpecificationEnd((SoGetPucSpecificationAction *)action, elm);
}

//----------------------------------------------------------------------------
// writes 
// <active-if />
// <labels/>
// sub-nodes 

void SoGetPucSpecificationAction::stdElementBaseInformation(SoAction *action_, SoNode *node)
{
    DEBUG_CODE(printf("SoGetPucSpecificationAction::stdElementBaseInformation %x,%s\n",node,node->getTypeId().getName().getString()));

	std::string ret;
	std::string storePucPath;
	SbString value;
	SoPucElementBase *elm = (SoPucElementBase *)node;
	SoGetPucSpecificationAction *action = (SoGetPucSpecificationAction *) action_;

	if (elm->isOfType(SoPucGroup::getClassTypeId())) {
		storePucPath = std::string(action->pucPath);
		action->pucPath += "." + elm->getStateName();
		cout << "pucPath = "<< action->pucPath << ENDL;
		elm->pucName = std::string(action->pucPath.c_str());
	}

	//SoPucActiveIfNode *activeIfNodePtr = SO_GET_PART(elm, "activeIf", SoPucActiveIfNode);
	SoPucActiveIfNode *activeIfNodePtr = (SoPucActiveIfNode *) elm->getPart("activeIf", FALSE);
	if (activeIfNodePtr != NULL) 
	{
		activeIfNode(action, activeIfNodePtr);
	}

	// check labels only if specified 
	bool writeLabels = true;
	if (elm->labels.getNum() >= 1) 
	{ 
		elm->labels.get1(0, value);
		if (value.getLength() == 0)
			writeLabels = false;
	} 
	else
		writeLabels = false;

	if (writeLabels)
	{
		ret = "<labels>"+ENDL;
//		if (elm != NULL)
//			printf(" SoGetPucSpecificationAction::stdElementBaseInformation\n  StateName [%s] elm [%p] elm->label [%p]\n", elm->getStateName() ,elm , &(elm->labels ));
		for (int i=0; i< elm->labels.getNum(); i++)
		{
			elm->labels.get1(i, value);
			ret += "<label>";
			// replace start and end " with ' ' "test" -> test
			std::string t(value.getString());
			t[0] = ' ';
			t[t.length()-1] =  ' ';
			ret += t.c_str();
			ret += "</label>" + ENDL;
		}
		ret += "</labels>"+ENDL;
		action->pucSpec += ret.c_str() ;
		ret = "";
	}

	// write sub nodes
	node->doAction(action);

	if (elm->isOfType(SoPucGroup::getClassTypeId())) {
		action->pucPath = storePucPath;
	}
}

//----------------------------------------------------------------------------

void SoGetPucSpecificationAction::deviceSpecification(SoAction *action, SoNode *node)
{
    DEBUG_CODE(printf("SoGetPucSpecificationAction::callGetDeviceSpecAction %x,%s\n",node,node->getTypeId().getName().getString()));
	std::string ret;
	SbString value;

	SoPucDevice * device = (SoPucDevice * )node;
 	ret  = "<spec name=";
	device->deviceName.get(value);
	ret += value.getString();
	((SoGetPucSpecificationAction *)action)->pucPath = device->deviceName.getValue().getString();

//    ret += " >"+ ENDL;

	// NEW 
	ret += " version=\"PUC/2.0\" >"+ ENDL;
	ret += "<labels>";
	ret += "<label>";
	// TODO labels of devices !!
	ret += value.getString();
	ret += "</label>";
	ret += "</labels>";
	// NEW 


	ret += "<groupings>" + ENDL;

	SoNodeKitListPart *lp = SO_GET_ANY_PART(device,"groups",SoNodeKitListPart);
	int num = lp->getNumChildren();
 	//cout<< "groups found [" <<num << "]" << endl;
	((SoGetPucSpecificationAction *)action)->pucSpec += ret.c_str();
		
	for (int i= 0; i< num; i++) 
	{
		SoPucGroup *grp = (SoPucGroup *)lp->getChild(i);
		action->traverse(grp);
	}
	
	ret = "</groupings>" + ENDL;
	ret += "</spec>" + ENDL;

	((SoGetPucSpecificationAction *)action)->pucSpec += ret.c_str() ;
}

//----------------------------------------------------------------------------

void SoGetPucSpecificationAction::stateIntSpecification(SoAction *action, SoNode *node)
{
    DEBUG_CODE(printf("SoGetPucSpecificationAction::stateIntSpecification %x,%s\n",node,node->getTypeId().getName().getString()));

	std::string ret;
	SbString value;

	SoPucStateInt * stateInt = (SoPucStateInt * )node;
 	
	stdStateBaseSpecificationStart(action, node);
	
	// add min, max
	int min = stateInt->min.getValue();
	int max = stateInt->max.getValue();
	if (min != max)
	{
		ret += "<min><number>";
		stateInt->min.get(value);
		ret += value.getString();
		ret += "</number></min>" + ENDL;
		ret += "<max><number>";
		char buffer[20];
		sprintf(buffer,"%d", max);
		ret +=  buffer; // bug fixed in version PUC/2.0
		ret += "</number></max>" + ENDL;
	}

	if (stateInt->incr.getValue() != 0)
	{
		// add incr
		ret += "<incr><number>";
		stateInt->incr.get(value);
		ret += value.getString();
		ret += "</number></incr>" + ENDL;
	}
	
	((SoGetPucSpecificationAction *)action)->pucSpec += ret.c_str() ;
	stdStateBaseSpecificationEnd(action, node);
}

//----------------------------------------------------------------------------

void SoGetPucSpecificationAction::stateFloatSpecification(SoAction *action, SoNode *node)
{
    DEBUG_CODE(printf("SoGetPucSpecificationAction::stateFloatSpecification %x,%s\n",node,node->getTypeId().getName().getString()));

	std::string ret;
	SbString value;
	char buffer[200];

	SoPucStateFloat * stateFloat = (SoPucStateFloat * )node;
 	
	stdStateBaseSpecificationStart(action, node);
	
	// add min, max
	float min = stateFloat->min.getValue();
	float max = stateFloat->max.getValue();
	if (min != max)
	{
		ret += "<min>";
		sprintf(buffer,"%.5f",min);
		ret += buffer;
		ret += "</min>" + ENDL;
		ret += "<max>";
		sprintf(buffer,"%.5f",max);
		ret += buffer;
		ret += "</max>" + ENDL;
	}
	else
	{
		specificationActionError("Error SoPucStateFloat::min == SoPucStateFloat::max !! ");
	}


	((SoGetPucSpecificationAction *)action)->pucSpec += ret.c_str() ;
	stdStateBaseSpecificationEnd(action, node);
}

//----------------------------------------------------------------------------

void SoGetPucSpecificationAction::stateFixedSpecification(SoAction *action, SoNode *node)
{
    DEBUG_CODE(printf("SoGetPucSpecificationAction::stateFixedSpecification %x,%s\n",node,node->getTypeId().getName().getString()));

	std::string ret;
	SbString value;
	char buffer[200];

	SoPucStateFixed * stateFixed = (SoPucStateFixed * )node;
 	
	stdStateBaseSpecificationStart(action, node);
	
	// add min, max
	float min = stateFixed->min.getValue();
	float max = stateFixed->max.getValue();
	if (min != max)
	{
		ret += "<min><number>";
		sprintf(buffer,"%.*f",stateFixed->pointpos.getValue(),min);
		ret += buffer;
		ret += "</number></min>" + ENDL;
		ret += "<max><number>";
		sprintf(buffer,"%.*f",stateFixed->pointpos.getValue(),max);
		ret += buffer;
		ret += "</number></max>" + ENDL;
	}
	else
	{
		specificationActionError("Error SoPucStateFixed::min == SoPucStateFixed::max !! ");
	}
	
	float incr = stateFixed->incr.getValue();
	
	if (incr > 0)
	{
		// add incr
		ret += "<incr><number>";
		sprintf(buffer,"%.*f",stateFixed->pointpos.getValue(),incr);
		ret += buffer;
		ret += "</number></incr>" + ENDL;
	}
	else
	{
		specificationActionError("Error SoPucStateFixed::incr is 0 !! ");
	}
	
	// add pointpos
	// no <number> in <pointpos> !!
	ret += "<pointpos>";
	stateFixed->pointpos.get(value);
	ret += value.getString();
	ret += "</pointpos>" + ENDL;
	

	((SoGetPucSpecificationAction *)action)->pucSpec += ret.c_str() ;
	stdStateBaseSpecificationEnd(action, node);
}

//----------------------------------------------------------------------------

void SoGetPucSpecificationAction::stateEnumeratedSpecification(SoAction *action, SoNode *node)
{
    DEBUG_CODE(printf("SoGetPucSpecificationAction::stateFloatSpecification %x,%s\n",node,node->getTypeId().getName().getString()));

	std::string ret;
	SbString value;

	SoPucStateEnumerated *stateEnum = (SoPucStateEnumerated *)node;
 	
	stdStateBaseSpecificationStart(action, node);

	// <items> 4 </items>
	// since PUC/2.0 its "itemCount" instead of "items"
	char buffer[40];
	ret += "<item-count>";
	itoa(stateEnum->valueLabels.getNum(),buffer,10);
	ret += buffer;
	ret += "</item-count>";
	
	((SoGetPucSpecificationAction *)action)->pucSpec += ret.c_str() ;
	stdStateBaseSpecificationEnd(action, node);
}

//----------------------------------------------------------------------------
// 	SoPucActiveIfNode { ignore ALL  activeIf SoPucActiveIfClause {...} }
//
// converted to PUC xml:
//
//   <equals state="SIZE">3</equals>
//

void SoGetPucSpecificationAction::activeIfNode(SoGetPucSpecificationAction *action, SoPucActiveIfNode *activeIf)
{
     DEBUG_CODE(printf("SoGetPucSpecificationAction::activeIfNode %x,%s\n",activeIf,activeIf->getTypeId().getName().getString()));
	
	std::string ret;

//	SoNode *activeIfSubNode = SO_GET_PART(activeIf, "activeIf", SoNode);
	SoNode *activeIfSubNode = activeIf->getPart( "activeIf", FALSE);
	if (activeIfSubNode == NULL)
	{
		//TODO: error should not happen !!!
	    DEBUG_CODE(printf("SoGetPucSpecificationAction::activeIfNode node is NULL !! %x,%s\n",activeIf,activeIf->getTypeId().getName().getString()));

		return; 
	}

	ret = "<active-if";
	if (activeIf->isIgnoredSet())
	{
		ret += " ignore=\"";
		ret += getPucActiveIfIgnoreStr(activeIf->ignore);
		ret += "\"";

	}
	
	ret += ">"+ENDL;
	action->pucSpec += ret.c_str() ;
	ret = "";

	// write active-if sub nodes !!
	action->withInActiveIf = true;
	activeIf->doAction(action);
	action->withInActiveIf = false;

	ret = "</active-if>"+ENDL;
	action->pucSpec += ret.c_str() ;
	ret = "";


}

//----------------------------------------------------------------------------
// 	SoPucActiveIfClause { state = USE SIZE.value op EQUALS value 3 }
//
// converted to PUC xml:
//
//   <equals state="GROUP.SIZE">3</equals>
//

void SoGetPucSpecificationAction::activeIfClause(SoAction *action_, SoNode *node)
{
	SoGetPucSpecificationAction *action = (SoGetPucSpecificationAction *)action_;

	//TODO find out why it doesnt work as expected 
	if (node->isOfType(SoPucActiveIfOperator::getClassTypeId()))
	{
		activeIfOperator(action_, node);
		return;
	}
    
	DEBUG_CODE(printf("SoGetPucSpecificationAction::activeIfClause %x,%s\n",node,node->getTypeId().getName().getString()));
	if (!action->withInActiveIf) 
	{
		DEBUG_CODE(printf("ERROR SoGetPucSpecificationAction::activeIfClause exit without writting %x ,%s, because not within a SoPucActiveIfNode !!\n",node,node->getTypeId().getName().getString()));
		return;
	}
		
	SoPucActiveIfClause *clause = (SoPucActiveIfClause *)node;
 
	// new style -> find connected SoPucStateBase
	// ret+= clause->state.getValue().getString();
	std::string stateName;
	
	bool isOk = false;
	SoField *connectedField = NULL;
	SoPucStateBase *state = NULL;
	if (clause->state.getConnectedField(connectedField )) 
	{
		SoFieldContainer *container = connectedField->getContainer();
		if (container != NULL)
		{
			if (container->getTypeId().isDerivedFrom(SoPucStateBase::getClassTypeId()))
			{
				state = (SoPucStateBase *)container;
				stateName = state->pucName;
				isOk = true;
			}
		}
	}
	if (!isOk)
	{
		DEBUG_CODE(printf("ERROR SoGetPucSpecificationAction::activeIfClause exit without writting %x ,%s, because state is not connected to a SoPucStateBase field !!\n",node,node->getTypeId().getName().getString()));
		return;	
	}

	std::string value = "";
	// check value that should be compared to -> SoPucStateEnumerated special case !!
	if (state->isOfType(SoPucStateEnumerated::getClassTypeId()))
	{
		// if it is an enumerated add +1 to the value (Puc defines Enumstates starting with 1 !!)
		int valueint;
		if (sscanf(clause->value.getValue().getString(),"%d",&valueint) != 1)
		{
			DEBUG_CODE(printf("ERROR SoGetPucSpecificationAction::activeIfClause exit without writting %x ,%s, because state is connected to a SoPucStateEnumerated but the value [%s] can not be coverted to an int !!\n",node,node->getTypeId().getName().getString(), clause->value.getValue().getString()););
			return;	
		}
		char buffer[100];
		sprintf(buffer,"%d", valueint+1);
		value += buffer;
	}
	else
		value += clause->value.getValue().getString();
	
	std::string ret;

	ret+= "<" + getPucActiveIfClauseOpStr(clause->op);
	ret+= " state=\"";
	ret+= stateName;
	ret+= "\">";

	// insert value that should be compared to
	ret += value;

	ret += "</" + getPucActiveIfClauseOpStr(clause->op)+ ">" + ENDL;
	
	action->pucSpec += ret.c_str() ;
}
/*
    activeIf SoPucActiveIfNode {
		activeIf SoPucActiveOperator {
		op AND
		clauses NodeKitListPart { containerNode Group {
 			SoPucActiveIfClause { state = USE SIZE.value op EQUALS value 3 }
			SoPucActiveIfClause { state = USE RADIUS.value op GREATER_THAN value 3}
			SoPucActiveIfClause { state = USE RADIUS.value op LESS_THAN value 5 }
		} }
	  }
	}
 }

 converted to PUC xml:

 <active-if>
   <and>
        <equals state="GROUP.SIZE">3</equals>
        <greaterthan state="GROUP.RADIUS">3</greaterthan>
        <lessthan  state="GROUP.RADIUS">5</lessthan>
   </and>
 </active-if>
*/

void SoGetPucSpecificationAction::activeIfOperator(SoAction *action_, SoNode *node)
{
	SoGetPucSpecificationAction *action = (SoGetPucSpecificationAction *)action_;

	if (node->isOfType(SoPucActiveIfClause::getClassTypeId()))
	{
		activeIfClause(action_, node);
		return;
	}

    DEBUG_CODE(printf("SoGetPucSpecificationAction::activeIfOperator %x,%s\n",node,node->getTypeId().getName().getString()));

	if (!action->withInActiveIf) 
	{
		DEBUG_CODE(printf("SoGetPucSpecificationAction::activeIfOperator exit without writting %x ,%s\n",node,node->getTypeId().getName().getString()));
		return;
	}

	std::string ret;
	SbString value;

	SoPucActiveIfOperator *op = (SoPucActiveIfOperator *)node;
	
	ret = "<" + getPucActiveIfOperatorOpStr(op->op) +">" + ENDL;
	action->pucSpec += ret.c_str() ;
	// go into subtree
	callDoAction(action, node);
	// come back and close tag			
	ret = "</" + getPucActiveIfOperatorOpStr(op->op) +">" + ENDL;
	action->pucSpec += ret.c_str() ;
}

/*
**
** =======================================================================================
**
*/

void SoGetPucSpecificationAction::specificationActionError(std::string msg) 
{
	std::cerr << msg << endl;
	exit(errorReturnCode);
}

/*
**
** =======================================================================================
**
*/

std::string SoGetPucSpecificationAction::getBaseAttributesStr(SoPucElementBase *elm)
{
	SbString value;

	std::string ret = " priority=\"";
	elm->priority.get(value);
	ret += value.getString();
	ret +="\"";

	ret += " name=\"";
	ret += elm->getStateName();
	ret += "\"";
	
	return ret;
}


std::string SoGetPucSpecificationAction::getPucActiveIfOperatorOpStr(SoSFEnum &op)
{
	switch (op.getValue()) 
	{
	case SoPucActiveIfOperator::OR:
		return "or";
	case SoPucActiveIfOperator::AND:
		return "and";
	default:
		;
	}
	specificationActionError("illigal PucActiveIfOperatorOp");
	return "forget to set string ?!";
}

std::string SoGetPucSpecificationAction::getPucActiveIfIgnoreStr(SoSFEnum &ignore) 
{
	switch (ignore.getValue())
	{
	case SoPucActiveIfNode::NONE:
		return ""; // no PUC sepecification of NONE !!
	case SoPucActiveIfNode::ALL:
		return "all";
	case SoPucActiveIfNode::PARENT:
		return "parent";
	default:
		;
	}
	specificationActionError("illigal PucActiveIfIgnore");

	return "#?@ error in ignore field #?@";
}

std::string SoGetPucSpecificationAction::getPucActiveIfClauseOpStr(SoSFEnum &op) 
{
	switch (op.getValue()) 
	{
	case SoPucActiveIfClause::EQUALS:
		return "equals";
	case SoPucActiveIfClause::GREATER_THAN:
		return "greaterthan";
	case SoPucActiveIfClause::LESS_THAN:
		return "lessthan";
	default:
		;
	}

	specificationActionError("illigal PucActiveIfClauseOp");
	
	return "forget to set string ?!";
}
	
//----------------------------------------------------------------------------

