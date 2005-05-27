/* ===========================================================================
    no copyright
   ===========================================================================
    NAME:       tolookup
    TYPE:       c++ code
    PROJECT:    Studierstube/VT
    CONTENT:    Converts scattered data to lookup-table
    VERSION:    1.15
   ===========================================================================
    AUTHORS:    fd      Falk Dobermann (fakl@iname.com)
                gh      Gerd Hesina
                jp      Jan Prikryl
   ===========================================================================
    HISTORY:

    22-Jan-01 23:47:38  jp      last modification
    07-apr-99 14:38:48  gh      created
=========================================================================== */

#include <assert.h>
#include <stdio.h>
#include <string.h>

#include <stbapi/tracker/DataInterpolator.h>
#include "tolookup.h"

//Variables that hold the parameters

char * sampleFileName;
char * outFileName;
int    number=3;			//number of points to interpolate
double radius;				//radius used for intepolation
int    size;

void read_indata(FILE *infile, in_data *in_data, int size, out_data_desc *table)
{
    int zv;
    float wx,wy,wz,tx,ty,tz;
    table->llb[0] = -1000; table->llb[1] = 1000; table->llb[2] = 1000;
    table->urt[0] = 1000; table->urt[1] = -1000; table->urt[2] = -1000;
    for (zv = 0; zv < size; zv++)
    {
	//read one sample (real point, measured point)
	if (fscanf(infile, "%f %f %f,%f %f %f\n",&wx,&wy,&wz,&tx,&ty,&tz) != 6)
	{
	    cout << "Error in sample file" << endl;
	    exit (-5);
	}

	in_data[zv].data[0] = wx;
	in_data[zv].data[1] = wy;
	in_data[zv].data[2] = wz;
	in_data[zv].measured[0] = tx;
	in_data[zv].measured[1] = ty;
	in_data[zv].measured[2] = tz;

	//set the bounds of the lookup table cube
	// llb = lower left bottom; urt = upper right top
	if (in_data[zv].measured[0] > table->llb[0] ) table->llb[0] = in_data[zv].measured[0];
	if (in_data[zv].measured[0] < table->urt[0] ) table->urt[0] = in_data[zv].measured[0];
	if (in_data[zv].measured[1] < table->llb[1] ) table->llb[1] = in_data[zv].measured[1];
	if (in_data[zv].measured[1] > table->urt[1] ) table->urt[1] = in_data[zv].measured[1];
	if (in_data[zv].measured[2] < table->llb[2] ) table->llb[2] = in_data[zv].measured[2];
	if (in_data[zv].measured[2] > table->urt[2] ) table->urt[2] = in_data[zv].measured[2];

    
    }
}

void allocate_table_mem(out_data_desc *table, in_data *in_data)
{
    cout << "allocate_table_mem" << endl;
    int zv;
    for (zv = 0; zv < 3; zv++)
    {

	table->dims[zv] = abs((int) ((table->urt[zv] - table->llb[zv]) / table->resolution +1));
	cout << "Dims : " << table->dims[zv] << endl;
	cout << "llb  : " << table->llb[zv] << endl;
	cout << "urt  : " << table->urt[zv] << endl;
    }

#ifdef DEBUG
    for (zv = 0; zv < size; zv++)
    {
	int t[3];
	t[0] = LOOK_X(zv);
	t[1] = LOOK_Y(zv);
	t[2] = LOOK_Z(zv);
	cout << t[0] << " " << t[1] << " " << t[2] << endl;
    }
#endif

    //allocate mem
    table -> outData = new class out_data [table ->dims[0] * 
					  table ->dims[1] * table ->dims[2]];
    //fill table with lookup information
    int ix, iy, iz;
    for (zv =0; zv <(table ->dims[0] * table ->dims[1] * table ->dims[2]); zv++)
    {
	//get the current 3d indizes for iterator
	ix = LOOK_X(zv);
	iy = LOOK_Y(zv);
	iz = LOOK_Z(zv);

	//initialize current field with measurment coordinates
	table->outData[zv].measured_point[0] = ix * (-table->resolution) + 
	    table->llb[0];
	table->outData[zv].measured_point[1] = iy * table->resolution + 
	    table->llb[1];
	table->outData[zv].measured_point[2] = iz * table->resolution + 
	    table->llb[2];
    }

} 

//returns the distance between tho 3d points
double get_distance(double one[3], double two[3])
{
    double t1 = two[0] - one[0];
    double t2 = two[1] - one[1];
    double t3 = two[2] - one[2];

    return sqrt(t1*t1 + t2*t2 + t3*t3);
}

//interpolates every point in the lookup table grid
void calculate_lookup(out_data_desc *table, in_data *in_data)
{
    int zv;
    class in_data **env = new p_in_data[number];
    int number_in;
    out_data *active;		//currently handeled grid point

    //every point in the grid (in 1d array)
    for (zv = 0; zv < (table->dims[0] * table->dims[1] * table->dims[2]); zv++)
    {
	int t[3];

	//get the 3d index
	t[0] = LOOK_X(zv);
	t[1] = LOOK_Y(zv);
	t[2] = LOOK_Z(zv);
	// Eeeeee? 
	// if(t[0]) ; // dummy
#ifdef DEBUG
	cout << t[0] << " " << t[1] << " " << t[2] << endl;
#endif

	double d;
	active = &(table->outData[zv]);
    
	//calculate the nearest points of measured data
	number_in = 0;
	for (int zv2 = 0; zv2 < size; zv2++)
	{
	    d = get_distance(active->measured_point, in_data[zv2].measured);
	    if (d < radius)
	    {
		//only consider number samples for interpolation
		if (number_in < number) 
		{
		    env[number_in] = &(in_data[zv2]);
		    number_in++;
		}
		else
		{
		    //remove the longest away sample from consideration list
		    int zv3;
		    int farest;
		    double f_dist = -10000;
		    for (zv3 = 0; zv3 < number_in; zv3++)
			if (get_distance(env[zv3]->measured, active->measured_point) > f_dist) 
			{
			    f_dist = get_distance(env[zv3]->measured, active->measured_point);
			    farest = zv3;
			}
		    if (d<f_dist) env[farest] = &(in_data[zv2]);
		}
	    }
	}
	//now interpolate
	double all_weight = 0;
	cout << number << " " << number_in << endl;
	if (number_in<number)
	{
	    active->set = false;
	}
	else
	{
	    for (int zv2 = 0; zv2 < number_in; zv2++)
	    {

		d = radius - get_distance(env[zv2]->measured, active->measured_point); 
		active->real_point[0] += env[zv2]->data[0] * d;
		active->real_point[1] += env[zv2]->data[1] * d;
		active->real_point[2] += env[zv2]->data[2] * d;
		all_weight += d;
		cout << "Point No. " << zv2 << " is x=" << env[zv2]->data[0] 
		     << " y=" << env[zv2]->data[1] << " z=" << env[zv2]->data[2] 
		     << " distance=" << d << endl;
	    }
	    if (all_weight)
	    {
		active->real_point[0] /= all_weight;
		active->real_point[1] /= all_weight;
		active->real_point[2] /= all_weight;
		cout << "Interpolated data  x=" << active->real_point[0] 
		     << " y=" << active->real_point[1] 
		     << " z=" << active->real_point[2] << endl;
		cout << "Measured Point x=" << active->measured_point[0] 
		     << " y=" << active->measured_point[1] 
		     << " z=" << active->measured_point[2] << endl;
		active->set = true;
	    } else active->set = false;
	}
    }
}

//saves the lookup table
void save_lookup(out_data_desc *table)
{
    FILE *outfile = fopen (outFileName, "w");
    if (!outfile)
    {
	cerr << "Could not write output file" << endl;
	exit(-20);
    }
    int zv, zv2;
    fprintf(outfile, "VT_LOOKUP_TABLE\n");			//ID
    for (zv = 0; zv < 3; zv++) fprintf(outfile, "%f\n", table->llb[zv]);	//lower left front corner
    for (zv = 0; zv < 3; zv++) fprintf(outfile, "%f\n", table->urt[zv]);	//upper right back corner
    for (zv = 0; zv < 3; zv++) fprintf(outfile, "%d\n", table->dims[zv]);	//number of indexes for 3d
    fprintf(outfile, "%f\n", table->resolution);				//grid resolution

    for (zv = 0; zv < (table->dims[0] * table->dims[1] * table->dims[2]); zv++)	//data
    {
	fprintf(outfile, "%d", table->outData[zv].set);
	for (zv2 = 0; zv2 < 3; zv2++)
	    fprintf(outfile, " %f", table->outData[zv].real_point[zv2]);
	fprintf(outfile, ",");
	for (int zv2 = 0; zv2 < 3; zv2++)
	    fprintf(outfile, "%f ", table->outData[zv].measured_point[zv2]);
	fprintf(outfile, "\n");
    }
}


void main(int argc, char* argv[])
{

    //command line options
    OptionString optSampleFile("sample","s");
    OptionString optOutfile("out", "o");
    OptionLong   optNumber("number", "n", 3);
    OptionDouble optRadius("radius", "r", 4);
    OptionDouble optRes("distance", "d");

    if(!Option::parse(argc,argv)) 
    {
	Option::printUsage();
	exit(-1);
    }
  
    if (optSampleFile.isSpecified()) sampleFileName = (char *) optSampleFile.getValue();
    else
    {
	sampleFileName = new char [256];
	cout << "Please enter scattered data file name (sample name): " << flush;
	cin >>  sampleFileName;
    }
  
    //copy command line options to global variables or ask user to specify

    if (optOutfile.isSpecified()) outFileName = (char *) optOutfile.getValue();
    else
    {
	outFileName = new char [256];
	cout << "Please enter output file name: " << flush;
	cin >> outFileName;
    }

    if (optNumber.isSpecified()) number = optNumber.getValue();
    else
    {
	cout << "Please enter the maximum number of surrounding points to be considered: " << flush;
	cin >> number;
    }

    if (optRadius.isSpecified()) radius = optRadius.getValue();
    else

    {
	cout << "Please enter the radius for surrounding points to be considered :" << flush;
	cin >> radius;
    }

    FILE *infile;
    infile = fopen(sampleFileName, "r");
    if (!infile)
    {
	cout << "Could not open sample file" << endl;
	exit(-2);
    }

    // Check the format of the input data
    
    char buffer[1000];
    fscanf(infile, "%s\n", buffer);
    if ( strcmp(buffer, SAMPLEDATA_HEADER) )
    {
	fprintf(stderr, "ERROR: Wrong input data header!\n");
	exit(-1);
    }

    if (fscanf(infile, "%i", &size) != 1) 
    {
	fprintf(stderr, "ERROR: Cannot get the number of samples!\n");
	exit (-3);
    }

    cout << "Number of raw data " << size << endl;
  
    in_data *in_data = new class in_data [size];
    if (!in_data) 
    {
	cout << "Could not allocate enough memory for data samples" << endl;
	exit(-4);
    }
    out_data_desc *table = new out_data_desc;

    if (optRes.isSpecified()) table->resolution = optRes.getValue();
    else
    {
	cout << "Please enter the resolution for the lookup cube :" << flush;
	cin >> table->resolution;
    }

    //read the sample file
    read_indata(infile, in_data, size, table);

    //allocate memory for lookup table and initialize
    allocate_table_mem(table, in_data);

    //fill lookup table
    calculate_lookup(table, in_data);

    //save ready table
    save_lookup(table);

}
  
/* ===========================================================================
   End of tolookup.cxx
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
  
