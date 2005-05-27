/* ===========================================================================
    Copyright (c) 1996,1997,1998,1999,2000
    Vienna University of Technology
    
    This software is provided AS IS, without any express or implied
    warranty.  In no event will the authors or the Vienna University of
    Technology be held liable for any damages or loss of profit arising
    from the use or non-fitness for a particular purpose of this software.
   ===========================================================================
    NAME:       fspext
    TYPE:       c++ code
    PROJECT:    Studierstube - Undistortion tools
    CONTENT:    Extrapolate a grid of FSP data
   ===========================================================================
    AUTHORS:    jp      Jan Prikryl
   ===========================================================================
    HISTORY:

    18-Jan-01 17:11:30  jp      last modification
    12-Jan-01  9:24:58  jp      created
=========================================================================== */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>

#ifdef WIN32
#include <float.h>
#define finite _finite
#endif

#include <stbapi/tracker/DataInterpolator.h>
#include <stbapi/util/option.h>
#include <Inventor/SbLinear.h>

#define SQ(_v) (_v*_v)
 
#define DATAPTR(_x, _y, _z) (data + (_x) + (_y) * dx + (_z) * dxy)

typedef struct {
  float xw, yw, zw;
  float xt, yt, zt;
  float a, b, c, d;
} FSPData;

void
Extrapolate(FSPData * p2, FSPData * p1, FSPData * p0)
{
    p0->xw = 2.0 * p1->xw - p2->xw;
    p0->yw = 2.0 * p1->yw - p2->yw;
    p0->zw = 2.0 * p1->zw - p2->zw;

    p0->xt = 2.0 * p1->xt - p2->xt;
    p0->yt = 2.0 * p1->yt - p2->yt;
    p0->zt = 2.0 * p1->zt - p2->zt;

    SbRotation r1(p1->b, p1->c, p1->d, p1->a);
    SbRotation r2(p2->b, p2->c, p2->d, p2->a);
    SbRotation r0 = r1 * ( r1 * r2.inverse() );
    r0.getValue(p0->b, p0->c, p0->d, p0->a);
}

void
Average2(FSPData * t1, FSPData * t2, FSPData * p0)
{
    p0->xw = 0.5 * (t1->xw + t2->xw);
    p0->yw = 0.5 * (t1->yw + t2->yw);
    p0->zw = 0.5 * (t1->zw + t2->zw);

    p0->xt = 0.5 * (t1->xt + t2->xt);
    p0->yt = 0.5 * (t1->yt + t2->yt);
    p0->zt = 0.5 * (t1->zt + t2->zt);

    SbRotation r1(t1->b, t1->c, t1->d, t1->a);
    SbRotation r2(t2->b, t2->c, t2->d, t2->a);
    SbRotation r0 = SbRotation::slerp(r2, r1, 0.5);
    r0.getValue(p0->b, p0->c, p0->d, p0->a);
}

void
Average3(FSPData * t1, FSPData * t2, FSPData * t3, FSPData * p0)
{
    p0->xw = 1.0/3.0 * (t1->xw + t2->xw + t3->xw);
    p0->yw = 1.0/3.0 * (t1->yw + t2->yw + t3->yw);
    p0->zw = 1.0/3.0 * (t1->zw + t2->zw + t3->zw);

    p0->xt = 1.0/3.0 * (t1->xt + t2->xt + t2->xt);
    p0->yt = 1.0/3.0 * (t1->yt + t2->yt + t3->yt);
    p0->zt = 1.0/3.0 * (t1->zt + t2->zt + t3->zt);

    SbRotation r1(t1->b, t1->c, t1->d, t1->a);
    SbRotation r2(t2->b, t2->c, t2->d, t2->a);
    SbRotation r3(t3->b, t3->c, t3->d, t3->a);
    SbRotation r0 = SbRotation::slerp(r2, r1, 0.5);
    r0 = SbRotation::slerp(r0, r3, 2.0/3.0);
    r0.getValue(p0->b, p0->c, p0->d, p0->a);
}

void
main(int argc, char* argv[])
{
    //--paramters
    OptionString optFileName("file",    "f",  NULL);
    OptionString optFileOut ("output",  "o",  NULL);

    if(!Option::parse(argc,argv)) { printf("syntax error\n"); exit(-1); }
    const char* fileName = optFileName.getValue();

    FILE* f;
    int n, nx, ny, nz, res, ix, iy, iz;
    // float wx,wy,wz,tx,ty,tz,ta,tb,tc,td;
    char dummyBuffer[1000];

    if (fileName != NULL)
    {
        f = fopen(fileName, "r");
        if (f == NULL)
        {
            perror("ERROR: Cannot open input file");
            exit (-1);
        }
    }
    else
    {
        f = stdin;
    }

    fprintf(stderr, "INFO: Reading full scattered data.\n");

    // Check the format of the input data
    
    fscanf(f,"%s\n",dummyBuffer);
    if ( strcmp(dummyBuffer, FSPDATA_HEADER) )
    {
        fprintf(stderr, "ERROR: Wrong input data header!\n");
        exit(-1);
    }

    // Read the grid dimensions (for completely scattered data
    // ny = nz = 1).

    res = fscanf(f,"%d %d %d\n", &nx, &ny, &nz);
    if (res != 3)
    {
        fprintf(stderr, "ERROR: Wrong dimensions!\n");
        exit(-1);
    }
    if (ny == 1 && nz == 1)
    {
        fprintf(stderr, "ERROR: This is not a grid, cannot extrapolate!\n");
        exit(-1);
    }
    n = nx * ny * nz;

    // Allocate memory for the extrapolated grid

    int dx  = (nx + 2);
    int dy  = (ny + 2);
    int dz  = (nz + 2);
    int dxy = dx * dy;
    FSPData * data = (FSPData *) calloc (dxy*dz, sizeof(FSPData));  

    if (data == NULL)
    {
        fprintf(stderr, "ERROR: Cannot allocate memory for FSPData!\n");
        exit(-1);
    }

    // Read the samples, for the moment we assume the VRVIS settings:
    // Z is the fastest changin coordinate, Y the slowest changing
    // one.

    fprintf(stderr,
            "      Allocated %d bytes for %d elements of %d x %d x %d grid\n",
            dxy * dz * sizeof(FSPData), dxy * dz, dx, dy, dz);
    fprintf(stderr, "      Reading %d samples.\n", n);
    n = 2;

    for ( iy=1 ; iy<dy-1 ; iy++ )
    {
        for ( ix=1 ; ix<dx-1 ; ix++ )
        {
            for ( iz=1 ; iz<dz-1 ; iz++ )
            {
                FSPData * p = DATAPTR(ix, iy, iz);
                printf("      Reading line #%d (%d)\n",
                       n, ix + iy * dx + iz * dxy);
                res = fscanf(f,
                             "%f %f %f, %f %f %f %f %f %f %f\n",
                             &p->xw, &p->yw, &p->zw,
                             &p->xt, &p->yt, &p->zt,
                             &p->a, &p->b, &p->c, &p->d);
                if (res != 10)
                {
                    fprintf(stderr, "ERROR: Short read on line %d!\n",
                            n);
                    exit(-1);
                }

                printf("     % 10.5f % 10.5f % 10.5f, "
                       "% 10.5f % 10.5f % 10.5f "
                       "% 8.5f % 8.5f % 8.5f % 8.5f\n",
                       p->xw, p->yw, p->zw,
                       p->xt, p->yt, p->zt,
                       p->a, p->b, p->c, p->d);

                n++;
            }
        }
    }

    if (fileName != NULL) fclose(f);

    // First pair of grid sides (xy planes)

    fprintf(stderr, "INFO: Extrapolation to xy front- und back-plane.\n");

    for ( iy=1 ; iy<dy-1 ; iy++ )
    {
        for ( ix=1 ; ix<dx-1 ; ix++ )
        {
            FSPData * p0, * p1, * p2;

            p0 = DATAPTR(ix, iy, 0);
            p1 = DATAPTR(ix, iy, 1);
            p2 = DATAPTR(ix, iy, 2);

            Extrapolate(p2, p1, p0);

            p0 = DATAPTR(ix, iy, dz-1);
            p1 = DATAPTR(ix, iy, dz-2);
            p2 = DATAPTR(ix, iy, dz-3);

            Extrapolate(p2, p1, p0);
        }
    }

    // Second pair of grid sides (xz planes)

    fprintf(stderr, "INFO: Extrapolation to xz front- und back-plane.\n");

    for ( ix=1 ; ix<dx-1 ; ix++ )
    {
        for ( iz=1 ; iz<dz-1 ; iz++ )
        {
            FSPData * p0, * p1, * p2;

            p0 = DATAPTR(ix, 0, iz);
            p1 = DATAPTR(ix, 1, iz);
            p2 = DATAPTR(ix, 2, iz);

            Extrapolate(p2, p1, p0);

            p0 = DATAPTR(ix, dy-1, iz);
            p1 = DATAPTR(ix, dy-2, iz);
            p2 = DATAPTR(ix, dy-3, iz);

            Extrapolate(p2, p1, p0);
        }
    }

    // Third pair of grid sides (yz planes)

    fprintf(stderr, "INFO: Extrapolation to yz front- und back-plane.\n");

    for ( iy=1 ; iy<dy-1 ; iy++ )
    {
        for ( iz=1 ; iz<dz-1 ; iz++ )
        {
            FSPData * p0, * p1, * p2;

            p0 = DATAPTR(0, iy, iz);
            p1 = DATAPTR(1, iy, iz);
            p2 = DATAPTR(2, iy, iz);

            Extrapolate(p2, p1, p0);

            p0 = DATAPTR(dx-1, iy, iz);
            p1 = DATAPTR(dx-2, iy, iz);
            p2 = DATAPTR(dx-3, iy, iz);

            Extrapolate(p2, p1, p0);
        }
    }

    // Extrapolate the four edges along x axis 

    for ( ix=1 ; ix<dx-1 ; ix++ )
    {
        FSPData * p0, * p1, * p2;
        FSPData t1, t2;

        /* [ix, 0, 0] */
        p0 = DATAPTR(ix, 0, 0);
        p1 = DATAPTR(ix, 1, 0);
        p2 = DATAPTR(ix, 2, 0);
        Extrapolate(p2, p1, &t1);
        p1 = DATAPTR(ix, 0, 1);
        p2 = DATAPTR(ix, 0, 2);
        Extrapolate(p2, p1, &t2);
        Average2(&t1, &t2, p0);

        /* [ix, 0, dz-1] */
        p0 = DATAPTR(ix, 0, dz-1);
        p1 = DATAPTR(ix, 0, dz-2);
        p2 = DATAPTR(ix, 0, dz-3);
        Extrapolate(p2, p1, &t1);
        p1 = DATAPTR(ix, 1, dz-1);
        p2 = DATAPTR(ix, 2, dz-1);
        Extrapolate(p2, p1, &t2);
        Average2(&t1, &t2, p0);

        /* [ix, dy-1, 0] */
        p0 = DATAPTR(ix, dy-1, 0);
        p1 = DATAPTR(ix, dy-2, 0);
        p2 = DATAPTR(ix, dy-3, 0);
        Extrapolate(p2, p1, &t1);
        p1 = DATAPTR(ix, dy-1, 1);
        p2 = DATAPTR(ix, dy-1, 2);
        Extrapolate(p2, p1, &t2);
        Average2(&t1, &t2, p0);

        /* [ix, dy-1, dz-1] */
        p0 = DATAPTR(ix, dy-1, dz-1);
        p1 = DATAPTR(ix, dy-1, dz-2);
        p2 = DATAPTR(ix, dy-1, dz-3);
        Extrapolate(p2, p1, &t1);
        p1 = DATAPTR(ix, dy-2, dz-1);
        p2 = DATAPTR(ix, dy-3, dz-1);
        Extrapolate(p2, p1, &t2);
        Average2(&t1, &t2, p0);
     }
        
    // Extrapolate the four edges along y axis 

    for ( iy=1 ; iy<dy-1 ; iy++ )
    {
        FSPData * p0, * p1, * p2;
        FSPData t1, t2;

        /* [0, iy, 0] */
        p0 = DATAPTR(0, iy, 0);
        p1 = DATAPTR(1, iy, 0);
        p2 = DATAPTR(2, iy, 0);
        Extrapolate(p2, p1, &t1);
        p1 = DATAPTR(0, iy, 1);
        p2 = DATAPTR(0, iy, 2);
        Extrapolate(p2, p1, &t2);
        Average2(&t1, &t2, p0);

        /* [0, iy, dz-1] */
        p0 = DATAPTR(0, iy, dz-1);
        p1 = DATAPTR(0, iy, dz-2);
        p2 = DATAPTR(0, iy, dz-3);
        Extrapolate(p2, p1, &t1);
        p1 = DATAPTR(1, iy, dz-1);
        p2 = DATAPTR(2, iy, dz-1);
        Extrapolate(p2, p1, &t2);
        Average2(&t1, &t2, p0);

        /* [dx-1, iy, 0] */
        p0 = DATAPTR(dx-1, iy, 0);
        p1 = DATAPTR(dx-2, iy, 0);
        p2 = DATAPTR(dx-3, iy, 0);
        Extrapolate(p2, p1, &t1);
        p1 = DATAPTR(dx-1, iy, 1);
        p2 = DATAPTR(dx-1, iy, 2);
        Extrapolate(p2, p1, &t2);
        Average2(&t1, &t2, p0);

        /* [dx-1, iy, dz-1] */
        p0 = DATAPTR(dx-1, iy, dz-1);
        p1 = DATAPTR(dx-1, iy, dz-2);
        p2 = DATAPTR(dx-1, iy, dz-3);
        Extrapolate(p2, p1, &t1);
        p1 = DATAPTR(dx-2, iy, dz-1);
        p2 = DATAPTR(dx-3, iy, dz-1);
        Extrapolate(p2, p1, &t2);
        Average2(&t1, &t2, p0);
    }
        
    // Extrapolate the four edges along z axis 

    for ( iz=1 ; iz<dz-1 ; iz++ )
    {
        FSPData * p0, * p1, * p2;
        FSPData t1, t2;

        /* [0, 0, iz] */
        p0 = DATAPTR(0, 0, iz);
        p1 = DATAPTR(1, 0, iz);
        p2 = DATAPTR(2, 0, iz);
        Extrapolate(p2, p1, &t1);
        p1 = DATAPTR(0, 1, iz);
        p2 = DATAPTR(0, 2, iz);
        Extrapolate(p2, p1, &t2);
        Average2(&t1, &t2, p0);

        fprintf(stdout,
                "##### % 10.5f % 10.5f % 10.5f, "
                "% 10.5f % 10.5f % 10.5f "
                "% 8.5f % 8.5f % 8.5f % 8.5f\n",
                p0->xw, p0->yw, p0->zw,
                p0->xt, p0->yt, p0->zt,
                p0->a, p0->b, p0->c, p0->d);

        /* [0, dy-1, iz] */
        p0 = DATAPTR(0, dy-1, iz);
        p1 = DATAPTR(0, dy-2, iz);
        p2 = DATAPTR(0, dy-3, iz);
        Extrapolate(p2, p1, &t1);
        p1 = DATAPTR(1, dy-1, iz);
        p2 = DATAPTR(2, dy-1, iz);
        Extrapolate(p2, p1, &t2);
        Average2(&t1, &t2, p0);

        fprintf(stdout,
                "##### % 10.5f % 10.5f % 10.5f, "
                "% 10.5f % 10.5f % 10.5f "
                "% 8.5f % 8.5f % 8.5f % 8.5f\n",
                p0->xw, p0->yw, p0->zw,
                p0->xt, p0->yt, p0->zt,
                p0->a, p0->b, p0->c, p0->d);

        /* [dx-1, 0, iz] */
        p0 = DATAPTR(dx-1, 0, iz);
        p1 = DATAPTR(dx-2, 0, iz);
        p2 = DATAPTR(dx-3, 0, iz);
        Extrapolate(p2, p1, &t1);
        p1 = DATAPTR(dx-1, 1, iz);
        p2 = DATAPTR(dx-1, 2, iz);
        Extrapolate(p2, p1, &t2);
        Average2(&t1, &t2, p0);

        fprintf(stdout,
                "##### % 10.5f % 10.5f % 10.5f, "
                "% 10.5f % 10.5f % 10.5f "
                "% 8.5f % 8.5f % 8.5f % 8.5f\n",
                p0->xw, p0->yw, p0->zw,
                p0->xt, p0->yt, p0->zt,
                p0->a, p0->b, p0->c, p0->d);

        /* [dz-1, dy-1, iz] */
        p0 = DATAPTR(dx-1, dy-1, iz);
        p1 = DATAPTR(dx-1, dy-2, iz);
        p2 = DATAPTR(dx-1, dy-3, iz);
        Extrapolate(p2, p1, &t1);
        p1 = DATAPTR(dx-2, dy-1, iz);
        p2 = DATAPTR(dx-3, dy-1, iz);
        Extrapolate(p2, p1, &t2);
        Average2(&t1, &t2, p0);

        fprintf(stdout,
                "##### % 10.5f % 10.5f % 10.5f, "
                "% 10.5f % 10.5f % 10.5f "
                "% 8.5f % 8.5f % 8.5f % 8.5f\n",
                p0->xw, p0->yw, p0->zw,
                p0->xt, p0->yt, p0->zt,
                p0->a, p0->b, p0->c, p0->d);
    }

    // Extrapolate corners

    {
        FSPData * p0, * p1, * p2;
        FSPData t1, t2, t3;

        /* [0, 0, 0] */
        p0 = DATAPTR(0, 0, 0);
        p1 = DATAPTR(1, 0, 0);
        p2 = DATAPTR(2, 0, 0);
        Extrapolate(p2, p1, &t1);
        p1 = DATAPTR(0, 1, 0);
        p2 = DATAPTR(0, 2, 0);
        Extrapolate(p2, p1, &t2);
        p1 = DATAPTR(0, 0, 1);
        p2 = DATAPTR(0, 0, 2);
        Extrapolate(p2, p1, &t3);
        Average3(&t1, &t2, &t3, p0);

        /* [0, 0, dz-1] */
        p0 = DATAPTR(0, 0, dz-1);
        p1 = DATAPTR(1, 0, dz-1);
        p2 = DATAPTR(2, 0, dz-1);
        Extrapolate(p2, p1, &t1);
        p1 = DATAPTR(0, 1, dz-1);
        p2 = DATAPTR(0, 2, dz-1);
        Extrapolate(p2, p1, &t2);
        p1 = DATAPTR(0, 0, dz-2);
        p2 = DATAPTR(0, 0, dz-3);
        Extrapolate(p2, p1, &t3);
        Average3(&t1, &t2, &t3, p0);

        /* [0, dy-1, 0] */
        p0 = DATAPTR(0, dy-1, 0);
        p1 = DATAPTR(1, dy-1, 0);
        p2 = DATAPTR(2, dy-1, 0);
        Extrapolate(p2, p1, &t1);
        p1 = DATAPTR(0, dy-2, 0);
        p2 = DATAPTR(0, dy-3, 0);
        Extrapolate(p2, p1, &t2);
        p1 = DATAPTR(0, dy-1, 1);
        p2 = DATAPTR(0, dy-1, 2);
        Extrapolate(p2, p1, &t3);
        Average3(&t1, &t2, &t3, p0);

        /* [dx-1, 0, 0] */
        p0 = DATAPTR(dx-1, 0, 0);
        p1 = DATAPTR(dx-2, 0, 0);
        p2 = DATAPTR(dx-3, 0, 0);
        Extrapolate(p2, p1, &t1);
        p1 = DATAPTR(dx-1, 1, 0);
        p2 = DATAPTR(dx-1, 2, 0);
        Extrapolate(p2, p1, &t2);
        p1 = DATAPTR(dx-1, 0, 1);
        p2 = DATAPTR(dx-1, 0, 2);
        Extrapolate(p2, p1, &t3);
        Average3(&t1, &t2, &t3, p0);

        /* [0, dy-1, dz-1] */
        p0 = DATAPTR(0, dy-1, dz-1);
        p1 = DATAPTR(1, dy-1, dz-1);
        p2 = DATAPTR(2, dy-1, dz-1);
        Extrapolate(p2, p1, &t1);
        p1 = DATAPTR(0, dy-2, dz-1);
        p2 = DATAPTR(0, dy-3, dz-1);
        Extrapolate(p2, p1, &t2);
        p1 = DATAPTR(0, dy-1, dz-2);
        p2 = DATAPTR(0, dy-1, dz-3);
        Extrapolate(p2, p1, &t3);
        Average3(&t1, &t2, &t3, p0);

        /* [dx-1, 0, dz-1] */
        p0 = DATAPTR(dx-1, 0, dz-1);
        p1 = DATAPTR(dx-2, 0, dz-1);
        p2 = DATAPTR(dx-3, 0, dz-1);
        Extrapolate(p2, p1, &t1);
        p1 = DATAPTR(dx-1, 1, dz-1);
        p2 = DATAPTR(dx-1, 2, dz-1);
        Extrapolate(p2, p1, &t2);
        p1 = DATAPTR(dx-1, 0, dz-2);
        p2 = DATAPTR(dx-1, 0, dz-3);
        Extrapolate(p2, p1, &t3);
        Average3(&t1, &t2, &t3, p0);

        /* [dx-1, dy-1, 0] */
        p0 = DATAPTR(dx-1, dy-1, 0);
        p1 = DATAPTR(dx-2, dy-1, 0);
        p2 = DATAPTR(dx-3, dy-1, 0);
        Extrapolate(p2, p1, &t1);
        p1 = DATAPTR(dx-1, dy-2, 0);
        p2 = DATAPTR(dx-1, dy-3, 0);
        Extrapolate(p2, p1, &t2);
        p1 = DATAPTR(dx-1, dy-1, 1);
        p2 = DATAPTR(dx-1, dy-1, 2);
        Extrapolate(p2, p1, &t3);
        Average3(&t1, &t2, &t3, p0);

        /* [dx-1, dy-1, dz-1] */
        p0 = DATAPTR(dx-1, dy-1, dz-1);
        p1 = DATAPTR(dx-2, dy-1, dz-1);
        p2 = DATAPTR(dx-3, dy-1, dz-1);
        Extrapolate(p2, p1, &t1);
        p1 = DATAPTR(dx-1, dy-2, dz-1);
        p2 = DATAPTR(dx-1, dy-3, dz-1);
        Extrapolate(p2, p1, &t2);
        p1 = DATAPTR(dx-1, dy-1, dz-2);
        p2 = DATAPTR(dx-1, dy-1, dz-3);
        Extrapolate(p2, p1, &t3);
        Average3(&t1, &t2, &t3, p0);
    }

#ifdef NEVER
#endif
        
    // Write it out

    fprintf(stderr, "INFO: Writing out the extrapolated data.\n");

    fileName = optFileOut.getValue();

    if (fileName == NULL)
    {
        f = stdout;
    }
    else
    {
        f = fopen(fileName, "w");
    }

    if (f == NULL)
    {
	perror("ERROR: Cannot open output file");
	exit(-1);
    }

    printf("INFO: Saving output data.\n");

    // Write the FSP header 

    fputs(FSPDATA_HEADER"\n", f);
    fprintf(f, "%d %d %d\n", dx, dy, dz);
    
    /// Write out the data

    for ( iy=0 ; iy<dy ; iy++ )
    {
        for ( ix=0 ; ix<dx ; ix++ )
        {
            for ( iz=0 ; iz<dz ; iz++ )
            {
                FSPData * p = DATAPTR(ix, iy, iz);

                assert(finite(p->xt));
                assert(finite(p->yt));
                assert(finite(p->zt));

                fprintf(f,
			"% 10.5f % 10.5f % 10.5f, "
			"% 10.5f % 10.5f % 10.5f "
			"% 8.5f % 8.5f % 8.5f % 8.5f\n",
                        p->xw, p->yw, p->zw,
                        p->xt, p->yt, p->zt,
                        p->a, p->b, p->c, p->d);
            }
        }
    }

    if (fileName != NULL) fclose (f);
    free (data);
}

/* ===========================================================================
   End of fspext.cxx
   ===========================================================================
   Automatic Emacs configuration follows.
   Local Variables:
   mode:c++
   c-basic-offset: 4
   eval: (c-set-offset 'substatement-open 0)
   eval: (c-set-offset 'case-label '+)
   eval: (c-set-offset 'statement 'c-lineup-runin-statements)
   eval: (setq indent-tabs-mode nil)
   End:
   =========================================================================== */
