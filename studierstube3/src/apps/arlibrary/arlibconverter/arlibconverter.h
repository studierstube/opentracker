// arlibconverter.h : Haupt-Header-Datei für die Anwendung ARLIBCONVERTER
//

#if !defined(AFX_ARLIBCONVERTER_H__39DBB91E_6C42_4300_8BA2_BD2BEE84FBB5__INCLUDED_)
#define AFX_ARLIBCONVERTER_H__39DBB91E_6C42_4300_8BA2_BD2BEE84FBB5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// Hauptsymbole

/////////////////////////////////////////////////////////////////////////////
// ArlibconverterApp:
// Siehe arlibconverter.cpp für die Implementierung dieser Klasse
//

class ArlibconverterApp : public CWinApp
{
public:
	ArlibconverterApp();

// Überladungen
	// Vom Klassenassistenten generierte Überladungen virtueller Funktionen
	//{{AFX_VIRTUAL(ArlibconverterApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementierung

	//{{AFX_MSG(ArlibconverterApp)
		// HINWEIS - An dieser Stelle werden Member-Funktionen vom Klassen-Assistenten eingefügt und entfernt.
		//    Innerhalb dieser generierten Quelltextabschnitte NICHTS VERÄNDERN!
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // !defined(AFX_ARLIBCONVERTER_H__39DBB91E_6C42_4300_8BA2_BD2BEE84FBB5__INCLUDED_)
