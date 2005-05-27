/* ===========================================================================
    (C) 1999 Vienna University of Technology
   ===========================================================================
    NAME:       stbComm.c
    TYPE:       c code
    PROJECT:    Integrating Scientific Visualizations into STB Workspace
    CONTENT:    network communication routines
    VERSION:    2.4
   ===========================================================================
    AUTHORS:    peci    Clemens Pecinovsky
                jp      Jan Prikryl
   ===========================================================================
    HISTORY:

    08-Nov-00 17:14:26  jp      last modification
    21-apr-99 08:00:00  peci    created
=========================================================================== */

#define NODEBUG

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <errno.h>

#ifdef WIN32
#include <windows.h>
#include <winsock.h>
#else
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <limits.h>
#include <netdb.h>
#include <arpa/inet.h>
#endif

#include "stb.h"
#include "stbComm.h"

/*------------------------------------------------------------------------------
// myatoport
//
// Take a service name, and a service type, and return a port number.  If the
// service name is not found, it tries it as a decimal number.  The number
// returned is byte ordered for the network.                                     
//
//Parameter:
// service: the service type
// proto:   the protocol type ("tcp" or "UDP")
//
//returns:
// the port, or -1 if an error occured                                          */
int myatoport(const char *service, const char *proto)
{
  int port;
  long int lport;
  struct servent *serv;
  char *errpos;

  /* First try to read it from /etc/services */
  serv = getservbyname(service, proto);
  if (serv != NULL)
    port = serv->s_port;
  else { /* Not in services, maybe a number? */
    lport = strtol(service,&errpos,0);
    if ( (errpos[0] != 0) || (lport < 1) )
      return -1; /* Invalid port address */
    port = lport; /*htonl(lport); */
  }
  return port;
}

/*------------------------------------------------------------------------------
// myatoaddr
//
// Converts ascii text to in_addr struct.  NULL is returned if the address
// can not be found.
//
//Parameter:
// address: the domain name or the ip adress in string format...
//
//returns:
// the struct in_addr of the adress                                             */
struct in_addr *myatoaddr(const char *address)
{
  struct hostent *host;
  static struct in_addr saddr;

  /* First try it as aaa.bbb.ccc.ddd. */
  saddr.s_addr = inet_addr(address);
  if (saddr.s_addr != -1) {
    return &saddr;
  }
  host = gethostbyname(address);
  if (host != NULL) {
    return (struct in_addr *) *host->h_addr_list;
  }
  return NULL;
}


/*------------------------------------------------------------------------------
// stbNCInit
//
// initialized the stbNetCom structure.
// this function is used by the client.
//
//Parameter:
// hostName: domain name of the host
// portName: port or service
// nc: a pointer to the stbNetCom structure which has to be initialized
//     this should be a valid pointer, the structure is not allocated here..     */
void stbNCInit(char *hostName,char *portName,stbNetCom *nc)
{
  Debug(printf("stbNCInit\n"));
  nc->isConnected=FALSE;
  nc->socket=0;

  
  nc->port = myatoport(portName,"tcp");
  if (nc->port < 0) {
    fprintf(stderr,"unable to find port %s.\n", portName);
    exit(-1);
  }
  nc->host=hostName;
}

/*------------------------------------------------------------------------------
// stbNCServerInit
//
// initialized the stbNetCom structure and waits for a connection.
// this function is used by the server and is blocking...
//
//Parameter:
// Cport: port or service
// nc: a pointer to the stbNetCom structure which has to be initialized
//     this should be a valid pointer, the structure is not allocated here..     */
int stbNCServerInit(char *Cport,stbNetCom *nc)
{
  struct sockaddr_in sin;
  struct sockaddr_in sout;
  int s,one;
#ifndef _LINUX_SOURCE
  int cl;
#else
  socklen_t cl;
#endif

  s = socket(AF_INET, SOCK_STREAM, 0);
  if ((nc->port=myatoport(Cport,"tcp"))==-1) return FALSE; 
  
  one=1;
#ifndef WIN32
  /*********************** unix code */
  setsockopt( s, SOL_SOCKET, SO_REUSEADDR,  &one, sizeof(one));
#else
  /*********************** WIN32 code */
  {
    u_long para=FALSE;
	ioctlsocket(nc->socket,FIONBIO,&para);
    setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (char *)&one, sizeof(one));
  }
#endif

  memset((void *) &sin,0,sizeof(sin));

  sin.sin_family      = AF_INET;
  sin.sin_addr.s_addr = htonl(INADDR_ANY);
  sin.sin_port        = htons(nc->port);
  bind(s, (struct sockaddr *)&sin, sizeof(sin));

  listen(s,100);

  memset((void *)&sout,0,sizeof(struct sockaddr_in));
  cl=sizeof(sout);
  nc->socket=accept(s,(struct sockaddr *)&sout, &cl);

  one=1;
#ifndef WIN32
  /*********************** unix code */
  setsockopt( nc->socket, SOL_SOCKET, SO_REUSEADDR,  &one, sizeof(one));
#else
  /*********************** WIN32 code */
  {
    u_long para=FALSE;
	ioctlsocket(nc->socket,FIONBIO,&para);
    setsockopt(nc->socket, SOL_SOCKET, SO_REUSEADDR, (char *)&one, sizeof(one));
  }
#endif
#ifdef WIN32
   closesocket(s);
#else
   close(s);
#endif

  nc->isConnected=TRUE;
  return TRUE;
}

int stbNCServerClose(stbNetCom *nc)
{
  if (nc->isConnected)
  {
    stbNCSend(nc,0,STB_SERVERQUIT,0,NULL);
    return (stbNCClose(nc));
  }
  
  return FALSE;

}


/*------------------------------------------------------------------------------
// myRead
//
// does the same as the ANSI read function
// but tries to read the whole length
//
//Parameter:
// socket:   the socket from which to read
// data:     a pointer to the data, has to be allocated
// len:      number of bytes which has to be red
// dontwait: should the read be blocking or not
//
//returns:
// the number of bytes stored into the buffer, or a value <0 if an error occured */
int myRead(int socket,void *data,int len,int dontwait)
{
  int readLen,recvLen;
  char *curptr;
  
  curptr=(char *)data;

#ifdef WIN32
  if (dontwait)
  {
    u_long para=TRUE;
    ioctlsocket(socket,FIONBIO,&para);
  }
#endif

  readLen=len;
  while (readLen>0)
  {
#ifdef WIN32
    recvLen=recv(socket,curptr,readLen,0);
#else
    if (dontwait)
    {
      recvLen=recv(socket,curptr,readLen,MSG_DONTWAIT);
    }
    else
    {
      recvLen=recv(socket,curptr,readLen,0);
    }
#endif
    if (recvLen<=0) return recvLen;
    curptr+=recvLen;
    readLen-=recvLen;
  }

#ifdef WIN32
  if (dontwait)
  {
    u_long para=FALSE;
    ioctlsocket(socket,FIONBIO,&para);
  }
#endif


  return len;
}


/*------------------------------------------------------------------------------
// stbNCConnect
//
// initializes the stbNetCom structure,
// and connects to the server
// and sends the first message
//
//Parameter:
// Chost:    the hostname
// Cport:    the port or service-name
// moduleId: the ModuleId of the first message
// para:     the data of the first message
// nc: a pointer to the stbNetCom structure 
//     this should be a valid pointer, the structure is not allocated here..     
//
//returns:
// TRUE if the connection is etablshed or FALSE otherwhise                      */
int stbNCConnect(char *Chost,char *Cport,int moduleId,char *para,stbNetCom *nc)
{
  struct sockaddr_in sin;
  stbMessage msg;
  char *buffer,*tmp;
  int one;
  int size;

  if (nc->isConnected)
  {
    return FALSE;
  }

  /* set the stbNetCom structure */
  nc->host=Chost;
  if ((nc->port=myatoport(Cport,"tcp"))==-1) return FALSE;

  /* initializes the socket */  
  if ((nc->socket=socket(AF_INET, SOCK_STREAM, 0))==-1) return FALSE;
  one=1;
#ifndef WIN32
  /*********************** unix code */
  setsockopt( nc->socket, SOL_SOCKET, SO_REUSEADDR,  &one, sizeof(one));
#else
  /*********************** WIN32 code */
  {
    u_long para=FALSE;
	ioctlsocket(nc->socket,FIONBIO,&para);
    setsockopt(nc->socket, SOL_SOCKET, SO_REUSEADDR, (char *)&one, sizeof(one));
  }
#endif

  /* initializes the sockaddr_in structure */  
  memset((void *) &sin,0,sizeof(sin));
  sin.sin_family = AF_INET;
  sin.sin_port = htons(nc->port);
  sin.sin_addr.s_addr = myatoaddr(nc->host)->s_addr;

  /* does the connect */  
  Debug(printf("connect %d %s\n",nc->port,nc->host)); 
  if ((connect(nc->socket,(const struct sockaddr *)&sin, sizeof(sin)))==-1) return FALSE;
  nc->isConnected=TRUE;

  /* prepares the LOGIN packet */  
  size=strlen(STB_ID)+strlen(para)+2+sizeof(int);
  buffer=(char *) malloc(size);

  memcpy(buffer,STB_ID,strlen(STB_ID)+1);
  tmp=buffer+strlen(STB_ID)+1;
  stbNCPutString(&tmp,para);    

  /* sends the login packet */
  if (!stbNCSend(nc,moduleId,STB_LOGIN,size,(unsigned char *)buffer))
  {
    nc->isConnected=FALSE;
    return FALSE;
  }

  /* waits for the Acknowledge packet from the server */
  stbNCReceiveMsg(nc,&msg);
  if (msg.type!=STB_LOGIN_ACK)
  {
    nc->isConnected=FALSE;
    return FALSE;
  }

  /* everything worked fine... */
  nc->isConnected=TRUE;
  return TRUE;
}


/*------------------------------------------------------------------------------
// stbNCDisconnect
//
// sends a STB_LOGOUT message and closes the connection, used by the client
//
//Parameter:
// moduleId: the ModuleId of the client
// nc:       a pointer to the stbNetCom structure 
//
//returns:
// TRUE if the connection was closed correctly FALSE otherwhise                  */
int stbNCDisconnect(int moduleId,stbNetCom *nc)
{
  if (nc->isConnected)
  {
    stbNCSend(nc,moduleId,STB_LOGOUT,0,NULL);
    return (stbNCClose(nc));
  }
  return FALSE;
}

/*------------------------------------------------------------------------------
// stbNCClose
//
// simply closes the connection (server and client)
//
//Parameter:
// moduleId: the ModuleId of the client
// nc:       a pointer to the stbNetCom structure 
//
//returns:
// TRUE if the connection was closed correctly FALSE otherwhise                  */
int stbNCClose(stbNetCom *nc) 
{ 
  int ret;
  if (nc->isConnected)
  {
    Debug(printf("stbNCClose\n")); 
#ifdef WIN32
   ret=closesocket(nc->socket);
#else
   ret=close(nc->socket);
#endif
    nc->isConnected=FALSE; 
    return (ret>=0);
  }

  return FALSE;
}

/*------------------------------------------------------------------------------
// stbNCCheckConnection
//
// this routine is used to check if the connection is alive
// it sends a STB_STILLRUNNING packet and waits for an Acknowledge
//
//Parameter:
// nc:       a pointer to the stbNetCom structure 
//
//returns:
// TRUE if the connection is still alive FALSE otherwhise                       */
int stbNCCheckConnection(stbNetCom *nc)
{
  char msgdata[256],*tmp;
  int msgLength,ok,dataLen;
  char *data;

  Debug(printf("Check Connection\n"));

  if (nc->isConnected)
  {
    stbNCSend(nc,0,STB_STILLRUNNING,0,0);
    msgLength=3*sizeof(int);
    ok=FALSE;
    while ((msgLength==(3*sizeof(int))) && (!ok))
    {
      msgLength=myRead(nc->socket,msgdata,3*sizeof(int),TRUE);
      tmp=msgdata;  
      if (msgLength==(3*sizeof(int)))
      {
        if (stbNCGetInt(&tmp)==0)
        {
          if (stbNCGetInt(&tmp)==STB_STILLRUNNING_ACK)
          {
            dataLen=stbNCGetInt(&tmp);
            if (dataLen==0)
            {
              ok=TRUE;
            }
            else
            {
              data=(char *) malloc(dataLen);
              myRead(nc->socket,data,dataLen,FALSE);  
              free(data);
            }
          }
          else
          {
            dataLen=stbNCGetInt(&tmp);
            if (dataLen>0)
            {
              data=(char *) malloc(dataLen);
              myRead(nc->socket,data,dataLen,FALSE); 
              free(data);
            }
          }
        }
      }
    }
    free(msgdata);
    return ok;
  }
  else
  {
    return FALSE;
  }
}


/*------------------------------------------------------------------------------
// stbNCSendCheckConnectionAck
//
// this routine is used to send the ack, to tell that the connection is alive
// it sends a STB_STILLRUNNING_ACK packet 
//
//Parameter:
// nc:       a pointer to the stbNetCom structure 
//
//returns:
// TRUE if the sending of the acknowledge packet was OK FALSE otherwhise        */
int stbNCSendCheckConnectionAck(stbNetCom *nc)
{
  unsigned char *msgdata;
  char *tmp;
  int ret;

  Debug(printf("Check Connection Ack\n"));

  if (nc->isConnected)
  {
    msgdata=(unsigned char*)malloc(3*sizeof(int));
    tmp=(char *)msgdata;
    stbNCPutInt(&tmp,0);
    stbNCPutInt(&tmp,STB_STILLRUNNING_ACK);
    stbNCPutInt(&tmp,0);

    ret=send(nc->socket,(const char *)msgdata,3*sizeof(int),0); 
    free (msgdata);
    return (ret==(3*sizeof(int)));
  }
  return FALSE;
}


/*------------------------------------------------------------------------------
// stbNCSend
//
// sends a packet with the given data, length ,modulId and type
//
//Parameter:
// nc:       a pointer to the stbNetCom structure 
// modulId:  the Id of the sender
// type:     the type of the packet
// length:   the length of the data (not the whole packet!!)
// data:     pointer to the data
//
//returns:
// TRUE if the sending of the packet was OK FALSE otherwhise                    */
int stbNCSend(stbNetCom *nc,int moduleId,int type,int length,unsigned char *data)
{
  unsigned char *msgdata;
  char *tmp;
  int ret;
  int len=length+3*sizeof(int);

  if (nc->isConnected)
  {
    msgdata=(unsigned char*)malloc(len);
    tmp=(char *)msgdata;
    stbNCPutInt(&tmp,moduleId);
    stbNCPutInt(&tmp,type);
    stbNCPutInt(&tmp,length);

    if (data)
      memcpy(msgdata+3*sizeof(int),data,length);

    /*ret=write(nc->socket,msgdata,len);  */
    ret=send(nc->socket,(const char *)msgdata,len,0); 
    free (msgdata);
  Debug(printf("MsGSend id=%d type=%d ret=%d len=%d\n",moduleId,type,ret,len)); 

    return (ret==len);
  }

  return FALSE;
}


/*------------------------------------------------------------------------------
// stbNCSendMsg
//
// sends a packet with the given message
// simply calls the stbNCSend function
//
//Parameter:
// nc:       a pointer to the stbNetCom structure 
// msg:      pointer to the stbMessage structure which includes the
//           sender,length,data ...
//
//returns:
// TRUE if the sending of the packet was OK FALSE otherwhise                    */
int stbNCSendMsg(stbNetCom *nc,stbMessage *msg)
{
  return stbNCSend(nc,msg->moduleId,msg->type,msg->length,msg->data);
}


/*------------------------------------------------------------------------------
// stbNCReceiveMsg
//
// receives a message 
// this function waits till the message is complete (blocking!!)
//
//Parameter:
// nc:       a pointer to the stbNetCom structure 
// msg:      pointer to the stbMessage structure 
//           this pointer has to be valid, 
//           but the msg->data is allocated if data is available 
//
//returns:
// TRUE if the receiving of the message was OK FALSE otherwhise                    */
int stbNCReceiveMsg(stbNetCom *nc,stbMessage *msg)
{
  int msgLength;
  int tmp;
  unsigned char stbMsgbuf[MAX_MSGLEN];

  if (nc->isConnected)
  {
    msgLength=myRead(nc->socket,stbMsgbuf,3*sizeof(int),FALSE);    
    if (msgLength==(3*sizeof(int)))
    {
      memcpy(&tmp,stbMsgbuf,sizeof(int));
      msg->moduleId=ntohl(tmp);
      memcpy(&tmp,stbMsgbuf+sizeof(int),sizeof(int));
      msg->type=ntohl(tmp);
      memcpy(&tmp,stbMsgbuf+2*sizeof(int),sizeof(int));
      msg->length=ntohl(tmp);

      if (msg->length==0)
        msg->data=NULL; 	        
      else
      {
        msg->data=(unsigned char *) malloc(msg->length);
        msgLength+=myRead(nc->socket,msg->data,msg->length,FALSE);
       }
      Debug(printf("stbNCReceiveMsg %d %d %d\n",msg->moduleId,msg->type,msg->length));
    }
    else
    {
      return FALSE;
    }
  }
  else
  {
    return FALSE;
  }

  return TRUE;
}


/*------------------------------------------------------------------------------
// stbNCReceiveMsgNoWait
//
// receives a message 
// this function does not waits till the message is complete (nonblocking!!)
//
//Parameter:
// nc:       a pointer to the stbNetCom structure 
// msg:      pointer to the stbMessage structure 
//           this pointer has to be valid, 
//           but the msg->data is allocated if data is available 
//
//returns:
// TRUE if the receiving of the message was OK FALSE otherwhise                    */
int stbNCReceiveMsgNoWait(stbNetCom *nc,stbMessage *msg)
{
  int msgLength;
  unsigned char stbMsgbuf[MAX_MSGLEN]; 
  int tmp;

  if (nc->isConnected)
  {
    msgLength=myRead(nc->socket,stbMsgbuf,3*sizeof(int),TRUE);

    if (msgLength==(3*sizeof(int)))
    {
      memcpy(&tmp,stbMsgbuf,sizeof(int));
      msg->moduleId=ntohl(tmp);
      memcpy(&tmp,stbMsgbuf+sizeof(int),sizeof(int));
      msg->type=ntohl(tmp);
      memcpy(&tmp,stbMsgbuf+2*sizeof(int),sizeof(int));
      msg->length=ntohl(tmp);
      if (msg->length==0)
        msg->data=NULL; 	        
      else
      {
        msg->data=(unsigned char *) malloc(msg->length);
        msgLength+=myRead(nc->socket,msg->data,msg->length,FALSE);
      }
    }
    else
    {
      return FALSE;
    }
  }
  else
  {
    return FALSE;
  }

  return TRUE;
}


/*------------------------------------------------------------------------------
// stbNCGetInt
//
// gets an integer out of the data buffer 
// increases the data pointer
//
//Parameter:
// data:     a pointer to the pointer of the data buffer
//           this is necessary because the pointer has to be increased 
//
//returns:
// the correct integer value                                                    */
int stbNCGetInt(char **data)
{
  int tmp;

  memcpy(&tmp,*data,sizeof(int));
  (*data)+=sizeof(int);
  Debug(printf("stbNCGetInt with %d\n",ntohl(tmp)));
  return ntohl(tmp);
}


/*------------------------------------------------------------------------------
// stbNCPutInt
//
// stores an integer into the data buffer 
// increases the data pointer
//
//Parameter:
// data:     a pointer to the pointer of the data buffer
//           this is necessary because the pointer has to be increased 
// x:        the integer value which has to be stored into the buffer           */
void stbNCPutInt(char **data,int x) 
{
  int xx;

  xx=htonl(x);
  memcpy(*data,&xx,sizeof(int));
  (*data)+=sizeof(int);
  Debug(printf("stbNCPutInt with %d\n",x));
} 


/*------------------------------------------------------------------------------
// stbNCGetFloat
//
// gets an float out of the data buffer 
// increases the data pointer
//
//Parameter:
// data:     a pointer to the pointer of the data buffer
//           this is necessary because the pointer has to be increased 
//
//returns:
// the correct float value                                                      */
float stbNCGetFloat(char **data)
{
  int tmp;
  float ftmp;

  tmp=0;
  memcpy(&tmp,*data,sizeof(float));
  (*data)+=sizeof(int);
  tmp=ntohl(tmp);
  memcpy(&ftmp,&tmp,sizeof(float));
  Debug(printf("stbNCGetFloat with %f\n",ftmp));
  return ftmp;
}


/*------------------------------------------------------------------------------
// stbNCPutFloat
//
// stores a float into the data buffer 
// increases the data pointer
//
//Parameter:
// data:     a pointer to the pointer of the data buffer
//           this is necessary because the pointer has to be increased 
// x:        the float value which has to be stored into the buffer             */
void stbNCPutFloat(char **data,float x) 
{
  int tmp;

  memcpy(&tmp,&x,sizeof(float));
  tmp=htonl(tmp);
  memcpy(*data,&tmp,sizeof(int));
  (*data)+=sizeof(int);
  Debug(printf("stbNCPutFloat with %d\n",x));
} 


/*------------------------------------------------------------------------------
// stbNCGetChar
//
// gets an char(byte) out of the data buffer 
// increases the data pointer
//
//Parameter:
// data:     a pointer to the pointer of the data buffer
//           this is necessary because the pointer has to be increased 
//
//returns:
// the correct char(byte) value                                                 */
char stbNCGetChar(char **data)
{
  char tmp;

  memcpy(&tmp,*data,1);
  (*data)+=1;
  return tmp;
}


/*------------------------------------------------------------------------------
// stbNCPutChar
//
// stores a char(byte) into the data buffer 
// increases the data pointer
//
//Parameter:
// data:     a pointer to the pointer of the data buffer
//           this is necessary because the pointer has to be increased 
// x:        the char(byte) value which has to be stored into the buffer        */
void stbNCPutChar(char **data,char x) 
{
  memcpy(*data,&x,1);
  (*data)+=1;
} 


/*------------------------------------------------------------------------------
// stbNCGetString
//
// gets an string out of the data buffer 
// the string begins with an integer which tells the length of the string
//
//Parameter:
// data:     a pointer to the pointer of the data buffer
//           this is necessary because the pointer has to be increased 
//
//returns:
// the pointer of the beginning of the string in the buffer
// the string will not be allocated                                              */
char* stbNCGetString(char **data)
{
  char *tmp;
  int l;
  l=stbNCGetInt(data);
  if (l==0) tmp=NULL; else tmp=(*data);
  (*data)+=l;
  tmp[l]=0; 
  Debug(printf("stbNCGetString with %d %s\n",l,tmp));
  return tmp;
}


/*------------------------------------------------------------------------------
// stbNCPutString
//
// stores a string into the data buffer 
// first stores the length of the string 
// if the length is >0 then the string is appended to the buffer
// increases the data pointer
//
//Parameter:
// data:     a pointer to the pointer of the data buffer
//           this is necessary because the pointer has to be increased 
// str:      the string which has to be stored into the buffer                  */
void stbNCPutString(char **data,char *str) 
{
  int l;

  if (str==NULL) l=0; else l=strlen(str)+1;
  stbNCPutInt(data,l);
  if (l>0) 
  {
    memcpy(*data,str,l);
    (*data)+=l;
  }
  Debug(printf("stbNCPutString with %d %s\n",l,str));
} 


/*------------------------------------------------------------------------------
// strChrReplace
//
// replaces all characters s with r in the string str
// this routine is usefull if you want to parse strings with sscanf and
// the string has ' '. so replace the ' ' with other characters
//
//Parameter:
// str:     the string
// s:       the character which has to be searched 
// r:       the character with which the search character has to be replaced    */
void strChrReplace(char *str,char s,char r)
{
  while (*str)
  {
    if ((*str)==s) (*str)=r;
    str++;
  }

}
