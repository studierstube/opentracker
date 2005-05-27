/* ========================================================================
 * Copyright (C) 2000-2003  Vienna University of Technology
 *
 * This framework is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This framework is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this framework; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * For further information please contact Dieter Schmalstieg under
 * <schmalstieg@ims.tuwien.ac.at> or write to Dieter Schmalstieg,
 * Vienna University of Technology, Favoritenstr. 9-11/188, A1040 Vienna,
 * Austria.
 * ========================================================================
 * PROJECT: Studierstube
 * ======================================================================== */
/** Inventor file tools implementation
  *
  * @author  Andreas Zajic, Markus Krutz, Gerd Hesina, Dieter Schmalstieg, Jan Prikryl
  *
  * $Id: ivio.cxx 4117 2004-12-21 15:37:42Z reitmayr $
  * @file                                                                   */
 /* ======================================================================= */

#include <vector>
#include <cassert>
#include <cstring>
#include <Inventor/SoInput.h>
#include <Inventor/actions/SoWriteAction.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/errors/SoReadError.h>
#include <Inventor/misc/SoChildList.h>
#include <Inventor/nodes/SoTexture2.h>
#include <Inventor/nodes/SoFile.h>
#include "ivio.h"

using namespace std;

//----------------------------------------------------------------------------

SbBool writeImages = TRUE;

void writeTexture(SoAction *action, SoNode *node)
{
    SbBool imageDefault = FALSE;
    SbBool nameDefault = FALSE;
    if( TRUE == writeImages )
    {
        assert( node->isOfType( SoTexture2::getClassTypeId()));
        SoTexture2 * texture = (SoTexture2 *) node;
        SbBool imageDefault = texture->image.isDefault();
        SbBool nameDefault = texture->filename.isDefault();
        if( TRUE == imageDefault )
        {
            texture->image.setDefault( FALSE );
        }
        if( FALSE == nameDefault )
        {
            texture->filename.setDefault( TRUE );
        }
		texture->write((SoWriteAction *) action);
        texture->image.setDefault( imageDefault );
        texture->filename.setDefault( nameDefault );
    }
	else
	{
		node->write((SoWriteAction *) action);
	}    
}

void
writeFile(const char* filename, SoNode* node, SbBool withImages) 
{
    SbBool oldWrite = writeImages;
    writeImages = withImages; 
    SoWriteAction wa;
    wa.getOutput()->openFile(filename);
    wa.getOutput()->setBinary(FALSE);
    wa.apply(node);
    wa.getOutput()->closeFile();
    writeImages = oldWrite;
}

//----------------------------------------------------------------------------

void * reallocCB(void *ptr, size_t newsize)
{
    return realloc( ptr, newsize );
}

char * writeMemory( SoNode * node, size_t * size, size_t initSize, SbBool binary, SbBool withImages )
{
    void * outputBuffer = new char[initSize];
    *size = writeBuffer( node, &outputBuffer, &initSize, 0, binary, withImages );
    return (char *)outputBuffer;
}

size_t writeBuffer( SoNode * node, void ** buffer, size_t * bufferSize, size_t offset, SbBool binary, SbBool withImages )
{
    assert( *buffer != NULL );
    assert( bufferSize > 0 );
    SbBool oldWrite = writeImages;
    writeImages = withImages;
    SoOutput output;
    output.setBuffer(*buffer, *bufferSize, reallocCB, offset);
    output.setBinary( binary );
    SoWriteAction wa(&output);
    wa.apply(node);
    size_t size;
    output.getBuffer(*buffer, size);
    *bufferSize = output.getBufferSize();
    writeImages = oldWrite;
    return size;
}

SoNode * copyNode( SoNode * node, SbBool copyConnections, SbBool withImages )
{
    vector<SoSFImage *> changedImages;
    if( TRUE == withImages )
    {
        // find all SoTexture2 nodes
        SoSearchAction search;
        search.setType( SoTexture2::getClassTypeId());
        search.setInterest( SoSearchAction::ALL );
        search.setSearchingAll( TRUE );
        search.apply( node );
        SoPathList & list = search.getPaths();
     
        // printf("copyNode found %i texture nodes\n", list.getLength());

        // set their images to not default to copy the contents
        for( int i = 0; i < list.getLength(); i++ )
        {
            SoFullPath * path = (SoFullPath *) list[i];
            assert( path->getTail()->isOfType( SoTexture2::getClassTypeId()));
            SoSFImage * image = &((SoTexture2 *)path->getTail())->image;
            if( TRUE == image->isDefault())
            {
                ((SoTexture2 *)path->getTail())->image.setDefault( FALSE );
                changedImages.push_back( image );
            }
        }
        // printf("copyNode changed %i image fields\n", changedImages.size());
    }
    // the actual copy operation
    SoNode * result = node->copy(copyConnections);
    // reset the changed ones back
    for( vector<SoSFImage *>::iterator it = changedImages.begin(); 
         it != changedImages.end(); it++ )
    {
        (*it)->setDefault( TRUE );
    }
    return result;
}

//----------------------------------------------------------------------------

SbBool checkFile( SbString & name, SbBool & isValid )
{
    // We stop any SoReadError output during this method to avoid
    // error messages, if this function is used to test a non Inventor
    // file. 
    // gerhard 25082003
    SoErrorCB * cb = SoReadError::getHandlerCallback();
    void * data = SoReadError::getHandlerData();
    SoReadError::setHandlerCallback( NULL, NULL );

    SoInput in;
    SbBool result = FALSE;
    if( in.openFile( name.getString()))
    {
        isValid = in.isValidFile();
        name = in.getCurFileName();
        result = TRUE;
    }
    else
    {
        isValid = FALSE;
        result = FALSE;
    }
    in.closeFile();
    SoReadError::setHandlerCallback(cb, data);
    return result;
}

//----------------------------------------------------------------------------

SoSeparator* 
readFile(const char* filename) 
{
    SoInput myinput;
    if((strlen(filename)>1)&&((filename[1]==':')||(filename[0]=='\\')))
    {
        FILE *fPointer=fopen(filename,"rb");
        if(fPointer==NULL)
        {
            fprintf(stderr, "ivio readFile: Cannot open file: %s\n", filename);
            return NULL;
        }
        myinput.setFilePointer(fPointer);
    }
    else if (!myinput.openFile(filename)) 
    {
        fprintf(stderr, "ivio readFile: Cannot open file: %s\n", filename);
        return NULL;
    }

#ifdef USE_TGS
    SoVRMLGroup* mygraph = SoDB::readAllVRML(&myinput);
#else
    SoSeparator* mygraph = SoDB::readAll(&myinput);
#endif

    if (mygraph==NULL) 
    {
        fprintf(stderr, "ivio readFile: Problem reading file: %s\n",filename);
        return NULL;
    }
    myinput.closeFile();
#ifdef USE_TGS
    SoSeparator * separator = new SoSeparator;
    separator->addChild((SoNode *)mygraph);
    return separator;
#else
    return mygraph;
#endif
}

//----------------------------------------------------------------------------
// No #ifdef USE_TGS in this case as there are methods that explicitely
// require reading files as Inventor data and not as a mix of VRML and Inventor 
// nodes (and, morover, we want to limit ourselves to pure Open Inventor and
// get rid of the VRML stuff anyway). Rather then calling 'readFile(name)',
// these methods may call 'readFile(name,"")'.

SoSeparator* 
readFile(const char* filename, const char* pathname) 
{
    SoInput myinput;
	SoInput::addDirectoryFirst(pathname);
    if (!myinput.openFile(filename)) 
    {
        fprintf(stderr, "Cannot open file: %s\n", filename);
		SoInput::removeDirectory(pathname);
        return NULL;
    }
    SoSeparator* mygraph = SoDB::readAll(&myinput);
    if (mygraph==NULL) 
    {
        fprintf(stderr, "Problem reading file: %s\n",filename);
		SoInput::removeDirectory(pathname);
        return NULL;
    }
    myinput.closeFile();
	SoInput::removeDirectory(pathname);
    return mygraph;
}

//----------------------------------------------------------------------------

SoNode*
readNodeFromFile(const char* filename) 
{
    SoInput myinput;
    if (!myinput.openFile(filename)) 
    {
        fprintf(stderr, "Cannot open file: %s\n", filename);
        return NULL;
    }
    SoNode* node;
    SbBool res = SoDB::read(&myinput,node);
    if (!res) 
    {
        fprintf(stderr, " Problem reading file: %s\n",filename);
        return NULL;
    }
    myinput.closeFile();
    return node;
}

//----------------------------------------------------------------------------

SoNode*
readNodeFromFile(const char* filename, const char* pathname) 
{
    if( strlen(filename) < 1 )
        return NULL;
    SoInput myinput;
	SoInput::addDirectoryFirst(pathname);
    if (!myinput.openFile(filename)) 
    {
        fprintf(stderr, "Cannot open file: %s\n", filename);
		SoInput::removeDirectory(pathname);
        return NULL;
    }
    SoNode* node;
    SbBool res = SoDB::read(&myinput,node);
    if (!res) 
    {
        fprintf(stderr, " Problem reading file: %s\n",filename);
		SoInput::removeDirectory(pathname);
        return NULL;
    }
    myinput.closeFile();
	SoInput::removeDirectory(pathname);
    return node;
}

//----------------------------------------------------------------------------

SoSeparator*
readMemory(const char* buffer, size_t len ) 
{
    SoInput myinput;
    myinput.setBuffer((void*)buffer,len);
    SoSeparator* mygraph = SoDB::readAll(&myinput);
    if (mygraph==NULL) 
    {
        fprintf(stderr, "Problem reading buffer\n");
        return NULL;
    }
    return mygraph;
}

SoSeparator * readMemory( const char * buffer )
{
    return readMemory( buffer, strlen( buffer ));
}

//----------------------------------------------------------------------------

SoNode*
readNodeFromMemory(const char* buffer, size_t len ) 
{
    SoInput myinput;
    myinput.setBuffer((void*)buffer,len);
    SoNode* node;
    SbBool res = SoDB::read(&myinput, node);
    if (!res) 
    {
        fprintf(stderr, "Problem reading buffer\n");
        return NULL;
    }
    return node;
}

SoNode * readNodeFromMemory( const char * buffer )
{
    return readNodeFromMemory( buffer, strlen( buffer ));
}
//----------------------------------------------------------------------------

SoNode*
findNode(SoNode* root, SbString name)
{
    SoSearchAction sa;
	sa.setSearchingAll(TRUE);
    sa.setName(name);
    sa.apply(root);
    SoFullPath* path = (SoFullPath*)sa.getPath();
    assert(path != NULL);
    return path->getTail();
}

//----------------------------------------------------------------------------

void
printNode(SoNode* node, SbString comment)
{
    if (comment == "") comment = "INFO";
    if (node == NULL) printf("%s: NULL\n",comment.getString());
    else printf("%s: node=%p name='%s' (%s)\n", 
                comment.getString(), (void *)node, 
                node->getName().getString(),
                node->getTypeId().getName().getString());
}

//----------------------------------------------------------------------------

void
printPath(SoPath* p, SbString comment)
{
    if(comment == "") comment = "INFO";
    if(p == NULL) printf("%s: NULL path\n",comment.getString());
    else 
    {
        SoFullPath* path = (SoFullPath*)p;
        printf("%s: ",comment.getString());
        for(int i=0; i<path->getLength(); i++)
        {
            SoNode* node = path->getNode(i);
            SbString text = node->getName().getString();
            if(text == "") text = node->getTypeId().getName().getString();
            printf("%p(%s)->",(void*)node,text.getString());
        }
    }
}

//----------------------------------------------------------------------------

struct ReplaceState {
    unsigned int maxDepth;
    SbPList index;
    SoNodeList parent;
};

SoCallbackAction::Response stopCallback( void * userdata, SoCallbackAction * action, const SoNode * node)
{
    return SoCallbackAction::PRUNE;
}

SoCallbackAction::Response fileNodeCallback( void * userdata, SoCallbackAction * action, const SoNode * node )
{
    assert( userdata );
    assert( node->isOfType(SoFile::getClassTypeId()));

    struct ReplaceState * state = (struct ReplaceState *) userdata;

    SoFullPath * path = (SoFullPath *) action->getCurPath();
    
    if( state->maxDepth == 0 || (unsigned int)path->getLength() <= state->maxDepth )
    {
        state->index.append( (void *)path->getIndexFromTail(0));
        state->parent.append( path->getNodeFromTail(1));
    }
    return SoCallbackAction::PRUNE;
}

unsigned int 
replaceFileNodes( SoNode * scene, unsigned int depth, const SoTypeList * stopTypes )
{
    SoCallbackAction cba;
    struct ReplaceState state;
    unsigned int count = 0;

    cba.addPreCallback( SoFile::getClassTypeId(), fileNodeCallback, &state );
    if( stopTypes != NULL )
    {
        int i;
        for( i = 0; i < stopTypes->getLength(); i++)
            cba.addPreCallback( (*stopTypes)[i], stopCallback, &state );
    }
    state.maxDepth = depth;

    scene->ref();
    do 
    {
        state.parent.truncate(0);
        state.index.truncate(0);
        cba.apply( scene );
        int i;
        for( i = 0; i < state.parent.getLength(); i++)
        {
            assert( state.parent[i]->isOfType(SoGroup::getClassTypeId()));
            SoGroup * parent = (SoGroup *)state.parent[i];
            int index = (int) state.index[i];
            assert( parent->getChild(index)->isOfType(SoFile::getClassTypeId()));
            SoFile * file = (SoFile * )parent->getChild(index);

            // Here we use SoFile::getChildren which is documented as SoInternal,
            // and should not be used therefore. However, it is exactly what we
            // want because we replace the SoFile node anyway. It is present in
            // Coin3D and Open Inventor, therefore it should not pose a problem.
            // gerhard [8/25/2003]
            SoChildList * children = file->getChildren();
            SoGroup * content = new SoGroup;
            content->ref();
            for( int j = 0; j < children->getLength(); j++ )
                content->addChild( (*children)[j] );
            parent->replaceChild(index, content);
            content->unref();
        }
        count += state.parent.getLength();
    } while(state.parent.getLength() > 0);

#if 0
    writeImages = FALSE;
    SoWriteAction wa;
    wa.apply( scene );
#endif

    scene->unref();
    return count;
}

/* ===========================================================================
    End of ivio.cxx
   ===========================================================================
    Automatic Emacs configuration follows.
    Local Variables:
    mode:c++
    c-basic-offset: 4
    eval: (c-set-offset 'substatement-open 0)
    eval: (c-set-offset 'case-label '+)
    eval: (c-set-offset 'statement 'c-lineup-runin-statements)
    eval: (setq indent-tabs-mode nil)
    End:
=========================================================================== */

