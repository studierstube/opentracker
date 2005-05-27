/* ===========================================================================
    Copyright (c) 1999,2000
    Vienna University of Technology
    
    This software is provided AS IS, without any express or implied
    warranty.  In no event will the authors or the Vienna University of
    Technology be held liable for any damages or loss of profit arising
    from the use or non-fitness for a particular purpose of this software.
   ===========================================================================
    NAME:       raw2sample
    TYPE:       c++ code
    PROJECT:    Studierstube - Undistortion tools
    CONTENT:    Convert raw output to sample or fsp formats
   ===========================================================================
    AUTHORS:    jp      Jan Prikryl
                ds      Dieter Schmalstieg
   ===========================================================================
    HISTORY:

    22-Jan-01 23:47:11  jp      last modification
    01-Jan-99 00:00:00  ds      created
=========================================================================== */

/*
    TODO

    - describe input file format .raw
    - describe whole process tmeas->raw2sample->tolookup
    - allow everything as command line options
    - order of data xyz may be changed
    - integrate with stapi code base
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stbapi/tracker/DataInterpolator.h>
#include <stbapi/util/option.h>

#define BUFFER_SIZE	1000

void
main(int argc, char* argv[])
{
    //--paramters
    OptionString optFileName("file",    "f",  NULL);
    OptionString optFileOut ("output",  "o",  NULL);
    OptionDouble optSpacingX("spacingX","sx", 0.1);
    OptionDouble optSpacingY("spacingY","sy", 0.1);
    OptionDouble optSpacingZ("spacingZ","sz", 0.1);
    OptionDouble optOffsetX ("offsetX", "ox", 0.0);
    OptionDouble optOffsetY ("offsetY", "oy", 0.0);
    OptionDouble optOffsetZ ("offsetZ", "oz", 0.0);

    if(!Option::parse(argc,argv)) { printf("syntax error\n"); exit(-1); }
    const char* fileName = optFileName.getValue();

    float spacingX = (float) optSpacingX.getValue();
    float spacingY = (float) optSpacingY.getValue();
    float spacingZ = (float) optSpacingZ.getValue();
    float offsetX  = (float) optOffsetX.getValue();
    float offsetY  = (float) optOffsetY.getValue();
    float offsetZ  = (float) optOffsetZ.getValue();

    //--variables and inits
    int    x,y,z,nx,ny,nz;
    float  px,py,pz;
    float  a,b,c,d;
    int    dummy, res;
    char   dummyBuffer[BUFFER_SIZE];

    // Input file
    FILE * fi = fopen(fileName, "r");
    if (fi == NULL)
    {
	perror("ERROR: Cannot open input file");
	exit(-1);
    }

    // Open output file if it was specified. Write data do standard
    // output otherwise.

    const char *outName = optFileOut.getValue();
    FILE * fo;

    if (outName == NULL)
    {
	fo = stdout;
    }
    else
    {
	fo = fopen(outName, "w");
    }

    if (fo == NULL)
    {
	perror("ERROR: Cannot open output file");
	exit(-1);
    }

    // Check the format of the input data
    
    fscanf(fi,"%s\n",dummyBuffer);
    if ( strcmp(dummyBuffer, RAWDATA_HEADER) )
    {
	fprintf(stderr, "ERROR: Wrong input data header!\n");
	exit(-1);
    }

    res = fscanf(fi,"%d %d %d\n", &nx, &ny, &nz);
    if (res != 3)
    {
	fprintf(stderr, "ERROR: Wrong grid dimensions!\n");
	exit(-1);
    }

    // Main loop
    
#ifdef _RAW2FSP_
    fputs(FSPDATA_HEADER"\n", fo); 
#else
    fputs(SAMPLEDATA_HEADER"\n", fo); 
#endif
    fprintf(fo, "%d %d %d\n", nx, ny, nz);

    for ( y=0; y<ny; y++ )
	for ( x=0; x<nx; x++ )
        {
	    fgets(dummyBuffer, BUFFER_SIZE, fi); //skip measure set heading
	    for ( z=0; z<nz; z++ )
            {
		res = fscanf(fi,
			     "%d: X=%f Y=%f Z=%f, A=%f B=%f C=%f D=%f\n",
			     &dummy, &px, &py, &pz, &a, &b, &c, &d);
		if(!res)
		{
		    fprintf(stderr, "ERROR: Wrong data format!\n");
		    exit(-1);
		}

		float xworld = x * spacingX + offsetX;
		float yworld = y * spacingY + offsetY;
		float zworld = z * spacingZ + offsetZ;
#ifdef _RAW2FSP_
		fprintf(fo,
			"% 10.5f % 10.5f % 10.5f, "
			"% 10.5f % 10.5f % 10.5f "
			"% 8.5f % 8.5f % 8.5f % 8.5f\n",
			xworld, yworld, zworld, px, py, pz, a, b, c, d);
#else
		fprintf(fo,
			"% 10.5f % 10.5f % 10.5f, "
			"% 10.5f % 10.5f % 10.5f\n",
			xworld, yworld, zworld, px, py, pz);
#endif
            }
        }

    fclose(fi);
    fclose(fo);
}

/* ===========================================================================
   End of raw2sample.cxx
   ===========================================================================
   Automatic Emacs configuration follows.
   Local Variables:
   mode:c++
   c-basic-offset: 4
   eval: (c-set-offset 'substatement-open 0)
   eval: (c-set-offset 'case-label '+)
   eval: (c-set-offset 'statement 'c-lineup-runin-statements)
   End:
   =========================================================================== */
