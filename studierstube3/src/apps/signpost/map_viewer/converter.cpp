/******************************************
 ** IV-file CONVERTER
 * @author 09.05.2002 by Thomas Lidy
 * last edited: 09.07.2002
 ******************************************
 * current capabilities:
 * ---------------------
 * processes fields of SoStationKit/SoARRoom nodes
 * to produce an overview of room maps
 * *currently supported fields:
 * SoStationKit
 * - stations
 * - stationPosition
 * - stationOrientation
 * - Transform
 * - floor
 * - roomName
 *
 * IMPORTANT:
 * after every above mentioned token, there 
 * must be at least 1 whitespace!
 * 
 *******************************************
 * @todo
 * check correct Separator } closing
 * portals
 * labels (roomname, marker, x,y coords)
 *******************************************/


#include <stdio.h>
#include <string.h>
#include "converter.h"


#define DEBUG 0


extern void mainerror(const char *);


/* -------- GLOBALS -------- */

FILE * out = stdout;

options * Options;

/* ------------------------- */

void cverror(const char * msg)
{
	if (DEBUG)
	{
		printf("CONVERT: %s\n", msg);
	//	exit (1);
	}
	else
	{
		char temp[255];
		sprintf(temp,"CONVERT: %s\n", msg);
		mainerror(temp);
	}
}


bool read_blanks(FILE * fp)
{
	char c;
	do 
	{
		c = fgetc(fp);
	} while ((c == ' ') || (c == '\t') || (c == '\n') || (c == '\r'));

	if (c == EOF) return false;

	ungetc(c, fp);
	return true;
}


bool read_token(FILE * fp, char * buf)
{
	int i = 0;
	char c = 0;

	while (1)
	{
		c = fgetc(fp);
		if ((c == ' ') || (c == '\t') || (c == '\n') || (c == '\r'))
		{
			buf[i] = '\0';
			ungetc(c, fp);
			return true;
		}
		if (c == EOF)
		{
			buf[i] = '\0';
			return false;
		}
		buf[i] = c;
		i++;
	}


}

void print_header()
{
	fprintf(out,"#Inventor V2.1 ascii\n\n");
	fprintf(out,"Group {\n");
	fprintf(out,"\tSwitch {  #inactive\n");   
	//fprintf(out,"\tDEF MARKER Cube { width 0.4 height 0.4 depth 0.4 } \n");
	fprintf(out,"\tDEF ROT_MARKER Rotation { rotation 0 0 1 -1.57 } \n");
	fprintf(out,"\tDEF ROT_MARKER2 Rotation { rotation 0 1 0 -1.57 } \n");
	fprintf(out,"\tDEF MARKER Cone { height 0.4 bottomRadius 0.2 } \n");
	fprintf(out,"\tDEF MAT_DOORS Material { diffuseColor 0 1 1 } \n");
	fprintf(out,"\tDEF MAT_MARKS Material { diffuseColor 1 0 0 } \n");
	fprintf(out,"\tDEF MAT_FLOOR Material { diffuseColor 0 1 0 } \n");
	fprintf(out,"\tDEF MAT_ZERO  Material { diffuseColor 1 1 0 } \n");
	fprintf(out,"\tDEF PT_ZERO  Sphere { radius 0.1 } \n");
	fprintf(out,"\tDEF ROT_TEXT Rotation { rotation 1 0 0 -1.570796  } \n");
	fprintf(out,"\tDEF SCALE_TEXT Transform { scaleFactor 0.05 0.05 0.05 } \n");
	fprintf(out,"\t}\n\n");
//	fprintf(out,"\tRotation { rotation 1 0 0 1.57 } \n");

	fprintf(out,"\t#absolute (0,0,0)\n");
	fprintf(out,"\tMaterial { diffuseColor 0 0 1 } \n");
	fprintf(out,"\tSphere { radius 0.15 }\n");


}


void deleteMarkerlist(marker_struct * list)
{
	marker_struct * help;
	while (list != NULL)
	{
		help = list;
		list = list->next;
		delete help;
	}
}


void printMarkerlist(marker_struct * list)
{
	if (!Options->markers) return;

	fprintf(out,"\n\tUSE MAT_MARKS\n");

	while (list != NULL)
	{
		//printf("M: %s CO: %s %s %s ORI: %s\n",list->num, list->pos_x, list->pos_y, list->pos_z, list->orientation);
		
		fprintf(out,"\tTransformSeparator { \n");
		fprintf(out,"#\t 'Marker %s'\n",list->num); 
		fprintf(out,"\t  Transform { translation ");
		fprintf(out,"%s 0 %s }\n",list->pos_x,list->pos_z);

	  if (Options->marker_ids)
	  {
		fprintf(out,"\tSeparator {\n");
		fprintf(out,"\t\tUSE ROT_MARKER2\n");
		fprintf(out,"\t  Rotation { rotation %s }\n",list->orientation);
		fprintf(out,"\t\tUSE ROT_TEXT\n");
		fprintf(out,"\t\tUSE SCALE_TEXT\n");
		fprintf(out,"\t\tAsciiText { string \"%s    \" justification RIGHT }\n",list->num);
		fprintf(out,"\t}\n");
	  }
		fprintf(out,"\t  Rotation { rotation %s }\n",list->orientation);
		fprintf(out,"\t  USE ROT_MARKER2\n");
		fprintf(out,"\t  USE ROT_MARKER\n");
		fprintf(out,"\t  USE MARKER\n");
		fprintf(out,"\t}\n");
	  
		
		list = list->next;
	}

}


// read until one of the following appears: , ] }
bool read_to_comma(FILE * fp, char * buf)
{
	char c;
	int i = 0;

	read_blanks(fp);

	while(1) 
	{
		c = fgetc(fp);
		if (c == EOF) return false;
		
		buf[i] = c; 
		i++;
		if ((c == ',') || (c == ']') || (c == '}')) break;
	} 

	buf[i] = '\0';
	return true;
}



marker_struct * cv_STATIONKIT(FILE * fp)
{
	char buf[1024];

	read_blanks(fp);
	read_token(fp,buf);

	if (buf[0] != '{') cverror("{ expected after stationKit");

	read_blanks(fp);
	read_token(fp,buf);

	if (strcmp(buf,"stations") != 0) 	cverror("'stations' expected after stationKit {");

	read_blanks(fp);
	if (fgetc(fp) != '[') cverror("[ expected after stations");

	int nstat = 0;
	char c;
	int l;
	marker_struct * Marker;
	marker_struct * firstMarker;
	marker_struct * prevMarker;

	while(1)
	{
		read_to_comma(fp,buf);
		l = strlen(buf) - 1;
		c = buf[l];
		buf[l] = '\0';

		Marker = new marker_struct;
		strcpy(Marker->num,buf);
		nstat++;
		
		if (nstat == 1)
			firstMarker = Marker;
		if (nstat > 1)
			prevMarker->next = Marker;

		prevMarker = Marker;

		if (c==']') break;
	}

	Marker->next = NULL;
	return firstMarker;
}


bool cv_stationPosition(FILE * fp, marker_struct * Markerlist)
{
	char buf[256];
	unsigned int i;
	int l, s, coord;
	int len[3];
	bool finished = false;
	bool notblank;

	read_blanks(fp);

	if (fgetc(fp) != '[') cverror("[ expected after stationPosition");

	while (!finished)
	{
		read_to_comma(fp,buf);
		l = strlen(buf) - 1;
		if (buf[l] == ']') finished = true;
		buf[l] = '\0';
	
		coord = 0; s = 0; notblank = false;
		len[0] = len[1] = len[2] = 0;

		if (Markerlist == NULL) cverror("Markerlist NULL pointer!!");

		for (i = 0; i < strlen(buf); i++)
		{
			if ((buf[i] == ' ') && (notblank))
			{
				//goto next token/coord.			
				coord ++;
				s = 0;
				notblank = false;
			}
			else
			{
				notblank = true;
				switch(coord)
				{
				case 0:	Markerlist->pos_x[s] = buf[i];
						break;				
				case 1:	Markerlist->pos_y[s] = buf[i];
						break;
				case 2:	Markerlist->pos_z[s] = buf[i];
						break;
				default: //ERROR
					    ;
				}
				len[coord]++;
				s++;
			}
		}

		Markerlist->pos_x[len[0]] = '\0';
		Markerlist->pos_y[len[1]] = '\0';
		Markerlist->pos_z[len[2]] = '\0';
		Markerlist = Markerlist->next;
	}

	return true;
}
/*
	fprintf(out,"\n  Separator {\n");
	fprintf(out,"\tUSE MAT_MARKS\n");

	while(!finished)
	{
		fprintf(out,"\tTransformSeparator { \n");
		fprintf(out,"\t  Transform { translation ");

		read_to_comma(fp,buf);
		l = strlen(buf) - 1;
		if (buf[l] == ']') finished = true;
		buf[l] = '\0';

		fprintf(out,buf);

		fprintf(out," }\n");
		fprintf(out,"\t  USE MARKER\n\t}\n");
	}

	fprintf(out,"  }\n");

	return true;
}
*/

bool cv_stationOrientation(FILE * fp, marker_struct * Markerlist)
{
	char buf[512];
	bool finished = false;
	int l;

	read_blanks(fp);

	if (fgetc(fp) != '[') cverror("[ expected after stationOrientation");

	while(!finished)
	{
		read_to_comma(fp,buf);
		l = strlen(buf) - 1;
		if (buf[l] == ']') finished = true;
		buf[l] = '\0';

		if (Markerlist == NULL) cverror("Markerlist NULL pointer!!");

		strcpy(Markerlist->orientation, buf);
		Markerlist = Markerlist->next;
	}

	return true;
}



bool cv_FLOOR(FILE * fp)
{
	char flbuf[256];
	char firstbuf[255];
	unsigned int i = 0, ln = 0;
	int numverts = 0;
	bool inserted;
	bool lastturn = false;
	bool finished = false;

	read_blanks(fp);
//	read_token(fp,flbuf);

	if (fgetc(fp) != '[') cverror("[ expected after floor");

	fprintf(out,"\n\tUSE MAT_FLOOR\n");
	fprintf(out,"\tCoordinate3 { point [");

	while(!finished)
	{
		if (!lastturn)
			read_to_comma(fp,flbuf);
		else 
			finished = true;

		numverts++;
		inserted = false;
		if (numverts == 1)
		{
			strcpy(firstbuf,flbuf);
		}
		
		ln = strlen(flbuf);

		fputc(' ',out);
		for (i = 0; i < ln; i++)
		{
			if ((flbuf[i] == ' ') && (!inserted))
			{
				fprintf(out," 0 ");  //insert 0 between 2 numbers
				inserted = true;
			}
			else if (flbuf[i] == ']')
			{
				if (numverts == 1)  //just 1 point
				{
					finished = true;	
				}
				else if (lastturn == true)
				{
					fputc(flbuf[i], out);
				}
				else
				{
					lastturn = true;
					fprintf(out,", ");
					strcpy(flbuf,firstbuf); //add 1st point again to close polygon
					flbuf[strlen(firstbuf)-1] = ']';
				}
			}
			else 
			{
				fputc(flbuf[i], out);
			}

		}
	}

	fprintf(out," }\n");
	fprintf(out,"\tLineSet { numVertices %i }\n",numverts);

	return true;
}






bool cv_ROOMNAME(FILE * fp)
{
	char buf[256];

	read_blanks(fp);
	read_token(fp,buf);

	fprintf(out,"\n###\t%s\n",buf);
	return true;
}



bool cv_Transform(FILE * fp)
{
	char buf[255];
	char c;
	unsigned int i = 0;

	read_blanks(fp);
	read_token(fp,buf);

	if (buf[0] != '{') cverror("{ expected after Transform");
	
	fprintf(out,"    Transform {");

	//read until }
	while(1) 
	{
		c = fgetc(fp);
		if (c == EOF) return false;

		fputc(c,out);
		if (c == '}') break;
	} 

	fprintf(out,"\n");

	return true;
}



bool cv_translation(FILE * fp)
{
	char num1[255];
	char num2[255];
	char num3[255];

	read_blanks(fp);
	read_token(fp,num1);

	read_blanks(fp);
	read_token(fp,num2);

	read_blanks(fp);
	read_token(fp,num3);

	fprintf(out,"\tTransform { translation %s %s %s }\n",num1, num2, num3);

	return true;
}



bool convert_file(options * Opt) //(const char * filename)
{
	FILE *fp;
	char buffer[255];
	bool SEP_open = false;  //SEPARATOR open/close { }

	Options = Opt; //GLOBAL

	if ((fp = fopen(Options->inputfile,"r")) == NULL)
	{
		cverror("Could not open input file!");
		return false;
	}

 if (!DEBUG)
 {
	//open OUTPUT file
	if ((out = fopen(Options->outputfile,"w")) == NULL)
	{
		cverror("Could not open output temp file!");
		fclose(fp);
		return false;
	}
 }
	print_header();

	marker_struct * Markerlist = NULL;

	while(1)
	{
		if(!read_blanks(fp)) break; //(EOF)
		if(read_token(fp,buffer))
		{
			//printf("T: %s\n",buffer);

			//to upper case
			strupr(buffer);
			
			if (strcmp(buffer,"SOSTATIONKIT")==0) Markerlist = cv_STATIONKIT(fp);
			else if (strcmp(buffer,"STATIONPOSITION")==0) cv_stationPosition(fp, Markerlist);
			else if (strcmp(buffer,"STATIONORIENTATION")==0) cv_stationOrientation(fp, Markerlist);
			else if (strcmp(buffer,"TRANSFORM")==0) 
			//BE CAREFUL: is SoTransform really always necessary???
			{
				if (SEP_open) 	//room Separator from PREVIOUS room
				{
					fprintf(out," }\n");  //close it
					SEP_open = false;
				}

				//ROOM Separator starts here
				fprintf(out,"  Separator { \n");
				SEP_open = true;

				cv_Transform(fp);

				if (Options->zeropoints)
				{
					fprintf(out,"\tUSE MAT_ZERO\n");
					fprintf(out,"\tUSE PT_ZERO\n");
				}
			}
			else if (strcmp(buffer,"TRANSLATION")==0) 
			{
				if (SEP_open) 	//room Separator from PREVIOUS room
				{
					fprintf(out," }\n");  //close it
					SEP_open = false;
				}

				//ROOM Separator starts here
				fprintf(out,"  Separator { \n");
				SEP_open = true;

				cv_translation(fp);
				if (Options->zeropoints)
				{
					fprintf(out,"\tUSE MAT_ZERO\n");
					fprintf(out,"\tUSE PT_ZERO\n");
				}

			}
			else if (strcmp(buffer,"FLOOR")==0)
			{
				cv_FLOOR(fp);

				if (Markerlist) 
				{	
					printMarkerlist(Markerlist);
					deleteMarkerlist(Markerlist);
					Markerlist = NULL;
				}

				if (SEP_open) 	//ROOM Separator
				{
					fprintf(out," }\n");  //close it
					SEP_open = false;
				}
			}
			else if (strcmp(buffer,"ROOMNAME")==0) 	cv_ROOMNAME(fp);


		}
		else break; //(EOF)
	}

	fclose(fp); //close INPUT file

	if (SEP_open) 	//room Separator from PREVIOUS room
	{
		fprintf(out," }\n");  //close it
		SEP_open = false;
	}

	//close OUTPUT file
	fprintf(out,"}\n");
  
  if (!DEBUG)
	fclose(out);

	

	return true;
}




/*
int main()
{
	//TODO: create, set and pass Options struct
	convert_file(INPUT_FILE);
	return 0;
}
*/
