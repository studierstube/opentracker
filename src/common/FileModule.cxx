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
/** source file for FileModule module.
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/common/FileModule.cxx,v 1.3 2001/07/12 16:56:01 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

#include "FileModule.h"
#include "FileSink.h"
//#include "FileSource.h"

#ifdef WIN32
#include <iostream>
#endif

FileModule::FileModule()
{
//...
}
 
FileModule::~FileModule()
{
    // clear containers
    sinks.clear();
    sources.clear();
	files.clear();
}

void FileModule::init(StringTable& attributes, ConfigNode * localTree)
{
    if( localTree != NULL )
    {
        ConfigNode * base = localTree;
        for( unsigned int i = 0; i < base->countChildren(); i++ )
        {
            ConfigNode * config = (ConfigNode *)base->getChild( i );
            if( config->getName().compare("File") == 0 )
            {
                string file = config->getAttributes().get("file");
                string id = config->getAttributes().get("id");
				files[id] = new File(file);
            }
        }
    }
    Module::init( attributes, localTree );
}


Node * FileModule::createNode( string& name,  StringTable& attributes)
{
    if( name.compare("FileSink") == 0 )
    {
        string id;
        id = attributes.get("id");
        int station;
        if( sscanf( attributes.get("station").c_str()," %i", &station ) == 0 ) 
            station = 0;                    
        // search for File otherwise return NULL and print out an error
        map<string,File *>::iterator file = files.find( id );
        if( file != files.end())
        {
            FileSink * sink = new FileSink(*(*file).second, station );
            sinks.push_back( sink );
            cout << "Built FileSink node writting into " << id << " with station " 
                 << station << endl;       
            return sink;
        }
        cout << "FileSink referencing non-existing file " << id << endl;
    } else if( name.compare("FileSource") == 0 )
    {
		//...
    }
    return NULL;
}

void FileModule::pushState()
{
	//...
}

void FileModule::pullState()
{
    for( NodeVector::iterator it = sinks.begin(); it != sinks.end(); it ++ )
	{
		FileSink * sink = (FileSink*)(*it);
	    if( sink->changed == 1 ) 
        {
			sink->file.write( sink->state, sink->station );
		    sink->changed = 0;
        }
	}
}

void FileModule::start()
{
    for( map<string, File*>::iterator it = files.begin(); it != files.end(); it ++ )
	{
        
		File * file = (File*)(*it).second;
		file->open( 0 );
	}
}

void FileModule::close()
{
    for( map<string, File*>::iterator it = files.begin(); it != files.end(); it ++ )
	{
        
		File * file = (File*)(*it).second;
		delete file;
	}
    files.clear();
}
