/* Copyright (c) 2001, Stanford University
 * All rights reserved
 *
 * See the file LICENSE.txt for information on redistributing this software.
 */

#include "cr_mem.h"
#include "cr_spu.h"
#include "cr_error.h"
#include "cr_string.h"
#include "cr_mothership.h"
#include "cr_url.h"
#include "renderspu.h"
#include <stdio.h>

static SPUNamedFunctionTable render_table[1000];

SPUFunctions render_functions = {
	NULL, /* CHILD COPY */
	NULL, /* DATA */
	render_table /* THE ACTUAL FUNCTIONS */
};

RenderSPU render_spu;

#ifdef CHROMIUM_THREADSAFE
CRtsd _RenderTSD;
#endif

void swapsyncConnect()
{
	char hostname[4096], protocol[4096];
	unsigned short port;

	crNetInit(NULL, NULL);

	if (!crParseURL( render_spu.swap_master_url, protocol, hostname, 
					&port, 9876))
		crError( "Bad URL: %s", render_spu.swap_master_url );

	if (render_spu.is_swap_master)
	{
		int a;

		render_spu.swap_conns = (CRConnection **)crAlloc(
						render_spu.num_swap_clients*sizeof(CRConnection *));
		for (a=0; a<render_spu.num_swap_clients; a++)
		{
			render_spu.swap_conns[a] = crNetAcceptClient( protocol, hostname, port,
														render_spu.swap_mtu, 1);
		}
	}
	else
	{
		render_spu.swap_conns = (CRConnection **)crAlloc(sizeof(CRConnection *));

		render_spu.swap_conns[0] = crNetConnectToServer(render_spu.swap_master_url,
									port, render_spu.swap_mtu, 1);
		if (!render_spu.swap_conns[0])
			crError("Failed connection");
	}
}


SPUFunctions *renderSPUInit( int id, SPU *child, SPU *self,
		unsigned int context_id, unsigned int num_contexts )
{
	int numFuncs, numSpecial;
	/* Don't ask for ALPHA, if we don't have it, we fail immediately */
	const GLuint visualBits = CR_RGB_BIT | CR_DEPTH_BIT | CR_DOUBLE_BIT | CR_STENCIL_BIT | CR_ALPHA_BIT;

	(void) child;
	(void) context_id;
	(void) num_contexts;

	self->privatePtr = (void *) &render_spu;

#ifdef CHROMIUM_THREADSAFE
	crDebug("Render SPU: thread-safe");
#endif

	crMemZero(&render_spu, sizeof(render_spu));

	render_spu.id = id;
	renderspuGatherConfiguration(&render_spu);
	if (render_spu.swap_master_url)
		swapsyncConnect();


	/* Get our special functions. */
	numSpecial = renderspuCreateFunctions( render_table );

	/* Get the OpenGL functions. */
	numFuncs = crLoadOpenGL( &render_spu.ws, render_table + numSpecial );
	if (numFuncs == 0) {
		crError("The render SPU was unable to load the native OpenGL library");
		return NULL;
	}

	numFuncs += numSpecial;

	render_spu.contextTable = crAllocHashtable();
	render_spu.windowTable = crAllocHashtable();

	/*
	 * Get the OpenGL extension functions.
	 * SIGH -- we have to wait until the very bitter end to load the 
	 * extensions, because the context has to be bound before
	 * wglGetProcAddress will work correctly.  No such issue with GLX though.
	 */
	numFuncs += crLoadOpenGLExtensions( &render_spu.ws, render_table + numFuncs );
	CRASSERT(numFuncs < 1000);

	render_spu.barrierHash = crAllocHashtable();

	render_spu.cursorX = 0;
	render_spu.cursorY = 0;
	render_spu.use_L2 = 0;

	render_spu.gather_conns = NULL;

	return &render_functions;
}

void renderSPUSelfDispatch(SPUDispatchTable *self)
{
	crSPUInitDispatchTable( &(render_spu.self) );
	crSPUCopyDispatchTable( &(render_spu.self), self );

	render_spu.server = (CRServer *)(self->server);
}

int renderSPUCleanup(void)
{
	return 1;
}


extern SPUOptions renderSPUOptions[];

int SPULoad( char **name, char **super, SPUInitFuncPtr *init,
	     SPUSelfDispatchFuncPtr *self, SPUCleanupFuncPtr *cleanup,
	     SPUOptionsPtr *options, int *flags )
{
	*name = "oiv render";
	*super = NULL;
	*init = renderSPUInit;
	*self = renderSPUSelfDispatch;
	*cleanup = renderSPUCleanup;
	*options = renderSPUOptions;
	*flags = (SPU_NO_PACKER|SPU_IS_TERMINAL|SPU_MAX_SERVERS_ZERO);
	
	return 1;
}
