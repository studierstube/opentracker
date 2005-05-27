#ifndef __DATATYPES_H_
#define __DATATYPES_H_


#include "stdio.h"


// data structure for one shelf box

typedef struct
{
	float topleft[3];
	float botright[3];
} box;


// data structure for one shelf

typedef struct
{
	char name[100];
	float rgb[3];
	char code[100];

	int numboxes;
	box boxes[20];
	int numbooks[20];		// how many books already in this shelf?
	int activebox;			// how many boxes already filled?
} shelf;



// data structure for one book, very dumb with fixed size arrays, but generously sized ;)

typedef struct book
{
	char isbn[100];
	int numauthors, numkeywords;
	char authors[20][100];
	char keywords[50][255];
	char code[100]; // internal use only
	char title[255];
	char shelf[255];
	float color[3];
	int year;
	int id;
	struct book* next;
} book;





#endif