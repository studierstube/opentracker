/* ===========================================================================
//  (C) 1999 Vienna University of Technology
// ===========================================================================
//  NAME:       stbFifo.c
//  TYPE:       c code
//  PROJECT:    Integrating Scientific Visualizations into Studierstube Workspace
//  CONTENT:    defines and prototypes for the FIFO communication
//  VERSION:    2.0
// ===========================================================================
//  AUTHORS:    krutz     Markus Krutz
//              peci      Clemens Pecinovsky
// ===========================================================================
//  HISTORY:
//    the original file by krutz was x3fifo.c
//
//  24-sep-99 08:00:00  krutz     created (as x3fifo.h)
//  21-apr-99 08:00:00  peci      modified
// ===========================================================================  */

#define NODEBUG

#include "stb.h"
#include "stbFifo.h"
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <unistd.h>
#include <stdlib.h>

#define PERMS (2+4+16+32+128+256)


int stbFifoCreate(int key) 
{
  char fifoname[256];

  sprintf(fifoname, "/tmp/fifo%d\0", key);
  Debug(printf("Fifoname is <%s>", fifoname));
  if ((mknod(fifoname, S_IFIFO | PERMS, 0) < 0) && (errno != EEXIST)) return FALSE;
  Debug(printf("mknod successful"));
  return TRUE;
}

int stbFifoRemove(int key) 
{
  char fifoname[256];

  sprintf(fifoname, "/tmp/fifo%d\0", key);
  if (unlink(fifoname) < 0) return FALSE;
  return TRUE;
}

int stbFifoOpen(int key, int mode) 
{
  int fd;
  char fifoname[256];
   
  sprintf(fifoname, "/tmp/fifo%d\0", key);
  Debug(printf("stbFifoOpen <%s> %d", fifoname, mode));
  fd=open(fifoname, mode);
  Debug(printf("stbFifoOpen fd is %d", fd));
  return fd;
}


void stbFifoClose(int fd) 
{
  close(fd);
}

int stbFifoSend(int fd, stbFifoMsg *msg) 
{
  char *buffer;
  int tmp;
  int len=3*sizeof(int)+msg->length;

  Debug(printf("stbFifoSend: %d %d %d %d",msg->moduleId, msg->type, msg->length, msg->data));
  if ((buffer=(char *)malloc(len)) == NULL) return FALSE;
  tmp=htonl(msg->moduleId);
  memcpy(buffer,&tmp,sizeof(int));
  tmp=htonl(msg->type);
  memcpy(buffer+sizeof(int),&tmp,sizeof(int));
  tmp=htonl(msg->length);
  memcpy(buffer+2*sizeof(int),&tmp,sizeof(int));
  if (msg->length!=NULL)
    memcpy(buffer+3*sizeof(int), msg->data, msg->length);

  /* write has to be atomic (multiple writers, one reader) */
  if (write(fd, buffer, len) != len) 
  {
    free (buffer);
    return FALSE;
  }
  free (buffer);
  Debug(printf("stbFifoSend: success(%d)", len));
  return TRUE;
}


stbFifoMsg *stbFifoReceive(int fd) 
{
  stbFifoMsg *msg=(stbFifoMsg *)malloc(sizeof(stbFifoMsg));
  int readLen;  

  if (msg==NULL)
  {
    return NULL;
  }
  if (read(fd, msg, 3*sizeof(int)) != 3*sizeof(int)) 
  {
    free(msg);
    return NULL; 
  }
  
  msg->moduleId=ntohl(msg->moduleId);
  msg->type=ntohl(msg->type);
  msg->length=ntohl(msg->length);
  if (msg->length>0)
  {
    msg->data=(char *)malloc(msg->length);
    readLen=read(fd, msg->data, msg->length);
    if ( readLen != msg->length) 
    { 
      free(msg->data); 
      free(msg); 
      return NULL; 
    }
  }
  else
  {
    msg->data=NULL;
  }
 
  return msg;
}

int CreateSema(key_t key, int *master) 
{
  int semid;

  semid=semget(key, 1, PERMS | IPC_CREAT | IPC_EXCL);
  if (semid == -1 && errno==EEXIST) 
  {
    Debug(printf("semaphore already exists"));
    *master=FALSE;
    return semget(key, 1, PERMS);
  }

  Debug(printf("semid is %d", semid));
  if (semctl(semid, 0, SETVAL, 1) < 0) 
  {
    Debug(printf(stderr, "CreateSema: semctl failed\n"));
    return -1;
  }
	
  *master=TRUE;
  return semid;
}

int V(int semid) 
{
  struct sembuf semp;
  semp.sem_num=0;
  semp.sem_op=1;
  semp.sem_flg=0;
  return semop(semid, &semp, 1);
}

int P(int semid) 
{
  struct sembuf semp;
  semp.sem_num=0;
  semp.sem_op=-1;
  semp.sem_flg=0;
  return semop(semid, &semp, 1);
}

int CreateShm(key_t key, char **shmaddr) 
{
  int shmid;

  if ((shmid=shmget(key, sizeof(int), PERMS)) < 0) 
  {
    Debug(printf(stdout, "CreateShm: shared memory probably doesn't exist, trying to create it\n"));
    if ((shmid=shmget(key, 2*sizeof(int), PERMS | IPC_CREAT)) < 0) 
    {
      Debug(printf(stderr, "CreateShm: shmget failed\n"));
      return shmid;
    }
  }

  /* attach */
  if (((*shmaddr)=(char *)shmat(shmid, 0, 0)) == NULL) 
  {
    Debug(printf(stderr, "CreateShm: shmat failed\n"));
    if (shmctl(shmid, IPC_RMID, NULL) < 0) 
    {
      Debug(printf(stderr, "CreateShm: shmctl failed\n"));
    }
    return -1;
  }

  return shmid;
}

int stbFifoCreateID(STBFIFO_IPC_INFO *info) 
{
  int semid;
  int shmid;
  int master;
  char *shmaddr;
  int id;

  key_t key=ftok(STBFIFO_PATH, 1);

  if ((semid=CreateSema(key, &master)) < 0) 
  {
    Debug(printf(stderr, "stbFifoCreateID: CreateSema failed\n"));
    return -1;
  }
	
  if (!master) P(semid);
  if ((shmid=CreateShm(key, &shmaddr)) == -1) return -1;
	
  if (master) 
  {
    *((int *) shmaddr)=1; /* first id is 1 */
    *((int *) shmaddr+1)=1; /* initial reference counter is 1 */
  } 
  else
  {
    *((int *) shmaddr+1)=*((int *) shmaddr+1)+1; /* increase reference counter */
  }
	
  id=*((int *)shmaddr);
  *((int *)shmaddr)=*((int*)shmaddr)+1;
  V(semid);
  info->semid=semid;
  info->shmid=shmid;
  info->shmaddr=shmaddr;

  return id;
}

int DestroySema(int semid) 
{
  union 
  {
    int val;
    struct semid_ds *buf;
    ushort *array;
  } semctl_arg;

  Debug(printf("About to Destroy semaphore"));
  semctl_arg.val=0;
  if (semctl(semid, 0, IPC_RMID, semctl_arg)<0) 
  {
    fprintf(stderr, "DestroySema: can't remove semaphore(semid is %d)\n", semid);
    return FALSE;
  }
  return TRUE;
}

int DestroyShm(int shmid, char *shmaddr) 
{
  Debug(printf("About to destroy shm"));
  if (shmdt(shmaddr)<0) 
  {
    fprintf(stderr, "DestroyShm: can't detach shared memory segment\n");
    return FALSE;
  }
  if (shmctl(shmid, IPC_RMID, (struct shmid_ds *)0)<0) 
  {
    fprintf(stderr, "DestroyShm: can't remove shared memory segment (shmid is %d)\n", shmid);
    return FALSE;
  }
  return TRUE;
}

int stbFifoDestroyID(STBFIFO_IPC_INFO *info) 
{
  Debug(printf("About to destroy..."));
  P(info->semid);
  *((int *) info->shmaddr+1)=*((int *) info->shmaddr+1)-1;
  if (*((int *) info->shmaddr+1)<=0) 
  {
   /* destroy shm and semaphore since reference counter decreased to/below zero */
    DestroyShm(info->shmid, info->shmaddr);
    DestroySema(info->semid);
  } 
  else 
  {
    V(info->semid);
  }
  
  return 0;
}

