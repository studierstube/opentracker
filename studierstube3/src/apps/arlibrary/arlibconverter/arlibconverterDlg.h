// arlibconverterDlg.h : Header-Datei
//

#if !defined(AFX_ARLIBCONVERTERDLG_H__6B535CBB_9DDE_44C2_8E3B_42104C8DE57A__INCLUDED_)
#define AFX_ARLIBCONVERTERDLG_H__6B535CBB_9DDE_44C2_8E3B_42104C8DE57A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MODE_TABSTOP 0
#define MODE_SPACE 1

#include "datatypes.h"


/////////////////////////////////////////////////////////////////////////////
// ArlibconverterDlg Dialogfeld

class ArlibconverterDlg : public CDialog
{
// Konstruktion
public:
	ArlibconverterDlg(CWnd* pParent = NULL);	// Standard-Konstruktor

	int readshelves();
	int readbooks();
	int writefile();

	int getshelf(char *code);


	
// Dialogfelddaten
	//{{AFX_DATA(ArlibconverterDlg)
	enum { IDD = IDD_ARLIBCONVERTER_DIALOG };
	CEdit	m_log;
	CString	m_logtext;
	BOOL	m_usestationkit;
	float	shelfthick;
	float	m_bookdepth;
	float	m_bookheight;
	float	m_bookwidth;
	CString	m_srcname;
	CString	m_dstname;
	CString	m_libname;
	CString	m_stationkitfname;
	CString	m_stationfname;
	CString	m_shelvesfname;
	//}}AFX_DATA

	// Vom Klassenassistenten generierte Überladungen virtueller Funktionen
	//{{AFX_VIRTUAL(ArlibconverterDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV-Unterstützung
	//}}AFX_VIRTUAL

// Implementierung
protected:
	HICON m_hIcon;

	int filemode;
	int numshelves;
	int numbooks;
	
	shelf shelves[100];
	book *books;

	float bookw, bookh, bookd;

	void noumlaut(char *, char *);

	// Generierte Message-Map-Funktionen
	//{{AFX_MSG(ArlibconverterDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnConvert();
	afx_msg void OnQuit();
	afx_msg void OnStationkituse();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // !defined(AFX_ARLIBCONVERTERDLG_H__6B535CBB_9DDE_44C2_8E3B_42104C8DE57A__INCLUDED_)
