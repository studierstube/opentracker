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
/** SoGroupSwitch implementation file
  *
  * @author Gerhard Reitmayr
  *
  * $Id: SoGroupSwitch.cxx 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#include "SoGroupSwitch.h"
#include "SoGroupSwitchElement.h"

#include <Inventor/misc/SoChildList.h>
#include <Inventor/misc/SoState.h>
#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/actions/SoGetMatrixAction.h>
#include <Inventor/actions/SoHandleEventAction.h>
#include <Inventor/actions/SoPickAction.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/actions/SoWriteAction.h>

#include <stbapi/event3d/SoHandle3DEventAction.h>

#include <vector>
#include <set>
#include <algorithm>

using namespace std;

SO_NODE_SOURCE(SoGroupSwitch);

void SoGroupSwitch::initClass()
{
    SoGroupSwitchElement::initClass();

    SO_NODE_INIT_CLASS(SoGroupSwitch, SoMultiSwitch, "SoMultiSwitch");

    SO_ENABLE(SoCallbackAction, SoGroupSwitchElement);
    SO_ENABLE(SoGLRenderAction, SoGroupSwitchElement);
    SO_ENABLE(SoGetBoundingBoxAction, SoGroupSwitchElement);
    SO_ENABLE(SoGetMatrixAction, SoGroupSwitchElement);
    SO_ENABLE(SoHandleEventAction, SoGroupSwitchElement);
    SO_ENABLE(SoPickAction, SoGroupSwitchElement);
    SO_ENABLE(SoSearchAction, SoGroupSwitchElement);
    SO_ENABLE(SoHandle3DEventAction, SoGroupSwitchElement);
}

// constructor

SoGroupSwitch::SoGroupSwitch() : cbGroupFunc( NULL )
{
    SO_NODE_CONSTRUCTOR(SoGroupSwitch);
    SO_NODE_ADD_FIELD(numChildren, (-1));
    isBuiltIn = TRUE;   
}

// constructor that takes approximate number of children.

SoGroupSwitch::SoGroupSwitch(int nChildren) : SoMultiSwitch(nChildren), cbGroupFunc( NULL )
{
    SO_NODE_CONSTRUCTOR(SoGroupSwitch);
    SO_NODE_ADD_FIELD(numChildren, (-1));
    isBuiltIn = TRUE;
}

//    Destructor

SoGroupSwitch::~SoGroupSwitch()
{}

// Overrides method in SoMultiSwitch to return default value
// because we can only determine within an action which nodes to process 

SbBool SoGroupSwitch::affectsState() const
{
    return SoNode::affectsState();
}

// function object that tests membership of an element in a set
template <class T> 
class NotInSet {
private:
    const set<T> & theSet;
public:
    NotInSet( const set<T> & s) : theSet(s) {}

    inline bool operator() (T & elem) const {
        return theSet.find( elem ) == theSet.end();
    }
};

// Typical action traversal. this does the real work

void SoGroupSwitch::doAction(SoAction *action)
{
    int	numIndices = -1;
    const int *indices;
    int lastChild = getNumChildren() - 1;
    int switchCase;

    const int32_t * groups;
    const int32_t * children = whichChildren.getValues(0);
    int32_t groupsNum;
    int32_t childrenNum = whichChildren.getNum();
    vector<int32_t> indexVector;

    // either process no children or get the smallest index (makes -3 override the others) 
    if (whichChildren.isIgnored() || numChildren.isIgnored() || getNumChildren() == 0)
    	switchCase = SO_SWITCH_NONE;
    else 
    {
        // get the groups to traverse from the element
        groups = SoGroupSwitchElement::get(action->getState(), &groupsNum );
    
        // just to be fail safe, if no element is in the state, display nothing !
        if( groups == NULL )
            switchCase = SO_SWITCH_NONE;
        else
        {
            // check for special cases
            switchCase = (*min_element( groups, groups+groupsNum));

            // copy the children over into the vector according to the groups stored
            for( int i = 0; i < groupsNum; i++ )
            {
                int group = groups[i];
                if( group >= numChildren.getNum() || group < 0 )
                    group %= numChildren.getNum();
                int count = 0;
		
				int j;		
                for( j = 0; j <= group -1; j ++ )
                    count += numChildren[j];
                if( cbGroupFunc != NULL )
                    if( cbGroupFunc( userDataGroup, this, action, group ) != TRUE )
                        continue;
                for( j = count; j < numChildren[group] + count; j++ )            
                    indexVector.push_back( children[j % childrenNum] );
            }

            // if we need them ordered, do so and remove any duplicates
            if (ordered.getValue() == TRUE )
            {
                sort(indexVector.begin(), indexVector.end());
                indexVector.resize( unique(indexVector.begin(), indexVector.end()) - indexVector.begin());
            }

            // if we are in path remove all children that need not to be traversed
            if(action->getPathCode(numIndices, indices) == SoAction::IN_PATH)
            {                
                lastChild = indices[numIndices -1];

                // remove the ones that
                // are not in the path set, however don't change the order etc
                std::set<int32_t> pathSet( indices, indices+numIndices );
                NotInSet<int32_t> test( pathSet );
                indexVector.resize(remove_if(indexVector.begin(), indexVector.end(),test) - indexVector.begin());
            }
        }
    }

    switch (switchCase) {
        case SO_SWITCH_NONE:
	        break;

        case SO_SWITCH_ALL:        
            if( lastChild < 0 )
                break;
	        // If traversing to compute bounding box, we have to do some
	        // work in between children
	        if (action->isOfType(SoGetBoundingBoxAction::getClassTypeId())) 
            {
	            SoGetBoundingBoxAction *bba = (SoGetBoundingBoxAction *) action;
	            SbVec3f	totalCenter(0.0, 0.0, 0.0);
	            int numCenters = 0;

	            for (int i = 0; i <= lastChild; i++) 
                {
		            doChildren(bba, i);
		            if (bba->isCenterSet()) 
                    {
		                totalCenter += bba->getCenter();
		                numCenters++;
		                bba->resetCenter();
		            }
	            }
	            // Now, set the center to be the average:
	            if (numCenters != 0)
		            bba->setCenter(totalCenter /(float) numCenters, FALSE);
	        }
	        else { 
                doChildren(action, 0, lastChild );                
	        }
	        break;

       default:
            // IN_PATH or not, in indexVector only the right values are stored.
            // now we can easily iterate over them and do our thing

            // If traversing to compute bounding box, we have to do some
	        // work in between children
	        if (action->isOfType(SoGetBoundingBoxAction::getClassTypeId())) 
            {
	            SoGetBoundingBoxAction *bba = (SoGetBoundingBoxAction *) action;
	            SbVec3f	totalCenter(0.0, 0.0, 0.0);
	            int numCenters = 0;

                for( vector<int32_t>::iterator it = indexVector.begin(); it != indexVector.end(); it++ )
                {
                    doChildren(bba, ((*it) > getNumChildren())?((*it) % getNumChildren()):(*it));
                    if (bba->isCenterSet()) 
                    {
		                totalCenter += bba->getCenter();
		                numCenters++;
		                bba->resetCenter();
		            }
                }
	            // Now, set the center to be the average:
	            if (numCenters != 0)
		            bba->setCenter(totalCenter /(float) numCenters, FALSE);
	        }
	        else { 
                for( vector<int32_t>::iterator it = indexVector.begin(); it != indexVector.end(); it++ )
                {
                    doChildren(action, ((*it) > getNumChildren())?((*it) % getNumChildren()):(*it));
                }
	        }
	        break;
    }
}

void SoGroupSwitch::setGroupCallback( SoGroupSwitchCB * func, void * data )
{
    cbGroupFunc = func;
    userDataGroup = data;
}

void SoGroupSwitch::removeGroupCallback()
{
    cbGroupFunc = NULL;
}
