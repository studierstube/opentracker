/* ===========================================================================
//  (C) 1999 Vienna University of Technology
// ===========================================================================
//  NAME:       stbFifo.h
//  TYPE:       c code
//  PROJECT:    Studierstube
//  CONTENT:    defines and prototypes for the FIFO communication
//  VERSION:    2.0
// ===========================================================================
//  AUTHORS:    krutz     Markus Krutz
//              peci      Clemens Pecinovsky
// ===========================================================================
//  HISTORY:
//    the original file by krutz was x3fifo.h
//
//  24-sep-99 08:00:00  krutz     created (as x3fifo.h)
//  21-apr-99 08:00:00  peci      modified
// ===========================================================================  */
 
#ifndef _STBFIFO_H
#define _STBFIFO_H

#include "stbComm.h"
#define STBFIFO_PATH "/usr/cg/avs/bin/avs"
/****************************************************************************
message id's
****************************************************************************/
#define STBFIFO_REGISTER_MODULE     STB_REGISTER_MODULE
#define STBFIFO_UNREGISTER_MODULE   STB_UNREGISTER_MODULE
#define STBFIFO_EVENT_HANDLED       'h'
#define STBFIFO_UPDATE_MODULE       STB_UPDATE_MODULE
#define STBFIFO_GEOM_MODULE         STB_GEOMETRY

/****************************************************************************
message format
****************************************************************************/
/*
typedef struct {
	int msg_type;
	int msg_length;
	char *msg_data;
} stbFifoMsg;
*/
#define stbFifoMsg stbMessage
/****************************************************************************
ipc information block
****************************************************************************/
typedef struct {
  int semid;
  int shmid;
  char *shmaddr;
} STBFIFO_IPC_INFO;

#define MODE_READ       0
#define MODE_WRITE      1

/****************************************************************************
prototype:      int stbFifoCreate(int key);
purpose:        Create FIFO.
args:           key ... key, used to compose fifo-name ("/tmp/fifo%d", key)
return: TRUE on success, else FALSE.
notes:  ---
****************************************************************************/
int stbFifoCreate(int key);

/****************************************************************************
prototype:      int stbFifoRemove(int key);
purpose:        Remove FIFO.
args:           key ... key, see StbFifoCreate
return: TRUE on success, else FALSE.
notes:  A Stb module can't call this function, since there's no destruction
		function available for coroutines.
****************************************************************************/
int stbFifoRemove(int key);


/****************************************************************************
prototype:      int stbFifoOpen(int key, int mode);
purpose:        Open FIFO.
args:           key ... key, see StbFifoCreate
		mode ... MODE_READ or MODE_WRITE
return: File descriptor on success, else -1.
notes:  ---
****************************************************************************/
int stbFifoOpen(int key, int mode);

/****************************************************************************
prototype:      void stbFifoClose(int fd);
purpose:        Close FIFO.
args:           fd ... file descriptor of FIFO
return: ---
notes:  ---
****************************************************************************/
void stbFifoClose(int fd);


/****************************************************************************
prototype:      int stbFifoSend(int fd, stbFifoMsg *msg);
purpose:        Send a message via a FIFO.
args:           fd ... file descriptor of FIFO
		msg ... message to be sent
return: TRUE on success, else FALSE.
notes:  Atomic, if message size is smaller than 4K.
****************************************************************************/
int stbFifoSend(int fd, stbFifoMsg *msg);

/****************************************************************************
prototype:      stbFifoMsg *StbFifoReceive(int fd);
purpose:        Receive a message via a FIFO.
args:           fd ... file descriptor of FIFO
return: StbFifoReceive returns a pointer to the received message, or NULL in
		case of an error.
notes:  Blocking.
****************************************************************************/
stbFifoMsg *stbFifoReceive(int fd);


/****************************************************************************
prototype:      int StbFifoCreateID(STBFIFO_IPC_INFO *info);
purpose:        Create a unique id among all Stb Modules.
args:           info ... pointer to structure, where the ipc information will be stored
return: StbFifoCreateID returns an appropriate id, if successful, else -1.
notes:  Uses one semaphore and one shared memory segment. For each successfully
		created id, an id counter is incremented (see StbFifoDestroyID).
****************************************************************************/
int stbFifoCreateID(STBFIFO_IPC_INFO *info);

/****************************************************************************
prototype:      int StbFifoDestroyID(STBFIFO_IPC_INFO *info);
purpose:        Free id created by StbFifoCreateID.
args:           info ... pointer to an ipc information block
return: ?
notes:  Decrements the id counter. If this counter reaches zero,
		all involved ipc constructs are removed.
****************************************************************************/
int stbFifoDestroyID(STBFIFO_IPC_INFO *info);


#endif

