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
#ifndef _FILEBROWSER_H_
#define _FILEBROWSER_H_

#include <Inventor/sensors/SoTimerSensor.h>
#include <Inventor/nodes/SoRotation.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/nodes/SoText3.h>
#include <Inventor/nodes/SoEventCallback.h>

#include <stbapi/stbapi.h> 
#include <stbapi/widgets/So3DCheckBox.h> 
#include <stbapi/widgets/So3DButton.h> 


#define MAX_BROWSER_BUTTONS 8

struct direntry	// directory entry struct
{
        char  	name[256];			// filename
        int     type;				// filetype, 0=file, 1=dir
        direntry *next;		// next entry in list
};

typedef void(*fbCallback)(void *,const char *);

/**
 * This class provides the file browser functionality on the system pipsheet.
 * Note that under windows different drives are not supported !!
 * @ingroup oldwidgets
 * @ingroup deprecated
 */
class STBAPI_API fileBrowser  
{
	fbCallback fbCB;
	void *CBdata;
	SoText3 *filterText;
	So3DCheckBox *filterCheckBox;
	int filterFixed;
	So3DButton *direntryButton[MAX_BROWSER_BUTTONS];			// MAX_BROWSER_BUTTONS buttons on pip-sheet
	So3DButton *upButton, *downButton; // up/down on pip-sheet
	So3DButton *upDownButtonPressed; // up/down on pip-sheet
	SoTimerSensor *upDownRepeater;
	SoTimerSensor *ivRotationSensor;
	SoSwitch *	direntryButtonIVSwitch[MAX_BROWSER_BUTTONS];			
	SoSwitch *	buttonSwitch[MAX_BROWSER_BUTTONS];		// switch-node for en-/
   	char			currentDir[1024];		// working dir of user
	direntry *	actualIVFiles;			// .IV-files in current dir used as icons
	direntry *	actualFiles;			// .IV-files in current dir used as icons
	int 			actualFileNumber;		// number of .stb files in current directory
	int			currentFilePos;			// position of pip-sheet
	SoSeparator *direntryGeometrySep;
	SoSeparator *direntryPressedGeometrySep;
	SoSeparator *ivGeometrySep;
	SoRotation  *ivRotation;
	char 			currentFilter[256];
	char 			filterInput[256];

	void			readFilesInDir();		// reads the files in
											// the current directory

	void			loadIVFiles();		// reads the IV files in
											// the current directory

	SoSeparator *readThumbnail(const char *path);

	SoSeparator *createFilterText(void);

	void			createButtonGeometry();	// prepares all icons     
											// for use on pip-buttons

	void 			setButtons();			// sets the pip-buttons
											// to the actual state
	void 			freeEntries();			

// static callback functions ------------------------------------

	static void 	filterCallback(void *data, So3DCheckBox *fb);

	static void 	objectLoadCallback(void *, So3DButton *);
											// called when a pip
											// button was pressed
	static void 	upDownRepeaterCallback(void *, SoSensor *);
											// called when up or
											// down button pressed
	
	static void 	upDownPressCallback(void *, So3DButton *);
											// called when up or
											// down button pressed

	static void 	upDownReleaseCallback(void *, So3DButton *);
											// called when up or
											// down button released

	static void 	ivRotationSensorCallback(void *, SoSensor *);


	static void		readTextures(/* StbCommunicator * */);
											// class initialization
    static void     filterKeyEventHandle(void *data, SoEventCallback *myEvent);

public:
	fileBrowser(void);
	~fileBrowser(void);

	SoSeparator *	setupLoaderSheet(/* StbCommunicator * */);		// create the pip-sheet
	void 			setFilter(char *,int readFiles = FALSE);
	void 			fixFilter(int readFiles = FALSE);
	void 			setFileButtonCallback(fbCallback, void *);	
    /// Changes the currentDir variable which defines the loading directory
    /// The workspace directory is still kept as 
    /// the default directory (e.g. .../bin/workspace)
    void            changeLoadPath(const char*);
};


#endif
