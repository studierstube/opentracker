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
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/misc/xml/XMLWriter.cxx,v 1.10 2003/07/31 07:55:43 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

#include <xercesc/dom/DOM.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/framework/LocalFileFormatTarget.hpp>

#include <fstream>
#include <memory>

#include "XMLWriter.h"

using namespace std;
XERCES_CPP_NAMESPACE_USE

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
    writeNode( ((DOMNode *)(context.getRootNode()->parent))->getOwnerDocument()->getFirstChild(), 
               myFormatTarget.get());
}
    
void XMLWriter::write( ostream & stream )
{
   // empty
}

void writeNode(XERCES_CPP_NAMESPACE_QUALIFIER DOMNode * toWrite, XERCES_CPP_NAMESPACE_QUALIFIER XMLFormatTarget * target )
{
    auto_ptr<XMLCh> lsCh ( XMLString::transcode("LS"));
    DOMImplementation * impl = DOMImplementationRegistry::getDOMImplementation(lsCh.get());
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
        auto_ptr<char> message( XMLString::transcode(toCatch.getMessage()));
        cout << "Exception message is: \n"
            << message.get() << "\n";
    }
    catch (const DOMException& toCatch) {
        auto_ptr<char> message( XMLString::transcode(toCatch.msg));
        cout << "Exception message is: \n"
            << message.get() << "\n";
    }
    catch (...) {
        cout << "Unexpected Exception \n" ;
    }
    writer->release();
}
