 /* ========================================================================
  * Copyright (c) 2006,
  * Institute for Computer Graphics and Vision
  * Graz University of Technology
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without
  * modification, are permitted provided that the following conditions are
  * met:
  *
  * Redistributions of source code must retain the above copyright notice,
  * this list of conditions and the following disclaimer.
  *
  * Redistributions in binary form must reproduce the above copyright
  * notice, this list of conditions and the following disclaimer in the
  * documentation and/or other materials provided with the distribution.
  *
  * Neither the name of the Graz University of Technology nor the names of
  * its contributors may be used to endorse or promote products derived from
  * this software without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
  * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
  * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
  * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER
  * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
  * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
  * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
  * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
  * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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

#ifndef OT_NO_XMLWRITER_SUPPORT


// selects between usage of XERCES and TinyXML
#include "../../tool/XMLSelection.h"

#ifdef USE_XERCES
  #include <xercesc/dom/DOM.hpp>
  #include <xercesc/util/XMLString.hpp>
  #include <xercesc/framework/LocalFileFormatTarget.hpp>
#endif //USE_XERCES

#include <memory>

#include "XMLWriter.h"
#include "../../tool/OT_ACE_Log.h"

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
	std::auto_ptr<XMLFormatTarget> myFormatTarget ( new LocalFileFormatTarget( file ));
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



#else
#pragma message(">>> OT_NO_XMLWRITER_SUPPORT")
#endif //OT_NO_XMLWRITER_SUPPORT
