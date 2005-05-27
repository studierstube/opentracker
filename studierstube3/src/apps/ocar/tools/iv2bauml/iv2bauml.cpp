/* ========================================================================
 * Copyright (C) 2000,2001,2002  Vienna University of Technology
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
 * PROJECT: ocar
 * ======================================================================== */
/**
 * iv2bauml tool
 *
 * iv2bauml reads a flat iv file and generates a bauml file out of it. 
 * In this first iteration, it outputs buildings for each SoIndexedFaceSet.
 * It also transforms the vertex data into the world coordinate system, 
 * before constructing the XML file. 
 *
 * Inputs are an input file and optionally an output file. If no output file
 * is specified it prints the resulting XML file to stdout.
 *
 * @author Gerhard Reitmayr
 *
 * @todo add some configurability
 * @todo add schema location option
 * @todo other types of transformations ?
 *
 * @author Gerhard Reitmayr
 *
 * $Id: iv2bauml.cpp 3427 2004-07-14 12:20:49Z tamer $
 * @file                                                                   */
/* ======================================================================= */

#include <iostream>
#include <memory>
#include <vector>

#include <Inventor/Sb.h>
#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/nodes/SoIndexedFaceSet.h>
#include <Inventor/nodes/SoSeparator.h>

#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/dom/DOM.hpp>

#include <xercesc/framework/StdOutFormatTarget.hpp>
#include <xercesc/framework/MemBufFormatTarget.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/util/XMLUni.hpp>

#include <stbapi/util/option.h>

using namespace std;
using namespace xercesc;

SbName currentName;

SoCallbackAction::SoCallbackActionCB indexedShapeCB;

SoCallbackAction::SoCallbackActionCB separatorCB;

const char * vector2string( vector<int> & );

SbString & printVec( const SbVec3f & );
SbString & printRot( const SbRotation & );

void printUsage(void);

// ---------------------------------------------------------------------------
//  This is a simple class that lets us do easy (though not terribly efficient)
//  trancoding of char* data to XMLCh data.
// ---------------------------------------------------------------------------
class XStr
{
public :
    // -----------------------------------------------------------------------
    //  Constructors and Destructor
    // -----------------------------------------------------------------------
    XStr(const char* const toTranscode)
    {
        // Call the private transcoding method
        fUnicodeForm = XMLString::transcode(toTranscode);
    }
    
    ~XStr()
    {
        delete [] fUnicodeForm;
    }
    
    
    // -----------------------------------------------------------------------
    //  Getter methods
    // -----------------------------------------------------------------------
    const XMLCh* unicodeForm() const
    {
        return fUnicodeForm;
    }
    
private :
    // -----------------------------------------------------------------------
    //  Private data members
    //
    //  fUnicodeForm
    //      This is the Unicode XMLCh format of the string.
    // -----------------------------------------------------------------------
    XMLCh*   fUnicodeForm;
};

#define X(str) XStr(str).unicodeForm()


OptionFlag globalCoords  ("global", "g");
//                  OptionLong          numberOpt("number", "n", 1);
//                  OptionDouble        ratioOpt ("ratio",  "r", 1.0);
//                  OptionString        stringOpt("string", "s");

int main(int argc, char* argv[])
{    
    if(!Option::parse(argc,argv))
    {
        printUsage();
        return 0;
    }

    // simple check for correct command line
    if( argc < 2 || argc > 3 )
    {
        printUsage();
        return 0;
    }
    
    // initialization of libraries
    SoDB::init();
    // Initialize the Xerces system.
    try
    {
        XMLPlatformUtils::Initialize();
    }
    
    catch(const XMLException& toCatch)
    {
        char *pMsg = XMLString::transcode(toCatch.getMessage());
        cerr << "Error during Xerces-c Initialization.\n"
            << "  Exception message:"
            << pMsg;
        delete [] pMsg;
        return 1;
    }
    
    // read in inventor file
    cout << "Reading in OpenInventor file " << argv[1] << " ...\n";
    SoInput input;
    if( !input.openFile(argv[1]))
    {
        cout << "Could not open input file " << argv[1] << " !" << endl;
        XMLPlatformUtils::Terminate();        
        return 1;
    }
    SoSeparator * root = SoDB::readAll( &input );
    if( !root )
    {
        cout << "Error reading input file " << argv[1] << " !" << endl;
        XMLPlatformUtils::Terminate();        
        return 1;
    }
    
    {
        // create a XML document
        DOMImplementation* impl =  DOMImplementationRegistry::getDOMImplementation( NULL ); // X("Core")
        
        DOMDocument* doc = impl->createDocument(
            0,                    // root element namespace URI.
            X("SpatialContainer"),         // root element name
            0);                   // document type object (DTD).
        
        // set up the document root element and some annotation
        DOMElement * rootXML = doc->getDocumentElement();
        // set up namespace information an schema location
        // xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:noNamespaceSchemaLocation="aml.xsd"
        //rootXML->setAttributeNS(X("xmlns"),X("xsi"),X("http://www.w3.org/2001/XMLSchema-instance"));
        rootXML->setAttribute(X("xmlns:noNamespaceSchemaLocation"),X("bauml.xsd"));
        
        rootXML->setAttribute(X("baseType"),X("SpatialContainerType"));
        DOMElement * annotation = doc->createElement(X("annotation"));
        rootXML->appendChild( annotation );
        SbString annot("building file, automatically created by iv2bauml from the file " );
        annot += argv[1];
        DOMText * text = doc->createTextNode(X(annot.getString()));
        annotation->appendChild( text );
        DOMElement * children = doc->createElement(X("children"));
        rootXML->appendChild( children );        
        
        SoCallbackAction action;
        action.addPostCallback( SoIndexedFaceSet::getClassTypeId(), indexedShapeCB, children );
        action.addPreCallback( SoSeparator::getClassTypeId(), separatorCB, NULL );
                
        cout << "Applying callback action to build XML tree ...\n";    
        action.apply( root );
        
        // now print out the XML result tree :
        cout << "Printing out XML Tree ";    
        
        try
        {
            // get a serializer, an instance of DOMWriter
            XMLCh tempStr[100];
            XMLString::transcode("LS", tempStr, 99);
            DOMImplementation *impl          = DOMImplementationRegistry::getDOMImplementation(tempStr);
            DOMWriter         *theSerializer = ((DOMImplementationLS*)impl)->createDOMWriter();
            
            // set user specified end of line sequence and output encoding
            // theSerializer->setNewLine(gMyEOLSequence);
            // theSerializer->setEncoding(gOutputEncoding);
            
            
            // set feature if the serializer supports the feature/mode                
            if (theSerializer->canSetFeature(XMLUni::fgDOMWRTFormatPrettyPrint, true))
                theSerializer->setFeature(XMLUni::fgDOMWRTFormatPrettyPrint, true );
            
            //
            // Plug in a format target to receive the resultant
            // XML stream from the serializer.
            //
            // StdOutFormatTarget prints the resultant XML stream
            // to stdout once it receives any thing from the serializer.
            //
            
            if( argc == 2) // print to stdout
            {
                StdOutFormatTarget *myFormTarget = new StdOutFormatTarget;
                cout << "to stdout\n\n";
                theSerializer->writeNode(myFormTarget, *doc);
                delete myFormTarget;                
            }
            else
            {
                MemBufFormatTarget * myFormTarget = new MemBufFormatTarget(1024 * 1024);
                cout << "to file " << argv[2] << endl << endl;
                theSerializer->writeNode(myFormTarget, *doc);
                FileHandle handle = XMLPlatformUtils::openFileToWrite( argv[2] );
                XMLPlatformUtils::writeBufferToFile(handle, myFormTarget->getLen(),
                    myFormTarget->getRawBuffer());
                XMLPlatformUtils::closeFile(handle);
                delete myFormTarget;
            }
            
            delete theSerializer;            
        }
        catch (XMLException& e)
        {
            char *pMsg = XMLString::transcode(e.getMessage());
            cerr << "An error occurred during creation of output transcoder.\n"
                << "  Exception message:"
                << pMsg;
            delete [] pMsg;
        }
    }
    XMLPlatformUtils::Terminate();    

    return 0;
}

/**
 * this callback creates for each SoIndexedFaceSet a XML fragment of the following
 * form :
 *@beginverbatim
<Building id="name">
    <representation>
        <Vertex position="0 0 0"/>
        <Polygon vertices="0 1 2 3" type="wall"/>      
    </representation>
</Building>@endverbatim
 */
SoCallbackAction::Response indexedShapeCB( void *data, SoCallbackAction * action, 
                                          const SoNode * node)
{
    // cout << "Entering callback for " << node << endl;
    if( node->getTypeId() == SoIndexedFaceSet::getClassTypeId())
    {
        SoIndexedFaceSet * faceSet = (SoIndexedFaceSet *) node;
        // this is the element to add the buildings to
        DOMElement * root = (DOMElement *) data;
        DOMDocument * doc = root->getOwnerDocument();
        DOMElement * building = NULL;
        
        const int32_t * indices = faceSet->coordIndex.getValues(0);
        int numIndices = faceSet->coordIndex.getNum();
        const SbVec3f * ivVertices;
        SoVertexProperty * vertexProp = (SoVertexProperty *)faceSet->vertexProperty.getValue();
        
        if( vertexProp != NULL )
        {
            ivVertices = vertexProp->vertex.getValues(0);
            int numVertices = vertexProp->vertex.getNum();
            int i;
            try {
                building = doc->createElement(X("Building"));
                // set the id of the building element
                if( currentName != "" )
                {
                    // cout << "Set name " << currentName.getString() << endl;
                    building->setAttribute(X("id"),X(currentName.getString()));
                    currentName = "";
                }
                building->setAttribute(X("baseType"),X("SpatialContainerType"));

                auto_ptr<SbVec3f> vertices(new SbVec3f[numVertices]);
                if( globalCoords.isSpecified())
                {
                    // compute the vertices in world coordinates
                    SbMatrix toWorld = action->getModelMatrix();
                    for( i = 0; i < numVertices; i++ )
                        toWorld.multVecMatrix( ivVertices[i], vertices.get()[i]);
                }
                else
                {
                    for( i = 0; i < numVertices; i++ )
                        vertices.get()[i] = ivVertices[i];
                    DOMElement * pose = doc->createElement(X("pose"));
                    DOMElement * trans = doc->createElement(X("Transformation"));
                    SbMatrix toWorld = action->getModelMatrix();
                    SbVec3f translation;
                    SbRotation rotation, scaleRot;
                    SbVec3f scale;
                    toWorld.getTransform(translation, rotation, scale, scaleRot );
                    trans->setAttribute(X("translation"),X(printVec(translation).getString()));
                    trans->setAttribute(X("scale"),X(printVec(scale).getString()));
                    trans->setAttribute(X("rotation"),X(printRot(rotation).getString()));
                    trans->setAttribute(X("rotationType"),X("axisangle"));
                    pose->appendChild(trans);
                    building->appendChild(pose);
                }

                // create representation
                DOMElement * rep = doc->createElement(X("representation"));
                building->appendChild( rep );
                for( i = 0; i < numVertices; i++ )
                {
                    DOMElement * vertex = doc->createElement(X("Vertex"));
                    vertex->setAttribute(X("position"),X(printVec(vertices.get()[i]).getString()));
                    rep->appendChild( vertex );
                }
                vector<int> index;
                for( i = 0; i < numIndices; i++ )
                {
                    if( indices[i] == -1 )  // end of polygon
                    {
                        if( index.size() > 2 ) // at least three points per polygon
                        {
                            DOMElement * polygon = doc->createElement(X("Polygon"));
                            polygon->setAttribute(X("type"),X("wall"));                            
                            polygon->setAttribute(X("vertices"),X(vector2string(index)));
                            rep->appendChild( polygon );                        
                            index.clear();
                        }
                    }
                    else                    // add another index to the string
                    {
                        index.push_back(indices[i]);
                    }
                }
                if( index.size() > 0 )  // a last polygon ??
                {
                    DOMElement * polygon = doc->createElement(X("Polygon"));
                    polygon->setAttribute(X("type"),X("wall"));
                    polygon->setAttribute(X("vertices"),X(vector2string(index)));
                    rep->appendChild( polygon );                    
                }
                // everything worked, we add it to the file
                root->appendChild( building );
                DOMText * text = doc->createTextNode(X("\n"));
                root->appendChild( text );
            }
            catch(DOMException ex)
            {
                cout << "Error during XML construction\n";
                if( building != NULL )
                {
                    building->release();
                    building = NULL;
                }
            }            
        }
        else
        {
            cout << "VertexProperty is empty !\n";
        }
    }
    else 
    {
        cout << "Callback with wrong type " << node->getTypeId().getName().getString() << endl;
    }
    return SoCallbackAction::CONTINUE;
}

SoCallbackAction::Response separatorCB( void *data, SoCallbackAction * action, 
                                          const SoNode * node)
{
    if( node->getTypeId() == SoSeparator::getClassTypeId())
    {
        if( node->getName() != "" )
        {
            currentName = node->getName();
            // cout << "current name is " << currentName.getString() << endl;
        }        
    }
    return SoCallbackAction::CONTINUE;
}

const char * vector2string( vector<int> & index )
{
    static SbString str;
    str = "";
    vector<int>::iterator it;
    for( it = index.begin(); it != index.end(); it++)
    {
        str += SbString( (*it));
        if( (it+1) != index.end())
           str += " ";
    }
    return str.getString();
}

SbString & printVec( const SbVec3f & vec)
{
    char buffer[1024];
    static SbString result;

    sprintf(buffer,"%f %f %f", vec[0], vec[1], vec[2] );
    result = buffer;
    return result;
}

SbString & printRot( const SbRotation & rot )
{
    SbVec3f axis;
    float angle;
    char buffer[1024];   
    static SbString result;
    
    rot.getValue(axis, angle);
    sprintf(buffer,"%f %f %f %f", axis[0], axis[1], axis[2], angle );
    result = buffer;
    return result;
}


void printUsage(void)
{
    cout << "usage : iv2bauml [options] input.iv [output.iv]" << endl;
    cout << "        -g --global transform coordinates to world coordinate system instead of setting pose\n";
}