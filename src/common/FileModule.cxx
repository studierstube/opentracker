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
  * $Id$
  * @file                                                                   */
 /* ======================================================================= */

#include "FileModule.h"
#include "FileSink.h"
#include "FileSource.h"
#include <stdio.h>

#include <iostream>

using namespace std;

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
}

// This method is called to construct a new Node

Node * FileModule::createNode( const string& name, StringTable& attributes)
{
    if( name.compare("FileSink") == 0 )
    {
        string id = attributes.get("file");
        int station;
        if( attributes.get("station", &station) != 1 ) 
            station = 0;                    
        // search for File
        map<string,File *>::iterator it = files.find( id );
        File * file;
        if( it != files.end()) // found one or create a new one ? 
        {
            file = (*it).second;
            
        } else // create a new one
        {
            file = new File( id, File::OUT, append );
            files[id] = file;        
        }
        if( file->mode == File::OUT ) // test for right direction and add to store
        {            
            NodeVector & vector = nodes[id];
            NodeVector::iterator nodeIt = vector.begin();             
            FileSink * sink = new FileSink(*file, station );
            vector.push_back( sink );
            cout << "Built FileSink node writting into " << id << " with station " 
                 << station << endl;       
            return sink;
        } 
        cout << "FileSink referencing input file " << id << endl;

    } else if( name.compare("FileSource") == 0 )
    {
		string id = attributes.get("file");
        // file source needs an existing file, therefore we can use the directory stack !
        string fullname;
        if( context->findFile( id, fullname ))
        {
            id = fullname;
        }
        else
        {
            cout << "FileModule could not find file " << id << " for FileSource !\n";
            return NULL;
        }

        int station;
        if( attributes.get("station", &station ) == 0 ) 
            station = 0;                    

        bool localTime = ( attributes.get("localtime").compare("true") == 0 );
            
        // search for File
        map<string,File *>::iterator it = files.find( id );
        File * file;
        if( it != files.end()) // found one
        {
            file = (*it).second;

        } else // create a new one
        {
            file = new File( id, File::IN, false, loop );
            files[id] = file;
        }
        if( file->mode == File::IN ) // test for right direction and add to store
        {
            NodeVector & vector = nodes[id];
            NodeVector::iterator nodeIt = vector.begin();    
            for( ; nodeIt != vector.end(); nodeIt++ )  // test for another node with the same station
                if(((FileSource *)(*nodeIt))->station == station )
                    break;
            if( nodeIt != vector.end())
            {
                cout << "Allready another FileSource node for station " << station << endl;
                return NULL;
            }
            FileSource * source = new FileSource( station, localTime );
            vector.push_back( source );
            cout << "Built FileSource node reading from " << id << " with station " 
                 << station << " and localtime " << localTime << endl;       
            return source;
        } 
        cout << "FileSource referencing output file " << id << endl;
    }
    return NULL;
}

// reads from the input files and fires new events, if necessary

void FileModule::pushState()
{
    State state;
    int station;
    
    // store a time stamp to use for all localTime file sources
    double time = OSUtils::currentTime();

    if( interval != -1 && (time - lastTime) < interval )
        return;

    lastTime = time;

	for( map<string, File*>::iterator it = files.begin(); it != files.end(); it++ )
    {
        if((*it).second->mode == File::IN )
        {
            NodeVector & vector = nodes[(*it).first];
            NodeVector::iterator jt;
            for( jt = vector.begin(); jt != vector.end(); jt++ )
            {
                ((FileSource*)(*jt))->changed = false;
            }            
            while( 1 ){
                if( (*it).second->read( state, &station ) == false )
                {
                    break;
                }
                jt = vector.begin();
                while( jt != vector.end())
                {
                    if(((FileSource*)(*jt))->station == station && 
                       ((FileSource*)(*jt))->changed == false)
                        break;
                    jt++;
                }        
                if( jt != vector.end())
                {
                    ((FileSource*)(*jt))->changed = true;
                    if(((FileSource*)(*jt))->localTime )
                        state.time = time;
                    ((FileSource*)(*jt))->updateObservers( state );
                }                    
                else 
                    break;
           }
        }
    }
}

// Closes the files and cleans up datastructures

void FileModule::close()
{
    for( map<string, File*>::iterator it = files.begin(); it != files.end(); it ++ )
	{
        
		File * file = (File*)(*it).second;
		delete file;
	}
    files.clear();
}
