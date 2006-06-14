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
/** header file for File support class for the FileModule.
 *
 * @author Gerhard Reitmayr
 *
 * $Id$
 * @file                                                                   */
/* ======================================================================= */

#ifndef _FILE_H
#define _FILE_H

#include "../OpenTracker.h"

#include <string>

#include <iomanip>
#include <fstream>

//using namespace std;

/**
 * File class is a simple class that provides formatted input or output to a file.
 *
 * If the File#ot11Format flag is not set, events are simply streamed into the file. Otherwise,
 * the obsolete file format of OpenTracker 1.1 is used, which just writes out the timestamp and
 * the former standard attributes (position, orientation, button, and confidence). This format
 * is provided for testing purposes only and should not be used in new modules or applications.
 * If changed code needs to be tested and compared to the 'Gold Standard' provided by OpenTracer
 * 1.1 however, use the #ot11Format flag for using the same input and straightforward comparison
 * of both versions' results.
 *
 * The OpenTracker 1.1 file format separates the former standard attributes by spaces:
 * @verbatim
 station timestamp position[0] position[1] position[2] orientation[0] ... orientation[3] button confidence
 ...@endverbatim
 * @author Gerhard Reitmayr, Jochen von Spiczak
 * @ingroup common
 */

namespace ot {

    class OPENTRACKER_API File
    {
        // Members
    protected :
	/// Output stream for output mode
	std::ofstream * output;
        /// Input stream for input mode
        std::ifstream * input;
        /// flag for looping
        bool loop;
        /// flag whether to use OT v1.1 compatible file format (for testing purposes only)
        bool ot11Format;

    public :
	/// the full filename
	const std::string filename;
        /// the mode i.e. input or output
        const enum modeFlags { FILE_OUT = 0, FILE_IN } mode;

        // Methods
    public:
        /** constructor method, sets some default values and opens the
         * file in the correct mode.
         * @param filename_ the filename of the file to open
         * @param mode the mode to open (either IN or OUT )
         * @param append if OUT mode clear file or append to it
         * @param loop_ if IN mode loop input file or not
         * @param ot11Format_ if OT v1.1 compatible output format should be used (for testing purposes only)
         */
        File(const std::string filename_ , modeFlags mode_ = FILE_OUT, bool append = false, bool loop_ = false, bool ot11Format_ = false ) :
            loop( loop_ ), ot11Format( ot11Format_ ), filename( filename_), mode( mode_ )
	{
            if( mode == FILE_OUT ) // output mode
	    {
                if( append )
                    output = new std::ofstream( filename.c_str(), std::ios_base::out | std::ios_base::app );
                else
                    output = new std::ofstream( filename.c_str(), std::ios_base::out | std::ios_base::trunc );
                input = NULL;
	    }
            else {          // otherwise input mode
                input = new std::ifstream( filename.c_str());
                input->setf( std::ios::skipws );
                output = NULL;
            }
	}

        /** destructor, closes the streams and deletes them again
         */
        ~File()
	{
            if( input != NULL )
	    {
                input->close();
                delete input;
	    }
            if( output != NULL )
	    {
                output->close();
                delete output;
	    }
	}

        /** writes a event to the output stream, only useable
         * if File object was opened in output mode. Here you
         * can do any special formatting etc.
         * @param event reference to the output event
         * @param station number of the statione
         */
        void write( Event & event, int station )
	{
            if( output != NULL)
	    {
                if (!ot11Format)
                    *output << station << " " << event << std::endl;
                else
		{
                    char str[220];
                    sprintf(str, "%d %.15f %.15f %.15f %.15f %.15f %.15f %.15f %.15f %d %.15f\n",
                            station,
                            event.time,
                            event.getPosition()[0],
                            event.getPosition()[1],
                            event.getPosition()[2],
                            event.getOrientation()[0],
                            event.getOrientation()[1],
                            event.getOrientation()[2],
                            event.getOrientation()[3],
                            event.getButton(),
                            event.getConfidence());
                    *output << str << std::flush;
		}
	    }
	}

        /** tries to read in the next station data, stored in
         * the input file, only useable if File object was opened
         * in input mode. Here you should be able to read in
         * the right format :) ( obviously ).
         * @param event reference where to put the new event
         * @param station pointer to an int containing the station
         * @returns true if a new station could be read, otherwise false.
         */
        bool read( Event & event, int * station )
	{
            if( !input->is_open())
                return false;

            if (!ot11Format)
	    {
                input->clear();
                *input >> *station;
                *input >> event;
	    }
            else
	    {
                input->clear();
                *input >> *station;
                *input >> event.time;
                *input >> event.getPosition()[0] >> event.getPosition()[1] >> event.getPosition()[2];
                *input >> event.getOrientation()[0] >> event.getOrientation()[1] >> event.getOrientation()[2] >> event.getOrientation()[3];
                *input >> event.getButton();
                *input >> event.getConfidence();
	    }

            bool failed = input->fail();
            if( failed )
	    {
                if ( loop )
		{
                    input->clear();
                    input->seekg(0, std::ios::beg);
                    failed = false;
		}
                else
                    input->close();
	    }
            return !failed;
	}
    };

} // namespace ot

#endif


/* 
 * ------------------------------------------------------------
 *   End of File.h
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
