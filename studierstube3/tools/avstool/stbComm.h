/* ===========================================================================
//  (C) 1999 Vienna University of Technology
// ===========================================================================
//  NAME:       stbComm.h
//  TYPE:       c code
//  PROJECT:    Studierstube
//  CONTENT:    defines and prototypes for the network communication
//              this file includes server and client routines
//              and is also included in the Stb-Version 
//  VERSION:    2.0
// ===========================================================================
//  AUTHORS:    peci      Clemens Pecinovsky
// ===========================================================================
//  HISTORY:
//
//  21-apr-99 08:00:00  peci      created
// ===========================================================================  */

#ifndef _STB_COMM_H
#define _STB_COMM_H

#define SERVER_HOST     "knieriem"/*"localhost"*/
#define SERVER_PORT 	"33335"
#define SERVER_ENV	"STBSERVER"

#define MAX_MSGLEN      1024
/****************************************************************************
message identifiers
****************************************************************************/
#define STB_LOGIN	         1
#define STB_LOGIN_ACK	         2
#define STB_LOGOUT	         3
#define STB_STILLRUNNING	 5
#define STB_STILLRUNNING_ACK     6
#define STB_SERVERQUIT	         9

#define STB_REGISTER_MODULE     10
#define STB_UNREGISTER_MODULE   13
#define STB_UPDATE_MODULE   	14

#define STB_GEOMETRY            20

#define STB_GEOMETRY_TRANSFORM 	23

/*------------------------------------------------------------------------------
//structure used for storing Information of the current network Connection       */
typedef struct _NetCom
{
	char *host;
	int  port;
	int socket;
	int isConnected;
} stbNetCom;


/*------------------------------------------------------------------------------
//Message structure used for sending and receiving                               */
typedef struct _Message
{
        int moduleId;
	int type;       /* id */
	int length;     /* body length */
	unsigned char *data;     /* body */
} stbMessage;


/*------------------------------------------------------------------------------
//initialized the stbNetCom for the client                                      */
void stbNCInit(char *hostName,char *portName,stbNetCom *nc);

/*------------------------------------------------------------------------------
//initialized the stbNetCom for the server, waits for connection (blocking!!)   */
int stbNCServerInit(char *Cport,stbNetCom *nc);

/*------------------------------------------------------------------------------
//sends a STB_SERVERQUIT message and closes the connection, used by the server  */
int stbNCServerClose(stbNetCom *nc);

/*------------------------------------------------------------------------------
//connects the client to the server                                             */
int stbNCConnect(char *Chost,char *Cport,int moduleId,char *param,stbNetCom *nc);

/*------------------------------------------------------------------------------
//sends a STB_LOGOUT message and closes the connection, used by the client      */
int stbNCDisconnect(int moduleId,stbNetCom *nc);

/*------------------------------------------------------------------------------
//simply closes the connection (server and client)                              */
int stbNCClose(stbNetCom *nc);

/*------------------------------------------------------------------------------
//this routine is used to check if the connection is alive                      */
int stbNCCheckConnection(stbNetCom *nc);

/*------------------------------------------------------------------------------
//this routine is used to send the ack, to tell that the connection is alive    */
int stbNCSendCheckConnectionAck(stbNetCom *nc);


/*------------------------------------------------------------------------------
//sends the data                                                                */
int stbNCSend(stbNetCom *nc,int modulId,int type,int length,unsigned char *data);

/*------------------------------------------------------------------------------
//sends the Msg                                                                 */
int stbNCSendMsg(stbNetCom *nc,stbMessage *msg);

/*------------------------------------------------------------------------------
//receives the Msg (blocking)                                                   */
int stbNCReceiveMsg(stbNetCom *nc,stbMessage *msg);

/*------------------------------------------------------------------------------
//receives the Msg (nonblocking)                                                */
int stbNCReceiveMsgNoWait(stbNetCom *nc,stbMessage *msg);

/*------------------------------------------------------------------------------
//routines to Put and Get data types into and from a data-string                */
int   stbNCGetInt(char **data);
void  stbNCPutInt(char **data,int x);
float stbNCGetFloat(char **data);
void  stbNCPutFloat(char **data,float x);
char  stbNCGetChar(char **data);
void  stbNCPutChar(char **data,char x);
char* stbNCGetString(char **data);
void  stbNCPutString(char **data,char *str);

/*------------------------------------------------------------------------------
//replaces all characters s with r in the string str                            */
void strChrReplace(char *str,char s,char r);

#endif
