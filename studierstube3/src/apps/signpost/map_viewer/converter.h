

#define INPUT_FILE "stockwerk.iv"
#define TEMP_FILE "__mapv.iv"


typedef struct option_struct
{
	bool convert;
	char * inputfile;
	char * outputfile;
	bool zeropoints;
	bool markers;
	bool marker_ids;
	bool marker_names;
} options;


typedef struct marker 
{
	char pos_x[10];
	char pos_y[10];
	char pos_z[10];

	char orientation[50];
	char num[5];
	char label[25]; 
	struct marker * next;
} marker_struct;


bool convert_file(options * Opt);
