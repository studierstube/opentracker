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
/** header file for SpeechModule module.
  *
  * @author Reinhard Steiner
  *
  * $Id$
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page module_ref Module Reference
 * @section speechmodule SpeechModule
 * The SpeechModule provides and drives @ref speechrecosource nodes that generate
 * sr events. The appropriate speech sources are called when a command is
 * successfully recognised. The sr event is coded in the event translation
 * field, and the id´s could be resolved via the GetCommand method.
 * There is a TTS facility integrated into this module, so you could
 * quickly speak something via TTS and the system standard voice. 
 * if you want to use TTS with several options use the @ref SpeechVoiceModule
 * class.
 *
 * It uses several configuration elements, the root is @c SpeechRecoConfig.
 * There you can define several @c CommandSet, with several @c Command.
 * The @c SpeechRecoConfig element has the following attributes:
 *
 * @li @c language the language of the SR system.
 *
 * The @c CommandSet element has the following attributes:
 *
 * @li @c id the unique id of the command set
 * 
 * The @c Command element has the following attributes:
 *
 * @li @c id the unique id of the command (must be unique over the whole config, must be a number)
 * @li @c name the command string the sr system should try to recognize
 * @li @c weight the weight of the command string, default 1.0
 *
 * An example configuration element looks like this:
 * @verbatim
<SpeechRecoConfig language="english">
  <CommandSet id="CmdSet1">
    <Command id="1234" name="one"/>
    <Command id="1235" name="two"/>
    <Command id="1236" name="six"/>
    <Command id="1237" name="seven"/>
  </CommandSet>
</SpeechRecoConfig>@endverbatim
 * 
 */

#ifndef _SPEECHMODULE_H
#define _SPEECHMODULE_H

#include "../OpenTracker.h"

#include "SpeechDef.h"
#include "SpeechVoiceModule.h"

namespace ot {

typedef std::vector<Node*> NodeVector;

/**
 * The module and factory to drive the speech source nodes. It constructs
 * SpeechSource nodes via the NodeFactory interface and pushes events into
 * the tracker tree according to the nodes configuration.
 * @author Reinhard Steiner
 * @ingroup input
 */
class OPENTRACKER_API SpeechModule : public Module, public NodeFactory
{
// Members
protected:
    /// list of SpeechSource nodes in the tree
    NodeVector m_Nodes;

    /// next node id/rule name
    DWORD m_NextNodeId;
    
    /// Speech Core (SR) component
//#ifdef USE_SAPISPEECH
//    CSpeechCore *m_SpeechCore;
//#else
	SpeechCoreBase *m_SpeechCore;
//#endif

    /// Speech Voice (TTS) component
    SpeechVoiceModule *m_Voice;

    /// Configuration Tree for following SpeechSources
    ConfigNode *m_ConfigTree;

    /// vector of all nodes that should push a speech event a second time
    NodeVector m_Push2Nodes;


// Methods
public:
    /** constructor method. */
    SpeechModule() : Module(), NodeFactory()
    {
      m_NextNodeId = 1;
      m_SpeechCore = 0;
      m_Voice = 0;
    };
    
    /** Destructor method, clears nodes member. */
    virtual ~SpeechModule();


    /** This method is called to construct a new Node. It compares
     * name to the SpeechSource element name, and if it matches
     * creates a new SpeechSource node.
     * @param name reference to string containing element name
     * @attributes refenrence to StringMap containing attribute values
     * @return pointer to new Node or NULL. The new Node must be
     *         allocated with new ! */
    virtual Node* createNode(const std::string& name, StringTable& attributes);


    /**
     * initializes the tracker module. This class provides an implementation
     * that sets the initialization flag to true. Subclasses should call this
     * method if they override this method. It takes the attributes of the
     * element configuring this module and a local tree consisting of the
     * children of the element. This tree must be build of Nodes.
     * @param attributes StringTable of elements attribute values. Should be
     *        possibly , but is not for convenience.
     * @param localTree pointer to root of configuration nodes tree
     */
    virtual void init(StringTable& attributes, ConfigNode *localTree);

    /**
     * closes the module. A place for cleanup code etc.
     * This class provides an empty implementation for subclasses not doing
     * anything here. */
    virtual void close();

    /**
     * pushes state information into the tracker tree. It enables the module
     * to push new data into the tree by updating EventGenerator nodes and
     * thereby triggering an event.
     */
    virtual void pushState();


    /**
     * Speak a sentence with the default voice
     */
    void Speak(const char *p_Sentence, bool p_Async = true);

    /**
     * Get a command string from a command id
     * @return true if successfull, false in case of a failure
     */
    bool GetCommand(DWORD p_CommandId, DWORD p_SpeechSetId, std::string &p_Command);
};

} // namespace ot

#endif
