 /* ========================================================================
  * Copyright (C) 2001  Vienna University of Technology
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
  * For further information please contact Gerhard Reitmayr under
  * <reitmayr@ims.tuwien.ac.at> or write to Gerhard Reitmayr,
  * Vienna University of Technology, Favoritenstr. 9-11/188, A1040 Vienna,
  * Austria.
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** source file for XMLWriter class.
  *
  * @author Gerhard Reitmayr
  *
  * $Id$
  * @file                                                                   */
 /* ======================================================================= */

#include "../../OpenTracker.h"

// selects between usage of XERCES and TinyXML
#include "../../tool/XMLSelection.h"

#ifdef USE_XERCES
  #include <xercesc/dom/DOM.hpp>
  #include <xercesc/util/XMLString.hpp>
  #include <xercesc/framework/LocalFileFormatTarget.hpp>
#endif //USE_XERCES

#include <memory>

#include "XMLWriter.h"
#include "../../tool/OT_ACE_LOG.h"

//using namespace std;

#ifdef USE_XERCES
XERCES_CPP_NAMESPACE_USE
#endif //USE_XERCES

namespace ot {


#ifdef USE_XERCES

/** internal method that writes out the graph recursively. This may change 
 * and therefore is not part of the interface.
 * @param toWrite the current XML element to write out
 * @param target the output stream to write to
 */
void writeNode(XERCES_CPP_NAMESPACE_QUALIFIER DOMNode * toWrite, XERCES_CPP_NAMESPACE_QUALIFIER XMLFormatTarget * target );

XMLWriter::XMLWriter( Context & context_ , unsigned int indent_ ) :
  context( context_ ), indent( indent_ )
{}

XMLWriter::~XMLWriter()
{
  
}
         
void XMLWriter::write( const char * file )
{
    auto_ptr<XMLFormatTarget> myFormatTarget ( new LocalFileFormatTarget( file ));
    writeNode( ((DOMNode *)(context.getRootNode()->parent))->getOwnerDocument(), 
               myFormatTarget.get());
}
/*
void XMLWriter::write( ostream & stream )
{
   // empty
}
*/
void writeNode(XERCES_CPP_NAMESPACE_QUALIFIER DOMNode * toWrite, XERCES_CPP_NAMESPACE_QUALIFIER XMLFormatTarget * target )
{
    XMLCh * lsCh =  XMLString::transcode("LS");
    DOMImplementation * impl = DOMImplementationRegistry::getDOMImplementation(lsCh);
    DOMWriter * writer = impl->createDOMWriter();
    if (writer->canSetFeature(XMLUni::fgDOMWRTDiscardDefaultContent, true))
        writer->setFeature(XMLUni::fgDOMWRTDiscardDefaultContent, true);
    if (writer->canSetFeature(XMLUni::fgDOMWRTFormatPrettyPrint, true))
        writer->setFeature(XMLUni::fgDOMWRTFormatPrettyPrint, true);
    try {
        // do the serialization through DOMWriter::writeNode();
        writer->writeNode(target, *toWrite);
    }
    catch (const XMLException& toCatch) {
        char * message = XMLString::transcode(toCatch.getMessage());
        LOG_ACE_ERROR("ot:XMLWriter Exception message is: %s\n", message);
        XMLString::release( &message );
    }
    catch (const DOMException& toCatch) {
        char * message = XMLString::transcode(toCatch.msg);
        LOG_ACE_ERROR("ot:XMLWriter Exception message is: %s\n", message);
        XMLString::release( &message );
    }
    catch (...) {
        LOG_ACE_ERROR("ot:XMLWriter Unexpected Exception \n");
    }
    writer->release();
    XMLString::release( &lsCh );
}

#endif //USE_XERCES


#ifdef USE_TINYXML

XMLWriter::XMLWriter( Context & context_ , unsigned int indent_ ) :
  context( context_ ), indent( indent_ )
{}

XMLWriter::~XMLWriter()
{
  
}
         
void XMLWriter::write( const char * file )
{
	TiXmlNode* node = (TiXmlNode*)context.getRootNode()->parent;
	TiXmlDocument* doc = node->GetDocument();

	doc->SaveFile(file);
}

#endif //USE_TINYXML


}  // namespace ot
