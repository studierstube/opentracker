#include <stdio.h>
#include <stdlib.h>
#include "bmp.h"

CBmp::CBmp(int iWidth, int iHeight, char cDepth) :  m_iWidth(iWidth),
                                                    m_iHeight(iHeight),
                                                    m_cDepth(cDepth)                                                   
{
    //---- calculate, how many bytes are stored in one row
    m_iInternalWidth    = (cDepth == DEPTH8BITS) ? m_iWidth : m_iWidth * 3;
    m_iInternalWidth    = (m_iInternalWidth + 3) & 0xfffffffc;

    //---- allocate and initialize memory
	int	size = m_iInternalWidth * m_iHeight;
    m_pData = new unsigned char[size];

	while (size--)
		m_pData[size] = 0;

    m_pPalette = (cDepth == DEPTH8BITS) ? new RGBQUAD[256] : NULL;
}

CBmp::~CBmp()
{
    delete[] m_pData;
    if (m_pPalette) delete[] m_pPalette;
}

int    CBmp::Write(const char * sFilename) const
{
    FILE    * f   = fopen(sFilename, "wb");
    if (f)
    {
        //---- deal with the bitmap file header
        BITMAPFILEHEADER    bmpfh;

        bmpfh.bfType        = 19778;
        bmpfh.bfOffBits     = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
        if (m_cDepth == DEPTH8BITS) bmpfh.bfOffBits+= 256 * sizeof(RGBQUAD);

        bmpfh.bfSize        =  bmpfh.bfOffBits + m_iInternalWidth * m_iHeight;       
        bmpfh.bfReserved1   =  bmpfh.bfReserved2 = 0;
        
        fwrite((const void *)&bmpfh, sizeof(BITMAPFILEHEADER), 1, f);

        //---- deal with the bitmap info header
		BITMAPINFOHEADER	bminfhd;

        bminfhd.biSize			=	sizeof(BITMAPINFOHEADER); 
        bminfhd.biWidth			=	m_iWidth;
        bminfhd.biHeight		=	m_iHeight; 
        bminfhd.biPlanes		=	1; 
        bminfhd.biBitCount		=	(m_cDepth == DEPTH8BITS) ? 8 : 24; 
        bminfhd.biCompression	=	0; 
        bminfhd.biSizeImage		=	m_iInternalWidth * m_iHeight;
        bminfhd.biXPelsPerMeter	=	3780; 
        bminfhd.biYPelsPerMeter	=	3780;
        bminfhd.biClrUsed		=	0; 
        bminfhd.biClrImportant	=	0;

		fwrite((const void *)&bminfhd, sizeof(BITMAPINFOHEADER), 1, f);

		//---- write the palette if necessary
		if (m_cDepth == DEPTH8BITS)			
			fwrite((const void *)m_pPalette, sizeof(RGBQUAD), 256, f);

		//---- add the image data itself
		int				y	= m_iHeight;

		while (y--)                    
			fwrite((const void *)&m_pData[y * m_iInternalWidth], 1, m_iInternalWidth, f);            

		//---- close file and indicate success
        fclose(f);

        return 0;
    }

    return -1;
}

CBmp *	CBmp::Load(const char * sFilename)
{
	FILE    * f   = fopen(sFilename, "rb");
    if (f)
    {
		CBmp	* pBmp;
		size_t	readBytes;

		//---- read the bitmap file header
        CBmp::BITMAPFILEHEADER    bmpfh;

        readBytes	= fread((void *)&bmpfh, sizeof(BITMAPFILEHEADER), 1, f);
		
		if ((readBytes != 1) || (bmpfh.bfType != 19778))
		{
			fclose(f);
			return NULL;
		}

		//---- read the bitmap info header
		BITMAPINFOHEADER	bminfhd;

		readBytes	= fread((void *)&bminfhd, sizeof(BITMAPINFOHEADER), 1, f);
		
		if ((readBytes != 1) ||
			((bminfhd.biBitCount != 8) && (bminfhd.biBitCount != 24)) ||
			(bminfhd.biCompression != 0))
		{
			fclose(f);
			return NULL;
		}

		pBmp	= new CBmp(	bminfhd.biWidth, bminfhd.biHeight,
							(bminfhd.biBitCount == 8) ? DEPTH8BITS : DEPTH24BITS);

		//---- read the palette if necessary
		if (bminfhd.biBitCount == 8)
			fread((void *)pBmp->m_pPalette, sizeof(RGBQUAD), 256, f);

		//---- add the image data itself
		while (bminfhd.biHeight--)
			fread((void *)&(pBmp->m_pData[bminfhd.biHeight * pBmp->m_iInternalWidth]), pBmp->m_iInternalWidth, 1, f);

		//---- close and return the result
		fclose(f);

		return pBmp;
	}

	return NULL;
}

int     CBmp::GetPixel(const int & x, const int & y) const
{
	if (m_cDepth == DEPTH8BITS)
	{
		return (int)(m_pData[y * m_iInternalWidth + x]);		
	}
	else
	{
		int	i	= y * m_iInternalWidth + x * 3;
		return RGBVAL(m_pData[i + 2], m_pData[i + 1], m_pData[i]);
	}
}

void    CBmp::SetPixel(const int & x, const int & y, const int & col)
{
	if (m_cDepth == DEPTH8BITS)
	{
		m_pData[y * m_iInternalWidth + x]	= (unsigned char)col;
	}
	else
	{
		int	i	= y * m_iInternalWidth + x * 3;
		m_pData[i + 2]	= RGBRED(col);
		m_pData[i + 1]	= RGBGREEN(col);
		m_pData[i]		= RGBBLUE(col);        
	}
}

void    CBmp::SetColorEntry(const unsigned char & n,
                            const unsigned char & red,
                            const unsigned char & green,
                            const unsigned char & blue)
{
    if (m_pPalette)
    {
        m_pPalette[n].rgbRed        = red;
        m_pPalette[n].rgbGreen      = green;
        m_pPalette[n].rgbBlue       = blue;
        m_pPalette[n].rgbReserved   = 0;
    }
}

void    CBmp::GetColorEntry(  const unsigned char & n,
                            unsigned char & red,
                            unsigned char & green,
                            unsigned char & blue) const
{
    if (m_pPalette)
    {
        red     = m_pPalette[n].rgbRed;
        green   = m_pPalette[n].rgbGreen;
        blue    = m_pPalette[n].rgbBlue;
    }
}
