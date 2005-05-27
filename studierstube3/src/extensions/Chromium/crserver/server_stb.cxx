#ifdef WIN32
#pragma warning(disable:4786)
#endif

#include <map>
#include <string>

using namespace std;

extern "C" {
#include "server_stb.h"
#include "server.h"
#include "cr_net.h"
#include "cr_unpack.h"
#include "cr_error.h"
#include "cr_glstate.h"
#include "cr_string.h"
#include "cr_mem.h"
#include <signal.h>
#include <stdlib.h>    

void crServerClose( unsigned int id );
void crServerCleanup( int sigio );        
void crServerInitializeTiling(void);
}

map<CRServer *, ChromiumState> stateMap;
map<CRServer *, GLboolean> swapFlagMap;


CRServer * initChromium(const char *mymothership)
{
    unsigned int j;
   
    signal( SIGTERM, crServerCleanup );
#ifndef WINDOWS
    signal( SIGPIPE, crServerCleanup );
#endif
    
    crNetInit(crServerRecv, crServerClose);
    crStateInit();
    
    /* default alignment is the identity */
    crMemset(cr_server.alignment_matrix, 0, 16*sizeof(GLfloat));
    cr_server.alignment_matrix[0]  = 1;
    cr_server.alignment_matrix[5]  = 1;
    cr_server.alignment_matrix[10] = 1;
    cr_server.alignment_matrix[15] = 1;
    
    crServerGatherConfiguration((char *)mymothership);
    
    crStateLimitsInit( &(cr_server.limits) );
    
    for (j = 0 ; j < cr_server.numClients ; j++)
    {
        crServerAddToRunQueue( &cr_server.clients[j] );
    }
    
    crServerInitializeTiling();
    
    crServerInitDispatch();
    crStateDiffAPI( &(cr_server.head_spu->dispatch_table) );
    
    crUnpackSetReturnPointer( &(cr_server.return_ptr) );
    crUnpackSetWritebackPointer( &(cr_server.writeback_ptr) );
    
    cr_barriers = crAllocHashtable();
    cr_semaphores = crAllocHashtable();

    return & cr_server;
}

void closeChromium(const CRServer * server)
{
}

ChromiumState * getChromiumState(const CRServer * server)
{
    if( stateMap.find((CRServer *)server) == stateMap.end())
    {
        ChromiumState & state = stateMap[(CRServer *)server];
        state.alphaTest = GL_TRUE;
        state.blend = GL_TRUE;
        state.depthTest = GL_TRUE;
        state.fog = GL_TRUE;
        state.lineSmooth = GL_TRUE;
        state.stencilTest = GL_TRUE;
        state.cullFace = GL_TRUE;
    }
    return & stateMap[(CRServer *)server];
}

void signalSwapBuffer(const CRServer * server)
{
    swapFlagMap[(CRServer *)server] = GL_TRUE;
}

GLboolean isSwapBufferCalled( const CRServer * server)
{
    return swapFlagMap[(CRServer *)server];
}

void clearSwapBuffer(const CRServer * server)
{
    swapFlagMap[(CRServer *)server] = GL_FALSE;
}
