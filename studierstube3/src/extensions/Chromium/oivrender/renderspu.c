/* Copyright (c) 2001, Stanford University
 * All rights reserved
 *
 * See the file LICENSE.txt for information on redistributing this software.
 */

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <gl/gl.h>

#include <stdlib.h>
#include "cr_string.h"
#include "cr_error.h"
#include "cr_mem.h"
#include "cr_spu.h"
#include "renderspu.h"

#include "../crserver/server_stb.h"

static void __doSync()
{
	CRMessage *in, out;

	out.header.type = CR_MESSAGE_OOB;
	
	if (render_spu.is_swap_master)
	{
		int a;
	
		for (a=0; a<render_spu.num_swap_clients; a++)
		{
			crNetGetMessage( render_spu.swap_conns[a], &in );			
			crNetFree( render_spu.swap_conns[a], in);
		}

		for (a=0; a<render_spu.num_swap_clients; a++)
			crNetSend( render_spu.swap_conns[a], NULL, &out, sizeof(CRMessage));
	}
	else
	{
		crNetSend( render_spu.swap_conns[0], NULL, &out, sizeof(CRMessage));

		crNetGetMessage( render_spu.swap_conns[0], &in );			
		crNetFree( render_spu.swap_conns[0], in);
	}
}

/*
 * Visual functions
 */

/* used for debugging and giving info to the user */
void renderspuMakeVisString( GLbitfield visAttribs, char *s )
{
	s[0] = 0;

	if (visAttribs & CR_RGB_BIT)
		crStrcat(s, "RGB");
	if (visAttribs & CR_ALPHA_BIT)
		crStrcat(s, "A");
	if (visAttribs & CR_DOUBLE_BIT)
		crStrcat(s, ", Doublebuffer");
	if (visAttribs & CR_STEREO_BIT)
		crStrcat(s, ", Stereo");
	if (visAttribs & CR_DEPTH_BIT)
		crStrcat(s, ", Z");
	if (visAttribs & CR_STENCIL_BIT)
		crStrcat(s, ", Stencil");
	if (visAttribs & CR_ACCUM_BIT)
		crStrcat(s, ", Accum");
	if (visAttribs & CR_MULTISAMPLE_BIT)
		crStrcat(s, ", Multisample");
}


/*
 * Find a VisualInfo which matches the given display name and attribute
 * bitmask, or return a pointer to a new visual.
 */
VisualInfo *renderspuFindVisual(const char *displayName, GLbitfield visAttribs )
{
#ifndef WINDOWS
	int i;

	if (!displayName)
		displayName = "";

	/* first, try to find a match */
	for (i = 0; i < render_spu.numVisuals; i++) {
		if (crStrcmp(displayName, render_spu.visuals[i].displayName) == 0
			&& visAttribs == render_spu.visuals[i].visAttribs) {
			return &(render_spu.visuals[i]);
		}
	}

	if (render_spu.numVisuals >= MAX_VISUALS)
	{
		crWarning( "Couldn't create a visual, too many visuals already" );
		return NULL;
	}

	/* create a new visual */
	i = render_spu.numVisuals;
	render_spu.visuals[i].displayName = crStrdup(displayName);
	render_spu.visuals[i].visAttribs = visAttribs;
	if (renderspu_SystemInitVisual(&(render_spu.visuals[i]))) {
		render_spu.numVisuals++;
		return &(render_spu.visuals[i]);
	}
	else {
		crWarning( "Couldn't get a visual, renderspu_SystemInitVisual failed" );
		return NULL;
	}
#else
	render_spu.visuals[0].visAttribs = visAttribs;
	return &(render_spu.visuals[0]);
#endif
}

/*
 * Context functions
 */

GLint RENDER_APIENTRY renderspuCreateContext( const char *dpyName, GLint visBits )
{
	static GLint freeID = 0;
	ContextInfo *context;
	VisualInfo *visual;

	if (!dpyName || crStrlen(render_spu.display_string)>0)
		dpyName = render_spu.display_string;

	visual = renderspuFindVisual( dpyName, visBits );
	if (!visual)
		return -1;

	context = (ContextInfo *) crCalloc(sizeof(ContextInfo));
	if (!context)
		return -1;
	if (!renderspu_SystemCreateContext( visual, context ))
		return -1;

	crHashtableAdd(render_spu.contextTable, freeID, context);
	freeID++;

	return freeID - 1;
}


static void RENDER_APIENTRY renderspuDestroyContext( GLint ctx )
{
	ContextInfo *context;

	CRASSERT(ctx);

	context = (ContextInfo *) crHashtableSearch(render_spu.contextTable, ctx);
	CRASSERT(context);
	renderspu_SystemDestroyContext( context );
	crHashtableDelete(render_spu.contextTable, ctx);
}

void RENDER_APIENTRY renderspuMakeCurrent(GLint crWindow, GLint nativeWindow, GLint ctx)
{
	WindowInfo *window;
	ContextInfo *context;

	window = (WindowInfo *) crHashtableSearch(render_spu.windowTable, crWindow);
	context = (ContextInfo *) crHashtableSearch(render_spu.contextTable, ctx);

	if (window && context)
	{
#ifdef CHROMIUM_THREADSAFE
		crSetTSD(&_RenderTSD, context);
#else
		render_spu.currentContext = context;
#endif
		if (!window)
		{
			crDebug("renderspuMakeCurrent: invalid window id: %d", crWindow);
			return;
		}
		if (!context)
		{
			crDebug("renderspuMakeCurrent: invalid context id: %d", ctx);
			return;
		}

		renderspu_SystemMakeCurrent( /*thread,*/ window, nativeWindow, context );
		if (!context->everCurrent) {
			/* print OpenGL info */
			crDebug( "Render SPU: GL_VENDOR:   %s", render_spu.ws.glGetString( GL_VENDOR ) );
			crDebug( "Render SPU: GL_RENDERER: %s", render_spu.ws.glGetString( GL_RENDERER ) );
			crDebug( "Render SPU: GL_VERSION:  %s", render_spu.ws.glGetString( GL_VERSION ) );
			context->everCurrent = GL_TRUE;
		}
		if (crWindow == 0 && window->mapPending &&
				!render_spu.render_to_app_window) {
			/* Window[0] is special, it's the default window and normally hidden.
			 * If the mapPending flag is set, then we should now make the window
			 * visible.
			 */
			renderspu_SystemShowWindow( window, GL_TRUE );
			window->mapPending = GL_FALSE;
		}
	}
	else
	{
#ifdef CHROMIUM_THREADSAFE
		crSetTSD(&_RenderTSD, NULL);
#else
		render_spu.currentContext = NULL;
#endif
	}
}


/*
 * Window functions
 */

GLint RENDER_APIENTRY renderspuWindowCreate( const char *dpyName, GLint visBits )
{
	static GLint freeID = 0;
	WindowInfo *window;
	VisualInfo *visual;
	GLboolean showIt;
	int i;

    return 0;

	if (!dpyName || crStrlen(render_spu.display_string) > 0)
		dpyName = render_spu.display_string;

	visual = renderspuFindVisual( dpyName, visBits );
	if (!visual)
	{
		crWarning( "Couldn't create a window, renderspuFindVisual returned NULL" );
		return -1;
	}

	/* Allocate WindowInfo */
	window = (WindowInfo *) crCalloc(sizeof(WindowInfo));
	if (!window)
	{
		crWarning( "render SPU: Couldn't create a window" );
		return -1;
	}

	crHashtableAdd(render_spu.windowTable, freeID, window);
	i = freeID;
	freeID++;

	/* Have GLX/WGL create the window */
	if (render_spu.render_to_app_window)
		showIt = 0;
	else
		showIt = i > 0;

	if (!renderspu_SystemCreateWindow( visual, showIt, window ))
	{
		crWarning( "Couldn't create a window, renderspu_SystemCreateWindow failed" );
		return -1;
	}

	return i;
}

static void RENDER_APIENTRY renderspuWindowDestroy( GLint win )
{
	WindowInfo *window;
	CRASSERT(win >= 0);
	window = (WindowInfo *) crHashtableSearch(render_spu.windowTable, win);
	CRASSERT(window);
	renderspu_SystemDestroyWindow( window );
	crHashtableDelete(render_spu.windowTable, win);
}

static void RENDER_APIENTRY renderspuWindowSize( GLint win, GLint w, GLint h )
{
	WindowInfo *window;
	CRASSERT(win >= 0);
	CRASSERT(w > 0);
	CRASSERT(h > 0);
	window = (WindowInfo *) crHashtableSearch(render_spu.windowTable, win);
	if (window)
		renderspu_SystemWindowSize( window, w, h );
}

static void RENDER_APIENTRY renderspuWindowPosition( GLint win, GLint x, GLint y )
{
	WindowInfo *window;
	CRASSERT(win >= 0);
	window = (WindowInfo *) crHashtableSearch(render_spu.windowTable, win);
	if (window)
		renderspu_SystemWindowPosition( window, x, y );
}

/*
 * Set the current raster position to the given window coordinate.
 */
static void SetRasterPos( GLint winX, GLint winY )
{
	GLfloat fx, fy;

	/* Push current matrix mode and viewport attributes */
	render_spu.self.PushAttrib( GL_TRANSFORM_BIT | GL_VIEWPORT_BIT );

	/* Setup projection parameters */
	render_spu.self.MatrixMode( GL_PROJECTION );
	render_spu.self.PushMatrix();
	render_spu.self.LoadIdentity();
	render_spu.self.MatrixMode( GL_MODELVIEW );
	render_spu.self.PushMatrix();
	render_spu.self.LoadIdentity();

	render_spu.self.Viewport( winX - 1, winY - 1, 2, 2 );

	/* set the raster (window) position */
	/* huh ? */
	fx = (GLfloat) (winX - (int) winX);
	fy = (GLfloat) (winY - (int) winY);
	render_spu.self.RasterPos4f( fx, fy, 0.0, 1.0 );

	/* restore matrices, viewport and matrix mode */
	render_spu.self.PopMatrix();
	render_spu.self.MatrixMode( GL_PROJECTION );
	render_spu.self.PopMatrix();

	render_spu.self.PopAttrib();
}


/*
 * Draw the mouse pointer bitmap at (x,y) in window coords.
 */
static void DrawCursor( GLint x, GLint y )
{
#define POINTER_WIDTH   32
#define POINTER_HEIGHT  32
	/* Somebody artistic could probably do better here */
	static const char *pointerImage[POINTER_HEIGHT] =
	{
		"XX..............................",
		"XXXX............................",
		".XXXXX..........................",
		".XXXXXXX........................",
		"..XXXXXXXX......................",
		"..XXXXXXXXXX....................",
		"...XXXXXXXXXXX..................",
		"...XXXXXXXXXXXXX................",
		"....XXXXXXXXXXXXXX..............",
		"....XXXXXXXXXXXXXXXX............",
		".....XXXXXXXXXXXXXXXXX..........",
		".....XXXXXXXXXXXXXXXXXXX........",
		"......XXXXXXXXXXXXXXXXXXXX......",
		"......XXXXXXXXXXXXXXXXXXXXXX....",
		".......XXXXXXXXXXXXXXXXXXXXXXX..",
		".......XXXXXXXXXXXXXXXXXXXXXXXX.",
		"........XXXXXXXXXXXXX...........",
		"........XXXXXXXX.XXXXX..........",
		".........XXXXXX...XXXXX.........",
		".........XXXXX.....XXXXX........",
		"..........XXX.......XXXXX.......",
		"..........XX.........XXXXX......",
		"......................XXXXX.....",
		".......................XXXXX....",
		"........................XXX.....",
		".........................X......",
		"................................",
		"................................",
		"................................",
		"................................",
		"................................",
		"................................"

	};
	static GLubyte pointerBitmap[POINTER_HEIGHT][POINTER_WIDTH / 8];
	static GLboolean firstCall = GL_TRUE;
	GLboolean lighting, depthTest, scissorTest;

	if (firstCall) {
		/* Convert pointerImage into pointerBitmap */
		GLint i, j;
		for (i = 0; i < POINTER_HEIGHT; i++) {
			for (j = 0; j < POINTER_WIDTH; j++) {
				if (pointerImage[POINTER_HEIGHT - i - 1][j] == 'X') {
					GLubyte bit = 128 >> (j & 0x7);
					pointerBitmap[i][j / 8] |= bit;
				}
			}
		}
		firstCall = GL_FALSE;
	}

	render_spu.self.GetBooleanv(GL_LIGHTING, &lighting);
	render_spu.self.GetBooleanv(GL_DEPTH_TEST, &depthTest);
	render_spu.self.GetBooleanv(GL_SCISSOR_TEST, &scissorTest);
	render_spu.self.Disable(GL_LIGHTING);
	render_spu.self.Disable(GL_DEPTH_TEST);
	render_spu.self.Disable(GL_SCISSOR_TEST);
	render_spu.self.PixelStorei(GL_UNPACK_ALIGNMENT, 1);

	render_spu.self.Color3f(1, 1, 1);
	SetRasterPos(x, y);
	render_spu.self.Bitmap(POINTER_WIDTH, POINTER_HEIGHT, 1.0, 31.0, 0, 0,
								(const GLubyte *) pointerBitmap);

	if (lighting)
	   render_spu.self.Enable(GL_LIGHTING);
	if (depthTest)
	   render_spu.self.Enable(GL_DEPTH_TEST);
	if (scissorTest)
		render_spu.self.Enable(GL_SCISSOR_TEST);
}

void RENDER_APIENTRY renderspuSwapBuffers( GLint window, GLint flags )
{
    // don't do anything here :)
    // is not working ??
/*
	WindowInfo *w = (WindowInfo *) crHashtableSearch(render_spu.windowTable, window);

	if (!w)
	{
		crDebug("renderspuSwapBuffers: invalid window id: %d", window);
		return;
	}

	if (flags & CR_SUPPRESS_SWAP_BIT)
	{
		render_spu.self.Finish();
		return;
	}

	if (render_spu.drawCursor)
		DrawCursor( render_spu.cursorX, render_spu.cursorY );

	if (render_spu.swap_master_url)
		__doSync();

	renderspu_SystemSwapBuffers( w, flags );
    */
}


/*
 * Barrier functions
 * Normally, we'll have a crserver somewhere that handles the barrier calls.
 * However, if we're running the render SPU on the client node, then we
 * should handle barriers here.  The threadtest demo illustrates this.
 * If we have N threads calling using this SPU we need these barrier
 * functions to synchronize them.
 */

typedef struct {
	CRbarrier barrier;
	GLuint count;
} Barrier;


static void RENDER_APIENTRY renderspuBarrierCreateCR( GLuint name, GLuint count )
{
	Barrier *b = (Barrier *) crHashtableSearch( render_spu.barrierHash, name );
	if (b) {
		/* HACK -- this allows everybody to create a barrier, and all
           but the first creation are ignored, assuming the count
           match. */
		if ( b->count != count ) {
			crError( "Barrier name=%u created with count=%u, but already "
					 "exists with count=%u", name, count, b->count );
		}
	}
	else {
		b = (Barrier *) crAlloc( sizeof(Barrier) );
		b->count = count;
		crInitBarrier( &b->barrier, count );
		crHashtableAdd( render_spu.barrierHash, name, b );
	}
}

static void RENDER_APIENTRY renderspuBarrierDestroyCR( GLuint name )
{
	crHashtableDelete( render_spu.barrierHash, name );
}

static void RENDER_APIENTRY renderspuBarrierExecCR( GLuint name )
{
	Barrier *b = (Barrier *) crHashtableSearch( render_spu.barrierHash, name );
	if (b) {
		crWaitBarrier( &(b->barrier) );
	}
	else {
		crWarning("Bad barrier name in renderspuBarrierExec()");
	}
}


/*
 * Semaphore functions
 * XXX we should probably implement these too, for the same reason as
 * barriers (see above).
 */

static void RENDER_APIENTRY renderspuSemaphoreCreateCR( GLuint name, GLuint count )
{
	(void) name;
	(void) count;
}

static void RENDER_APIENTRY renderspuSemaphoreDestroyCR( GLuint name )
{
	(void) name;
}

static void RENDER_APIENTRY renderspuSemaphorePCR( GLuint name )
{
	(void) name;
}

static void RENDER_APIENTRY renderspuSemaphoreVCR( GLuint name )
{
	(void) name;
}


/*
 * Misc functions
 */



static void RENDER_APIENTRY renderspuChromiumParameteriCR(GLenum target, GLint value)
{
	(void) target;
	(void) value;


#if 0
	switch (target) 
	{
		default:
			crWarning("Unhandled target in renderspuChromiumParameteriCR()");
			break;
	}
#endif
}

static void RENDER_APIENTRY renderspuChromiumParameterfCR(GLenum target, GLfloat value)
{
	(void) target;
	(void) value;

#if 0
	switch (target) {
	default:
		crWarning("Unhandled target in renderspuChromiumParameterfCR()");
		break;
	}
#endif
}


static void RENDER_APIENTRY renderspuChromiumParametervCR(GLenum target, GLenum type, GLsizei count, const GLvoid *values)
{
	unsigned int client_num;
	unsigned short port;
	CRMessage *msg, pingback;
	unsigned char *privbuf = NULL;

	switch (target) {

		case GL_GATHER_CONNECT_CR:
			if (render_spu.gather_userbuf_size)
				privbuf = (unsigned char *)malloc(1024*768*4);
		
			port = ((GLint *) values)[0];

			if (render_spu.gather_conns == NULL)
				render_spu.gather_conns = crAlloc(render_spu.server->numClients*sizeof(CRConnection *));
			else
			{
				crError("Oh bother! duplicate GL_GATHER_CONNECT_CR getting through");
			}

			for (client_num=0; client_num< render_spu.server->numClients; client_num++)
			{
				switch (render_spu.server->clients[client_num].conn->type)
				{
					case CR_TCPIP:
						crDebug("AcceptClient from %s on %d", 
							render_spu.server->clients[client_num].conn->hostname, render_spu.gather_port);
						render_spu.gather_conns[client_num] = 
								crNetAcceptClient("tcpip", NULL, port, 1024*1024,  1);
						break;
					
					case CR_GM:
						render_spu.gather_conns[client_num] = 
								crNetAcceptClient("gm", NULL, port, 1024*1024,  1);
						break;
						
					default:
						crError("Unknown Network Type to Open Gather Connection");
				}

		
				if (render_spu.gather_userbuf_size)
				{
					render_spu.gather_conns[client_num]->userbuf = privbuf;
					render_spu.gather_conns[client_num]->userbuf_len = render_spu.gather_userbuf_size;
				}
				else
				{
					render_spu.gather_conns[client_num]->userbuf = NULL;
					render_spu.gather_conns[client_num]->userbuf_len = 0;
				}

				if (render_spu.gather_conns[client_num])
				{
					crDebug("success! from %s", render_spu.gather_conns[client_num]->hostname);
				}
			}

			break;

	case GL_CURSOR_POSITION_CR:
		if (type == GL_INT && count == 2) {
			render_spu.cursorX = ((GLint *) values)[0];
			render_spu.cursorY = ((GLint *) values)[1];
		}
		else {
			crWarning("Bad type or count for ChromiumParametervCR(GL_CURSOR_POSITION_CR)");
		}
		break;


	case GL_GATHER_DRAWPIXELS_CR:
		pingback.header.type = CR_MESSAGE_OOB;

		for (client_num=0; client_num< render_spu.server->numClients; client_num++)
		{
			crNetGetMessage(render_spu.gather_conns[client_num], &msg); 
			if (msg->header.type == CR_MESSAGE_GATHER)
			{
				crNetFree(render_spu.gather_conns[client_num], msg);
			}
			else
			{
				crError("expecting MESSAGE_GATHER. got crap! (%d of %d)", client_num, 
								render_spu.server->numClients-1);
			}
		}

		/* 
		 * We're only hitting the case if we're not actually calling 
		 * child.SwapBuffers from readback, so a switch about which
		 * call to __doSync() we really want [this one, or the one
		 * in SwapBuffers above] is not necessary -- karl
		 */
		
		if (render_spu.swap_master_url)
			__doSync();

		for (client_num=0; client_num< render_spu.server->numClients; client_num++)
			crNetSend(render_spu.gather_conns[client_num], NULL, &pingback,
										sizeof(CRMessageHeader));

		render_spu.self.RasterPos2i(((GLint *)values)[0], ((GLint *)values)[1]);
		render_spu.self.DrawPixels(  ((GLint *)values)[2], ((GLint *)values)[3], 
										((GLint *)values)[4], ((GLint *)values)[5], 
									render_spu.gather_conns[0]->userbuf);


		render_spu.self.SwapBuffers(((GLint *)values)[6], 0);
		break;
		

	default:
#if 0
		crWarning("Unhandled target in renderspuChromiumParametervCR(0x%x)", (int) target);
#endif
		break;
	}
}

static void RENDER_APIENTRY renderspuGetChromiumParametervCR(GLenum target, GLuint index, GLenum type, GLsizei count, GLvoid *values)
{
	switch (target) {
	case GL_WINDOW_SIZE_CR:
		{
			GLint w, h, *size = (GLint *) values;
			WindowInfo *window;
			CRASSERT(type == GL_INT);
			CRASSERT(count == 2);
			CRASSERT(values);
			size[0] = size[1] = 1000;  /* default */
            crDebug("set window size to 1000");
            /*
			window = (WindowInfo *) crHashtableSearch(render_spu.windowTable, index);
			if (window)
			{
				renderspu_SystemGetWindowSize(window, &w, &h);
				size[0] = w;
				size[1] = h;
			}
            */
		}
		break;
	default:
		; /* nothing - silence compiler */
	}
}


static void RENDER_APIENTRY renderspuBoundsInfoCR( GLrecti *bounds, GLbyte *payload, GLint
 len, GLint num_opcodes )
{
	(void) bounds;
	(void) payload;
	(void) len;
	(void) num_opcodes;
}

static void RENDER_APIENTRY renderspuWriteback( GLint *writeback )
{
	(void) writeback;
}


static void remove_trailing_space(char *s)
{
	int k = crStrlen(s);
	while (k > 0 && s[k-1] == ' ')
        k--;
    s[k] = 0;
}

static const GLubyte * RENDER_APIENTRY renderspuGetString( GLenum pname )
{
    if (pname == GL_EXTENSIONS)
    {
        const char *nativeExt;
        char *crExt, *s1, *s2;
        if (!render_spu.ws.glGetString)
            return NULL;
          /*
            * XXX Our only dependency on the state tracker is in the use of
            * the __stateExtensionsString, __stateAppOnlyExtensions and
            * __stateChromiumExtensions string.
        */
        nativeExt = (const char *) render_spu.ws.glGetString(GL_EXTENSIONS);
        crExt = crStrjoin3(__stateExtensionString, " ", __stateAppOnlyExtensions);
        s1 = crStrIntersect(nativeExt, crExt);
        remove_trailing_space(s1);
        s2 = crStrjoin3(s1, " ", __stateChromiumExtensions);
        remove_trailing_space(s2);
        crFree(crExt);
        crFree(s1);
        return (const GLubyte *) s2;  /* leak - this never gets freed! */
    
    }else if (pname == GL_VENDOR)
        return (const GLubyte *) CR_VENDOR;
    else if (pname == GL_VERSION)
        return (const GLubyte *) CR_VERSION;
    else if (pname == GL_RENDERER)
        return (const GLubyte *) CR_RENDERER;
    else
        return NULL;
}

/* 
* additional functions to drop or modify some of the more problematic calls.
*/

void RENDER_APIENTRY renderspuClear( GLbitfield mask )
{
    // crDebug("dropping clear call");
}

void RENDER_APIENTRY renderspuLoadIdentity( void )
{
    //crDebug("dropping load identity call");
    GLint mode;
    render_spu.self.GetIntegerv( GL_MATRIX_MODE, &mode );
    if( mode == GL_MODELVIEW)
    {
        glPopMatrix();
        glPushMatrix();
    }
}


void RENDER_APIENTRY renderspuLoadMatrixf( GLfloat *m )
{
    GLint mode;
    render_spu.self.GetIntegerv( GL_MATRIX_MODE, &mode );
    if( mode == GL_PROJECTION )
    {
        // crDebug("Dropped LoadMatrixf on projection matrix");
    }
    else
    {
        glPopMatrix();
        glPushMatrix();        
        glMultMatrixf( m );
    }
}

void RENDER_APIENTRY renderspuLoadMatrixd( GLdouble *m )
{
    GLint mode;
    render_spu.self.GetIntegerv( GL_MATRIX_MODE, &mode );
    if( mode == GL_PROJECTION )
    {
        // crDebug("Dropped LoadMatrixd on projection matrix");
    }
    else
    {
        glPopMatrix();
        glPushMatrix();                
        glMultMatrixd( m );
    }
}

void RENDER_APIENTRY renderspuMultMatrixf( GLfloat *m )
{
    GLint mode;
    render_spu.self.GetIntegerv( GL_MATRIX_MODE, &mode );
    if( mode == GL_PROJECTION )
    {
        //crDebug("Dropped MultMatrixf on projection matrix");
    }
    else
    {
        glMultMatrixf( m );
    }
}

void RENDER_APIENTRY renderspuMultMatrixd( GLdouble *m )
{
    GLint mode;
    render_spu.self.GetIntegerv( GL_MATRIX_MODE, &mode );
    if( mode == GL_PROJECTION )
    {
        //crDebug("Dropped MultMatrixd on projection matrix");
    }
    else
    {
        glMultMatrixd( m );
    }
}

void RENDER_APIENTRY renderspuOrtho( GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar )
{
    GLint mode;
    render_spu.self.GetIntegerv( GL_MATRIX_MODE, &mode );
    if( mode == GL_PROJECTION )
    {
        //crDebug("Dropped Ortho on projection matrix");
    }
    else
    {
        glOrtho( left, right, bottom, top, zNear, zFar );
    }
}

void RENDER_APIENTRY renderspuFrustum( GLdouble left, GLdouble right, GLdouble bottom,
                                        GLdouble top, GLdouble znear, GLdouble zfar )
{
    GLint mode;
    render_spu.self.GetIntegerv( GL_MATRIX_MODE, &mode );
    if( mode == GL_PROJECTION )
    {
        //crDebug("Dropped Frustum on projection matrix");
    }
    else
    {
        glFrustum( left, right, bottom, top, znear, zfar );
    }
}

void RENDER_APIENTRY renderspuEnable( GLenum cap )
{  
    switch(cap) 
    {
    case GL_DEPTH_TEST:
        if(getChromiumState(render_spu.server)->depthTest == GL_TRUE)
        {
            glEnable( cap );
        }
        else
        {
            crDebug("glEnable(GL_DEPTH_TEST) disallowed");
        }
        break;
    case GL_ALPHA_TEST:
        if(getChromiumState(render_spu.server)->alphaTest == GL_TRUE)
        {
            glEnable( cap );
        }
        else
        {
            crDebug("glEnable(GL_ALPHA_TEST) disallowed");
        }
        break;
    case GL_BLEND:
        if(getChromiumState(render_spu.server)->blend == GL_TRUE)
        {
            glEnable( cap );
        }
        else
        {
            crDebug("glEnable(GL_BLEND) disallowed");
        }
        break;
    case GL_CULL_FACE:
        if(getChromiumState(render_spu.server)->cullFace == GL_TRUE)
        {
            glEnable( cap );
        }
        else
        {
            crDebug("glEnable(GL_CULL_FACE) disallowed");
        }
        break;
    case GL_LINE_SMOOTH :
        if(getChromiumState(render_spu.server)->lineSmooth == GL_TRUE)
        {
            glEnable( cap );
        }
        else
        {
            crDebug("glEnable(GL_LINE_SMOOTH) disallowed");
        }
        break;
    case GL_FOG:
        if(getChromiumState(render_spu.server)->fog == GL_TRUE)
        {
            glEnable( cap );
        }
        else
        {
            crDebug("glEnable(GL_FOG) disallowed");
        }
        break;
    case GL_SCISSOR_TEST:
        // crDebug("glEnable(GL_SCISSOR_TEST) disallowed");
        break;    
    default :
        glEnable( cap );
    }
}

void RENDER_APIENTRY renderspuDisable( GLenum cap )
{  
    switch(cap) 
    {
    case GL_DEPTH_TEST:
        if(getChromiumState(render_spu.server)->depthTest == GL_TRUE)
        {
            glDisable( cap );
        }
        else
        {
            crDebug("glDisable(GL_DEPTH_TEST) disallowed");
        }
        break;
    case GL_ALPHA_TEST:
        if(getChromiumState(render_spu.server)->alphaTest == GL_TRUE)
        {
            glDisable( cap );
        }
        else
        {
            crDebug("glDisable(GL_ALPHA_TEST) disallowed");
        }
        break;
    case GL_BLEND:
        if(getChromiumState(render_spu.server)->blend == GL_TRUE)
        {
            glDisable( cap );
        }
        else
        {
            crDebug("glDisable(GL_BLEND) disallowed");
        }
        break;
    case GL_CULL_FACE:
        if(getChromiumState(render_spu.server)->cullFace == GL_TRUE)
        {
            glDisable( cap );
        }
        else
        {
            crDebug("glDisable(GL_CULL_FACE) disallowed");
        }
        break;
    case GL_LINE_SMOOTH :
        if(getChromiumState(render_spu.server)->lineSmooth == GL_TRUE)
        {
            glDisable( cap );
        }
        else
        {
            crDebug("glDisable(GL_LINE_SMOOTH) disallowed");
        }
        break;
    case GL_FOG:
        if(getChromiumState(render_spu.server)->fog == GL_TRUE)
        {
            glDisable( cap );
        }
        else
        {
            crDebug("glDisable(GL_FOG) disallowed");
        }
        break;
    default :
        glDisable( cap );
    }
}

void RENDER_APIENTRY renderspuViewport( GLint x, GLint y, GLsizei width, GLsizei height)
{
    //crDebug("glViewport dropped");
}

void RENDER_APIENTRY renderspuScissor( GLint x, GLint y, GLsizei width, GLsizei height )
{
    //crDebug( "glScissor dropped" );
}

#define FILLIN( NAME, FUNC ) \
  table[i].name = crStrdup(NAME); \
  table[i].fn = (SPUGenericFunction) FUNC; \
  i++;


/* These are the functions which the render SPU implements, not OpenGL.
 */
int renderspuCreateFunctions( SPUNamedFunctionTable table[] )
{
	int i = 0;
	FILLIN( "SwapBuffers", renderspuSwapBuffers );
	FILLIN( "CreateContext", renderspuCreateContext );
	FILLIN( "DestroyContext", renderspuDestroyContext );
	FILLIN( "MakeCurrent", renderspuMakeCurrent );
	FILLIN( "WindowCreate", renderspuWindowCreate );
	FILLIN( "WindowDestroy", renderspuWindowDestroy );
	FILLIN( "WindowSize", renderspuWindowSize );
	FILLIN( "WindowPosition", renderspuWindowPosition );
	FILLIN( "BarrierCreateCR", renderspuBarrierCreateCR );
	FILLIN( "BarrierDestroyCR", renderspuBarrierDestroyCR );
	FILLIN( "BarrierExecCR", renderspuBarrierExecCR );
	FILLIN( "BoundsInfoCR", renderspuBoundsInfoCR );
	FILLIN( "SemaphoreCreateCR", renderspuSemaphoreCreateCR );
	FILLIN( "SemaphoreDestroyCR", renderspuSemaphoreDestroyCR );
	FILLIN( "SemaphorePCR", renderspuSemaphorePCR );
	FILLIN( "SemaphoreVCR", renderspuSemaphoreVCR );
	FILLIN( "Writeback", renderspuWriteback );
	FILLIN( "ChromiumParameteriCR", renderspuChromiumParameteriCR );
	FILLIN( "ChromiumParameterfCR", renderspuChromiumParameterfCR );
	FILLIN( "ChromiumParametervCR", renderspuChromiumParametervCR );
	FILLIN( "GetChromiumParametervCR", renderspuGetChromiumParametervCR );
//	FILLIN( "GetString", renderspuGetString );
    FILLIN( "Clear", renderspuClear);
    FILLIN( "LoadIdentity", renderspuLoadIdentity);
    FILLIN( "MultMatrixd", renderspuMultMatrixd );
    FILLIN( "MultMatrixf", renderspuMultMatrixf );
    FILLIN( "LoadMatrixd", renderspuLoadMatrixd );
    FILLIN( "LoadMatrixf", renderspuLoadMatrixf );
    FILLIN( "Ortho", renderspuOrtho );
    FILLIN( "Frustum", renderspuFrustum );
    FILLIN( "Enable", renderspuEnable );
    FILLIN( "Disable", renderspuDisable );
    FILLIN( "Viewport", renderspuViewport );
    FILLIN( "Scissor", renderspuScissor );
    
    return i;
}
