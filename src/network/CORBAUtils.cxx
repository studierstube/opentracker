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
/** source file for CORBAUtils class
 *
 * @author Joseph Newman
 *
 * $Id: CORBAUtils.cxx 1151 2006-06-13 12:31:43Z jfn $
 * @file                                                                   */
/* ======================================================================= */

#include <ace/OS.h>
#include "CORBAUtils.h"
#include <iostream>
#include "../tool/OT_ACE_Log.h"

namespace ot {

// This method converts an ot::Event to an OT_CORBA::Event
    void CORBAUtils::convertToCORBAEvent(Event& ot_event, OT_CORBA::Event& corba_event) 
{
    int i;
    std::vector<float> position = ot_event.getPosition();
    std::vector<float> orientation = ot_event.getOrientation();
    for( i = 0; i < 3; i++ )
    {
      corba_event.pos[i] = position[i];
      corba_event.ori[i] = orientation[i];
    }
  corba_event.ori[3] = (float) orientation[3];
  corba_event.but = ot_event.getButton();
  corba_event.time = ot_event.time;
} 

// This method converts from an OT_CORBA::Event to an ot::Event
    void CORBAUtils::convertFromCORBAEvent(Event& ot_event, const OT_CORBA::Event& corba_event) 
    {
        int i;
        std::vector<float> position(3);
        std::vector<float> orientation(4);
        for( i = 0; i < 3; i++ )
        {
            position[i] = corba_event.pos[i];
            orientation[i] = corba_event.ori[i];
        }
        orientation[3] = corba_event.ori[3];
        ot_event.setPosition(position);
        ot_event.setOrientation(orientation);
        ot_event.setButton(corba_event.but);
        ot_event.time = corba_event.time;
    } 
    
    // This method returns an object id based on the id of the last NameComponent of a Name sequence
    PortableServer::ObjectId_var CORBAUtils::getObjectId(CORBA::ORB_ptr orb, CosNaming::NamingContextExt::StringName_var string_name) {
        CosNaming::NamingContextExt_var extContext = getRootContext(orb);
        CosNaming::Name_var name;
        try {
            // Create a name object from the string_name
            name = extContext->to_name(string_name);
        } catch(CosNaming::NamingContext::InvalidName) {
            // This exception is thrown if the name is not valid
            cerr << "Name is not valid\n Exiting...." << endl;
            exit(-1);
        }
        CORBA::String_var oid_string = CORBA::string_dup(name[name->length()-1].id);
        return PortableServer::string_to_ObjectId(oid_string);
    }
    
    // This method binds an object reference to a Name specified using a string

    void CORBAUtils::bindObjectReferenceToName(CORBA::ORB_ptr orb, CORBA::Object_var obj, CosNaming::NamingContextExt::StringName_var string_name)
    {
        CosNaming::NamingContextExt_var extContext = getRootContext(orb);
        if (CORBA::is_nil(extContext)) {
            LOG_ACE_INFO("Could not obtain a reference to the root Context\nExiting....\n");
            exit(-1);
        }
        CosNaming::Name_var name;
        try {
            // Create a name object from the string_name
            name = extContext->to_name(string_name);
        } catch(CosNaming::NamingContext::InvalidName) {
            // This exception is thrown if the name is not valid
            cerr << "Name is not valid\n Exiting...." << endl;
            exit(-1);
        }
        CosNaming::Name object_name;
        object_name.length(1);
        CORBA::ULong length = name->length();
        cerr << "length = " << length << endl;
        cerr << "name[" << length-1 << "] = " << name[length-1].id << "." << name[length-1].kind << endl;
        object_name[0].id = name[length-1].id;
        object_name[0].kind = name[length-1].kind;
        name->length(length-1); // shorten name so it only refers to the contexts
        CosNaming::NamingContext_var desired_context = CosNaming::NamingContext::_narrow(getContext(CosNaming::NamingContext::_narrow(extContext), name));
        if (CORBA::is_nil(desired_context)) {
            cerr << "Was unable to obtain the appropriate context\nExiting..." << endl;
            exit(-1);
        }
        try {
            desired_context->bind(object_name, obj);
        } catch(CosNaming::NamingContext::AlreadyBound) {
            cerr << "The context " << object_name[0].id << "." << object_name[0].kind
                 << " is already bound. Rebinding...";
            desired_context->rebind(object_name, obj);
        }
    }
   
    CosNaming::NamingContext_var CORBAUtils::getContextFromName(CosNaming::NamingContext_var root_context, CosNaming::Name_var name)
    {
        CORBA::Object_var obj;
        try {
            obj = root_context->resolve(name);
        } catch (CosNaming::NamingContext::NotFound) {
            cerr << "Name not found" << endl;
            return CosNaming::NamingContextExt::_nil();
        } 
        CosNaming::NamingContext_var sink_context = CosNaming::NamingContextExt::_narrow(obj);
        if  (CORBA::is_nil(sink_context)) {
            cerr << "Object reference isn't a Context" << endl;
        }
        return sink_context;
    }

    CosNaming::NamingContext_var CORBAUtils::getContext(CosNaming::NamingContext_var root_context, CosNaming::Name_var name)
    {
        // This method obtains a context, creating it if necessary, having bound
        // it to all the other contexts specified in the 
        // name variable
        CosNaming::NamingContextExt_var context_already_bound;// = CosNaming::NamingContextExt::_nil();
        CORBA::ULong length = name->length();
        CORBA::ULong max = length;
        CosNaming::Name uncontextualised_names;
        cerr << "CORBA::is_nil(context_already_bound)" << CORBA::is_nil(context_already_bound) << endl;
        while (CORBA::is_nil(context_already_bound) && (name->length() > 0)) {
            cerr << "inside while loop " << endl;
            context_already_bound = CosNaming::NamingContextExt::_narrow(getContextFromName(root_context, name));
            if (CORBA::is_nil(context_already_bound)) {
                cerr << "uncontextualised_names.length(" << (max+1)-length << ")" << endl;
                uncontextualised_names.length(max-length+1); // extend the NameComponent sequence
                uncontextualised_names[max-length] = name[length-1];
                length--;
                name->length(length); // knock-off the last CosNaming::NameComponent
            } //} else {
                //cerr << "going to bind directly to root context" << endl;
                //context_already_bound = CosNaming::NamingContextExt::_narrow(root_context);
                //}
        }
        if (CORBA::is_nil(context_already_bound)) {
            cerr << "binding directly to root context" << endl;
            context_already_bound = CosNaming::NamingContextExt::_narrow(root_context);
                
        }
        CosNaming::NamingContext_var previous_context = CosNaming::NamingContextExt::_duplicate(context_already_bound);
        cerr << "uncontextualised_names.length() = " << uncontextualised_names.length() << endl;
        for (CORBA::ULong i = uncontextualised_names.length(); i > 0; --i) { 
            cerr << "i = " << i << endl;
            // Go through sequence of uncontextualised names in reverse order
            CosNaming::Name new_name;
            new_name.length(1);
            new_name[0].id = uncontextualised_names[i].id;
            new_name[0].kind = uncontextualised_names[i].kind;
            
            CosNaming::NamingContext_var next_context = previous_context->bind_new_context(new_name);
        }
        return previous_context;
    } 

    CosNaming::NamingContextExt_var CORBAUtils::getRootContext(CORBA::ORB_ptr orb) 
    {
        CosNaming::NamingContextExt_var extContext;
        try {
            // Obtain a reference to the root context of the Name service:
            CORBA::Object_var obj = orb->resolve_initial_references("NameService");
            
            // Narrow the reference returned.
            extContext = CosNaming::NamingContextExt::_narrow(obj);
            if( CORBA::is_nil(extContext) ) {
                cerr << "Failed to narrow the root naming context." << endl;
                //return CORBA::Object::_nil();
                return CosNaming::NamingContextExt::_nil();
            }
        } catch(CORBA::ORB::InvalidName& ex) {
            // This should not happen!
            cerr << "Service required is invalid [does not exist]." << endl;
            return CosNaming::NamingContextExt::_nil();
        } catch(CORBA::TRANSIENT) {
            cerr << "Naming Service isn't running" << endl;
            return CosNaming::NamingContextExt::_nil();
        }
        return extContext;
    }

    CORBA::Object_ptr CORBAUtils::getObjectReference(CORBA::ORB_ptr orb, CosNaming::NamingContextExt::StringName_var string_name)
    {
        
        CosNaming::NamingContextExt_var extContext = getRootContext(orb);
        if (CORBA::is_nil(extContext)) {
            LOG_ACE_INFO("Could not obtain a reference to the root Context\nExiting....\n");
            exit(-1);
      }

        CosNaming::Name_var name;
        try {
            // Create a name object from the string_name
            name = extContext->to_name(string_name);
        } catch(CosNaming::NamingContext::InvalidName) {
            // This exception is thrown if the name is not valid
            cerr << "Name is not valid" << endl;
            return CORBA::Object::_nil();
        }
        
        try {
            // Resolve the name to an object reference.
            return extContext->resolve(name);
        } catch(CosNaming::NamingContext::NotFound& ex) {
            // This exception is thrown if any of the components of the
            // path [contexts or the object] aren't found:
            cerr << "Context not found." << endl;
        } catch(CORBA::COMM_FAILURE& ex) {
            cerr << "Caught system exception COMM_FAILURE -- unable to contact the "
                 << "naming service." << endl;
        } catch(CORBA::SystemException&) {
            cerr << "Caught a CORBA::SystemException while using the naming service."
                 << endl;
        }
        return CORBA::Object::_nil();
    }
    
} // namespace ot

/* 
 * ------------------------------------------------------------
 *   End of CORBAUtils.h
 * ------------------------------------------------------------
 *   Automatic Emacs configuration follows.
 *   Local Variables:
 *   mode:c++
 *   c-basic-offset: 4
 *   eval: (c-set-offset 'substatement-open 0)
 *   eval: (c-set-offset 'case-label '+)
 *   eval: (c-set-offset 'statement 'c-lineup-runin-statements)
 *   eval: (setq indent-tabs-mode nil)
 *   End:
 * ------------------------------------------------------------ 
 */
