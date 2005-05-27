/* ===========================================================================
    Copyright (c) 1996,1997,1998,1999,2000
    Vienna University of Technology
    
    This software is provided AS IS, without any express or implied
    warranty.  In no event will the authors or the Vienna University of
    Technology be held liable for any damages or loss of profit arising
    from the use or non-fitness for a particular purpose of this software.
   ===========================================================================
    NAME:       tmeas.cxx
    TYPE:       c++ code
    PROJECT:    Studierstube - Undistortion tools
    CONTENT:    Tracker undistortion data acqusition tool
    VERSION:    1.35
   ===========================================================================
    AUTHORS:    ds      Dieter Schmalstieg
                jp      Jan Prikryl
   ===========================================================================
    HISTORY:

    22-Jan-01 23:46:30  jp      last modification
    01-Jan-99 00:00:00  ds      created
=========================================================================== */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifndef WIN32
#include <unistd.h>
#ifdef _SGI_SOURCE
#include <alloca.h>
#endif
#else
#include <malloc.h>
#define alloca _alloca
#endif

#include <stbapi/tracker/DataInterpolator.h>
#include <stbapi/tracker/TrackerBuilder.h>

// Auxiliary data structure for oversampling
typedef struct {
    double pos[3];
    double rot[4];
} SumState;

int
main(int argc, char *argv[])
{
    // Commandline options for the tracker client
    TrackerBuilderOptions   tbo;
    // Additional command line options
    OptionLong   optst("st","s",0);
    OptionLong   optOversample("oversample","e",10);
    OptionLong   optNoX("numXvalues","nx",0);
    OptionLong   optNoY("numYvalues","ny",0);
    OptionLong   optNoZ("numZvalues","nz",0);
    OptionString optFileName("output", "o", NULL);
    
    if(!Option::parse(argc,argv))
    {
        Option::printUsage();
        exit(-1);
    }
    
    // User has to specify dimensions of the calibration grid
    int nx = optNoX.getValue();
    int ny = optNoY.getValue();
    int nz = optNoZ.getValue();
    
    if (nx*ny*nz == 0)
    {
        fprintf(stderr, "ERROR: Specify number of points in X, Y, and Z!\n");
        exit(-1);
    }

    // Set up tracker
    TrackerState state;
    TrackerBuilderParameter tbp = tbo.getParameter();
    Tracker * tracker = TrackerBuilder::createTracker(tbp);
    tracker->activate();
    
    // Set up oversampling
    int oversample = optOversample.getValue();

    if (oversample <= 0)
    {
	fprintf(stderr, "ERROR: Oversample count has to be at least one!\n");
	exit(-1);
    }

    if (nz > tbp.stationNumber) 
    {
	fprintf(stderr,
		"ERROR: Cannot sample %d values in Z with only %d stations!\n",
		nz, tbp.stationNumber);
	exit(-1);
    }

    int        sumSize = oversample * tbp.stationNumber * sizeof(SumState);
    SumState * sum     = (SumState *) alloca(sumSize);
    
    if (sum == NULL)
    {
        fprintf(stderr, "ERROR: Cannot allocate oversample data!\n");
        exit(-1);
    }
    
    // Which station has the button?
    int st    = optst.getValue();
    int count = 0;
    
    FILE *output;
    const char *fileName = optFileName.getValue();
    
    if (fileName == NULL)
    {
        output = stdout;
    }
    else
    {
        output = fopen(fileName, "w");
        if (output == NULL)
        {
            perror("ERROR: Can't open output");
            exit(1);
        }
    }
    
    fprintf(stderr, "INFO: Tracker ready, output file opened.\n");
    fprintf(stderr, "      You can start measuring data.\n");
    
    // Output file header
    fputs(RAWDATA_HEADER"\n", output);
    fprintf(output, "%d %d %d\n", nx, ny, nz);
    
    // Measure the data
    for ( int y=0; y<ny; y++)
        for ( int x=0; x<nx; x++)
        {
            // Active waiting on button press 
            while (!state.station[st].buttonState[0])
            {
                tracker->getState(state);
            }
            
            // Z-set header
            fprintf(output, "# x%02d y%02d\n", x, y);
            if (output != stdout) printf("# x%02d y%02d\n", x, y);
            
            // Oversample position and rotation data
            memset(sum, 0, sumSize);
	    printf("memset ok\n");
            for ( int n=0; n<oversample; n++ )
            {
                tracker->getState(state);
		printf("getstate ok\n");
                // Read data from 'nz' sensors in Z-axis
                for ( int z=0; z<nz; z++ )
                {
                    sum[z].pos[0] += state.station[z].position[0];        
                    sum[z].pos[1] += state.station[z].position[1];        
                    sum[z].pos[2] += state.station[z].position[2];
                    sum[z].rot[0] += state.station[z].orientation[0];
                    sum[z].rot[1] += state.station[z].orientation[1];  
                    sum[z].rot[2] += state.station[z].orientation[2];
                    sum[z].rot[3] += state.station[z].orientation[3];
                }
                // Assuming data coming with at least 20Hz - pause for
                // 50 milliseconds or 50000 microseconds.
		printf("sum ok\n");
#ifdef WIN32
                Sleep(50);
#else
                usleep(50000);
#endif
            }
	    printf("loop ok\n");
            
            // Normalize it and write it out
            for (int z=0; z<nz; z++)
            {
                sum[z].pos[0] /= (double) oversample;
                sum[z].pos[1] /= (double) oversample;
                sum[z].pos[2] /= (double) oversample;
                sum[z].rot[0] /= (double) oversample;
                sum[z].rot[1] /= (double) oversample;
                sum[z].rot[2] /= (double) oversample;
                sum[z].rot[3] /= (double) oversample;
                
                fprintf(output,
                    "%4d: X=%9.5f Y=%9.5f Z=%9.5f, "
                    "A=%9.5f B=%9.5f C=%9.5f D=%9.5f\n",
                    count,
                    sum[z].pos[0], sum[z].pos[1], sum[z].pos[2],
                    sum[z].rot[0], sum[z].rot[1], sum[z].rot[2],
                    sum[z].rot[3]);
                fflush(output);
                fprintf(stdout,
                    "%4d: X=%9.5f Y=%9.5f Z=%9.5f, "
                    "A=%9.5f B=%9.5f C=%9.5f D=%9.5f\n",
                    count,
                    sum[z].pos[0], sum[z].pos[1], sum[z].pos[2],
                    sum[z].rot[0], sum[z].rot[1], sum[z].rot[2],
                    sum[z].rot[3]);
                fflush(output);
                count++;
            }
	    printf("norm ok\n");
            
            // Active wait until button released
            while (state.station[st].buttonState[0])
            {
                tracker->getState(state);
            }
        }
        
        fclose(output);
        return 0;
}

/* ===========================================================================
   End of tmeas.cxx
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
