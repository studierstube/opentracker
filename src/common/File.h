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
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/common/File.h,v 1.5 2001/07/31 21:54:05 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _FILE_H
#define _FILE_H

#include "../OpenTracker.h"

#include <string>

#ifdef WIN32 
#include <iomanip>
#include <fstream>  // VisualC++ uses STL based IOStream lib
#else
#include <iomanip.h>
#include <fstream.h>
#endif

using namespace std;

/**
 *
 * @author Gerhard Reitmayr
 * @ingroup common
 */
class OPENTRACKER_API File
{
// Members

protected :
	/// Output stream
    ofstream * output;
	/// Input stream
    ifstream * input;
	/// the full filename
    string filename;

// Methods
public:
    /** constructor method, sets some default values*/
    File(const string filename_)
    {
        filename = filename_;
        input = NULL;
        output = NULL;
    }

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

    void open(int mode = 0 )
    {

		if( mode == 0 ) // output mode
		{
            output = new ofstream( filename.c_str());
			input = NULL;
		} 
		else {          // otherwise input mode
            input = new ifstream( filename.c_str());
			output = NULL;
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
            *output << station << " " << setprecision( 12 ) << state.time << " " 
                << setprecision(6) << state.position[0] << " " 
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
        return 0;
    }
  
};

#endif
