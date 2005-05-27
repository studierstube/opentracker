#ifndef CCVT_TYPES_H
#define CCVT_TYPES_H

typedef struct
{
	unsigned char b;
	unsigned char g;
	unsigned char r;
	unsigned char z;
} PIXTYPE_bgr32;

typedef struct
{
	unsigned char b;
	unsigned char g;
	unsigned char r;
} PIXTYPE_bgr24;

typedef struct
{
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char z;
} PIXTYPE_rgb32;

typedef struct
{
	unsigned char r;
	unsigned char g;
	unsigned char b;
} PIXTYPE_rgb24;


#define SAT(c) \
	if (c & (~255)) { if (c < 0) c = 0; else c = 255; }



#endif
