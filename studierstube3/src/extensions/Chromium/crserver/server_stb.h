#ifndef _SERVER_STB_H
#define _SERVER_STB_H

#include "../chromiumext.h"
#include "server.h"

typedef struct {
    GLboolean depthTest;
    GLboolean alphaTest;
    GLboolean stencilTest;
    GLboolean blend;
    GLboolean fog;
    GLboolean lineSmooth;   
    GLboolean cullFace;
} ChromiumState;

/** 
 * a wrapper interface around basic setup and bookkeeping operations for the
 * Chromium library. It initializes it, closes it and manages a static store of
 * ChromiumState structures to pass information between the Chromium node and
 * the oiv render spu. This serves to decouple the oiv render spu from any
 * actual openinventor headers etc :).
 *
 * @author Gerhard Reitmayr
 */

CHROMIUM_API CRServer * initChromium(const char * mothership);
CHROMIUM_API void closeChromium(const CRServer * server);
CHROMIUM_API void setChromiumState(const CRServer * server, ChromiumState * state);
CHROMIUM_API ChromiumState * getChromiumState(const CRServer * server);
CHROMIUM_API void signalSwapBuffer(const CRServer * server);
CHROMIUM_API GLboolean isSwapBufferCalled( const CRServer * server);
CHROMIUM_API void clearSwapBuffer(const CRServer * server);

#endif