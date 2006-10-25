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
/** header file for FOBModule module.
 *
 * @author Thomas Peterseil, Gerhard Reitmayr
 *
 * $Id$
 * @file                                                                   */
/* ======================================================================= */

/**
 * @page module_ref Module Reference
 * @section fobmodule FOBModule (Flock of Birds)
 * 
 * The FOBModule is a device driver for the Ascension Flock of Birds tracker. It
 * supports single as well as multi port configurations and the extended range
 * transmitter. It provides the FOBSource source nodes in the data flow graph. 
 * It uses a configuration element called @c FOBConfig which in turn contains one
 * element called @c Bird for each bird in the setup. Only configured birds will
 * be used. 
 * The @c FOBConfig element has the following attributes :
 *
 * @li @c mode ( multi | single ) defines whether the flock is used in single mode
 *         (one serial comm port to the master bird), or multi mode (a serial port
 *         to each bird).
 * @li @c master the number of the master bird
 * @li @c transmitter if the transmitter is attached to another bird than the master,
 *         set the bird number here (i.e. for an ERT)
 * @li @c hemisphere ( forward | rear | upper | lower | left | right ) defines which
 *         hemisphere to set on the flock.
 * @li @c referenceframe a transformation of the position data
 * @li @c xyzframe ( true | false ) a flag defining whether position data should be 
 *        transformed by the referenceframe set. default is @c true.
 * @li @c scale ( 36 | 72 ) set the extend of the receivers
 * 
 * The @c Bird element is used to set the birds used in the setup. If the single
 * mode is used, only the master bird needs a device configured. Otherwise all
 * birds need their serial devices set.
 * The @c Bird element has the following attributes :
 * 
 * @li @c number the number of the bird
 * @li @c dev the device name of the bird (i.e. COM1, /dev/ttyS0)
 * @li @c anglealign  a transformation of the rotatin data to compensate for any 
 *        offset of the sensor itself. 
 *
 * An example configuration element looks like this :
 * @verbatim
 <FOBConfig mode="multi" master="1" scale="72">
 <Bird number="1" dev="COM1"/>
 <Bird number="2" dev="COM2"/>
 </FOBConfig>@endverbatim
 *
 */

#ifndef _FOBMODULE_H
#define _FOBMODULE_H

#include "../OpenTracker.h"

#include <map>


#ifndef OT_NO_FOB_SUPPORT


namespace ot {

    class Bird;
    /**
     * developer level information and implementation specifics here
     *
     * @author 
     * @ingroup input
     */
    class OPENTRACKER_API FOBModule : public ThreadModule, public NodeFactory
    {
        // Members
    protected:

        /// the hemisphere the tracker should operate in
        enum Hemisphere { FORWARD, REAR, UPPER, LOWER, LEFT, RIGHT } hemisphere;

        /** the serial line mode used, either multi (each bird connected via a serial) or single
         * (all birds via a single serial port). */ 
        enum Mode { MULTI, SINGLE } mode;

        /// number of the master bird
        int master;
    
        /// number of the erc transmitter, if present
        int transmitter;
    
        /// scale factor 
        float scale;

        /// reference frame values
        float referenceframe[3];

        /// xyzframe flag
        bool useXYZFrame;
    
        /// associative array of Bird data structures
        std::map<int, Bird *> birds;

        /// flag to stop the thread
        bool stop;

        // Methods
    protected:
        /** this method is executed in its own thread and reads data from the flock. */
        virtual void run(); 

    public:
        /** constructor method. */
        FOBModule();

        /** Destructor method, clears nodes member. */
        virtual ~FOBModule();
        /**
         * initializes the tracker module. For each configured tracker it
         * allocates a Bird structure and stores the configuration information.
         * @param attributes StringTable of elements attribute values. Should be
         *        possibly , but is not for convenience.
         * @param localTree pointer to root of configuration nodes tree
         */
        virtual void init(StringTable& attributes,  ConfigNode * localTree);
        /** This method is called to construct a new Node. It compares
         * name to the FOBSource element name, and if it matches
         * creates a new FOBSource node.
         * @param name reference to string containing element name
         * @attributes refenrence to StringTable containing attribute values
         * @return pointer to new Node or NULL. The new Node must be
         *         allocated with new ! */
        virtual Node * createNode( const std::string& name,  StringTable& attributes);    
    
        /** this method initializes the flock and starts the processing thread. */
        virtual void start();
	/**
         * closes the flock again */
        virtual void close();
        /**
         * pushes events into the tracker tree. Checks all trackers for new data
         * and fires the FOBSources, if new data is present.
         */
        virtual void pushEvent();

    private :
        /** inits the whole flock according to the parameters stored
         * in the module. It can be used to reset the flock after
         * some communication failure.
         * @return 0 if everything is, otherwise the FOB error code of the operation
         *  that went wrong. */
        int initFoB();

        /** resets all birds by toggling the modem lines. Does the right thing 
         * depending on the mode. 
         * @return the FOB error code */
        int resetBirds();

        /** sets the report mode of all birds to POSITION/QUATERNION.
         * @return the FOB error code */
        int setReportMode();

        /** issues the Next Transmitter command to the master bird, if another
         * transmitter is set. This is used to activate an ERC transmitter as well.
         * @return the FOB error code */
        int setNextTransmitter();

        /** starts the stream mode to receive data from the birds.
         * @return the FOB error code */
        int startStreamMode();

        void setHemisphere();

        void setScale();

        void setAngleAlign();

        void setReferenceFrame();
    
        void setXYZFrame();

        friend class Bird;
    };

	OT_MODULE(FOBModule);

} // namespace ot


#endif // OT_NO_FOB_SUPPORT


#endif

/* 
 * ------------------------------------------------------------
 *   End of Transformation.cxx
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
