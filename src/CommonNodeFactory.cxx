// ===========================================================================
//  (c) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:      CommonNodeFactory.cxx
//  TYPE:      cxx header file
//  PROJECT:   TrackerServer
//  CONTENT:   Implementation of class 'CommonNodeFactory'
//  VERSION:   1.0
// ===========================================================================
//  Author:    reitmayr  Gerhard Reitmayr
// ===========================================================================
//  HISTORY:
//
//  @INSERT_MODIFICATIONS(// )
// August 25, 2000 13:44 gr added NetworkSource classes
//     Update comment header
// ===========================================================================
//@START_USER1
//@END_USER1


// Master include file
#include "TrackerServer.h"


//@START_USER2
#include<string>
#include<math.h>
#include<stdio.h>
//@END_USER2


// Static members


/*@NOTE_3784
Constructor method.
*/
CommonNodeFactory::CommonNodeFactory() //@INIT_3784
    : NodeFactory()
{//@CODE_3784
    ConstructorInclude();

    // Put in your own code
}//@CODE_3784


/*@NOTE_1058
Destructor method.
*/
CommonNodeFactory::~CommonNodeFactory()
{//@CODE_1058
    DestructorInclude();

    // Put in your own code
}//@CODE_1058


/*@NOTE_4430
builds a new MaxConfidenceFilter
*/
MaxConfidenceFilter* CommonNodeFactory::buildMaxConfidenceFilter(StringMap& attributes)
{//@CODE_4430
    return new MaxConfidenceFilter();
}//@CODE_4430


/*@NOTE_4426
builds and returns a new Transformation node
*/
Transformation* CommonNodeFactory::buildTransformation(StringMap& attributes)
{//@CODE_4426
    float translation[3], scale[3], rotation[4];    
    float matrix[3][3];
    int num;    
    num = sscanf( attributes["translation"], " %f %f %f", &translation[0],
            &translation[1], &translation[2] );
    if( num != 3 )
    {
        translation[0] = 0;
        translation[1] = 0;
        translation[2] = 0;
    }
    num = sscanf( attributes["scale"], " %f %f %f", &scale[0], &scale[1],
            &scale[2] );
    if( num != 3 )
    {
        scale[0] = 1;
        scale[1] = 1;
        scale[2] = 1;
    }
    std::string type( attributes["rotationtype"] );
    if( type.compare("quaternion") == 0 )
    {
        num = sscanf( attributes["rotation"], " %f %f %f %f",
                &rotation[0], &rotation[1], &rotation[2], &rotation[3] );
        if( num != 4 )
        {
            rotation[0] = 0;
            rotation[1] = 0;
            rotation[2] = 0;
            rotation[3] = 1;
        }
    } 
    else if( type.compare("matrix") == 0 )
    {
        num = sscanf( attributes["rotation"], " %f %f %f %f %f %f %f %f %f",
                &matrix[0][0], &matrix[0][1], &matrix[0][2], 
                &matrix[1][0], &matrix[1][1], &matrix[1][2],
                &matrix[2][0], &matrix[2][1], &matrix[2][2] );
        if( num != 9 )
        {
            rotation[0] = 0;
            rotation[1] = 0;
            rotation[2] = 0;
            rotation[3] = 1;             
        } 
        else 
	{
	    MathUtils::matrixToQuaternion( matrix, rotation );
        }                                 
    } 
    else if( type.compare("euler") == 0 )
    {
        float roll, pitch, yaw;
        num = sscanf( attributes["rotation"], " %f %f %f",
                &roll, &pitch, &yaw );
        if( num != 3 )
        {
            rotation[0] = 0;
            rotation[1] = 0;
            rotation[2] = 0;
            rotation[3] = 1;
        } 
        else 
        {
	    MathUtils::eulerToQuaternion( roll, pitch, yaw, rotation );
        }        
    }
    else if( type.compare("axisangle") == 0 )
    {
        float axisa[4];
        num = sscanf( attributes["rotation"], " %f %f %f %f",
                &axisa[0], &axisa[1], &axisa[2], &axisa[3] );
        if( num != 4 )
        {
            rotation[0] = 0;
            rotation[1] = 0;
            rotation[2] = 0;
            rotation[3] = 1;
        } 
        else 
        {
	    MathUtils::axisAngleToQuaternion( axisa, rotation );
        }
    }
    return new Transformation( translation, scale, rotation );
}//@CODE_4426


/*@NOTE_4428
builds a new VirtualTransformation node
*/
VirtualTransformation* CommonNodeFactory::buildVirtualTransformation(StringMap& attributes)
{//@CODE_4428
    // little helper 
    Transformation *help = buildTransformation( attributes );
    VirtualTransformation *result = new VirtualTransformation( help->getTranslation(),
            help->getScale(), help->getRotation());
    delete help;
    return result;    
}//@CODE_4428


/*@NOTE_3612
This method is called to construct a new Node. It takes the element name and
any attribute name, value pairs and returns the new Node, if it is able to 
construct the corresponding type. Otherwise it returns NULL.
*/
TreeNode* CommonNodeFactory::createNode(char* const name, StringMap& attributes)
{//@CODE_3612
    std::string strName( name );
    if( strName.compare("Transformation") == 0 )
    {
        return buildTransformation( attributes );
    } else if( strName.compare("VirtualTransformation") == 0 )
    {
        return buildVirtualTransformation( attributes );
    } else if( strName.compare("MaxConfidenceFilter") == 0 )
    {
        return buildMaxConfidenceFilter( attributes );
    }
    return NULL;
}//@CODE_3612


//{{AFX DO NOT EDIT CODE BELOW THIS LINE !!!

/*@NOTE_1057
Method which must be called first in a constructor.
*/
void CommonNodeFactory::ConstructorInclude()
{
}


/*@NOTE_1059
Method which must be called first in a destructor.
*/
void CommonNodeFactory::DestructorInclude()
{
}


// Methods for the relation(s) of the class

//}}AFX DO NOT EDIT CODE ABOVE THIS LINE !!!

//@START_USER3
