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
 * It defines in which format the station data is written to the file and read
 * again. Right now the data has a csv format where spaces are used as separators :
 * @verbatim
 station timestamp position[0] position[1] position[2] orientation[0] ... orientation[3] button confidence
 ...@endverbatim
 * @author Gerhard Reitmayr
 * @ingroup common
 */

namespace ot
{
    class OPENTRACKER_API File
    {
        // Members
    protected :
	/// Output stream for output mode
        //ofstream * output;
	/// Input stream for input mode
        //ifstream * input;

	// General io file
	FILE* fp;

        /// flag for looping
        bool loop;

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
         */
        File(const std::string filename_ , modeFlags mode_ = FILE_OUT, bool append = false, bool loop_ = false ) :
            loop( loop_ ), filename( filename_), mode( mode_ )
        {
            if( mode == FILE_OUT ) // output mode
		{
		    if( append )
			//output = new ofstream( filename.c_str(), ios_base::out | ios_base::app );
			fp = fopen(filename.c_str(), "a");
		    else
			//output = new ofstream( filename.c_str(), ios_base::out | ios_base::trunc );
			fp = fopen(filename.c_str(), "w");
		    //input = NULL;
		}
            else {          // otherwise input mode
                //input = new ifstream( filename.c_str());
                //input->setf( ios::skipws );
                //output = NULL;
                fp = fopen(filename.c_str(), "r");
            }
        }

        /** destructor, closes the streams and deletes them again
         */
        ~File()
        {
            /*if( input != NULL )
              {
              input->close();
              delete input;
              }
              if( output != NULL )
              {
              output->close();
              delete output;
              }*/
            if(fp)
                fclose(fp);
        }

        /** writes a state to the output stream, only useable
         * if File object was opened in output mode. Here you
         * can do any special formatting etc.
         * @param state reference to the output state
         * @param station number of the station
         */
        void write( State & state, int station )
        {
            /*if( output != NULL )
             *output << station << " " << setw(15) << setfill(' ') << setprecision( 15 );
             *output << state.time << " ";
             *output << state.position[0] << " "
             << state.position[1] << " "
             << state.position[2] << " "
             << state.orientation[0] << " "
             << state.orientation[1] << " "
             << state.orientation[2] << " "
             << state.orientation[3] << " "
             << state.button << " "
             << state.confidence << endl;
             }*/

            if(fp)
		{
		    fprintf(fp, "%d %.15f %.15f %.15f %.15f %.15f %.15f %.15f %.15f %d %.15f\n",
			    station,
			    state.time,
			    state.position[0],
			    state.position[1],
			    state.position[2],
			    state.orientation[0],
			    state.orientation[1],
			    state.orientation[2],
			    state.orientation[3],
			    state.button,
			    state.confidence);
		    fflush(fp);
		}
        }

        /** tries to read in the next station data, stored in
         * the input file, only useable if File object was opened
         * in input mode. Here you should be able to read in
         * the right format :) ( obviously ).
         * @param state reference where to put the new state
         * @param station pointer to an int containing the station
         * @returns true if a new station could be read, otherwise false.
         */
        bool read( State & state, int * station )
        {
            /*if( !input->is_open())
              return false;

              input->clear();
              *input >> *station;
              *input >> state.time;
              *input >> state.position[0] >> state.position[1] >> state.position[2];
              *input >> state.orientation[0] >> state.orientation[1]
              >> state.orientation[2] >> state.orientation[3];
              *input >> state.button;
              *input >> state.confidence;

              bool result = input->fail();
              if( result )
              {
              if ( loop )
              {
              input->clear();
              input->seekg(0, ios::beg);
              result = false;
              }
              else
              input->close();
              }
              return !result;*/

            if(!fp)
                return false;

	    if( feof(fp) && loop)
		fseek(fp, 0, SEEK_SET);

	    if ( feof(fp) )
		{
		    fclose(fp);
		    return false;
		}

            fscanf(fp, "%d %lf %f %f %f %f %f %f %f %hu %f\n",
                   station,
                   &state.time,
                   &state.position[0],
                   &state.position[1],
                   &state.position[2],
                   &state.orientation[0],
                   &state.orientation[1],
                   &state.orientation[2],
                   &state.orientation[3],
                   &state.button,
                   &state.confidence);

	    return (!ferror(fp));
        }
    };

} // namespace ot

#endif
