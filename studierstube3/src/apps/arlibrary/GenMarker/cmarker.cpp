#include <stdlib.h>
#include "cmarker.h"
#include "bmp.h"

#define	SETPOINT(x, y, b) (mpPattern[(y) * miResolution + (x)] = (b))

CMarker::CMarker(	const int iResolution,
					const int iMarkerSize,
					const int iWhiteBorderSize,
					const bool * pPattern)
:	miResolution(iResolution),
	miMarkerSize(iMarkerSize),
	miWhiteBorderSize(iWhiteBorderSize)
{
	//---- generate pattern
	mpPattern	= new bool[iResolution * iResolution];

	int		x, y,
			i = 0;
	bool	bCurr;

	for (y = 0; y < (iResolution / 2); y++)
	{
		for (x = 0; x < (iResolution / 2); x++)
		{
			bCurr	= pPattern[y * (iResolution / 2) + x];

			//---- copy the image itself
			SETPOINT(x, y, bCurr);

			//---- and the rotated, modified versions
			SETPOINT(iResolution - y - 1, x, (i == 0) ? bCurr : !bCurr);
			SETPOINT(iResolution - x - 1, iResolution - y - 1, (i == 1) ? bCurr : !bCurr);
			SETPOINT(y, iResolution - x - 1, (i == 2) ? bCurr : !bCurr);

			//---- update index
			if (++i == 3)
				i = 0;
		}
	}

}

CMarker::~CMarker()
{
	delete[]	mpPattern;
}

bool	CMarker::WriteToBitmap(const char * sFilename) const
{
	//---- create bitmap and set color 0 to white and color 1 to black
	int		iEdgeLength	= 2 * (miMarkerSize + miWhiteBorderSize);

	CBmp	bmp(iEdgeLength, iEdgeLength, CBmp::DEPTH8BITS);
	
	bmp.SetColorEntry(0, 255, 255, 255);
	bmp.SetColorEntry(1, 0, 0, 0);

	//---- draw the border
	int		x, y,
			iBorderMargin1	= miWhiteBorderSize,
			iBorderMargin2	= iEdgeLength - miWhiteBorderSize;

	for (y = 0; y < iEdgeLength; y++)
	{
		for (x = 0; x < iEdgeLength; x++)
		{
			bmp.SetPixel(x, y, ((x > iBorderMargin1) && (x < iBorderMargin2) &&
								(y > iBorderMargin1) && (y < iBorderMargin2)) ? 1 : 0);
		}
	}

	//---- draw the pattern itself
	float	fDivValue	= (float)miMarkerSize / (float)miResolution;
	iBorderMargin1	= miMarkerSize / 2 + miWhiteBorderSize;	

	for (y = 0; y < miMarkerSize; y++)
	{
		for (x = 0; x < miMarkerSize; x++)
		{
			bmp.SetPixel(	x + iBorderMargin1, y + iBorderMargin1,
							(mpPattern[(int)(y / fDivValue) * miResolution + (int)(x / fDivValue)]) ? 1 : 0);
		}
	}

	//---- try to write bitmap file
	return (0 == bmp.Write(sFilename));
}