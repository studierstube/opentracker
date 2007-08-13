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
/** source file for EEGFilter node.
 *
 * @author Alexander Bornik
 *
 * $Id $
 * @file                                                                   */
/* ======================================================================= */

// this will remove the warning 4786
#include <OpenTracker/tool/disable4786.h>

#include <map>

#include <OpenTracker/common/EEGFilterNode.h>

namespace ot {

    // constructor method
    EEGFilterNode::EEGFilterNode( const std::vector<double> &ifreqs, 
                                  const unsigned int icodelength,
                                  const double &isamplerate,
                                  const unsigned int iharmonics,
                                  const double &iprepause,
                                  const double &ipostpause,
                                  const double &iinterpause,
                                  const double &icodeduration,
                                  const double &ithreshold,
                                  const double &ihitpercentage,
                                  const std::string &iinatt,
                                  const std::string &ioutatt,
                                  bool iconsume)
        : frequencies(ifreqs),
          codelength(icodelength),
          samplerate(isamplerate),
          harmonics(iharmonics),
          prepause(iprepause),
          postpause(ipostpause),
          interpause(iinterpause),
          codeduration(icodeduration),
          threshold(ithreshold),
          hitpercentage(ihitpercentage),
          inatt(iinatt),
          outatt(ioutatt),
          consume(iconsume)
    {
        using namespace std;
        eegsubs.clear();
        std::vector<double>::const_iterator it;

        for (it = frequencies.begin(); it != frequencies.end(); it++)
        {
            eegsubs.push_back(make_pair(EEGSub(*it, harmonics, samplerate), 1.0));
        }

        evbuffertargetsize = 
            static_cast<unsigned int>((prepause + 
                                       codelength*(codeduration+interpause) 
                                       - interpause + postpause) / samplerate);
        evbuffercodestart = 
            static_cast<unsigned int>(prepause / samplerate);
        evbufferpoststart = 
            static_cast<unsigned int>(
                                (prepause + 
                                 codelength*(codeduration+interpause) 
                                 - interpause) / samplerate);
        
    } 

    // Applies the filter to incoming events
    void EEGFilterNode::onEventGenerated( Event& event, Node & generator )
    {
        
        // The current event we're working on
        currentEvent = event;
        
        // The event we will be passing
        targetEvent = event;

        int eegoutval = -1;

        // do we need to process the event?
        if (currentEvent.hasAttribute(inatt) && 
            currentEvent.getAttributeTypeName(inatt) == "double")
        {
            std::vector<std::pair<EEGSub, double> >::iterator it;
            double actval;
            double maxval = 0.0;
            int i;
            int maxi = -1;
        
            // new value into filter and calculate maximum response frequency
            // index

            for (it=eegsubs.begin(),i=0; it != eegsubs.end(); it++, i++)
            {
                actval = it->first.filter(currentEvent.getAttribute(inatt,(double)0));
                actval *= it->second;

                if (actval > threshold && actval > maxval)
                {
                    maxval = actval;
                    maxi = i;
                }
            }

            // update event buffer

            if (evbuffer.size() < evbuffertargetsize)
            {
                evbuffer.push_front(maxi);                
            }
            else
            {
                evbuffer.pop_back();
                evbuffer.push_front(maxi);

                // calculate whether the code could be successfully recognized
                // first analyze event buffer and setup voting structure
                unsigned int precount = 0;
                unsigned int postcount = 0;
                std::map<int, int> digitmap[codelength];
                unsigned int i;
                for (i=0; i<evbuffertargetsize; i++)
                {
                    if (i<evbuffercodestart)
                    {
                        if (evbuffer[i] == -1) ++precount;
                    }
                    else if (i<evbufferpoststart)
                    {
                        digitmap[(i-evbuffercodestart)/((evbufferpoststart-evbuffercodestart)/codelength)][evbuffer[i]]++;
                    }
                    else
                    {
                        if (evbuffer[i] == -1) ++postcount;
                    }
                        
                }
                
                ///evaluate voting results
                int tempoutval = 0;
                for (i=0; i<codelength;  i++)
                {
                    int totalsamples =0;
                    std::map<int, int>::const_iterator it;
                    int actval,actcount;
                    int maxval=-1;
                    int maxcount=0;

                    /// find the dominant frequency for a code sequence 
                    /// part
                    for (it=digitmap[i].begin(); it != digitmap[i].end(); it++)
                    {
                        actval = it->first;
                        actcount = it->second;
                        totalsamples += actcount;
                        if (actcount>maxcount)
                        {
                            maxcount = actcount;
                            maxval = actval;
                        }
                    }       
                    /// only, if the predominant vote is for one of 
                    /// the target frequencies, we continue
                    if (maxval != -1)
                    {
                        /// it has to meet the hit percentage requirement also
                        if (maxcount >= ((evbufferpoststart-evbuffercodestart)/codelength)*hitpercentage/100.0)
                        {
                            /// calculate output value in a 
                            /// frequencies.size() - ary number system
                            tempoutval *= frequencies.size();
                            tempoutval += maxval;
                        }
                        else
                        {
                            /// don't continue if sequence part does
                            /// not meet hit percentage
                            break;
                        }
                    }
                    else
                    {
                        /// if one code sequence part is not responding to 
                        /// a certain frequency we may skip the other parts
                        /// as well
                        break;
                    }
                    
                }
            }

            // remove source event if requested

            if (consume) targetEvent.delAttribute(inatt);
        }

        // add/set output value to event

        targetEvent.setAttribute<int>(outatt, eegoutval);

        // Update the observers
        updateObservers( targetEvent );
    }

    void EEGFilterNode::pushEvent()
    {
        // nothing to do
    }

    void EEGFilterNode::pullEvent()
    {
        // nothing to do
    }
} // namespace ot


/* 
 * ------------------------------------------------------------
 *   End of EEGFilterNode.cxx
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
