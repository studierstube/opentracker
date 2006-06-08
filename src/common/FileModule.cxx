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
/** source file for FileModule module.
 *
 * @author Gerhard Reitmayr
 *
 * $Id$
 * @file                                                                   */
/* ======================================================================= */

// this will remove the warning 4786
#include "../tool/disable4786.h"

#include <stdlib.h>
#include "FileModule.h"
#include "FileSink.h"
#include "FileSource.h"
#include <stdio.h>

#include <ace/Log_Msg.h>
#include "../tool/OT_ACE_Log.h"


#ifndef OT_NO_FILEMODULE_SUPPORT


namespace ot {

  // Destructor method, clears nodes member

  FileModule::~FileModule()
  {
    // clear containers
    nodes.clear();
    files.clear();
  }

  // tests for append attibte

  void FileModule::init(StringTable& attributes,  ConfigNode * localTree)
  {
    Module::init(  attributes, localTree );
    if( attributes.get("append").compare("true") == 0 )
      append = true;
    else
      append = false;
    if( attributes.get("loop").compare("true") == 0 )
      loop = true;
    else
      loop = false;
    if( attributes.containsKey("interval"))
      {
        attributes.get("interval", &interval );
        interval *= 1000.0;
        lastTime = OSUtils::currentTime();
      }
    else
      interval = -1;
    if ( attributes.get("ot11Format").compare("true") == 0 )
      ot11Format = true;
    else
      ot11Format = false;
  }

  // This method is called to construct a new Node

  Node * FileModule::createNode( const std::string& name, StringTable& attributes)
  {
    if( name.compare("FileSink") == 0 )
      {
        std::string id = attributes.get("file");
        int station;
        if( attributes.get("station", &station) != 1 )
	  station = 0;
        // search for File
        std::map<std::string,File *>::iterator it = files.find( id );
        File * file;
        if( it != files.end()) // found one or create a new one ?
	  {
            file = (*it).second;

	  } else // create a new one
	  {
            file = new File( id, ot::File::FILE_OUT, append, false, ot11Format );
            files[id] = file;
	  }
        if( file->mode == File::FILE_OUT ) // test for right direction and add to store
	  {
            NodeVector & vector = nodes[id];
            NodeVector::iterator nodeIt = vector.begin();
            FileSink * sink = new FileSink(*file, station );
            vector.push_back( sink );
	    LOG_ACE_ERROR("ot:Built FileSink node writing into %s with station %d station\n", id.c_str(), station);
            return sink;
	  }
	ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:FileSink referencing input file %d\n"), id.c_str()));
      }
    else if( name.compare("FileSource") == 0 )
      {
	std::string id = attributes.get("file");
        // file source needs an existing file, therefore we can use the directory stack !
        std::string fullname;
        if( context->findFile( id, fullname ))
	  {
            id = fullname;
	  }
        else
	  {
	    LOG_ACE_ERROR("ot:FileModule could not find file %s for FileSource!\n", id.c_str());
            return NULL;
	  }
        int station;
        if( attributes.get("station", &station ) == 0 )
	  station = 0;
        bool localTime = ( attributes.get("localtime").compare("true") == 0 );
	// search for File
        std::map<std::string,File *>::iterator it = files.find( id );
        File * file;
        if( it != files.end()) // found one
	  {
            file = (*it).second;

	  } else // create a new one
	  {
            file = new File( id, File::FILE_IN, false, loop, ot11Format );
            files[id] = file;
	  }
        if( file->mode == File::FILE_IN ) // test for right direction and add to store
	  {
            NodeVector & vector = nodes[id];
            NodeVector::iterator nodeIt = vector.begin();
            for( ; nodeIt != vector.end(); nodeIt++ )  // test for another node with the same station
	      if(((FileSource *)(*nodeIt))->station == station )
		break;
            if( nodeIt != vector.end())
	      {
		ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:Already another FileSource node for station %d\n"), station));
                return NULL;
	      }
            FileSource * source = new FileSource( station, localTime );
            vector.push_back( source );
	    ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:Built FileSource node reading from %s with station %d and localtime %s\n"), id.c_str(), station, localTime ? "'true'" : "'false'"));
            return source;
	  }
	ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:FileSource referencing output file %d\n"), id.c_str()));
      }
    return NULL;
  }

  // reads from the input files and fires new events, if necessary

  void FileModule::pushEvent()
  {
    int station;

    // store a time stamp to use for all localTime file sources
    double time = OSUtils::currentTime();

    if( interval != -1 && (time - lastTime) < interval )
      return;

    lastTime = time;

    // iterate over all files
    for( std::map<std::string, File*>::iterator it = files.begin(); it != files.end(); it++ )
      {
        if((*it).second->mode == File::FILE_IN && (*it).second->read( event, &station ))
	  {
            NodeVector & vector = nodes[(*it).first];
            NodeVector::iterator it2;

	    // reset all file sources
            for( it2 = vector.begin(); it2 != vector.end(); it2++ )
	      {
                ((FileSource*)(*it2))->changed = false;
	      }
	    // update observers of according file source:
	    for (it2 = vector.begin(); it2 != vector.end(); it2++)
	      {
		FileSource *fileSrc = (FileSource*)(*it2);
		if(fileSrc->station == station && fileSrc->changed == false)
		  {
		    fileSrc->changed = true;
		    if(fileSrc->localTime )
		      event.time = time;
		    fileSrc->updateObservers( event );
		  }
	      }
	  }
      }
  }

  // Closes the files and cleans up datastructures

  void FileModule::close()
  {
    for( std::map<std::string, File*>::iterator it = files.begin(); it != files.end(); it ++ )
      {

	File * file = (File*)(*it).second;
	delete file;
      }
    files.clear();
  }

} //namespace ot


#else
#pragma message(">>> OT_NO_FILEMODULE_SUPPORT")
#endif //OT_NO_FILEMODULE_SUPPORT
