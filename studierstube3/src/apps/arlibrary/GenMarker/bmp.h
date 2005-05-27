#ifndef BMP_H
#define BMP_H

#ifndef RGBVAL
#define RGBVAL(r, g, b) (int)(((r) << 16) | ((g) << 8) | (b))
#endif

#ifndef RGBRED
#define RGBRED(c)	(unsigned char)((c) >> 16)
#endif

#ifndef RGBGREEN
#define RGBGREEN(c)	(unsigned char)((c) >> 8)
#endif

#ifndef RGBBLUE
#define RGBBLUE(c)	(unsigned char)(c)
#endif

class CBmp
{
public:
    enum
    {
        DEPTH8BITS = 0,
        DEPTH24BITS
    };

protected:
    //---- these structures are copied to be independent of window.h
    struct BITMAPFILEHEADER
    {
        unsigned short      bfType;
        unsigned int        bfSize; 
        unsigned short      bfReserved1, 
                            bfReserved2; 
        unsigned int        bfOffBits;
    };
    
    struct BITMAPINFOHEADER
    {
        unsigned int    biSize, 
                        biWidth,
                        biHeight; 
        unsigned short  biPlanes, 
                        biBitCount; 
        unsigned int    biCompression, 
                        biSizeImage,
                        biXPelsPerMeter,
                        biYPelsPerMeter,
                        biClrUsed,
                        biClrImportant; 
    };

    struct RGBQUAD
    {
        unsigned char   rgbBlue, 
                        rgbGreen,
                        rgbRed,
                        rgbReserved;
    };

public:
    CBmp(int iWidth, int iHeight, char cDepth);
    virtual ~CBmp();

    int     Write(const char * sFilename) const;
    
    unsigned char    GetData(const int & iOffset) const { return m_pData[iOffset]; };
    void    SetData(const int & iOffset, const unsigned char & c) { m_pData[iOffset] = c; };
    
    int     GetPixel(const int & x, const int & y) const;
    void    SetPixel(const int & x, const int & y, const int & col);
    
    int     GetWidth() const { return m_iWidth; };    
    int     GetHeight() const { return m_iHeight; };
    char    GetDepth() const { return m_cDepth; };
	int		GetLineLength() const { return m_iInternalWidth; };
    
    void    SetColorEntry(  const unsigned char & n,
                            const unsigned char & red,
                            const unsigned char & green,
                            const unsigned char & blue);
    void    GetColorEntry(  const unsigned char & n,
                            unsigned char & red,
                            unsigned char & green,
                            unsigned char & blue) const;

    static  CBmp * Load(const char * sFilename);

protected:
    const int       m_iWidth,
                    m_iHeight;
    int             m_iInternalWidth;
    const char      m_cDepth;
    unsigned char   * m_pData;
    RGBQUAD         * m_pPalette;
};

#endif

