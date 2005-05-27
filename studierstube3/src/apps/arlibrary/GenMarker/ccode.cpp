#include	<stdlib.h>
#include	"ccode.h"


CCode::CCode(const int iSize)
{
	//---- determine, how many base codes we've got
	miCntBaseCodes	= 31;

	while (0 == ((iSize >> miCntBaseCodes) & 1))
		--miCntBaseCodes;

	miSize	= 1 << miCntBaseCodes;

	++miCntBaseCodes;

	//---- allocate memory and create base codes
	mpBaseCode	= new bool *[miCntBaseCodes];

	bool	bState;
	int		iPeriod	= miSize,
			i, j, k;

	for (i = 0; i < miCntBaseCodes; i++)
	{
		mpBaseCode[i]	= new bool[miSize];

		for (bState = true, j = 0, k = 0; j < miSize; j++)
		{
			mpBaseCode[i][j]	= bState;

			if (++k == iPeriod)
			{
				bState = !bState;
				k = 0;
			}
		}		

		iPeriod>>= 1;
	}
}

CCode::~CCode()
{
	//---- free memory
	for (int i = 0; i < miCntBaseCodes; i++)
	{
		delete[] mpBaseCode[i];
	}

	delete[] mpBaseCode;
}
	
bool *	CCode::GetCode(	const int & iNumBaseCodes,
						const int * pBaseCodeIndices) const
{
	bool	* pResult;
	int		i;

	//---- test, if all indices are valid
	for (i = 0; i < iNumBaseCodes; i++)
	{
		if ((pBaseCodeIndices[i] < 0) || (pBaseCodeIndices[i] >= miCntBaseCodes))
			return NULL;
	}

	//---- allocate memory for the result and copy the first code (or clear all, if not set)
	pResult	= new bool[miSize];

	for (i = 0; i < miSize; i++)
	{
		pResult[i]	= (iNumBaseCodes) ? mpBaseCode[pBaseCodeIndices[0]][i] : false;
	}

	//---- add the other codes
	for (i = 1; i < iNumBaseCodes; i++)
	{
		Combine(pResult, mpBaseCode[pBaseCodeIndices[i]]);
	}

	return pResult;
}


void	CCode::Combine(	bool * pCodeDest,
						const bool * pCodeSrc) const
{
	for (int i = 0; i < miSize; i++)
	{
		pCodeDest[i] = pCodeSrc[i] ^ pCodeDest[i];
	}
}

