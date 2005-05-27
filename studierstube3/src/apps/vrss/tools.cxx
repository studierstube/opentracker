/* =========================================================================
 *  (c) 2000,2001 VRVis GmbH, Vienna, Austria
 *  (c) 1999,2000 Vienna University of Technology
 * =========================================================================
 *  PROJECT: Studierstube VR Slide Show system
 * ========================================================================= */
/** 
 * This file contains helper functions used by the VRSS application.
 * These functions deal with file node nuking, initialising caches of 
 * OpenInventor nodes, saving the scene graph in proper file format
 * et cetera.
 *
 * @author Jan Prikryl
 * @author Rainer Splechtna
 * @author Anton Furhmann
 * @author Dieter Schmalstieg
 *
 * 
 * $Id: tools.cxx 3426 2004-07-14 12:11:27Z tamer $
 * @ingroup VRSS
 * @file
 */
/* ========================================================================= */
/*@{*/


// #include <sys/types.h>
#include <assert.h>
#include <stdlib.h>
// #include <time.h>
// 
#ifdef WIN32
#include <string.h>
// #include <windows.h>
#define strcasecmp _stricmp
#endif
// 
// #include <Inventor/Xt/SoXt.h>
// #include <Inventor/Xt/viewers/SoXtExaminerViewer.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/actions/SoWriteAction.h>
// #include <Inventor/events/SoKeyboardEvent.h>
// #include <Inventor/sensors/SoAlarmSensor.h>
// #include <Inventor/nodes/SoNode.h>
#include <Inventor/nodes/SoSeparator.h>
// #include <Inventor/nodes/SoCallback.h>
// #include <Inventor/nodes/SoEventCallback.h>
#include <Inventor/nodes/SoFile.h>
// #include <Inventor/nodes/SoWWWInline.h>
// 
// #ifdef USE_TGS
// #include <Inventor/VRMLnodes/SoVRMLSwitch.h>
// #include <Inventor/VRMLnodes/SoVRMLGroup.h>
// #include <Inventor/VRMLnodes/SoVRMLColorInterpolator.h>
// #include <Inventor/VRMLnodes/SoVRMLCoordinateInterpolator.h>
// #include <Inventor/VRMLnodes/SoVRMLNormalInterpolator.h>
// #include <Inventor/VRMLnodes/SoVRMLOrientationInterpolator.h>
// #include <Inventor/VRMLnodes/SoVRMLPositionInterpolator.h>
// #include <Inventor/VRMLnodes/SoVRMLScalarInterpolator.h>
// #include <Inventor/VRMLnodes/SoVRMLTimeSensor.h>
// #include <Inventor/VRMLnodes/SoVRMLPROTODef.h>
// #endif
// 
// #include <stbapi/tracker/TrackerBuilder.h>
// #include <stbapi/tracker/SoTrakEngine.h>
// #include <stbapi/event3d/Stb3DEventGenerator.h>
// #include <stbapi/misc/SoAppNode.h>
// #include <stbapi/util/option.h>
// #include <stbapi/util/ivutil.h>
// #include <stbapi/util/ivio.h>
// #include <stbapi/util/timerutility.h>
// #include <stbapi/widgets/So3DButton.h>
// #include <stbapi/resource/SoUserKit.h>
// #include <stbapi/resource/SoPipSheetKit.h>
// #include <stbapi/workspace/StbCommunicator.h>
// #include <stbapi/workspace/StbWorkspace.h>
#include <stbapi/resource/viewer/SoStudierstubeViewer.h>

#include "vrss.h"
#include "tools.h"
#include "assoclist.h"

/**
 * Exit with message about both possible parent nodes being NULL.
 */

static void
_exitParentNull (void)
{
    fprintf (stderr,
	     "ERROR: "
#ifdef USE_TGS
	     "vrParent == NULL and "
#endif
	     "ivParent == NULL!\n");
    exit (1);
}

/**
 * Search and expand SoFile nodes.  This routine searches for and
 * expands all SoFile nodes in the given scene graph.  It does this by
 * making all the children of a SoFile node the children of its
 * parent. Every SoFile node is expanded only the first time it is
 * encountered. The method maintains a list of expanded nodes and
 * their addresses so that any subsequent occurences of an existing
 * SoFile node can be easily replaced by a reference to an already
 * expanded node.
 * @param root The root node of the scene graph where SoFile nodes shall
 *             be nuked. 
 * @warning This behaviour may theoretically create loops in the graph!
 */

void
nukeFileNodes(SoNode *&root)
{
#ifdef DEBUG_SAVE
    static int num=0;
#endif

    GroupAssocList * assoc = new GroupAssocList ();

    printf("INFO:  Expanding all SoFile nodes ...\n");

    //
    // Special case: if root is a file node, replace it with a group.
    //
    if (root->isOfType (SoFile::getClassTypeId ())) {
        SoFile *f = (SoFile *) root;
        SoGroup *g = f->copyChildren ();
        root->unref ();
        root = g;
        root->ref ();
    }
    
    /** Search for all file nodes. */
    SoSearchAction sa;
    sa.setType (SoFile::getClassTypeId ());
    sa.setInterest (SoSearchAction::FIRST);
    sa.setSearchingAll (TRUE);
    
    sa.apply (root);
    
    // We'll keep on searching until there are no more file nodes
    // left.  We don't search for all file nodes at once, because we
    // need to modify the scene graph, and so the paths returned may
    // be truncated (if there are several file nodes under a group, if
    // there are files within files, etc).  Dealing properly with that
    // is complicated-- it is easier (but slower) to just reapply the
    // search until it fails.
    // We need an SoFullPath here because we're searching node kit
    // contents.

    SoFullPath *p = (SoFullPath *) sa.getPath ();
    while (p != NULL)
    {
        SoNode      * tNode = (SoNode *) p->getNodeFromTail (1);
        SoGroup     * ivParent;
#ifdef USE_TGS
        SoVRMLGroup * vrParent;

        if (tNode->isOfType (SoVRMLGroup::getClassTypeId ()))
        {
            vrParent = (SoVRMLGroup *) tNode;
            ivParent = NULL;
            assert (vrParent != NULL);
        }
        else
#endif
	if (tNode->isOfType (SoGroup::getClassTypeId ()))
        {
#ifdef USE_TGS
            vrParent = NULL;
#endif
            ivParent = (SoGroup *) tNode;
            assert (ivParent != NULL);
        }
        else
        {
            fprintf (stderr,
		     "ERROR: Parent is not derived from "
#ifdef USE_TGS
		     "SoVRMLGroup or "
#endif
		     "SoGroup!\n");
            exit (1);
        }

        SoFile *file = (SoFile *) p->getTail ();
        
	const char * filename = file->name.getValue ().getString ();
	SoGroup    * parent;
	int          fileNodeIndex;

#ifdef USE_TGS
	parent = ivParent ? ivParent : vrParent ;
#else
	parent = ivParent;
#endif
	assert (parent);

	fileNodeIndex = parent->findChild (file);
	assert (fileNodeIndex != -1);

	// Check if the file of this name has been already
	// expanded. If not, read it in. If yes, just reference the
	// already existing group node.

	SoGroup * group = assoc->SearchFor (filename);

	if ( group )
	{
	    DPRINTF (("DEBUG: Node `%s' already at 0x%08x\n",
		      filename, (unsigned long int) group));

#ifdef USE_TGS
	    if (vrParent)
	    {
		SoVRMLGroup * vrGroup = (SoVRMLGroup *) group;
		vrGroup->ref ();
		vrParent->insertChild (group, fileNodeIndex+1);
		vrParent->removeChild (fileNodeIndex);
		DPRINTF (("       Added VRML-style reference.\n")); 
	    }
	    else 
#endif
	    if (ivParent)
	    {
		group->ref ();
		ivParent->insertChild (group, fileNodeIndex+1);
		ivParent->removeChild (fileNodeIndex);
		DPRINTF (("       Added Inventor-style reference.\n")); 
	    }
	    else
	    {
		_exitParentNull ();
	    }
	}
	else
	{
#ifdef DEBUG_PRINT
	    DPRINTF (("DEBUG: Node `%s' not expanded yet.\n",
		      filename));
#else
	    printf ("       Processing `%s' ...\n", filename);
#endif

	    // If the filename includes a directory path, add the
	    // directory name to the list of directories where to look
	    // for input files

	    char * slashPtr   = strrchr (filename, '/');
	    char * searchPath = NULL;

	    if (slashPtr != NULL)
	    {
		int pathLen = strlen (VRSS_DATA_DIR) + strlen (filename);
		searchPath = (char *) malloc (pathLen+3);
		*searchPath = '\0';
		strcat (searchPath, VRSS_DATA_DIR);
		strcat (searchPath, "/");
		strcat (searchPath, filename);
		slashPtr = strrchr (searchPath, '/');
		*slashPtr = '\0';

		SoInput::addDirectoryFirst (searchPath);
		DPRINTF (("       Added `%s' to SoInput search path.\n",
			  searchPath));
	    }

	    SoInput * in = new SoInput;
	    if (!in->openFile (filename))
	    {
		fprintf (stderr, "ERROR: Cannot open `%s'!\n", filename);
		exit (1);
	    }

	    // Now, add group of all children to file's parent's list
	    // of children, right after the file node. The procedure
	    // differs slightly for VRML and IV files.

#ifdef USE_TGS
	    if (vrParent)
	    {
		SoVRMLGroup * vrGroup;
		DPRINTF (("       Flattening SoFile node '%s' in VRML mode "
			  "as child #%d\n",
			  filename, fileNodeIndex));
            
		// Debug: write current scene graph
#ifdef DEBUG_SAVE
		{
		    char buff[100];
		    int  i;
		    int  l = p->getLength ();
		    SoNode * tRoot;
		    
		    for ( i=1; i<l; i++ )
		    {
			tRoot = (SoNode *) p->getNodeFromTail (i);
			sprintf (buff,"dbg%04d-A%03d.wrl", num, i);
			writeScene (tRoot, buff);
		    }
		    
		    if ( tRoot != root )
		    {
			DPRINTF (("FIRST: tRoot != root\n"));
		    }
		    else
		    {
			DPRINTF (("FIRST: path OK\n"));
		    }
		}
#endif
		 
		 vrGroup = SoDB::readAllVRML (in);
		 if ( vrGroup != NULL )
		 {
		     vrGroup->ref ();
		     vrParent->insertChild (vrGroup, fileNodeIndex+1);
		     group = vrGroup;
		 }
		 else
		 {
		     vrParent->insertChild(new SoVRMLGroup, fileNodeIndex+1);
		     DPRINTF(("  WARNING: Empty SoVRMLGroup inserted\n"));
		 }
		 
#ifdef DEBUG_SAVE
		 // Debug: write current scene graph
		 {
		     char buff[100];
		     int  i;
		     int  l = p->getLength ();
		     
		     for ( i=1; i<l; i++ )
		     {
			 SoNode * tRoot = (SoNode *) p->getNodeFromTail(i);
			 sprintf (buff, "dbg%04d-B%03d.wrl", num, i);
			 writeScene (tRoot, buff);
		     }
		 }
            
#endif            
		 vrParent->removeChild(fileNodeIndex);
            
#ifdef DEBUG_SAVE
		 // Debug: write current scene graph
		 {
		     char buff[100];
		     int  i;
		     int  l = p->getLength ();
		     
		     for ( i=1; i<l; i++ )
		     {
			 SoNode * tRoot = (SoNode *) p->getNodeFromTail (i);
			 sprintf (buff, "dbg%04d-C%03d.wrl", num, i);
			 writeScene (tRoot, buff);
			 SoType tType = tRoot->getTypeId();
			 printf ("node %0d is %s\n",
				 i, tType.getName().getString());
		     }
		 }
		  
#endif
	    }
	    else
#endif
            if ( ivParent )
	    {
		SoSeparator * ivGroup;

		DPRINTF (("       Flattening SoFile node '%s' in IV mode "
			  "as child #%d\n",
			  filename, fileNodeIndex));

		ivGroup = SoDB::readAll (in);

		// Insert the expanded content right after the
		// reference to the file node.

		if (ivGroup != NULL)
		{
		    ivGroup->ref ();
		    ivParent->insertChild (ivGroup, fileNodeIndex+1);
		    group = ivGroup;
		}
		else    
		{
		    // An empty SoSeparator so we can at least see
		    // where the file node contents were supposed to
		    // go.
		    ivParent->insertChild (new SoSeparator, fileNodeIndex+1);
		    DPRINTF (("WARNING: Empty SoSeparator inserted.\n"));
		}

		// And remove the file node.
		ivParent->removeChild (fileNodeIndex);
	    }
	    else
	    {
		_exitParentNull();
	    }
	    
	    delete in;

	    // If a group node has been created, insert it into the
	    // associative list so that it can be referenced in the
	    // future.

	    if ( group ) 
	    {
		DPRINTF (("       Adding group `%s' expanded at 0x%08x\n",
			  filename, (unsigned long int)group));
		assoc->AddPair (filename, group);
	    }

#ifdef DEBUG_SAVE
	    {
		char buff[100];
		int  l = p->getLength()-1;
		
		SoNode * tRoot = (SoNode *)p->getNodeFromTail(l);
		sprintf(buff,"dbg%04d-D000.wrl", num);
		writeScene(tRoot, buff);
		
		if (tRoot != root)
		{
		    DPRINTF(("LAST: tRoot != root\n"));
		}
		else
		{
		    DPRINTF(("LAST: path OK\n"));
		}
		
		num++;
	    }
	     
#endif        
	}

	sa.apply(root);
	p = (SoFullPath *) sa.getPath();
    }

    printf("       Done.\n");
}

/**
 * Initialize OIV caches.  This function builds all node caches by
 * simulating a GLRenderAction over the whole graph. Without cache
 * initialisation it would happen that as nodes build the caches
 * (display lists) in the moment when they are first being rendered it
 * would happen that some animations stop for a small time when slides
 * are being switched.
 * @param slideSwitch The master switch of the slide show (that one that
 * switches between the slide pairs).
 * @param viewer Current viewer class needed for determining the viewport
 * region (the veiwport is required for constructing the SoGLRenderAction). 
 */

#ifdef USE_TGS

void
cacheSlides(SoVRMLSwitch * slideSwitch, SoStudierstubeViewer *viewer)
{
    DPRINTF(("DEBUG: Caching slides ...\n"));

    if ( slideSwitch )
    {
	// GLRenderAction
	const SbViewportRegion sbr = viewer->getViewportRegion ();
	SoGLRenderAction sglr (sbr);
	sglr.setTransparencyType (SoGLRenderAction::BLEND);
	
	// Fields of the master switch node
	SoSFInt32 * whichChoiceField = & slideSwitch->whichChoice;
	const int   numChoices       = slideSwitch->getNumChoices ();
	
	// Render all slide once to build the caches
	for (int i=0; i<numChoices; i++)
	{
	    whichChoiceField->setValue(i);
	    DPRINTF(("       Rendering child %02d\n", i));
	    sglr.apply(slideSwitch);
	    DPRINTF(("       Node cached.\n"));
	}
	
	// And go back at the first slide
	whichChoiceField->setValue(1);
    }

    DPRINTF (("       Done.\n"));
}

#endif

/**
 * Writes the scene graph under the given name.
 * The file extenstion determines the file format which can be either
 * standerd OpenInventor or - in case use compiled with TGS Open
 * Inventor - also VRML.
 * @param rootToWrite The root node of the scene (sub)graph that shall
 * be written to the file.
 * @param name File name to save the scene graph in.
 */

void
writeScene (SoNode *rootToWrite, const char* name)
{
    char * suffix = strrchr (name, '.');
    SoWriteAction myAction;

    if (!strcasecmp (suffix, ".iv"))
    {
	printf ("INFO:  Saving Open Inventor scene to `%s'\n", name);
    }
    else if (!strcasecmp (suffix, ".wrl"))
    {
#ifdef USE_TGS
	printf ("INFO:  Saving VRML scene to `%s'\n", name);
	myAction.getOutput()->setHeaderString ("#VRML V2.0 utf8");
#else
	fprintf (stderr,
		 "ERROR: VRML output requires TGS Open Inventor!\n");
	exit (1);
#endif
    }
    else
    {
	fprintf (stderr,
		 "ERROR: writeScene() - Unknown file extension `%s'.\n",
		 suffix);
	exit (1);
    }

    myAction.getOutput ()-> openFile (name);
    myAction.getOutput ()-> setBinary (FALSE);
    myAction.apply (rootToWrite);
    myAction.getOutput ()-> closeFile();

    printf("       Done.\n");
}

/*@}*/
/* ===========================================================================
   End of tools.cxx
   ===========================================================================
   Automatic Emacs configuration follows.
   Local Variables:
   mode:c++
   c-basic-offset: 4
   eval: (c-set-offset 'substatement-open 0)
   eval: (c-set-offset 'case-label '+)
   eval: (c-set-offset 'statement 'c-lineup-runin-statements)
   End:
=========================================================================== */
