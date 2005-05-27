#include	<stdlib.h>
#include	<stdio.h>
#include	"ccode.h"
#include	"cmarker.h"

#define		DEF_RESOLUTION	4

//---- global variables
int		g_iNumMarker;

void	errormsg(const char * msg)
{
	fprintf(stderr, "%s\n", msg);
	exit(-1);
}

void	GenerateCombRec(int ** & rpComb, int * & rpCombLength, int & riIndex, int iDepth, const int & iLimit)
{
	int	i	= (iDepth) ? rpComb[riIndex - 1][iDepth - 1] + 1 : 0;

	for (; i < iLimit; i++)
	{
		if (riIndex < g_iNumMarker)
		{
			//---- generate own
			rpCombLength[riIndex]	= iDepth + 1;
			
			for (int j = 0; j < iDepth; j++)
			{
				rpComb[riIndex][j]	= rpComb[riIndex - 1][j];
			}

			rpComb[riIndex][iDepth]	= i;
			++riIndex;

			//---- and continue recursively
			GenerateCombRec(rpComb, rpCombLength, riIndex, iDepth + 1, iLimit);
		}
	}
}

void    main(int argv, char ** argc)
{
	//---- check the parameters
	if (argv != 5)
		errormsg("Usage: GenMarker number patternsize whitebordersize filename");

	g_iNumMarker		= atoi(argc[1]);

	int		iMarkerSize	= atoi(argc[2]),
			iBorderSize	= atoi(argc[3]);
	char	* psFilename = argc[4];
	
	if ((g_iNumMarker < 1) || (g_iNumMarker > (DEF_RESOLUTION * DEF_RESOLUTION) / 2))
		errormsg("Error: Invalid marker number");

	if (iMarkerSize < 2)
		errormsg("Error: Invalid marker size");

	//---- generate the base codes
	CCode	code((DEF_RESOLUTION * DEF_RESOLUTION) / 4);

	//---- generate all combinations
	int		**	pCombination,
			*	pCombLength,			
			iIndex;		

	pCombination	= new int *[g_iNumMarker];
	pCombLength		= new int[g_iNumMarker];

	for (iIndex = 0; iIndex < g_iNumMarker; iIndex++)
	{
		pCombination[iIndex]	= new int[code.GetNumBaseCodes()];
	}

	pCombLength[0]	= 0;	// the first code is the empty code
	iIndex	= 1;	

	GenerateCombRec(pCombination, pCombLength, iIndex, 0, code.GetNumBaseCodes());

	//---- create the markers
	CMarker		* pMarker;
	bool		* pCode;
	char		sFilename[512];

	for (iIndex = 0; iIndex < g_iNumMarker; iIndex++)
	{
		fprintf(stdout, "Generating marker number %d ...\n", iIndex);

		pCode	= code.GetCode(pCombLength[iIndex], pCombination[iIndex]);
		pMarker	= new CMarker(DEF_RESOLUTION, iMarkerSize, iBorderSize, pCode);

		sprintf(sFilename, "%s%d.bmp", psFilename, iIndex);
		if (false == pMarker->WriteToBitmap(sFilename))
		{
			fprintf(stderr, "Error: Could not write file: %s\n", sFilename);
		}

		delete	pMarker;
		delete	pCode;
	}

	//---- free memory
	for (iIndex = 0; iIndex < g_iNumMarker; iIndex++)
	{
		delete[] pCombination[iIndex];
	}

	delete[] pCombination;
	delete[] pCombLength;
}