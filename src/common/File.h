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
/** header file for File support class for the FileModule.
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/common/File.h,v 1.12 2003/04/15 17:16:18 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _FILE_H
#define _FILE_H

#include "../OpenTracker.h"

#include <string>

#include <iomanip>
#include <fstream>

using namespace std;

/**
 * File class is a simple class that provides formatted input or output to a file.
 * It defines in which format the station data is written to the file and read
 * again. Right now the data has a csv format where spaces are used as separators :
 * @verbatim 
station timestamp position[0] position[1] position[2] orientation[0] ... orientation[3]
...@endverbatim
 * @author Gerhard Reitmayr
 * @ingroup common
 */
class OPENTRACKER_API File
{
// Members
protected :
	/// Output stream for output mode
    ofstream * output;
	/// Input stream for input mode
    ifstream * input;
    
public :    
	/// the full filename
    const string filename;
    /// the mode i.e. input or output
    const enum modeFlags { OUT = 0, IN } mode;

// Methods
public:
    /** constructor method, sets some default values and opens the
     * file in the correct mode.
     * @param filename_ the filename of the file to open
     * @param mode the mode to open (either IN or OUT )*/
    File(const string filename_ , modeFlags mode_ = OUT, bool append = false ) :
        filename( filename_), mode( mode_ )
    {        
        if( mode == OUT ) // output mode
		{
            if( append )
                output = new ofstream( filename.c_str(), ios_base::out | ios_base::app );
            else
                output = new ofstream( filename.c_str(), ios_base::out | ios_base::trunc );
			input = NULL;
		} 
		else {          // otherwise input mode
            input = new ifstream( filename.c_str());
            input->setf( ios::skipws );
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

    /** writes a state to the output stream, only useable
     * if File object was opened in output mode. Here you
     * can do any special formatting etc. 
     * @param state reference to the output state
     * @param station number of the station
     */
    void write( State & state, int station )
    {
        if( output != NULL )
        {
            *output << station << " " << setw(15) << setfill(' ') << setprecision( 15 );
            *output << state.time << " " << setprecision(8);
            *output << state.position[0] << " " 
                << state.position[1] << " " 
                << state.position[2] << " "
                << state.orientation[0] << " " 
                << state.orientation[1] << " "
                << state.orientation[2] << " "
                << state.orientation[3] << endl;
        }
    }

    /** tries to read in the next station data, stored in
     * the input file, only useable if File object was opened
     * in input mode. Here you should be able to read in 
     * the right format :) ( obviously ).
     * @param state reference where to put the new state
     * @param station pointer to an int containing the station
     * @returns 1 if a new station could be read, otherwise 0.
     */
    int read( State & state, int * station )    
    {
        input->clear();
        *input >> *station;
        *input >> state.time;
        *input >> state.position[0] >> state.position[1] >> state.position[2];
        *input >> state.orientation[0] >> state.orientation[1] 
               >> state.orientation[2] >> state.orientation[3];
        return !input->fail();
    }  
};

#endif
