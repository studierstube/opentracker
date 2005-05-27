// arlibconverterDlg.cpp : Implementierungsdatei
//

#include "stdafx.h"
#include "arlibconverter.h"
#include "arlibconverterDlg.h"
#include "datatypes.h"
#include "math.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg-Dialogfeld für Anwendungsbefehl "Info"

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialogfelddaten
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// Vom Klassenassistenten generierte Überladungen virtueller Funktionen
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	//}}AFX_VIRTUAL

// Implementierung
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// Keine Nachrichten-Handler
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ArlibconverterDlg Dialogfeld

ArlibconverterDlg::ArlibconverterDlg(CWnd* pParent /*=NULL*/)
	: CDialog(ArlibconverterDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(ArlibconverterDlg)
	m_logtext = _T("");
	m_usestationkit = FALSE;
	shelfthick = 0.0f;
	m_bookdepth = 0.0f;
	m_bookheight = 0.0f;
	m_bookwidth = 0.0f;
	m_srcname = _T("");
	m_dstname = _T("");
	m_libname = _T("");
	m_stationkitfname = _T("");
	m_stationfname = _T("");
	m_shelvesfname = _T("");
	//}}AFX_DATA_INIT
	// Beachten Sie, dass LoadIcon unter Win32 keinen nachfolgenden DestroyIcon-Aufruf benötigt
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void ArlibconverterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(ArlibconverterDlg)
	DDX_Control(pDX, IDC_LOG, m_log);
	DDX_Text(pDX, IDC_LOG, m_logtext);
	DDX_Check(pDX, IDC_CHECK1, m_usestationkit);
	DDX_Text(pDX, IDC_EDIT1, shelfthick);
	DDV_MinMaxFloat(pDX, shelfthick, 1.e-003f, 1.5f);
	DDX_Text(pDX, IDC_BKD, m_bookdepth);
	DDX_Text(pDX, IDC_BKH, m_bookheight);
	DDX_Text(pDX, IDC_BKW, m_bookwidth);
	DDX_Text(pDX, IDC_SRCNAME, m_srcname);
	DDX_Text(pDX, IDC_DSTNAME, m_dstname);
	DDX_Text(pDX, IDC_LIBNAME, m_libname);
	DDX_Text(pDX, IDC_STATIONKITFILENAME, m_stationkitfname);
	DDX_Text(pDX, IDC_STATIONFILENAME, m_stationfname);
	DDX_Text(pDX, SHELVESFILENAME, m_shelvesfname);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(ArlibconverterDlg, CDialog)
	//{{AFX_MSG_MAP(ArlibconverterDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, OnConvert)
	ON_BN_CLICKED(IDC_BUTTON2, OnQuit)
	ON_BN_CLICKED(IDC_CHECK1, OnStationkituse)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ArlibconverterDlg Nachrichten-Handler

BOOL ArlibconverterDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Hinzufügen des Menübefehls "Info..." zum Systemmenü.

	// IDM_ABOUTBOX muss sich im Bereich der Systembefehle befinden.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{	
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Symbol für dieses Dialogfeld festlegen. Wird automatisch erledigt
	//  wenn das Hauptfenster der Anwendung kein Dialogfeld ist
	SetIcon(m_hIcon, TRUE);			// Großes Symbol verwenden
	SetIcon(m_hIcon, FALSE);		// Kleines Symbol verwenden
	


	// init stuff

	this->m_libname = "CG Institute Library";
	this->m_stationfname = "sourcefiles/stations.txt";
	this->m_stationkitfname = "sourcefiles/stationkits.txt";
	this->m_shelvesfname = "sourcefiles/shelves.txt";

	this->filemode = MODE_TABSTOP;
	shelfthick = 0.05;
	this->m_bookdepth = 0.2;
	this->m_bookheight = 0.3;
	this->m_bookwidth = 0.045;
	this->m_srcname = "sourcefiles/arlib.txt";
	this->m_dstname = "arlib.iv";
	UpdateData(FALSE);

	books = NULL;



	return TRUE;  // Geben Sie TRUE zurück, außer ein Steuerelement soll den Fokus erhalten
}

void ArlibconverterDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// Wollen Sie Ihrem Dialogfeld eine Schaltfläche "Minimieren" hinzufügen, benötigen Sie 
//  den nachstehenden Code, um das Symbol zu zeichnen. Für MFC-Anwendungen, die das 
//  Dokument/Ansicht-Modell verwenden, wird dies automatisch für Sie erledigt.

void ArlibconverterDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // Gerätekontext für Zeichnen

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Symbol in Client-Rechteck zentrieren
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Symbol zeichnen
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// Die Systemaufrufe fragen den Cursorform ab, die angezeigt werden soll, während der Benutzer
//  das zum Symbol verkleinerte Fenster mit der Maus zieht.
HCURSOR ArlibconverterDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}



int ArlibconverterDlg::readshelves()
{
	// read the shelf description file

	char line[255];
	int boxind;
	FILE *fp;
	char cmd[50];
	char foo[50];
	int bi;

	this->numshelves = 0;

	m_logtext = "Attempting to open shelves definition file...";
	UpdateData(FALSE);

	if((fp=fopen(this->m_shelvesfname,"r"))==NULL)
	{
		MessageBox("shelves file not found!","error",MB_OK);
		return -1;
	}

	m_logtext+= "done!\r\n";
	UpdateData(FALSE);


	while(fgets(line,sizeof(line),fp)!=NULL)
	{
		strcpy(cmd,"");

		// get command
		sscanf(line, "%s", &cmd);

		if(!strcmp(cmd,"#name"))
		{
			// found shelf name
			sscanf(line, "%s %s", &foo, &shelves[numshelves].name);
			boxind=0;
			m_logtext+= "Adding shelf.. ";
			UpdateData(FALSE);
		}
		else if(!strcmp(cmd, "#color"))
		{
			sscanf(line, "%s %f %f %f", &foo, &shelves[numshelves].rgb[0], &shelves[numshelves].rgb[1], &shelves[numshelves].rgb[2]);
			m_logtext+= "color ";
			UpdateData(FALSE);
		}
		else if(!strcmp(cmd, "#code"))
		{
			sscanf(line, "%s %s", &foo, &shelves[numshelves].code);
			m_logtext+= "code ";
			UpdateData(FALSE);
		}
		else if(!strcmp(cmd, "#numboxes"))
		{
			sscanf(line, "%s %d", &foo, &shelves[numshelves].numboxes);
			m_logtext+= "numboxes ";
			UpdateData(FALSE);
		}
		else if(!strcmp(cmd, "#topleft"))
		{
			sscanf(line, "%s %f %f %f", &foo, &shelves[numshelves].boxes[boxind].topleft[0],
				&shelves[numshelves].boxes[boxind].topleft[1],
				&shelves[numshelves].boxes[boxind].topleft[2]);
			m_logtext+= "topleft ";
			UpdateData(FALSE);
		}
		else if(!strcmp(cmd, "#botright"))
		{
			sscanf(line, "%s %f %f %f", &foo, &shelves[numshelves].boxes[boxind].botright[0],
				&shelves[numshelves].boxes[boxind].botright[1],
				&shelves[numshelves].boxes[boxind].botright[2]);
			boxind++;
			m_logtext+= "topright ";
			UpdateData(FALSE);
		}
		else if(!strcmp(cmd, "#end"))
		{
			// test if enough boxes
			if(boxind!=shelves[numshelves].numboxes)
			{
				MessageBox("number of shelf boxes doesn't match preset #numboxes parameter","error",MB_OK);
				fclose(fp);
				return -1;
			}
			m_logtext+= "...done!\r\n";
			UpdateData(FALSE);
			this->shelves[numshelves].activebox=0;
			for(bi=0; bi<20; bi++) this->shelves[numshelves].numbooks[bi]=0;
			this->numshelves++;
		}
		
	}

	fclose(fp);

	return 0;
}


int ArlibconverterDlg::readbooks()
{
	// read library text file

	char line[255], rawline[255];
	int i,j;
	int bookind = 0;
	int kc;
	int mod=0;
	FILE *fp;
	char isbn[100], year[100], author[100], title[255], code[100], buf[500], *token;
	char seps[] = " ,-:";
	book *btmp = books;

	this->numbooks=0;

	m_logtext+= "\r\nAttempting to open library text file...";
	UpdateData(FALSE);

	if((fp=fopen(m_srcname,"r"))==NULL)
	{
		MessageBox("sourcefile not found!","error",MB_OK);
		return -1;
	}

	m_logtext+= "done!\r\n";
	UpdateData(FALSE);

	// read the tab-separated file
	while(fgets(rawline,sizeof(rawline),fp)!=NULL)
	{
		mod = 0; i=0; j=0;
		memset(&isbn,0,sizeof(isbn));
		memset(&year,0,sizeof(year));
		memset(&author,0,sizeof(author));
		memset(&title,0,sizeof(title));
		memset(&code,0,sizeof(code));
		
		// remove umlauts
		noumlaut(rawline, line);

		
		sprintf(buf,"Processing line %s",line);
		m_logtext+= buf;
		UpdateData(FALSE);

		if(line[0]!='#') // if data line
		{
			i=0; j=0;
			while(i<strlen(line))
			{
				if(line[i]==0x9) { mod++; j=0; }
				else
				{
					switch(mod)
					{
					case 0: // isbn
						isbn[j] = line[i]; j++;
						break;
					case 1: // year
						year[j] = line[i]; j++;
						break;
					case 2: // code
						code[j] = line[i]; j++;
						break;
					case 3: // author
						author[j] = line[i]; j++;
						break;
					case 4: // title
						if(line[i]!='\n') {title[j] = line[i]; j++;} // strip newline char
						break;
					default:
						m_logtext+= " Line too long! ";
						UpdateData(FALSE);
						MessageBox("Line too long. Continuing...","warning",MB_OK);
						break;
					}
				}
				i++;
			}

			// ok baby now create a new record, if we've got a shelf for this book
			if(getshelf(code)>=0)
			{
				this->numbooks++;

				book *nubk = (book*)malloc(sizeof(book));

				// append at end of list
				if(btmp==NULL) 
				{
					books = nubk; btmp = books; // first book
				}
				else 
				{
					btmp->next = nubk;
					btmp = nubk;
				}
				btmp->next = NULL;

				strcpy(btmp->authors[0], author);
				btmp->numauthors=1;
				btmp->numkeywords=0;
				btmp->color[0] = shelves[getshelf(code)].rgb[0]; 
				btmp->color[1] = shelves[getshelf(code)].rgb[1];
				btmp->color[2] = shelves[getshelf(code)].rgb[2];
				btmp->id=bookind; bookind++;
				strcpy(btmp->isbn, isbn);
				strcpy(btmp->shelf, shelves[getshelf(code)].name);
				strcpy(btmp->code, code);
				strcpy(btmp->title, title);
				btmp->year = atoi(year);

				// add keywords! this is a hack since the database doesn't contain any keywords,
				// so add all words of the title to the keyword array.
				// this can be easily altered for future developments
	
				kc = 0;
				token = strtok(title, seps);
				while( token != NULL )
				{
					strcpy(btmp->keywords[kc],token); btmp->numkeywords++; kc++;
					token = strtok( NULL, seps );
				}

			}
			else
			{
				m_logtext+="\r\n No Shelf found for this book! ";
			}



		} // data line

		m_logtext+= "\r\n";
		UpdateData(FALSE);
	}
	

	return 0;
}


int ArlibconverterDlg::writefile()
{
	// write out IV file

	FILE *fp;
	FILE *st;
	char line[255];
	book *tmp;
	int i,j;
	float x1,x2,z1,z2,xd,zd,angle,len,w,h,startx,starty,startz, thisx, thisy, thisz;


	m_logtext+= "\r\nStarting to write IV file!\r\n";
	UpdateData(FALSE);

	// open file
	if((fp=fopen(m_dstname,"w"))==NULL)
	{
		m_logtext+= "\r\nCould not create IV file!\r\n";
		UpdateData(FALSE);
		MessageBox("could not create destination file","error",MB_OK);
		return -1;
	}

	// write header
	fprintf(fp,"#Inventor V2.1 ascii\n\nSeparator\n{\n\n\tDEF Bibliothek SoStationKit\n\t{\n");

	// write stationkit stuff
	if((st=fopen(this->m_stationfname,"r"))==NULL)
	{
		m_logtext+= "\r\ncould not open stationsfile!\r\n";
		UpdateData(FALSE);
		MessageBox("could not open stationsfile","error",MB_OK);
		return -1;
	}

	while(fgets(line,sizeof(line),st)!=NULL)
	{
		fprintf(fp,"\t\t%s",line);
	}
	fclose(st);

	// write bookheader
	fprintf(fp,"\n\t\tcontent SoGroup\n\t\t{\n\t\t\tDEF AllBooks So3DSeparator\n\t\t\t{\n");


	// now write all books
	while(books!=NULL)
	{
		// find out in which shelf this book is and derive its position and orientation
		int shn = getshelf(books->code);

		// now get the box where this book should be placed in
		int boxnum = shelves[shn].activebox;

		// derive orientation of box
		x1 = shelves[shn].boxes[boxnum].topleft[0];
		x2 = shelves[shn].boxes[boxnum].botright[0];
		z1 = shelves[shn].boxes[boxnum].topleft[2];
		z2 = shelves[shn].boxes[boxnum].botright[2];

		// calc shelf angle
		xd = x2-x1; zd = z2-z1;
		len = sqrt(xd*xd+zd*zd);
		xd/=len; zd/=len;
		angle = atan2(-xd,-zd)+1.5708;
		h = shelves[shn].boxes[boxnum].botright[1]+this->m_bookheight/2.0;
		startx = x1; startz = z1;

		// calc position of book in this box
		thisx = startx+xd*(this->shelfthick + m_bookwidth/2.0 + (this->m_bookwidth*shelves[shn].numbooks[boxnum]));
		thisz = startz+zd*(this->shelfthick + m_bookwidth/2.0 + (this->m_bookwidth*shelves[shn].numbooks[boxnum]));
		thisy = h;

		// advance book position counter
		if(shelves[shn].numbooks[boxnum]*this->m_bookwidth < (len-shelfthick*2)-m_bookwidth)
		{
			shelves[shn].numbooks[boxnum]++;
		}
		else if(shelves[shn].activebox<shelves[shn].numboxes-1) // box is full -> go to next box
		{
			shelves[shn].activebox++;
		}
		else
		{
			// do not advance position at all. bad luck, we've got too many books for that
			// shelf. maybe the user should set a smaller book width.
			// all following books for this shelf will be at the same position
		}

		fprintf(fp,"\t\t\t\tSwitch\n\t\t\t\t{\n\t\t\t\t\tSeparator\n\t\t\t\t\t{\n\t\t\t\t\t\t");
		fprintf(fp,"Transform { translation 0.0 0.0 %3.5f }\n",-m_bookdepth/2.0);
		fprintf(fp,"\t\t\t\t\t\tRotationXYZ { axis Y angle %2.5f }\n",angle);
		fprintf(fp,"\t\t\t\t\t\tTransform { translation %3.5f %3.5f %3.5f }\n",thisx,thisy,thisz);

		// rotate
		fprintf(fp,"\n\t\t\t\t\t\tDEF bk%d SoBookKit\n\t\t\t\t\t\t{\n",books->id);

		// ID
		fprintf(fp,"\t\t\t\t\t\t\tid\t\t\t%d\n",books->id);

		// authors
		fprintf(fp,"\t\t\t\t\t\t\tauthors\t\t\t[");
		for(i=0; i<books->numauthors; i++)
		{
			if(i==0) fprintf(fp,"\"%s\"",books->authors[i]);
			else fprintf(fp,", \"%s\"",books->authors[i]);
		}
		fprintf(fp,"]\n");

		// keywords
		fprintf(fp,"\t\t\t\t\t\t\tkeywords\t\t\t[");
		for(i=0; i<books->numkeywords; i++)
		{
			if(i==0) fprintf(fp,"\"%s\"",books->keywords[i]);
			else fprintf(fp,", \"%s\"",books->keywords[i]);
		}
		fprintf(fp,"]\n");

		// title
		fprintf(fp,"\t\t\t\t\t\t\ttitle\t\t\t\"%s\"\n",books->title);

		// isbn
		fprintf(fp,"\t\t\t\t\t\t\tisbn\t\t\t\"%s\"\n",books->isbn);

		// shelf
		fprintf(fp,"\t\t\t\t\t\t\tshelf\t\t\t\"%s\"\n",books->shelf);

		// color
		fprintf(fp,"\t\t\t\t\t\t\tcolor\t\t\t%2.2f %2.2f %2.2f\n",books->color[0], books->color[1], books->color[2]);

		// year
		fprintf(fp,"\t\t\t\t\t\t\tyear\t\t\t%d\n",books->year);

		// gfx
		fprintf(fp,"\n\t\t\t\t\t\t\tcontents SoSeparator\n\t\t\t\t\t\t\t{\n\t\t\t\t\t\t\t\tMaterial\n\t\t\t\t\t\t\t\t{\n\t\t\t\t\t\t\t\t\tdiffuseColor %2.2f %2.2f %2.2f\n\t\t\t\t\t\t\t\t\ttransparency 0.5\n\t\t\t\t\t\t\t\t}\n",books->color[0], books->color[1], books->color[2]);
		fprintf(fp,"\n\t\t\t\t\t\t\t\tCube { width %3.2f height %3.2f depth %3.2f }\n\t\t\t\t\t\t\t}\n\t\t\t\t\t\t}\n",this->m_bookwidth, this->m_bookheight, this->m_bookdepth);
		
		fprintf(fp,"\t\t\t\t\t}\n");
		fprintf(fp,"\t\t\t\t}\n\n");

		tmp = books;
		books = books->next;
		free(tmp);
	}
	books = NULL;

	// shelf header
	fprintf(fp,"\t\t\t}\n\n\t\t\tDEF Regale Separator\n\t\t\t{\n");

	// all shelves
	for(i=0; i<this->numshelves; i++)
	{
		fprintf(fp,"\t\t\t\tDEF %s Switch\n\t\t\t\t{\n\t\t\t\t\tSeparator\n\t\t\t\t\t{\n",shelves[i].name);
	
		fprintf(fp,"\t\t\t\t\t\tMaterial { diffuseColor %2.2f %2.2f %2.2f transparency 0.0 }\n", shelves[i].rgb[0], shelves[i].rgb[1], shelves[i].rgb[2]);

		// shelf gfx
		for(j=0; j<shelves[i].numboxes; j++)
		{
			x1 = shelves[i].boxes[j].topleft[0];
			x2 = shelves[i].boxes[j].botright[0];
			z1 = shelves[i].boxes[j].topleft[2];
			z2 = shelves[i].boxes[j].botright[2];

			// calc shelf angle
			xd = x2-x1; zd = z2-z1;
			len = sqrt(xd*xd+zd*zd);
			xd/=len; zd/=len;
			angle = atan2(-xd,-zd)+1.5708;
			w = len; h = shelves[i].boxes[j].topleft[1]-shelves[i].boxes[j].botright[1];
			startx = x1; starty = shelves[i].boxes[j].topleft[1]; startz = z1;
			xd = x2-x1; zd = z2-z1;

			//fprintf(fp,"\t\t\t\t\t\tSeparator { Transform { translation %3.5f %3.5f %3.5f } RotationXYZ { axis Y angle %3.5f } Cube { width %3.4f height %3.4f depth %3.5f } }\n", startx+xd/2.0, starty-h/2.0, startz+zd/2.0, angle, len, h, 0.05);
			// top
			fprintf(fp,"\t\t\t\t\t\tSeparator { RotationXYZ { axis Y angle %3.5f } Transform { translation %3.5f %3.5f %3.5f } Cube { width %3.4f height %3.4f depth %3.5f } }\n", angle, startx+xd/2.0, starty, startz+zd/2.0, len, shelfthick, shelfthick);
			// bottom
			fprintf(fp,"\t\t\t\t\t\tSeparator { RotationXYZ { axis Y angle %3.5f } Transform { translation %3.5f %3.5f %3.5f } Cube { width %3.4f height %3.4f depth %3.5f } }\n", angle, startx+xd/2.0, starty-h, startz+zd/2.0, len, shelfthick, shelfthick);
			// left
			fprintf(fp,"\t\t\t\t\t\tSeparator { RotationXYZ { axis Y angle %3.5f } Transform { translation %3.5f %3.5f %3.5f } Cube { width %3.4f height %3.4f depth %3.5f } }\n", angle, startx, starty-h/2.0, startz, shelfthick, h, shelfthick);
			// right
			fprintf(fp,"\t\t\t\t\t\tSeparator { RotationXYZ { axis Y angle %3.5f } Transform { translation %3.5f %3.5f %3.5f } Cube { width %3.4f height %3.4f depth %3.5f } }\n", angle, startx+xd, starty-h/2.0, startz+zd, shelfthick, h, shelfthick);

		}

		// closing shelf
		fprintf(fp,"\t\t\t\t\t}\n\t\t\t\t}\n");
	}

	// stationkit
	fprintf(fp,"\t\t\t}\n\t\t}\n\t}\n\n\n\tDEF MarkedBooks Separator\n\t{\n");

	if(this->m_usestationkit==1)
	{
		if((st=fopen(this->m_stationkitfname,"r"))==NULL)
		{
			m_logtext+= "\r\ncould not open stationkits file!\r\n";
			UpdateData(FALSE);
			MessageBox("could not open stationkits file","error",MB_OK);
			return -1;
		}
		
		while(fgets(line,sizeof(line),st)!=NULL)
		{
			fprintf(fp,"\t\t%s",line);
		}
		fclose(st);		
	}
	else
		fprintf(fp,"\t\t# insert SoStationKit definitions here!\n");

	// close stationkits
	fprintf(fp,"\t}\n}\n");

	// finally write some library info
	fprintf(fp,"DEF libraryinfo_name Info\n{\n\tstring \"Name: %s\"\n}\n\n", this->m_libname);
	fprintf(fp,"DEF libraryinfo_numbooks Info\n{\n\tstring \"Books: %d\"\n\n}", this->numbooks);


	// closing time..

	fflush(fp);
	fclose(fp);

	m_logtext+= "done!\r\n";
	UpdateData(FALSE);

	return 0;
}


int ArlibconverterDlg::getshelf(char *code)
{
	// get a shelf by name
	int i=0;
	
	while(i<this->numshelves)
	{
		if(!strcmp(code, shelves[i].code))
		{
			return i;
		}
		i++;
	}
	return -1;
}




void ArlibconverterDlg::OnConvert() 
{
	//	convert the database

	if(readshelves()<0)
	{
		MessageBox("Conversion cancelled!","not done", MB_OK);
		return;
	}

	if(readbooks()<0)
	{
		MessageBox("Conversion cancelled!","not done", MB_OK);
		return;
	}

	if(writefile()<0)
	{
		MessageBox("Conversion cancelled!","not done", MB_OK);
		return;
	}

	MessageBox("Conversion succeeded!","done",MB_OK);
}

void ArlibconverterDlg::OnQuit() 
{
	//	quit
	exit(0);
}


void ArlibconverterDlg::OnStationkituse() 
{
	UpdateData(TRUE);
}


void ArlibconverterDlg::noumlaut(char *rawline, char *line)
{
	// resolve umlauts

	int i=0, j=0;

	for(i=0; i<strlen(rawline); i++)
	{
		switch(rawline[i])
		{
		case 'ö':
			line[j] = 'o'; j++;
			line[j] = 'e';
			break;
		case 'Ö':
			line[j] = 'O'; j++;
			line[j] = 'e';
		break;
			case 'ä':
			line[j] = 'a'; j++;
			line[j] = 'e';
		break;
		case 'Ä':
			line[j] = 'A'; j++;
			line[j] = 'e';
		break;
		case 'ü':
			line[j] = 'u'; j++;
			line[j] = 'e';
		break;
		case 'Ü':
			line[j] = 'U'; j++;
			line[j] = 'e';
		break;
		default:
			line[j] = rawline[i];
			break;
		}
		j++;
	}
	line[j] = 0;

}

