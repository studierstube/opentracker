 /* ========================================================================
  * PROJECT: DirectShow Video Processing Library
  * FILE:    ARToolkit 2.6x WIN32/DirectShow video input
  * ========================================================================
  * Author:  Thomas Pintaric, Vienna University of Technology
  * Contact: pintaric@ims.tuwien.ac.at http://ims.tuwien.ac.at/~thomas
  * =======================================================================
  *
  * Copyright (C) 2003  Vienna University of Technology
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
  * For further information please contact Thomas Pintaric under
  * <pintaric@ims.tuwien.ac.at> or write to Thomas Pintaric,
  * Vienna University of Technology, Favoritenstr. 9-11/188, A1040 Vienna,
  * Austria.
  * ========================================================================*/

#include "DsVideoLib.h"
#include <string.h>
#include <AR/video.h>
#include <stdlib.h>
#include "comutil.h"


// -----------------------------------------------------------------------------------------------------------------
static CGraphManager* graphManager[STEREO_BUFFERS_MAX] = {NULL, NULL};
static MemoryBufferHandle g_Handle[STEREO_BUFFERS_MAX];
static __int64 g_Timestamp = 0; // deprecated, use (g_Handle.t) instead

static int     open_flag = 0;
static bool    stereo_enabled = false;
static void    error_exit(void);

// -----------------------------------------------------------------------------------------------------------------
#ifdef FLIPPED // compatibility with videoLinux*
static const bool FLIPPED_defined =  true;	// deprecated
#else
static const bool FLIPPED_defined =  false; // deprecated
#endif

// -----------------------------------------------------------------------------------------------------------------
const int ret_error = (-1);
const int ret_success = (0);
const int ret_not_implemented = (0);
const long frame_timeout_ms = INFINITE;   // set to INFINITE if arVideoGetImage()
										  // is called from a separate worker thread

//static bool flip_horizontal = false; // deprecated
//static bool flip_vertical = false;   // deprecated


// -----------------------------------------------------------------------------------------------------------------
char* strtok_prefix(char* str, const char* prefix)
{
	if(strstr(str,prefix) != 0)
		return(str+strlen(prefix));
	else return(NULL);
}

/*****************************************************************************/
int        DLL_API arVideoOpen( char *config, STEREO_BUFFER sb)
{
    if(sb == STEREO_LEFT && open_flag != 0 ) return(ret_error);
	if(graphManager[sb] != NULL) return(ret_error);
	CoInitialize(NULL);
	graphManager[sb] = new CGraphManager();
	if(FAILED(graphManager[sb]->BuildGraph(config))) return(ret_error);

	DS_MEDIA_FORMAT mf;
	graphManager[sb]->GetCurrentMediaFormat(&mf);
	
	//flip_horizontal = mf.flipH;
	//flip_vertical =   mf.flipV;

	if(FAILED(graphManager[sb]->EnableMemoryBuffer())) return(ret_error);
	
	open_flag = 1;
	return(ret_success);
}

// -----------------------------------------------------------------------------------------------------------------
int DLL_API arVideoOpenStereo(char *config_left, char* config_right)
// by popular demand from our friends at TU Graz
{
	HRESULT hr;
	if(FAILED(hr = arVideoOpen(config_left,STEREO_LEFT))) return(hr);
	if(FAILED(hr = arVideoOpen(config_right,STEREO_RIGHT))) return(hr);
	stereo_enabled = true;
	return(S_OK);
}
// -----------------------------------------------------------------------------------------------------------------

int arVideoClose(void)
{
    if(open_flag == 0 ) return(ret_error);
	if(graphManager[STEREO_LEFT] == NULL) return(ret_error);
	graphManager[STEREO_LEFT]->CheckinMemoryBuffer(g_Handle[STEREO_LEFT], true);
	graphManager[STEREO_LEFT]->Stop();
	delete graphManager[STEREO_LEFT];
	graphManager[STEREO_LEFT] = NULL;

	if(stereo_enabled)
	{
		if(graphManager[STEREO_RIGHT] == NULL) return(ret_error);
		graphManager[STEREO_RIGHT]->CheckinMemoryBuffer(g_Handle[STEREO_LEFT], true);
		graphManager[STEREO_RIGHT]->Stop();
		delete graphManager[STEREO_RIGHT];
		graphManager[STEREO_RIGHT] = NULL;
	}

    return(ret_success);
}

// -----------------------------------------------------------------------------------------------------------------

int arVideoDispOption(void)
{
	return(ret_not_implemented);
}

// -----------------------------------------------------------------------------------------------------------------

int arVideoInqSize( int *x, int *y, STEREO_BUFFER sb)
{
	if(open_flag == 0) return(ret_error);
	if(graphManager[sb] == NULL) return(ret_error);

	DS_MEDIA_FORMAT mf;
	graphManager[sb]->GetCurrentMediaFormat(&mf);
	*x = (int) mf.biWidth;
	*y = (int) mf.biHeight;
    return(ret_success);
}

// -----------------------------------------------------------------------------------------------------------------

int arVideoInqFlipping(bool* flipH, bool* flipV, STEREO_BUFFER sb)
{
	if(open_flag == 0) return(ret_error);
	if(graphManager[sb] == NULL) return(ret_error);

	DS_MEDIA_FORMAT mf;
	graphManager[sb]->GetCurrentMediaFormat(&mf);
	*flipH = (int) mf.flipH;
	*flipV = (int) mf.flipV;
	return(ret_success);
}

// -----------------------------------------------------------------------------------------------------------------

int arVideoInqFreq(float *fps, STEREO_BUFFER sb)
{
    if(open_flag == 0) return(ret_error);
	if(graphManager[sb] == NULL) return(ret_error);

	DS_MEDIA_FORMAT mf;
	graphManager[sb]->GetCurrentMediaFormat(&mf);
	*fps = (float) mf.frameRate;
    return(ret_success);
}

// -----------------------------------------------------------------------------------------------------------------

unsigned char *arVideoLockBuffer(MemoryBufferHandle* pHandle, STEREO_BUFFER sb)
{
    if(open_flag == 0) return(NULL);
	if(graphManager[sb] == NULL) return(NULL);

	unsigned char* pixelBuffer;
	if(FAILED(graphManager[sb]->CheckoutMemoryBuffer(pHandle, &pixelBuffer))) return(NULL);
	return(pixelBuffer);
}

// -----------------------------------------------------------------------------------------------------------------
int arVideoUnlockBuffer(MemoryBufferHandle Handle, STEREO_BUFFER sb)
{
    if(open_flag == 0) return(ret_error);
	if(graphManager[sb] == NULL) return(ret_error);

	if(FAILED(graphManager[sb]->CheckinMemoryBuffer(Handle))) return(ret_error);
	else return(ret_success);
}
// -----------------------------------------------------------------------------------------------------------------

unsigned char *arVideoGetImage(STEREO_BUFFER sb)
{
    if(open_flag == 0) return(NULL);
	if(graphManager[sb] == NULL) return(NULL);

	DWORD wait_result = graphManager[sb]->WaitForNextSample(frame_timeout_ms);
	if(wait_result == WAIT_OBJECT_0)
	{
		unsigned char* pixelBuffer;
		if(FAILED(graphManager[sb]->CheckoutMemoryBuffer(&g_Handle[sb], &pixelBuffer,NULL,NULL,NULL,&g_Timestamp))) return(NULL);
		return(pixelBuffer);
	}
	else return(NULL);
}

// -----------------------------------------------------------------------------------------------------------------

int arVideoCapStart(void)
{
    if(open_flag == 0) return(ret_error);
	if(graphManager[STEREO_LEFT] == NULL) return(ret_error);
	if(stereo_enabled && graphManager[STEREO_RIGHT] == NULL) return(ret_error);

	if(FAILED(graphManager[STEREO_LEFT]->Run())) return(ret_error);
	if(stereo_enabled)
		if(FAILED(graphManager[STEREO_RIGHT]->Run())) return(ret_error);
	return(ret_success);
}

// -----------------------------------------------------------------------------------------------------------------

int arVideoCapStop(void)
{
    if(open_flag == 0) return(ret_error);
	if(graphManager[STEREO_LEFT] == NULL) return(ret_error);
	if(stereo_enabled && graphManager[STEREO_RIGHT] == NULL) return(ret_error);

	graphManager[STEREO_LEFT]->CheckinMemoryBuffer(g_Handle[STEREO_LEFT], true);
	if(stereo_enabled)
		graphManager[STEREO_RIGHT]->CheckinMemoryBuffer(g_Handle[STEREO_RIGHT], true);

	if(FAILED(graphManager[STEREO_LEFT]->Stop())) return(ret_error);
	if(stereo_enabled)
		if(FAILED(graphManager[STEREO_RIGHT]->Stop())) return(ret_error);

	return(ret_success);
}

// -----------------------------------------------------------------------------------------------------------------

int arVideoCapNext(STEREO_BUFFER sb )
{
    if(open_flag == 0) return(ret_error);
	if(graphManager[sb] == NULL) return(ret_error);

	return(SUCCEEDED(graphManager[sb]->CheckinMemoryBuffer(g_Handle[sb])) ? 
			ret_success : ret_error);
}

// -----------------------------------------------------------------------------------------------------------------
