/* ===========================================================================
//  (C) 1999 Vienna University of Technology
// ===========================================================================
//  NAME:       Geom2SerIV.h
//  TYPE:       c code
//  PROJECT:    Integrating Scientific Visualizations into Studierstube Workspace
//  CONTENT:    defines and prototypes for the Geom2SerIV 
//  VERSION:    2.0
// ===========================================================================
//  AUTHORS:    krutz     Markus Krutz
//              peci      Clemens Pecinovsky
// ===========================================================================
//  HISTORY:
//
//  12-sep-97 08:00:00  krutz      created
//  12-dec-99 08:00:00  peci       modified
// ===========================================================================  */


/*
TEST-List:
  - supposed there is a geometry that consists of polytriangles and labelnc two STB_GEOMETRY messages will be generated.
    therefore, it's necessary to signal the end of a complete geometry to the display server (just an empty message body).
    to make things easier on the server site, a startup message seems to be useful.
    so what has to be done?
    1) send startup message
    2) send n partial geometries
    3) send end-of-data message
*/

#define NODEBUG

#include "stb.h"
#include "stbFifo.h"
#include "Geom2SerIV.h"
#include "Geom2SerIVProtocol.h"

#include <sys/time.h>


typedef unsigned char htype;
static fifo_fd;
static int theModuleId;

void wr_Header(char **b, htype header, int len) 
{
  stbNCPutChar(b,header);
  stbNCPutInt(b, len);
}


/******************************************************/

void wr_Coordinate3( int num, float *vtx) 
{
  int i,t,l;
  int len;
  stbFifoMsg msg;
  char *b,*hlp;

  Debug(printf("Coordinate 3 %d \n",num));

  msg.type=STB_GEOMETRY;
  msg.moduleId=theModuleId;
  msg.data=malloc(3*SF*64+SI+SI+SI+sizeof(htype));
  b=msg.data;
  hlp=b;

  for (i=0;i<=(num/MAX_VEC_PER_PACKET);i++)
  {
    l=num-i*MAX_VEC_PER_PACKET;
    if (l>MAX_VEC_PER_PACKET) l=MAX_VEC_PER_PACKET;
    if (l>0)
    {
      b=hlp;
      len=3*SF*l+SI+SI+SI+sizeof(htype);
      msg.length=len;
      wr_Header(&b, COORDINATE3, len);

      stbNCPutInt(&b, i*MAX_VEC_PER_PACKET); /*start of Triples */
      stbNCPutInt(&b, l);    /* Number of Triplets*/
      
      for (t=0;t<l;t++)
      {
        stbNCPutFloat(&b,vtx[i*MAX_VEC_PER_PACKET*3+t*3+0]);
        stbNCPutFloat(&b,vtx[i*MAX_VEC_PER_PACKET*3+t*3+1]);
        stbNCPutFloat(&b,vtx[i*MAX_VEC_PER_PACKET*3+t*3+2]);
      }
      stbFifoSend(fifo_fd, &msg);
    }
  }

  free(msg.data);
}

void wr_Info( char *p) 
{
  int len;
  stbFifoMsg msg;
  char *b;

  len=strlen(p)+1+SI+SI+sizeof(htype);
  msg.type=STB_GEOMETRY;
  msg.moduleId=theModuleId;
  msg.length=len;
  msg.data=malloc(len);
  b=msg.data;

  wr_Header(&b, INFO, len);
  stbNCPutString(&b, p);

  stbFifoSend(fifo_fd, &msg);
  free(msg.data);
}

void wr_Normal( int num, float *vtx) 
{
  int i,l,t;
  int len;
  stbFifoMsg msg;
  char *b,*hlp;

  msg.type=STB_GEOMETRY;
  msg.moduleId=theModuleId;
  msg.data=malloc(3*SF*64+SI+SI+SI+sizeof(htype));
  b=msg.data;
  hlp=b;

  for (i=0;i<=(num/MAX_VEC_PER_PACKET);i++)
  {
    l=num-i*MAX_VEC_PER_PACKET;
    if (l>MAX_VEC_PER_PACKET) l=MAX_VEC_PER_PACKET;
    if (l>0)
    {
      b=hlp;
      len=3*SF*l+SI+SI+SI+sizeof(htype);
      msg.length=len;
      wr_Header(&b, NORMAL, len);

      stbNCPutInt(&b, i*MAX_VEC_PER_PACKET); /*start of Triples */
      stbNCPutInt(&b, l); /* Number of Triplets*/
      for (t=0;t<l;t++)
      {
        stbNCPutFloat(&b,vtx[i*MAX_VEC_PER_PACKET*3+t*3+0]);
        stbNCPutFloat(&b,vtx[i*MAX_VEC_PER_PACKET*3+t*3+1]);
        stbNCPutFloat(&b,vtx[i*MAX_VEC_PER_PACKET*3+t*3+2]);
      }
      stbFifoSend(fifo_fd, &msg);
    }
  }

  free(msg.data);
}

void wr_NormalBinding( int type) 
{
  int len;
  stbFifoMsg msg;
  char *b;

  len=SI+SI+sizeof(htype);
  msg.type=STB_GEOMETRY;
  msg.moduleId=theModuleId;
  msg.length=len;
  msg.data=malloc(len);
  b=msg.data;

  wr_Header(&b, NORMALBINDING,len);
  stbNCPutInt(&b, type);

  stbFifoSend(fifo_fd, &msg);
  free(msg.data);
}


void wr_BaseColor( int num, float *col) 
{
  int i,l,t;
  int len;
  stbFifoMsg msg;
  char *b,*hlp;

  msg.type=STB_GEOMETRY;
  msg.moduleId=theModuleId;
  msg.data=malloc(3*SF*64+SI+SI+SI+sizeof(htype));
  b=msg.data;
  hlp=b;

  for (i=0;i<=(num/MAX_VEC_PER_PACKET);i++)
  {
    l=num-i*MAX_VEC_PER_PACKET;
    if (l>MAX_VEC_PER_PACKET) l=MAX_VEC_PER_PACKET;
    if (l>0)
    {
      b=hlp;
      len=3*SF*l+SI+SI+SI+sizeof(htype);
      msg.length=len;
      wr_Header(&b, BASECOLOR, len);

      stbNCPutInt(&b, i*MAX_VEC_PER_PACKET); /*start of Triples */
      stbNCPutInt(&b, l); /* Number of Triplets*/
      for (t=0;t<l;t++)
      {
        stbNCPutFloat(&b,col[i*MAX_VEC_PER_PACKET*3+t*3+0]);
        stbNCPutFloat(&b,col[i*MAX_VEC_PER_PACKET*3+t*3+1]);
        stbNCPutFloat(&b,col[i*MAX_VEC_PER_PACKET*3+t*3+2]);
      }
      stbFifoSend(fifo_fd, &msg);
    }
  }

  free(msg.data);
}

void wr_MaterialBinding( int type) 
{
  int len;
  stbFifoMsg msg;
  char *b;

  len=SI+SI+sizeof(htype);
  msg.type=STB_GEOMETRY;
  msg.moduleId=theModuleId;
  msg.length=len;
  msg.data=malloc(len);
  b=msg.data;

  wr_Header(&b, MATERIALBINDING,len);
  stbNCPutInt(&b, type);

  stbFifoSend(fifo_fd, &msg);
  free(msg.data);
}

void wr_DrawStyle( int style, float linewidth) 
{
  int len;
  stbFifoMsg msg;
  char *b;

  len=SF+SI+SI+sizeof(htype);
  msg.type=STB_GEOMETRY;
  msg.moduleId=theModuleId;
  msg.length=len;
  msg.data=malloc(len);
  b=msg.data;

  wr_Header(&b, DRAWSTYLE,len);
  stbNCPutInt(&b, style);
  stbNCPutFloat(&b, linewidth);

  stbFifoSend(fifo_fd, &msg);
  free(msg.data);
}

void wr_Translation( float *delta) 
{
  int len;
  stbFifoMsg msg;
  char *b;

  len=3*SF+SI+sizeof(htype);
  msg.type=STB_GEOMETRY;
  msg.moduleId=theModuleId;
  msg.length=len;
  msg.data=malloc(len);
  b=msg.data;

  wr_Header(&b, TRANSLATION, len);
  stbNCPutFloat(&b, *delta++);
  stbNCPutFloat(&b, *delta++);
  stbNCPutFloat(&b, *delta);

  stbFifoSend(fifo_fd, &msg);
  free(msg.data);
}

void wr_Sphere( float radius) 
{
  int len;
  stbFifoMsg msg;
  char *b;

  len=SF+SI+sizeof(htype);
  msg.type=STB_GEOMETRY;
  msg.moduleId=theModuleId;
  msg.length=len;
  msg.data=malloc(len);
  b=msg.data;

  wr_Header(&b, SPHERE, len);
  stbNCPutFloat(&b, radius);

  stbFifoSend(fifo_fd, &msg);
  free(msg.data);
}

void wr_Separator_beg() 
{
  int len;
  stbFifoMsg msg;
  char *b;

  len=SI+sizeof(htype);
  msg.type=STB_GEOMETRY;
  msg.moduleId=theModuleId;
  msg.length=len;
  msg.data=malloc(len);
  b=msg.data;

  wr_Header(&b, SEPARATOR_START, len);

  stbFifoSend(fifo_fd, &msg);
  free(msg.data);
}

void wr_Separator_end() 
{
  int len;
  stbFifoMsg msg;
  char *b;

  len=SI+sizeof(htype);
  msg.type=STB_GEOMETRY;
  msg.moduleId=theModuleId;
  msg.length=len;
  msg.data=malloc(len);
  b=msg.data;

  wr_Header(&b, SEPARATOR_END, len);

  stbFifoSend(fifo_fd, &msg);
  free(msg.data);
}


void wr_Font( char *name, float size) 
{
  int len;
  stbFifoMsg msg;
  char *b;

  len=SI+strlen(name)+1+SF+SI+sizeof(htype);
  msg.type=STB_GEOMETRY;
  msg.moduleId=theModuleId;
  msg.length=len;
  msg.data=malloc(len);
  b=msg.data;

  wr_Header(&b, FONT, len);
  stbNCPutString(&b, name);
  stbNCPutFloat(&b, size);

  stbFifoSend(fifo_fd, &msg);
  free(msg.data);
}

void wr_Material( float *col) 
{
  int len;
  stbFifoMsg msg;
  char *b;

  len=3*SF+SI+sizeof(htype);
  msg.type=STB_GEOMETRY;
  msg.moduleId=theModuleId;
  msg.length=len;
  msg.data=malloc(len);
  b=msg.data;

  wr_Header(&b, MATERIAL, len);
  stbNCPutFloat(&b, *col++);
  stbNCPutFloat(&b, *col++);
  stbNCPutFloat(&b, *col);

  stbFifoSend(fifo_fd, &msg);
  free(msg.data);
}

void wr_Text2( char *text, unsigned char just) 
{
  int len;
  stbFifoMsg msg;
  char *b;

  len=sizeof(htype)+SI+SI+strlen(text)+1+SB;
  msg.type=STB_GEOMETRY;
  msg.moduleId=theModuleId;
  msg.length=len;
  msg.data=malloc(len);
  b=msg.data;

  wr_Header(&b, TEXT2, len);
  stbNCPutString(&b, text);
  stbNCPutChar(&b, just);

  stbFifoSend(fifo_fd, &msg);
  free(msg.data);
}

void wr_IndexedLineset(int num) 
{
  int i;
  int len;
  stbFifoMsg msg;
  char *b;

  Debug(printf("Indexed Lineset %d\n",num));

  len=sizeof(htype)+SI+SI+3*SI*num-SI;
  msg.type=STB_GEOMETRY;
  msg.moduleId=theModuleId;
  msg.length=len;
  msg.data=malloc(len);
  b=msg.data;

  wr_Header(&b, INDEXEDLINESET, len);
/*  stbNCPutInt(&b, 3*SI*num-SI);     changed by peci*/
  if (num==0)
    stbNCPutInt(&b, 0);
  else
    stbNCPutInt(&b, (num/2)*3-1);
  for ( i = 0; i < num / 2; i++ ) 
  {
    stbNCPutInt(&b, i*2);
    stbNCPutInt(&b, i*2+1);
    if ( i != num / 2 -1 )  
      stbNCPutInt(&b, -1);
  }

  stbFifoSend(fifo_fd, &msg);
  free(msg.data);
}

void wr_IndexedFaceSet(int *pp,int num)
{
  int idxlen,i,j;
  int len;
  stbFifoMsg msg;
  char *b;
 
  idxlen=num*2; /*Indexwerte und -1   */
    for(i=0,j=0; i < num; i++) {
        idxlen+=pp[j];
        j+=(pp[j]+1);
    }
 
  len=sizeof(htype)+SI+idxlen;
  msg.type=STB_GEOMETRY;
  msg.moduleId=theModuleId;
  msg.length=len;
  msg.data=malloc(len);
  b=msg.data;

    wr_Header(&b, INDEXEDFACESET, len);
    stbNCPutInt(&b, idxlen);

    for ( i = 0; i < num; i++ ) {
        int     verts;
        verts = *pp; pp++;
        for ( j = 0; j < verts; j++, pp++ )
            stbNCPutInt(&b,*pp -1);
        stbNCPutInt(&b,-1);
    }

  stbFifoSend(fifo_fd, &msg);
  free(msg.data);
}

void wr_TriangleStripset(int num) 
{
  int len;
  stbFifoMsg msg;
  char *b;

  len=sizeof(htype)+SI+SI;
  msg.type=STB_GEOMETRY;
  msg.moduleId=theModuleId;
  msg.length=len;
  msg.data=malloc(len);
  b=msg.data;

  wr_Header(&b, TRIANGLESTRIPSET, len);
  stbNCPutInt(&b, num);

  stbFifoSend(fifo_fd, &msg);
  free(msg.data);
}

void wr_Lineset( int num)
{
  int len;
  stbFifoMsg msg;
  char *b;

  len=sizeof(htype)+SI+SI;
  msg.type=STB_GEOMETRY;
  msg.moduleId=theModuleId;
  msg.length=len;
  msg.data=malloc(len);
  b=msg.data;

  wr_Header(&b, LINESET, len);
  stbNCPutInt(&b, num);

  stbFifoSend(fifo_fd, &msg);
  free(msg.data);
}

void wr_Quadmesh( int vcol, int vrow) 
{
  int len;
  stbFifoMsg msg;
  char *b;

  len=sizeof(htype)+SI+SI+SI;
  msg.type=STB_GEOMETRY;
  msg.moduleId=theModuleId;
  msg.length=len;
  msg.data=malloc(len);
  b=msg.data;

  wr_Header(&b, QUADMESH, len);
  stbNCPutInt(&b, vcol);
  stbNCPutInt(&b, vrow);

  stbFifoSend(fifo_fd, &msg);
  free(msg.data);
}

void wr_Reset() 
{
  int len;
  stbFifoMsg msg;
  char *b;

  len=sizeof(htype)+SI;
  msg.type=STB_GEOMETRY;
  msg.moduleId=theModuleId;
  msg.length=len;
  msg.data=malloc(len);
  b=msg.data;

  wr_Header(&b, RESET, len);

  stbFifoSend(fifo_fd, &msg);
  free(msg.data);
}

void wr_Endofdata() 
{
  int len;
  stbFifoMsg msg;
  char *b;

  len=sizeof(htype)+SI;
  msg.type=STB_GEOMETRY;
  msg.moduleId=theModuleId;
  msg.length=len;
  msg.data=malloc(len);
  b=msg.data;

  wr_Header(&b, ENDOFDATA, len);

  stbFifoSend(fifo_fd, &msg);
  free(msg.data);
}

/*******************************************************/

static char *geom_type_list[] = 
{
  "0",
  "GEOM_MESH",
  "GEOM_POLYHEDRON",
  "GEOM_POLYTRI",
  "GEOM_SPHERE",
  "GEOM_LABEL"
};

static char     label_font[] = "Helvetica";
struct timeval tp;
Debug(static int count=0);
Debug(static int count2=0);

int do_output(GEOMedit_list in,int fifo_fd_in,int moduleId)
{
  stbFifoMsg msg;   
  char data[4]; 
  char *b;
  int output_iv(char *name, GEOMobj *input);
  
  fifo_fd=fifo_fd_in;
  theModuleId=moduleId;
  /***********        BODY    **************/
  Debug(printf("header written  packets: \n"));


/* write startup_message */
  msg.type=STB_GEOMETRY;
  msg.length=sizeof(char);
  msg.moduleId=theModuleId;
  msg.data=data;
  b=msg.data;
  stbNCPutChar(&b, GEOMETRY_START);
  stbFifoSend(fifo_fd, &msg);


  Debug(count =0;count2=0);
    
  gettimeofday(&tp);
  Debug(printf("\nStarttime: %d . %d\n",tp.tv_sec,tp.tv_usec));

  /*  Set / Call Geometry edit function       */
  GEOMset_new_geometry_func((AVS_FNCP)output_iv);
  GEOMprocess_edit_list(in);

  msg.type=STB_GEOMETRY;
  msg.length=sizeof(char);
  msg.moduleId=theModuleId;
  msg.data=data;
  b=msg.data;
  stbNCPutChar(&b, GEOMETRY_END);
  stbFifoSend(fifo_fd, &msg);
  

  gettimeofday(&tp);
  Debug(printf("\nEndtime: %d . %d\n",tp.tv_sec,tp.tv_usec));
  Debug(printf("\n >> Count: (%d) Count2: (%d)  \n",count,count2));
  return(1);
}
 

/*******************************************************************
 * GEOMETRY EDIT FUNCTION
 *
 * 
 *******************************************************************/

int output_iv(char *name, GEOMobj *input)
{
    Debug(printf("Output: (type: %d,%s) ",input->type, geom_type_list[input->type]));
    switch ( input->type ) {

      case GEOM_MESH:
        Debug(printf("Mesh\n"));
        if ( input->d.m.m < 1 || input->d.m.n < 1 )
            break;
        output_mesh(name, input);
        break;
        
      case GEOM_POLYHEDRON:
        Debug(printf("Polyhedron\n"));
        output_polyh(name, input);
        break;
        
      case GEOM_POLYTRI:
        Debug(printf("Polytriangle\n"));
        if ( input->d.pt.npts > 0 )
        {
          Debug(count2++);
          Debug(printf("Polytriangle\n"));
          Debug(gettimeofday(&tp));
          Debug(printf("\nStart polytri: %d . %d\n",tp.tv_sec,tp.tv_usec));
          output_polytri(name, input);        /* triangle strips */
          Debug(gettimeofday(&tp));
          Debug(printf("\nEnd polytri: %d . %d\n",tp.tv_sec,tp.tv_usec));
        }
        else 
        { 
          if ( input->d.pt.npls > 0)
          {
            Debug(printf("Poly Lines\n"));
            output_polyline(name, input); 
          }
          else
          {
            Debug(printf("Disjoint Lines\n"));
            output_djline(name, input);
          }
          Debug(count++);
        }
        /*if ( input->d.pt.npls > 0 && input->d.pt.npts < 1 )
            output_polyline(name, input);*/     /* polylines */
        /*if ( input->d.pt.dlverts.n > 1 && input->d.pt.npts < 1 )
            output_djline(name, input);*/               /* disjoint lines */
        break;

      case GEOM_SPHERE:
        Debug(printf("Sphere\n"));
        output_sphere(name, input);
        break;

      case GEOM_LABEL:
        Debug(printf("Label\n"));
        output_label(name, input);
        break;

      default:
        Debug(fprintf(stderr, "output_iv: %s includes %s object. ignore.\n", name, geom_type_list[input->type]));
        fflush(stderr);
        break;
    }
    fflush(stdout);
    return(1);
}

/*******************************************************************
 * OUTPUT POLYTRIANGLES
 *
 * beta 0.05*3/7
 *******************************************************************/

int output_polytri(char *name, GEOMobj *input)
{
    int  j;
    float defcol[3]={1.0,1.0,1.0};
    

  Debug(printf("npts: (%d)\n",input->d.pt.npts));

  /* Output: Geometry-Message */
    wr_Separator_beg();

    wr_Info(name);

    for ( j = 0; j < input->d.pt.npts; j++ ) {
        wr_Coordinate3(input->d.pt.ptverts[j].n,input->d.pt.ptverts[j].l); 
        
        if ( input->data & GEOM_NORMALS ) {
            wr_Normal(input->d.pt.ptnormals[j].n,input->d.pt.ptnormals[j].l);
            wr_NormalBinding(_PER_VERTEX);               
        }

        if ( input->data & GEOM_VCOLORS ) {
            wr_BaseColor(input->d.pt.ptverts[j].n,input->d.pt.ptcolors[j].l);
            wr_MaterialBinding(_PER_VERTEX);
        } else {
            wr_BaseColor(1, defcol);
        }
        wr_TriangleStripset(input->d.pt.ptverts[j].n); 

        wr_DrawStyle(_FILLED,-1);
    }
    wr_Separator_end();
    return(1);
}

/*******************************************************************
 * OUTPUT MESH
 *
 * untested !!!!!!
 *******************************************************************/

int output_mesh(char *name, GEOMobj *input)
{
    float defcol[3]={1.0,1.0,1.0};

    wr_Separator_beg();

    wr_Info(name);
        
    wr_Coordinate3(input->d.m.verts.n,input->d.m.verts.l);
                        
    if ( input->data & GEOM_NORMALS ) {
        wr_Normal(input->d.m.normals.n,input->d.m.normals.l);
    }

    if ( input->data & GEOM_VCOLORS ) {
        wr_BaseColor(input->d.m.colors.n,input->d.m.colors.l);
        wr_MaterialBinding(_PER_VERTEX);
    } else {
        wr_BaseColor(1,defcol);        
    }

    wr_Quadmesh(input->d.m.m,input->d.m.n);
        
    wr_DrawStyle(_FILLED,-1);    

    wr_Separator_end();

    return(1);
}

 /*******************************************************************
 * OUTPUT SPHERES
 *
 * untested !!!!!!
 *******************************************************************/

int output_sphere(char *name, GEOMobj *input)
{
    int i;
    wr_Separator_beg();

    wr_Info(name);
        
    for ( i = 0; i < input->d.sp.verts.n; i++ ) {
        wr_Separator_beg();
        
        wr_Translation(&(input->d.sp.verts.l[i*3]));
        
        if ( input->data & GEOM_VCOLORS ) {
            wr_BaseColor( 1,&(input->d.sp.colors.l[i*3]));                       
        }

        wr_Sphere(input->d.sp.radii.l[i]); 
        wr_Separator_end();
    }
    wr_Separator_end();

    return(1);
}

/*******************************************************************
 * OUTPUT POLYHEDRON
 *
 * untested !!!!!!
 *******************************************************************/
 
int output_polyh(char *name, GEOMobj *input)
{
    int   i, j, *pp, idxlen, len;
    float defcol[3]={1.0,1.0,1.0};
    stbFifoMsg msg;
    char *b;



    wr_Separator_beg();

    wr_Info(name);
    
    if ( input->data & GEOM_NORMALS ) {
        wr_Coordinate3(input->d.ph.normals.n,input->d.ph.normals.l); 
        wr_NormalBinding(_PER_VERTEX); 
    }

    if ( input->data & GEOM_VCOLORS ) {
        wr_BaseColor(input->d.ph.colors.n,input->d.ph.colors.l);
        wr_MaterialBinding(_PER_VERTEX);
    } else {
        wr_BaseColor(1,defcol);
    }

    wr_IndexedFaceSet(input->d.ph.ptlist.l,input->d.ph.ptlist.nps);

    wr_Separator_end();

    return(1);
}

/*******************************************************************
 * OUTPUT POLYLINES
 *
 * untested !!!!!!
 *******************************************************************/

int output_polyline(char *name, GEOMobj *input)
{
    int  j;
    float defcol[3]={1.0,1.0,1.0};

    wr_Separator_beg();

    wr_Info(name);

    for ( j = 0; j < input->d.pt.npls; j++ ) {

        wr_Coordinate3(input->d.pt.plverts[j].n, input->d.pt.plverts[j].l);

        if ( input->data & GEOM_VCOLORS ) {
            wr_BaseColor(input->d.pt.plverts[j].n, input->d.pt.plcolors[j].l);
            wr_MaterialBinding(_PER_VERTEX);                             
        } else {
            wr_BaseColor(1, defcol);
        }

        wr_Lineset(input->d.pt.plverts[j].n); 

        wr_DrawStyle(-1,POLYLINE_WIDTH);       
    }
    wr_Separator_end();
    return(1);
}

/*******************************************************************
 * OUTPUT DISJOINT LINES
 *
 * tested and fixed by peci99
 *******************************************************************/

int output_djline(char *name, GEOMobj *input)
{
    int         i, len;
    float defcol[3]={1.0,1.0,1.0};

  wr_Separator_beg();

  wr_Info(name);
  wr_Coordinate3(input->d.pt.dlverts.n, input->d.pt.dlverts.l);
  if ( input->data & GEOM_VCOLORS ) 
  {
    wr_BaseColor(input->d.pt.dlcolors.n, input->d.pt.dlcolors.l);
    wr_MaterialBinding(_PER_VERTEX);                             
  }
  else 
  {
    wr_BaseColor(1, defcol);
  }

  wr_IndexedLineset(input->d.pt.dlverts.n);

  wr_Separator_end();

  return(1);
}

/*******************************************************************
 * OUTPUT LABELs
 *
 * tested so far
 *******************************************************************/

int output_label(char *name, GEOMobj *input)
{
    int         i;
    int         flag;
    float       t[3];

    wr_Separator_beg();

    wr_Info(name);
        
    for ( i = 0; i < input->d.la.labels.n; i++ ) {
        flag = 0;GEOMget_label_flags(input, i);

        /** Ignore Title Labels **/
        if ( flag & GEOM_LABEL_ABSOLUTE ) continue;

        wr_Separator_beg();

        t[0]=input->d.la.verts.l[i*3] + input->d.la.offsets.l[i*3];
        t[1]=input->d.la.verts.l[i*3+1] + input->d.la.offsets.l[i*3+1];
        t[2]=input->d.la.verts.l[i*3+2] + input->d.la.offsets.l[i*3+2];

        wr_Font(label_font,input->d.la.heights.l[i]* LABEL_SCALE);

        wr_Translation(t); 

        if ( input->data & GEOM_VCOLORS ) {
                wr_Material(&(input->d.la.colors.l[i*3]));
        }

        if ( flag & GEOM_LABEL_CENTER )
            wr_Text2(input->d.la.labels.l[i],_CENTER);
        else if ( flag & GEOM_LABEL_RIGHT )
            wr_Text2(input->d.la.labels.l[i],_RIGHT);
        else
            wr_Text2(input->d.la.labels.l[i],_LEFT);

        wr_Separator_end();
    }
    wr_Separator_end();

    return(1);
}

/*** EOF ***/
