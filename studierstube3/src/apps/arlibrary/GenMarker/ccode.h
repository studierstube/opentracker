#ifndef CCODE_H__
#define	CCODE_H__

class	CCode
{
//---- method section
public:
	CCode(const int iSize);
	virtual ~CCode();
	
	bool *	GetCode(const int & iNumBaseCodes,
					const int * pBaseCodeIndices) const;

	int		GetCodeSize() const { return miSize; };
	int		GetNumBaseCodes() const { return miCntBaseCodes; };

protected:
	void	Combine(bool * pCodeDest,
					const bool * pCodeSrc) const;

//---- variables section
protected:
	bool		** mpBaseCode;
	int			miSize,
				miCntBaseCodes;
};

#endif	// CCODE_H__