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
/** The source file for the multi-modal Event class.
 *
 * @author Jochen von Spiczak
 *
 * $Id: Node.h 1011 2006-04-20 21:47:53Z spiczak $
 * @file                                                                   */
/* ======================================================================= */

#include <string>
#include <sstream>
#include <vector>

#include "Event.h"
#include "Translator.h"
#include "../tool/OT_ACE_Log.h"

namespace ot
{

    // typesafe null reference
    Event Event::null;

    // standard constructor
    Event::Event()
    {
        timeStamp();
    }

    // copy constructor, copying all attributes
    Event::Event(const Event &rv)
    {
        *this = rv;
    }

    // destructor, deleting all attributes
    Event::~Event()
    {
        clearAttributes();
    }

    // assignment operator, copying all attributes
    Event& Event::operator=(const Event& rv) //throw (std::runtime_error)
    {
        if( this == &rv ) return *this;

        time = rv.time;
        clearAttributes();
        const AttributeMap &rvMap = rv.attributes;
        for (AttributeMap::const_iterator it = rvMap.begin(); it != rvMap.end(); ++it)
        {
            std::string name = (*it).first;
            EventAttributeBase *rvAtt = (*it).second;
            EventAttributeBase *myAtt = EventAttributeBase::create(rvAtt->getGenericTypeName());
            *myAtt = *rvAtt;
            attributes[name] = myAtt;
        }
        return *this;
    }

    // copy all but standard data fields (used for transformation nodes)
    void Event::copyAllButStdAttr(const Event &rv)
    {
        const AttributeMap &rvMap = rv.attributes;
        for (AttributeMap::const_iterator it = rvMap.begin(); it != rvMap.end(); ++it)
        {
            std::string name = (*it).first;
            if (name != "position" &&
                name != "orientation" &&
                name != "confidence" &&
                name != "button")
            {
                EventAttributeBase *rvAtt = (*it).second;
                EventAttributeBase *newAtt = EventAttributeBase::create(rvAtt->getGenericTypeName());
                *newAtt = *rvAtt;
                delAttribute(name);
                attributes[name] = newAtt;
            }
        }
    }

    // get name of EventAttributeBase by index
    const std::string& Event::getAttributeName(const int index) const throw (std::invalid_argument)
    {
        int i = 0;
        for (AttributeMap::const_iterator it = attributes.begin(); it != attributes.end(); ++it)
        {
            if (i == index)
                return (*it).first;
            i++;
        }
        std::stringstream ss;
        ss << "event has no attribute with index '" << index << "', check size using Event::size() before access by index!";
        throw std::invalid_argument(ss.str());
    }

    // get index of EventAttributeBase by name
    int Event::getAttributeIndex(const std::string &name) const throw (std::invalid_argument)
    {
        int index = 0;
        for (AttributeMap::const_iterator it = attributes.begin(); it != attributes.end(); ++it)
        {
            if ((*it).first == name)
                return index;
            index++;
        }
        std::string errorStr = "event does not have attribute called '" + name + "'";
        throw std::invalid_argument(errorStr);
    }

    // get attribute type
    const std::type_info& Event::getAttributeType(const std::string &name) const throw (std::invalid_argument)
    {
        if (hasAttribute(name))
        {
            EventAttributeBase *att = (*attributes.find(name)).second;
            return att->getType();
        }
        std::string errorStr = "event does not have attribute called '" + name + "'";
        throw std::invalid_argument(errorStr);
    }

    // get generic attribute type name
    const std::string& Event::getAttributeTypeName(const std::string &name) const throw (std::invalid_argument)
    {
        if (hasAttribute(name))
        {
            EventAttributeBase *att = (*attributes.find(name)).second;
            return att->getGenericTypeName();
        }
        std::string errorStr = "event does not have attribute called '" + name + "'";
        throw std::invalid_argument(errorStr);
    }

    // get attribute value encoded in a string
    const std::string Event::getAttributeValueString(const std::string &name) const throw (std::invalid_argument)
    {
        if (hasAttribute(name))
        {
            std::stringstream ss;
            EventAttributeBase *att = (*attributes.find(name)).second;
            ss << *att;
            return ss.str();
        }
        std::string errorStr = "event does not have attribute called '" + name + "'";
        throw std::invalid_argument(errorStr);
    }

    // get position
    std::vector<float>& Event::getPosition()
    {
        return getAttribute("position", std::vector<float>(3, .0f));
    };

    // get orientation
    std::vector<float>& Event::getOrientation()
    {
        std::vector<float> defVec = std::vector<float>(3, .0f);
        defVec.push_back(1.f);
        return getAttribute("orientation", defVec);
    }

    // get confidence
    float& Event::getConfidence()
    {
        return getAttribute("confidence", 1.0f);
    }

    // get button
    unsigned short& Event::getButton()
    {
        return getAttribute(std::string("button"), (unsigned short)0);
    }

    // serialize the event
    void Event::serialize(std::ostream &out) const
    {
        out << "{" << std::fixed << time << "-" << attributes.size() << ":";
        for (AttributeMap::const_iterator it = attributes.begin(); it != attributes.end(); ++it)
        {
            if (it != attributes.begin())
                out << ",";
            out << (*it).second->getGenericTypeName() << "." << (*it).first << "=" << *((*it).second);
        }
        out << "}";
    }

    const std::string Event::serialize() const
    {
        std::stringstream ss;
        ss << *this;
        return ss.str();
    }

    void Event::deserialize(std::string &str)
    {
        std::stringstream ss(str);
        ss >> *this;
    }

    // deserialize the event
    std::istream& Event::deserialize(std::istream &in)
    {
        char c;
        std::string dataStr, typeStr, nameStr, attrStr;
        std::string::size_type typeDelimiter, nameDelimiter, attrDelimiter;
        AttributeMap::size_type i, mapSize;

        clearAttributes();

        // read "{", time , "-", size, ":" and data string
        if (!(in >> c) || c != '{'
            || !(in >> time)
            || !(in >> c) || c != '-'
            || !(in >> mapSize)
            || !(in >> c) || c != ':'
            || !(in >> dataStr))
        {
            in.setstate(std::ios_base::failbit);
            return in;
        }

        // read data
        for (i = 0; i < mapSize; i++)
        {
            // segmentation of data string
            typeDelimiter = dataStr.find(".");
            nameDelimiter = dataStr.find("=");
            attrDelimiter = dataStr.find_first_of(",}");

            typeStr = dataStr.substr(0, typeDelimiter);
            nameStr = dataStr.substr(typeDelimiter + 1, nameDelimiter - (typeDelimiter + 1));
            attrStr = dataStr.substr(nameDelimiter + 1, attrDelimiter - (nameDelimiter + 1));
            dataStr = dataStr.substr(attrDelimiter + 1, std::string::npos);

            // check if segmentation is correct
            if (typeDelimiter == std::string::npos
                || nameDelimiter == std::string::npos
                || attrDelimiter == std::string::npos
                || typeDelimiter >= nameDelimiter
                || typeDelimiter >= attrDelimiter
                || hasAttribute(nameStr))
            {
                in.setstate(std::ios_base::failbit);
                return in;
            }
            try
            {
                EventAttributeBase *att = EventAttributeBase::create(typeStr);
                std::stringstream ss(attrStr);
                if (ss >> *att)
                    attributes[nameStr] = att;
                else
                {
                    in.setstate(std::ios_base::failbit);
                    return in;
                }
            }
            catch (std::runtime_error)
            {
                in.setstate(std::ios_base::failbit);
                return in;
            }
        }
        return in;
    }

    // add attribute using strings encoding type and value
    bool Event::addAttribute(const std::string &type, const std::string &name, const std::string &value) //throw (std::runtime_error)
    {
        if (hasAttribute(name))
            return false;

        EventAttributeBase *att = EventAttributeBase::create(type); // may throw std::runtime_error
        std::stringstream ss(value);
        if (ss >> *att)
        {
            attributes[name] = att;
            return true;
        }
        else
        {
            delete att;
            return false;
        }
    }

    // set attribute using a string encoding the value
    bool Event::setAttribute(const std::string &type, const std::string &name, const std::string &value) //throw (std::runtime_error)
    {
        if (!hasAttribute(name))
            attributes[name] = EventAttributeBase::create(type); // may throw std::runtime_error
        EventAttributeBase *att = attributes[name];
        std::stringstream ss(value);
        if (ss >> *att)
            return true;
        return false;
    }

    // delete attribute by name
    bool Event::delAttribute(const std::string &name)
    {
        AttributeMap::iterator it = attributes.find(name);
        if (it == attributes.end())
            return false;
        else
        {
            delete (*it).second;
            (*it).second = NULL;
            attributes.erase(it);
            return true;
        }
    }

    // delete all attributes
    void Event::clearAttributes()
    {
        for (AttributeMap::iterator it = attributes.begin(); it != attributes.end(); ++it)
            delete (*it).second;
        attributes.clear();
    }

    // print out all attributes
    void Event::printout() const
    {
	LOG_ACE_INFO(getPrintOut().c_str());
	LOG_ACE_INFO("\n");
        //std::cout << getPrintOut() << std::endl;
    }

    // get string for print out
    const std::string Event::getPrintOut() const
    {
        std::string printOut;
        std::stringstream ss;

        ss << "  timestamp: " << std::fixed << time << std::endl;

        for (AttributeMap::const_iterator it = attributes.begin(); it != attributes.end(); ++it)
            ss << "  " << (*it).first << " (" << (*it).second->getGenericTypeName() << "): " << *((*it).second) << std::endl;
        return ss.str();
    }

    // rename attribute
    bool Event::renAttribute(const std::string &oldName, const std::string &newName)
    {
        AttributeMap::iterator oldIt = attributes.find(oldName);
        AttributeMap::iterator newIt = attributes.find(newName);

        if (newIt != attributes.end() || oldIt == attributes.end())
            return false;
        else
        {
            attributes[newName] = (*oldIt).second;
            attributes.erase(oldIt);
            return true;
        }
    }

    // check for attribute
    bool Event::hasAttribute(const std::string &name) const
    {
        return (attributes.find(name) != attributes.end());
    }

    // check whether type name was registered
    bool Event::knowsType(const std::string typeName)
    {
        return EventAttributeBase::translator.knowsType(typeName);
    }

    // register all known types
    void Event::registerAllKnownTypes()
    {
        registerGenericTypeName((bool*)NULL, "bool");
        registerGenericTypeName((char*)NULL, "char");
        registerGenericTypeName((signed char*)NULL, "signed_char");
        registerGenericTypeName((unsigned char*)NULL, "unsigned_char");
        registerGenericTypeName((int*)NULL, "int");
        registerGenericTypeName((long*)NULL, "long");
        registerGenericTypeName((short*)NULL, "short");
        registerGenericTypeName((unsigned int*)NULL, "unsigned_int");
        registerGenericTypeName((unsigned long*)NULL, "unsigned_long");
        registerGenericTypeName((unsigned short*)NULL, "unsigned_short");
        registerGenericTypeName((double*)NULL, "double");
        registerGenericTypeName((long double*)NULL, "long_double");
        registerGenericTypeName((float*)NULL, "float");
        registerGenericTypeName((std::string*)NULL, "string");
        registerGenericTypeName((std::vector<float>*)NULL, "vector<float>");
    }

    // timestamp the event to current time
    void Event::timeStamp()
    {
        time = OSUtils::currentTime();
    }

    // stream the event to an input stream
    std::istream& operator>>(std::istream &in, ot::Event &event)
    {
        event.deserialize(in);
        return in;
    };

    // stream the event to an output stream
    std::ostream& operator<<(std::ostream &out, const ot::Event &event)
    {
        event.serialize(out);
        return out;
    };

} // namespace ot

/* 
 * ------------------------------------------------------------
 *   End of Event.cxx
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
