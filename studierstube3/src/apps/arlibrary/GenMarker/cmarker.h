#ifndef CMARKER_H__
#define	CMARKER_H__

class CMarker
{
public:
	CMarker(const int iResolution,
			const int iMarkerSize,
			const int iWhiteBorderSize,
			const bool * pPattern);

	virtual ~CMarker();

	bool	WriteToBitmap(const char * sFilename) const;

protected:
	const int	miResolution,
				miMarkerSize,
				miWhiteBorderSize;
	bool		* mpPattern;		
};

#endif	// CMARKER_H__