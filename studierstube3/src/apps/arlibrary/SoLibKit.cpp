 /* ======================================================================= */
/** source file for SoLibKit Node.
  *
  * @author Harald Piringer, Eike Umlauf
  *
  * $Id: SoLibKit.cpp 3427 2004-07-14 12:20:49Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#include <Inventor/nodes/SoSphere.h>
#include <Inventor/nodes/SoInfo.h>
#include <Inventor/actions/SoSearchAction.h>
#include <SoLists.h>
#include <stbapi/widgets/So3DButton.h>
#include <stbapi/widgets/So3DTouchButton.h>
#include <stbapi/interaction/So3DSeparator.h>
#include <stbapi/util/ivio.h>
#include <stbapi/event3d/Stb3DEventGenerator.h>
#include <stbapi/resource/SoPipSheetKit.h>
#include <stbapi/resource/SoPipKit.h>
#include <stbapi/tracker/SoStationKit.h>
#include <Inventor/nodes/SoComplexity.h>
#include <stbapi/widgets/SoGestureKit.h>

#include <assert.h>

#include "SoBookKit.h"
#include "SoLibKit.h"

#define LIST_UP		0
#define LIST_DOWN	1

#define BYTITLE 1
#define BYAUTHOR 2
#define BYKEYWORDS 3
#define BYKEYWORD 4
#define BYYEAR 5
#define BYTRACKING 6

#define MAXLISTBOX 500

struct book
{
	char name[50];		// the name of the book
	char isbn[30];		// the isbn num (deprecated)
	SoBookKit *pBook;	// the pointer to the book
};

StbCommunicator *gcomm = NULL;

// listbox vars
book lib[MAXLISTBOX]; // ahem.. static ;) maximum number of books in a listbox
int listindex = 0;
int numbooks = 5;
int activebook = -1;
int searchmode = BYTITLE; // default search mode = by title

int activesheet = 0;
char searchstring[100];

//---- initialize static member variables
SoNodeList SoLibKit::m_SelectedNodes;

//--initialization of kit structure in Inventor
SO_KIT_SOURCE(SoLibKit);

//----------------------------------------------------------------------------
// static class initialization
//----------------------------------------------------------------------------

void SoLibKit::initClass(void)
{
    SO_KIT_INIT_CLASS(SoLibKit, SoContextKit, "SoContextKit");
	SoBookKit::initClass();
}

//----------------------------------------------------------------------------
// constructor
//----------------------------------------------------------------------------

SoLibKit::SoLibKit()
{
    SO_KIT_CONSTRUCTOR(SoLibKit);
    SO_KIT_INIT_INSTANCE();

	activesheet=0;
	strcpy(searchstring,"");
}

//----------------------------------------------------------------------------
// destructor
//----------------------------------------------------------------------------

SoLibKit::~SoLibKit()
{
}


//---- converts a single character to lowercase
char		SoLibKit::toLowercase(const char & c)
{
	if ((c >= 'A') && (c <= 'Z')) return c + ('a' - 'A');
	switch(c)
	{
	case 'Ä':	return 'ä'; break;
	case 'Ö':	return 'ö'; break;
	case 'Ü':	return 'ü'; break;
	default:	return c;
	}
}

//---- returns a copy of the specified string, that is converted to lowercase
SbString	SoLibKit::toLowercase(const SbString & s)
{
	int			i		= s.getLength();
	char		* pCopy	= new char[i + 1];
	const char	* pOld	= s.getString();

	pCopy[i]	= '\0';
	while (i--)	pCopy[i]	= toLowercase(pOld[i]);

	return SbString(pCopy);
}

//---- tests, whether the string s1 contains the string s2
bool	SoLibKit::StringContainsString(const SbString & s1, const SbString & s2)
{
	int			l1	= s1.getLength(),
				l2	= s2.getLength(),
				i	= 0,
				j;
	const char	* pS1	= s1.getString(),
				* pS2	= s2.getString();

	while (l1 - i >= l2)
	{
		for (j = 0; (j < l2) && (toLowercase(pS1[i + j]) == pS2[j]); j++);

		if (j == l2) return true;
		++i;
	}

	return false;
}


//---- enables/disables the shelf for a certain book
void	SoLibKit::setStateOfBookshelf(const SoBookKit * pBook, const bool & on)
{
	SoSwitch	* pShelf	= (SoSwitch *)SoNode::getByName(pBook->shelf.getValue().getString());

	if (pShelf)
	{
		pShelf->whichChild = (on) ? -3 : -1;
	}
}

//---- enables/disables a book
void	SoLibKit::setStateOfBook(const SoBookKit * pBook, const bool & on)
{
	SoSearchAction	pSearch;
	pSearch.setSearchingAll(TRUE);
	pSearch.setNode((SoNode *)pBook);

	So3DSeparator	* pAllBooks	= (So3DSeparator *)SoNode::getByName("AllBooks");
	pSearch.apply(pAllBooks);

	SoPath	* pPath	= pSearch.getPath();

	if (pPath)
	{
		SoSwitch	* pSwitch	= (SoSwitch *)pPath->getNode(1);	// first is the "AllBooks" Separator, second is the desired switch node
		pSwitch->whichChild = (on) ? -3 : -1;
	}
}

//---- returns the book with a certain index
SoBookKit	* SoLibKit::getBook(const int & index)
{
	So3DSeparator	* pAllBooks = (So3DSeparator *)SoNode::getByName("AllBooks");
	if ((index < 0) || (index >= pAllBooks->getNumChildren())) return NULL;

	SoSwitch	* pDesiredBook	= (SoSwitch *)pAllBooks->getChild(index);

	//---- this is not the quickest way to do this, but it permits the structure of the graph to change (slightly)
	SoSearchAction	pSearch;
	pSearch.setSearchingAll(TRUE);
	pSearch.setType(SoType::fromName("SoBookKit"));

	pSearch.apply(pDesiredBook);
	SoPath	* pPath	= pSearch.getPath();

	return (pPath) ? (SoBookKit *)pPath->getNodeFromTail(0) : NULL;
}

//---- clears all currently selected nodes and erases listbox
void	SoLibKit::clearSelection()
{
	deactivateSelection();

	for(int i=0; i<MAXLISTBOX; i++)
	{
		lib[i].pBook = NULL;
	}

	m_SelectedNodes.truncate(0);
}



// set visibility of selected nodes to false

void SoLibKit::deactivateSelection()
{
	SoBookKit	* pBook;

	for (int i = 0; i < m_SelectedNodes.getLength(); i++)
	{
		pBook	= (SoBookKit *)m_SelectedNodes.get(i);
		setStateOfBook(pBook, false);
		setStateOfBookshelf(pBook, false);
	}

	// also clear the details settings
	SoText3* title = (SoText3*)SoNode::getByName("detailstitle");
	SoText3* authors = (SoText3*)SoNode::getByName("detailsauthors");
	SoText3* keywords = (SoText3*)SoNode::getByName("detailskeywords");
	SoText3* year = (SoText3*)SoNode::getByName("detailsyear");
	SoText3* isbn = (SoText3*)SoNode::getByName("detailsisbn");

	title->string.setValue("-");
	authors->string.setValue("-");
	keywords->string.setValue("-");
	year->string.setValue("-");
	isbn->string.setValue("-");

}


// clear visibility for all books and shelves

void SoLibKit::clearall()
{
	SoBookKit* pBook;
	int index=0;

	while ((pBook = getBook(index)) != NULL)
	{
		setStateOfBook(pBook, false);
		setStateOfBookshelf(pBook, false);
		index++;
	}
}




// ----------------------------------------------------------------------------
// PRESELECT-FUNCTIONS
// ----------------------------------------------------------------------------


//---- selects all books, where 'title' matches the according field, and copy them to our library array
void	SoLibKit::preselectByTitle(const SbString & title)
{
	//---- clear the current selection
	clearSelection();

	SoBookKit	* pBook;
	int			index = 0;

	SbString	lcTitle	= toLowercase(title);

	//---- iterate through all books
	while ((pBook = getBook(index)) != NULL)
	{
		index++;

		if (StringContainsString(pBook->title.getValue(), lcTitle))
		{
			if(listindex<MAXLISTBOX)
			{
				lib[listindex].pBook = pBook;
				listindex++; numbooks++;
			}
		}
	}
}

//---- selects all books, where the ISBN number matches the according field
void	SoLibKit::preselectByISBN(const SbString & isbn)
{
	//---- clear the current selection
	clearSelection();

	SoBookKit	* pBook;
	int			index = 0;

	//---- iterate through all books
	while ((pBook = getBook(index)) != NULL)
	{
		index++;

		if (pBook->isbn.getValue() == isbn)
		{
			if(listindex<MAXLISTBOX)
			{
				lib[listindex].pBook = pBook;
				listindex++; numbooks++;
			}
		}
	}
}

//---- selects all books, where 'author' matches one of the entries of the according field
void	SoLibKit::preselectByAuthor(const SbString & author)
{
	//---- clear the current selection
	clearSelection();

	SoBookKit	* pBook;
	int			index = 0, i;
	bool		found;
	SbString	lcAuthor	= toLowercase(author);

	//---- iterate through all books
	while ((pBook = getBook(index)) != NULL)
	{
		index++;

		for (i = 0, found = false; (i < pBook->authors.getNum()) && (!found); i++)
		{
			//if (pBook->authors[i] == author)
			if (StringContainsString(pBook->authors[i], lcAuthor))
			{
				found = true;

				if(listindex<MAXLISTBOX)
				{
					lib[listindex].pBook = pBook;
					listindex++; numbooks++;
				}
			}
		}
	}
}

//---- selects all books, where 'author' matches the one of the entries of the according field
void	SoLibKit::preselectByKeywords(const SoMFString & keywords, const bool & all)
{
	//---- clear the current selection
	clearSelection();

	SoBookKit	* pBook;
	int			index = 0,
				cnt, i, j;
	bool		found;
	SbString	lcKeyword;

	//---- iterate through all books
	while ((pBook = getBook(index)) != NULL)
	{
		index++;

		//---- loop through all specified keywords
		for (i = cnt = 0; i < keywords.getNum(); i++)
		{
			lcKeyword	= toLowercase(keywords[i]);

			//---- loop through all keywords of this book
			for (j = 0, found = false; (j < pBook->keywords.getNum()) && (!found); j++)
			{
				if (StringContainsString(pBook->keywords[j], lcKeyword))
				{
					found = true;

					cnt++;
				}
			}

			if ((all) && (!found)) break;	// skip, since this book can never match the query
			if ((!all) && (found)) break;	// skip, since the conditions of the query have already been fulfilled
		}

		//---- select the book, if it matches our query
		if (((all) && (cnt == keywords.getNum())) ||
			((!all) && (cnt > 0)))
		{
			if(listindex<MAXLISTBOX)
			{
				lib[listindex].pBook = pBook;
				listindex++; numbooks++;
			}
		}
	}
}

//---- selects all books, which were published between 'startyear' and 'endyear' (including the borders)
void	SoLibKit::preselectByYear(const int & startyear, const int & endyear)
{
	//---- clear the current selection
	clearSelection();

	SoBookKit	* pBook;
	int			index	= 0,
				start	= startyear,
				end		= endyear;

	if (endyear < startyear)
	{
		index	= end;
		end		= start;
		start	= index;
		index	= 0;
	}

	//---- iterate through all books
	while ((pBook = getBook(index)) != NULL)
	{
		index++;

		if ((pBook->year.getValue() >= start) && (pBook->year.getValue() <= end))
		{
			if(listindex<MAXLISTBOX)
			{
				lib[listindex].pBook = pBook;
				listindex++; numbooks++;
			}
		}
	}
}




//----------------------------------------------------------------------------
// redraw text in the listbox
// ---------------------------------------------------------------------------

void buildlist(int direction)
{
	// re-set the listbox texts
	SoText3 *txt[5];

	int i;
	char buf[100];

	listindex+=direction;
	if(listindex>numbooks-5) listindex = numbooks-5;
	if(listindex<0) listindex = 0;

	// get book buttons
	for(i=0; i<5; i++)
	{
		sprintf(buf,"t%d",i+1);
		txt[i] = (SoText3*)SoNode::getByName(buf);
		if(txt[i]==NULL) printf("DIRT");
	}

	for(i=0; i<5; i++)
	{
		if(i+listindex>=numbooks) // if the listindex is exceeded
			txt[i]->string.setValue(SbString("-"));
		else
		{
			if(lib[i+listindex].pBook!=NULL) // if we got a valid book pointer for this
				txt[i]->string.setValue(SbString(lib[i+listindex].pBook->title.getValue()));
			else	// if the pointer isn't valid, use preprocessed text (only upon startup)
				//txt[i]->string.setValue(SbString(lib[i+listindex].name));
				txt[i]->string.setValue(SbString("-"));
		}
	}
}


// --------------------------------------------------------------------------
// listbox scroll-callback
// --------------------------------------------------------------------------

void SoLibKit::cbScrollList(void * data, So3DButton* btn)
{
	buildlist((((int)data) == LIST_UP) ? -1 : 1);
}


// --------------------------------------------------------------------------
// activate a desired book using its ISBN
// --------------------------------------------------------------------------

void SoLibKit::cbSelectBook(void * data, So3DButton* btn)
{
	int i;
	char buf[1000];
	activebook = ((int)data) + listindex;
	SoBookKit* pBook = lib[activebook].pBook;


	if(pBook!=NULL)
	{
		deactivateSelection();

		// mark the book to be visible
		m_SelectedNodes.append(pBook);
		setStateOfBook(pBook, true);
		setStateOfBookshelf(pBook, true);

		// automatically switch to book details
		activesheet=4;
		SoSwitch *pipswitch = (SoSwitch*)SoNode::getByName("pipswitch");
		pipswitch->whichChild=activesheet;

		// and update the Details-text
		SoText3* title = (SoText3*)SoNode::getByName("detailstitle");
		SoText3* authors = (SoText3*)SoNode::getByName("detailsauthors");
		SoText3* keywords = (SoText3*)SoNode::getByName("detailskeywords");
		SoText3* year = (SoText3*)SoNode::getByName("detailsyear");
		SoText3* isbn = (SoText3*)SoNode::getByName("detailsisbn");

		// set title details
		title->string.setValue(pBook->title.getValue());
		// set authors details
		memset(buf,0,1000);
		for(i=0;i<pBook->authors.getNum(); i++)
		{
			strcat(buf,pBook->authors[i].getString());
			strcat(buf,", ");
		}
		buf[strlen(buf)-2] = 0;
		authors->string.setValue(buf);
		// set keywords details
		memset(buf,0,1000);
		for(i=0;i<pBook->keywords.getNum(); i++)
		{
			strcat(buf,pBook->keywords[i].getString());
			strcat(buf,", ");
		}
		buf[strlen(buf)-2] = 0;
		keywords->string.setValue(buf);
		// set year details
		sprintf(buf,"%d",pBook->year.getValue());
		year->string.setValue(buf);
		// set ISBN details
		isbn->string.setValue(pBook->isbn.getValue());
	}
}



//----------------------------------------------------------------------------
// handles all user input except listbox
//----------------------------------------------------------------------------

void SoLibKit::cbMenuSelection(void * data, So3DButton* btn)
{
	int i, kwc;
	char chr = 0;
	char buf[30];
	int panel = (int)data;
	char searchbak[100];
	int newcrit;

	SoMFString kwds;
	SbString words[10];
	int syear, eyear;
	char *pch; // for strtok

	SoText3 *txt;
	SoSwitch *pipswitch = (SoSwitch*)SoNode::getByName("pipswitch");
	SoSwitch *searchmodeswitch = (SoSwitch*)SoNode::getByName("critswitch");

	strcpy(searchbak,searchstring);


	switch(panel)
	{
	case 1: // title panel
		activesheet = 1;
		pipswitch->whichChild=activesheet;
		break;
	case 2: // criteria panel
		sprintf(buf,"%s",(btn->getName()).getString());

		if(strcmp(buf,"bytitle")==0)
		{
			activesheet = 2;
			searchmode = BYTITLE;
			newcrit = 0;
		}
		else if(strcmp(buf,"byauthor")==0)
		{
			activesheet = 2;
			searchmode = BYAUTHOR;
			newcrit = 1;
		}
		else if(strcmp(buf,"bykeywords")==0)
		{
			activesheet = 2;
			searchmode = BYKEYWORDS;
			newcrit = 2;
		}
		else if(strcmp(buf,"bykeyword")==0)
		{
			activesheet = 2;
			searchmode = BYKEYWORD;
			newcrit = 3;
		}
		else if(strcmp(buf,"byyear")==0)
		{
			activesheet = 2;
			searchmode = BYYEAR;
			newcrit = 4;
		}
		else if(strcmp(buf,"bytracking")==0)
		{
			activesheet = 4;
			SoLibKit::clearSelection();
			searchmode = BYTRACKING;
			newcrit = 0;
		}

		// set switch
		pipswitch->whichChild=activesheet;

		// set criteria icon switch
		searchmodeswitch->whichChild = newcrit;

		break;
	case 3: // text input
		strcpy(buf,(btn->getName()).getString()+1);
		chr = atoi(buf); // convert button value
		if(chr==123)
		{
			// backspace
			if(strlen(searchstring)>0)
			{
				searchstring[strlen(searchstring)-1]=0;
			}
		}
		else if(chr==124)
		{
			// space
			strcat(searchstring, " ");
		}
		else if(chr==125)
		{
			// separator
			strcat(searchstring, ",");
		}
		else if(chr==126)
		{
			// OK
			activesheet = 3; listindex=0; numbooks=0;
			for(i=0; i<MAXLISTBOX; i++)
			{
				lib[i].pBook = NULL;
				strcpy(lib[i].name,"");
				strcpy(lib[i].isbn,"0");
			}

			// now start the search action and fill the listbox with hits!
			switch(searchmode)
			{
			case BYTITLE:
				SoLibKit::preselectByTitle(SbString(searchstring));
				break;
			case BYAUTHOR:
				SoLibKit::preselectByAuthor(SbString(searchstring));
				break;
			case BYKEYWORDS:
				// prepare keywords list
				kwc = 0; // keyword counter
				pch = strtok (searchbak,",");
				if(pch!=NULL)
				{
					words[kwc] = pch;
					kwc++;
					while(pch != NULL && kwc<10)
					{
						pch = strtok (NULL, ",");
						if(pch!=NULL)
						{
							words[kwc] = pch;
							kwc++;
						}
					}
				}

				kwds.setValues(0, kwc, words);

				SoLibKit::preselectByKeywords(kwds, true);
				break;
			case BYKEYWORD:
				// prepare keywords list
				kwc = 0; // keyword counter
				pch = strtok (searchbak,",");
				if(pch!=NULL)
				{
					words[kwc] = pch;
					kwc++;
					while(pch != NULL && kwc<10)
					{
						pch = strtok (NULL, ",");
						if(pch!=NULL)
						{
							words[kwc] = pch;
							kwc++;
						}
					}
				}

				kwds.setValues(0, kwc, words);

				SoLibKit::preselectByKeywords(kwds, false);
				break;
			case BYYEAR:
				kwc = 0; // keyword counter
				pch = strtok (searchbak,",");
				syear =0; eyear = 0;
				if(pch!=NULL)
				{
					syear = atoi(pch); kwc++;
					while(pch != NULL && kwc<2)
					{
						pch = strtok (NULL, ",");
						if(pch!=NULL)
						{
							eyear = atoi(pch);
							kwc++;
						}
					}
				}
				if(syear==0) syear = 900000; // invalid year
				if(eyear==0) eyear = syear;
				if(eyear<syear) {i=syear; syear = eyear; eyear = i; } // swap

				SoLibKit::preselectByYear(syear, eyear);
				break;
			}

			// switch the pip!
			pipswitch->whichChild=activesheet;
			if(numbooks==0)
			{
				strcpy(lib[0].name,"-");
				strcpy(lib[1].name,"No");
				strcpy(lib[2].name,"Search");
				strcpy(lib[3].name,"Results");
				strcpy(lib[4].name,"-");
				numbooks=5;
			}
			listindex=0; // TWICE! because used as internal var while searching
			buildlist(0); // update listbox
		}
		else
		{
			sprintf(buf,"%c",chr);
			strcat(searchstring,buf);
		}
		// set new text!!
		txt = (SoText3*)SoNode::getByName("searchtext");
		txt->string.setValue(searchstring);

		break;
	case 5: // book info (tracking only)

		// nothing to be selected here...

		break;
	case 6: // very special: tab-bar

		// find out which pipsheet was desired
		strcpy(buf,(btn->getName()).getString()+5);
		chr = atoi(buf);

		activesheet = chr-1;

		// if searchmode is by tracking then there's no result box and no keyboard selectable
		if(searchmode == BYTRACKING)
		{
			if(activesheet==2 || activesheet==3)
				activesheet = 4;
		}

		pipswitch->whichChild=activesheet;

		break;
	}



}


// handle keyboard / gesture kit switching

void SoLibKit::cbSwitchPic(void * data, So3DButton* btn)
{
	SoSFInt32 foo;

	SoSwitch *sbtn = (SoSwitch*)SoNode::getByName("grafittiswitch");
	if(btn!=NULL)
	{
		foo = sbtn->whichChild;
		if(foo.getValue()==1) sbtn->whichChild = 0;
		else sbtn->whichChild=1;
	}
	else printf("UAAAARGHHH");

}




//----------------------------------------------------------------------------
// register the buttonpress callbacks for the TABs of a sheet
//----------------------------------------------------------------------------

void SoLibKit::registerTabCBs(int sheet)
{
	So3DButton* btn = NULL;
	char buf[10];

	for(int i=0; i<5; i++)
	{
		sprintf(buf,"_%dtab%d",sheet,i+1);
		//printf("registering: %s\n",buf);
		btn = (So3DButton*)SoNode::getByName(SbString(buf));
		if(btn!=NULL) btn->addPressCallback(cbMenuSelection, (void *)6);
		else printf("SHIT!");
	}
}



//----------------------------------------------------------------------------
// builds the listbox Panel (results)
//----------------------------------------------------------------------------

SoSeparator* SoLibKit::buildlistbox()
{
	// builds the listbox elements
	int i;
	char buf[50];

	// init listbox entries
	strcpy(lib[0].name, "-");
	strcpy(lib[1].name, "No");
	strcpy(lib[2].name, "Search");
	strcpy(lib[3].name, "Results");
	strcpy(lib[4].name, "-");

	SoSeparator* pip = new SoSeparator;

	pip->addChild(readFile("gfx/tabline4.iv", gcomm->workingDirectory));


	SoSeparator *s1 = new SoSeparator;
	SoSeparator *s2 = new SoSeparator;

	SoTransform *trafo1 = new SoTransform; trafo1->translation.setValue(5.3,0.0,0.0);
	SoTransform *trafo2 = new SoTransform; trafo2->translation.setValue(0.0,0.0,3.5);

	So3DButton *btn[5];
	SoSeparator *ts[5];
	SoRotationXYZ *tr[5];
	SoScale *tz[5];
	SoTransform *foo[5];
	SoTransform *tzt[5];
	SoText3 *t[5];

	// listbuttons
	for(i=0; i<5; i++)
	{
		btn[i] = new So3DButton(9.0,0.7,0.95);	// the button
		sprintf(buf,"%d",i+1);
		btn[i]->setName(buf);
		btn[i]->addPressCallback(cbSelectBook, (void *)i);
		ts[i] = new SoSeparator;				// separator for the buttontext
		tr[i] = new SoRotationXYZ; tr[i]->axis = SoRotationXYZ::X; tr[i]->angle.setValue(-1.57); // rotate text
		tzt[i] = new SoTransform; tzt[i]->translation.setValue(0.0,-0.355,0.4);	// shift text
		tz[i] = new SoScale; tz[i]->scaleFactor.setValue(0.025,0.04,1.0);		// scale text
		sprintf(buf,"t%d",i+1);
		t[i] = new SoText3; t[i]->string.setValue("Generic Book"); t[i]->justification = SoText3::CENTER; t[i]->setName(buf);
		foo[i] = new SoTransform; foo[i]->translation.setValue(0.0,0,1.0);	// offset for next button
	}

	SoTransform *trns = new SoTransform;
	trns->translation.setValue(-1.75,0,-2.0);
	pip->addChild(trns);
	pip->addChild(s2);
	pip->addChild(s1);

	// scrollbuttons
	s1->addChild(trafo1);
	s1->addChild(readFile("gfx/scrollbars.iv",gcomm->workingDirectory));

	So3DButton *up;
	So3DButton *down;
	up = (So3DButton*)SoNode::getByName(SbString("scrollup"));
	down = (So3DButton*)SoNode::getByName(SbString("scrolldown"));
	up->addPressCallback(cbScrollList, (void *)LIST_UP);
	down->addPressCallback(cbScrollList, (void *)LIST_DOWN);

	// build buttongraph
	for(i=0; i<5; i++)
	{
		s2->addChild(btn[i]);
		s2->addChild(ts[i]); ts[i]->addChild(tr[i]); ts[i]->addChild(tz[i]); ts[i]->addChild(tzt[i]); ts[i]->addChild(t[i]);
		s2->addChild(foo[i]);
	}

	buildlist(0);

	return pip;
}


//----------------------------------------------------------------------------
// builds the book info panel (details)
//----------------------------------------------------------------------------

SoSeparator* SoLibKit::buildinfopanel()
{
	SoSeparator *sep = new SoSeparator;
	sep->addChild(readFile("gfx/tabline5.iv",gcomm->workingDirectory));
	SoTransform *trns = new SoTransform;
	trns->translation.setValue(-1.25,0,0);
	sep->addChild(trns);

	// text box
	SoMaterial *mat = new SoMaterial;
	mat->diffuseColor.setValue(0.8,0.8,0.8);
	SoMaterial *mat2 = new SoMaterial;
	mat2->diffuseColor.setValue(0.0,0.0,0.0);
	SoCube* cb = new SoCube;
	cb->depth = 7.0;
	cb->height = 0.5;
	cb->width = 10.5;

	sep->addChild(mat);
	sep->addChild(cb);
	sep->addChild(mat2);
	sep->addChild(readFile("gfx/details.iv", gcomm->workingDirectory));

	return sep;
}


void SoLibKit::cbGesture(void* userData, int gesture, SoGestureKit* gestureKit)
{
	char buf[2];


	buf[0] = (gesture >= 2000) ? (char)(gesture - 2000 + 'a') : (char)(gesture - 1000 + '0');
	buf[1] = '\0';

	strcat(searchstring,buf);

	SoText3 * txt = (SoText3*)SoNode::getByName("searchtext");
	txt->string.setValue(searchstring);

	printf("%s\n", searchstring);
}

//----------------------------------------------------------------------------
// builds the keyboard.. or whatever input device :)
//----------------------------------------------------------------------------

SoSeparator* SoLibKit::buildkeyboard()
{
	int i;
	char buf[10];

	SoSeparator *sep = new SoSeparator;

	sep->addChild(readFile("gfx/tabline3.iv", gcomm->workingDirectory));

	So3DButton* r[40];
	SoTransform* t[40];
	SoText3* txt[40];
	SoTransform* ttf[40];
	SoRotationXYZ* tr[40];
	SoSeparator* tsep[40];
	SoTransform* lineskip[4];
	int x=0, y=0;
	int val=48;

	// load criteria button
	sep->addChild(readFile("gfx/criteria-kbdicon.iv",gcomm->workingDirectory));

	// load switch input button
	sep->addChild(readFile("gfx/switchinput.iv",gcomm->workingDirectory));
	So3DButton *btn = (So3DButton*)SoNode::getByName("switchpic1");
	if(btn!=NULL) btn->addPressCallback(cbSwitchPic, (void *)0);
	else printf("DRECK!");


	// define a sep
	SoSeparator *groupie = new SoSeparator;

	// searchtext
	SoSeparator *textbar = new SoSeparator;
	SoSeparator *textsep = new SoSeparator;
	SoSeparator *texttext = new SoSeparator;

	SoRotationXYZ *titlerot = new SoRotationXYZ; titlerot->axis = SoRotationXYZ::X; titlerot->angle.setValue(-1.57); // rotate text
	SoTransform* texttrans = new SoTransform; texttrans->translation.setValue(-2.0,0.22,-2.7);	// shift text
	SoTransform* boxtrans = new SoTransform; boxtrans->translation.setValue(-2.0,0.0,-3.0);	// shift text
	SoTransform* textscale = new SoTransform;
	textscale->scaleFactor.setValue(0.04,1.0,0.04);		// scale text
	SoText3* searchtxt = new SoText3; searchtxt->justification = SoText3::CENTER;
	searchtxt->setName("searchtext");
	searchtxt->string.setValue("<Enter Search Criteria>");


	// text box for searchtext
	SoMaterial *mat = new SoMaterial;
	mat->diffuseColor.setValue(0.8,0.8,0.8);
	SoCube* cb = new SoCube;
	cb->depth = 0.9;
	cb->height = 0.2;
	cb->width = 9.0;

	// textbox seps
	textsep->addChild(textbar);
	textsep->addChild(texttext);

	// the BAR
	textbar->addChild(boxtrans);
	textbar->addChild(mat);
	textbar->addChild(cb);

	// the TEXT
	texttext->addChild(texttrans);
	texttext->addChild(textscale);
	texttext->addChild(titlerot);
	texttext->addChild(searchtxt);

	// add it all!
	sep->addChild(textsep);


	// switch kbd/grafitti

	SoSwitch* inputswitch = new SoSwitch;
	SoSeparator *grafittisep = new SoSeparator;
	inputswitch->whichChild=0;
	inputswitch->setName("grafittiswitch");



	// ----------- grafitti -------------

	SoGestureKit::initClass();
	SoGestureKit	* gesture_alph	= new SoGestureKit(5.0f, 4.0f, 0.7f),
					* gesture_num	= new SoGestureKit(3.0f, 4.0f, 0.7f);

	gesture_alph->multiple.setValue(false);
	gesture_num->multiple.setValue(false);

	SbString str_alph_file = comm->workingDirectory;
	str_alph_file+=	"grafitti-alphabet.gst";
	gesture_alph->setGestures(str_alph_file);

	SbString str_num_file = comm->workingDirectory;
	str_num_file+=	"grafitti-numbers.gst";
	gesture_num->setGestures(str_num_file);

	gesture_alph->setGestureCallback(cbGesture, NULL);
	gesture_num->setGestureCallback(cbGesture, NULL);

	SoTransform *gftrafo1 = new SoTransform;
	gftrafo1->translation.setValue(-4.0, 0.0, 0.0);
	grafittisep->addChild(gftrafo1);
	grafittisep->addChild(gesture_alph);

	// explanation
	SoSeparator *expla1 = new SoSeparator;
	SoTransform *exptr1 = new SoTransform; exptr1->translation.setValue(-1.0, 0.4, 1.7); exptr1->scaleFactor.setValue(0.05, 1, 0.05);
	SoRotationXYZ *exprr1 = new SoRotationXYZ; exprr1->axis = SoRotationXYZ::X; exprr1->angle.setValue(-1.57);
	SoText3 *exptxt1 = new SoText3; exptxt1->string.setValue("ABC");

	grafittisep->addChild(expla1);
	expla1->addChild(exptr1);
	expla1->addChild(exprr1);
	expla1->addChild(exptxt1);


	SoTransform* boxtrans2 = new SoTransform;
	boxtrans2->translation.setValue(4.6,0.0,0.0);
	grafittisep->addChild(boxtrans2);

	grafittisep->addChild(gesture_num);

	// explanation 2
	SoSeparator *expla2 = new SoSeparator;
	SoTransform *exptr2 = new SoTransform; exptr2->translation.setValue(-0.6, 0.4, 1.7); exptr2->scaleFactor.setValue(0.05, 1, 0.05);
	SoRotationXYZ *exprr2 = new SoRotationXYZ; exprr2->axis = SoRotationXYZ::X; exprr2->angle.setValue(-1.57);
	SoText3 *exptxt2 = new SoText3; exptxt2->string.setValue("123");

	grafittisep->addChild(expla2);
	expla2->addChild(exptr2);
	expla2->addChild(exprr2);
	expla2->addChild(exptxt2);

	inputswitch->addChild(grafittisep);

	/**************************/

	// Keyboard!

	SoSeparator *commands = new SoSeparator;

	SoTransform* shift = new SoTransform; shift->translation.setValue(-5.8,0.0,-1.8);
	groupie->addChild(shift);

	// build keyboard
	for(i=0; i<40; i++)
	{
		// build buttons
		if(i<36)
		{ r[i] = new So3DButton(1.1, 0.7, 1.0); }
		else
		{ r[i] = new So3DButton(2.0, 0.7, 1.0); }

		// button naming. important for callback!
		sprintf(buf,"%d",val);
		r[i]->setName(SbName(buf));

		if(i<36) groupie->addChild(r[i]);
		else commands->addChild(r[i]);

		r[i]->addPressCallback(cbMenuSelection, (void *)3);

		// text on buttons
		tsep[i] = new SoSeparator;
		tr[i] = new SoRotationXYZ; tr[i]->axis = SoRotationXYZ::X; tr[i]->angle.setValue(-1.57); // rotate text
		ttf[i] = new SoTransform; ttf[i]->translation.setValue(0.0,-0.1,0.4);	// shift text
		ttf[i]->scaleFactor.setValue(0.04,0.04,1.0);		// scale text
		txt[i] = new SoText3; txt[i]->justification = SoText3::CENTER;

		if(val<123)
		{
			sprintf(buf,"%c", val);
		}
		else
		{
			if(val==123) sprintf(buf,"Del");
			if(val==124) sprintf(buf,"Space");
			if(val==125) sprintf(buf,",");
			if(val==126) sprintf(buf,"OK");
		}
		txt[i]->string.setValue(buf);

		tsep[i]->addChild(tr[i]);
		tsep[i]->addChild(ttf[i]);
		tsep[i]->addChild(txt[i]);
		if(i<36) groupie->addChild(tsep[i]);
		else commands->addChild(tsep[i]);

		// advance VAL
		val++; if(val==58) val=97;

		// transformations for next button
		t[i] = new SoTransform;
		if(y<4) t[i]->translation.setValue(1.1,0.0,0.0); else t[i]->translation.setValue(2.03,0.0,0.0);

		if(i<36) groupie->addChild(t[i]);
		else
			commands->addChild(t[i]);

		x++;
		if(x>=9)
		{
			x=0;
			lineskip[y] = new SoTransform;
			if(y<3)lineskip[y]->translation.setValue(-1.1*9,0.0,1.02);
			else lineskip[y]->translation.setValue(-1.1*8.4,0.0,1.02);
			groupie->addChild(lineskip[y]);
			y++;
		}


	}


	inputswitch->addChild(groupie);
	sep->addChild(inputswitch);

	// shift commands in place
	SoTransform *cmdt = new SoTransform;
	cmdt->translation.setValue(-4.6, 0.0, 3.0);

	sep->addChild(cmdt);
	sep->addChild(commands);

	return sep;
}



//----------------------------------------------------------------------------
// builds the 'SEARCH BY..' (Options) panel
//----------------------------------------------------------------------------

SoSeparator* SoLibKit::buildcriteria()
{
	int i;
	char buf[100];

	SoSeparator *sep = new SoSeparator;

	sep->addChild(readFile("gfx/tabline2.iv", gcomm->workingDirectory));

	So3DButton* r[6];
	SoTransform* t[6];
	SoText3* txt[3];
	SoTransform* ttf[6];
	SoRotationXYZ* tr[6];
	SoSeparator* tsep[6];

	// radio buttons must be under a group node
	SoGroup *groupie = new SoGroup;
	SoTransform *trns = new SoTransform;
	trns->translation.setValue(-2.0,0,-2.5);
	groupie->addChild(trns);

	for(i=0; i<6; i++)
	{
		// build buttons
		r[i] = new So3DButton(8.7, 0.7, 0.9);

		// names & callbacks
		switch(i)
		{
		case 0: r[i]->setName("bytitle"); break;
		case 1: r[i]->setName("byauthor"); break;
		case 2: r[i]->setName("bykeywords"); break;
		case 3: r[i]->setName("bykeyword"); break;
		case 4: r[i]->setName("byyear"); break;
		case 5: r[i]->setName("bytracking"); break;
		}
		groupie->addChild(r[i]);
		sprintf(buf,"gfx/criteria-icon%d.iv",i+1);
		groupie->addChild(readFile(buf,gcomm->workingDirectory));
		r[i]->addPressCallback(cbMenuSelection, (void *)2);

		// text on buttons
		tsep[i] = new SoSeparator;
		tr[i] = new SoRotationXYZ; tr[i]->axis = SoRotationXYZ::X; tr[i]->angle.setValue(-1.57); // rotate text
		ttf[i] = new SoTransform; ttf[i]->translation.setValue(0.0,-0.1,0.4);	// shift text
		ttf[i]->scaleFactor.setValue(0.04,0.04,1.0);		// scale text
		txt[i] = new SoText3; txt[i]->justification = SoText3::CENTER;
		switch(i)
		{
		case 0: txt[i]->string.setValue("Search by Title"); break;
		case 1: txt[i]->string.setValue("Search by Author(s)"); break;
		case 2: txt[i]->string.setValue("Search by Keywords"); break;
		case 3: txt[i]->string.setValue("Search by single Keyword"); break;
		case 4: txt[i]->string.setValue("Search by Year"); break;
		case 5: txt[i]->string.setValue("Use Tracking"); break;
		}
		tsep[i]->addChild(tr[i]);
		tsep[i]->addChild(ttf[i]);
		tsep[i]->addChild(txt[i]);
		groupie->addChild(tsep[i]);

		// transformations for next button
		t[i] = new SoTransform;
		t[i]->translation.setValue(0.0,0.0,1.1);

		groupie->addChild(t[i]);

	}

	sep->addChild(groupie);

	return sep;
}


//----------------------------------------------------------------------------
// builds the Splash Screen panel (FBA)
//----------------------------------------------------------------------------

SoSeparator* SoLibKit::buildsplash()
{
	SoSeparator *sep = new SoSeparator;

	sep->addChild(readFile("gfx/tabline1.iv", gcomm->workingDirectory));
	SoTransform *trns = new SoTransform;
	trns->translation.setValue(-1.25,0,0);
	sep->addChild(trns);
	sep->addChild(readFile("gfx/splashpip.iv",gcomm->workingDirectory));

	// add callback to splash
	So3DButton* btn = (So3DButton*)SoNode::getByName("splashpic");
	btn->addPressCallback(cbMenuSelection, (void *)1);

	return sep;
}



//----------------------------------------------------------------------------
// build a PipSheetKit containing all the panels we need
//----------------------------------------------------------------------------

void SoLibKit::buildGUI(SoSeparator* pip, SoBaseKit* app)
{
	// we want anti-aliasing on our pipsheet!
	SoComplexity* blur = new SoComplexity;
	blur->textureQuality = 1.0;

	// prepare the pipsheetkit: we need a switch with the pipsheets below
	SoSwitch* pipswitch = new SoSwitch(5);
	pipswitch->setName("pipswitch");

	// build the sheets
	pipswitch->addChild(buildsplash());
	pipswitch->addChild(buildcriteria());
	pipswitch->addChild(buildkeyboard());
	pipswitch->addChild(buildlistbox());
	pipswitch->addChild(buildinfopanel());

	// activate first sheet
	pipswitch->whichChild=activesheet;

	// add to graph
	pip->addChild(blur);
	pip->addChild(pipswitch);

	// register TAB callbacks
	registerTabCBs(1);
	registerTabCBs(2);
	registerTabCBs(3);
	registerTabCBs(4);
	registerTabCBs(5);

	// now we'll try to set up the library info. if the library file has not been loaded yet, it will fail gracefully
    // take this and move it up to the checkPipGeometry or so
	SoText3* libnamefield = (SoText3*)SoNode::getByName("libinfo_name");
	SoInfo* libname = (SoInfo*)SoNode::getByName("libraryinfo_name");
	SoText3* libnumbooksfield = (SoText3*)SoNode::getByName("libinfo_numbooks");
	SoInfo* libnumbooks = (SoInfo*)SoNode::getByName("libraryinfo_numbooks");

	if(libname!=NULL)
		libnamefield->string.setValue(libname->string.getValue());

	if(libnumbooks!=NULL)
		libnumbooksfield->string.setValue(libnumbooks->string.getValue());

}




//----------------------------------------------------------------------------
// Reads the pip sheet file and initializes GUI building
//----------------------------------------------------------------------------

SbBool SoLibKit::checkPipGeometry()
{
	// we need comm globally later on
	// gcomm = comm;

    // set app name
    // comm->setContextAliasName(getName(),"AR Library");

    // load the geometry from the file into a separator
    SoSeparator * pipSheet = (SoSeparator) templatePipSheet.getValue();

	// build all the pipSheets
	buildGUI(pipSheet, this);

    return TRUE;
}

//---- gets triggered when we spotted a book that has a marker attached to it
void SoLibKit::cbMarkedBookSeen(void *userData, int stationId, SoStationKit *stationKit, SoHandle3DEventAction *action)
{
	if( searchmode==BYTRACKING )
	{
		SoGroup	*pContent = (SoGroup *)stationKit->getPart("content", TRUE);
		SoBookKit *pBook = (SoBookKit *)pContent->getChild(0);	// only has one child



		//---- now, let's turn the switch node on, regardless of its former state
		setStateOfBook(pBook, true);
		setStateOfBookshelf(pBook, true);

		// and update the Details-text
		int i;
		char buf[1000];
		SoText3* title = (SoText3*)SoNode::getByName("detailstitle");
		SoText3* authors = (SoText3*)SoNode::getByName("detailsauthors");
		SoText3* keywords = (SoText3*)SoNode::getByName("detailskeywords");
		SoText3* year = (SoText3*)SoNode::getByName("detailsyear");
		SoText3* isbn = (SoText3*)SoNode::getByName("detailsisbn");

		// set title details
		title->string.setValue(pBook->title.getValue());
		// set authors details
		memset(buf,0,1000);
		for(i=0;i<pBook->authors.getNum(); i++)
		{
			strcat(buf,pBook->authors[i].getString());
			strcat(buf,", ");
		}
		buf[strlen(buf)-2] = 0;
		authors->string.setValue(buf);
		// set keywords details
		memset(buf,0,1000);
		for(i=0;i<pBook->keywords.getNum(); i++)
		{
			strcat(buf,pBook->keywords[i].getString());
			strcat(buf,", ");
		}
		buf[strlen(buf)-2] = 0;
		keywords->string.setValue(buf);
		// set year details
		sprintf(buf,"%d",pBook->year.getValue());
		year->string.setValue(buf);
		// set ISBN details
		isbn->string.setValue(pBook->isbn.getValue());
	}
}

//---- gets triggered, when a yet visible marked book isn't within sight any more
void SoLibKit::cbMarkedBookTimeout( void *userData, SoStationKit *stationKit)
{
	SoGroup			* pContent	= (SoGroup *)stationKit->getPart("content", TRUE);
	SoBookKit		* pBook		= (SoBookKit *)pContent->getChild(0);	// only has one child

	//---- only deactivate, if not currently selected
	if ((m_SelectedNodes.find(pBook) == -1) && (pBook))
	{
		//---- now, let's turn the switch node off
		setStateOfBook(pBook, false);
		setStateOfBookshelf(pBook, false);
	}
}

//----------------------------------------------------------------------------
// createWindowGeometry
// create window and attach scene graph to it. scene graph contains
// transforms that are connected to global fields updating them from
// the tracking stations.
//----------------------------------------------------------------------------

SoWindowKit* SoLibKit::createWindowGeometry(int index)
{
	//--create a default window
    WindowCreate wc;
    SoWindowKit::defaultWindow(wc);
    SoWindowKit* windowKit = comm->createWindow(NULL, &wc, NULL, NULL);
    windowKit->size = SbVec3f(10,10,10);
	windowKit->setPosition(SbVec3f(0,0,0));

    // read the scenegraph from the file "samplegeometry.iv" and add it to
    // the window client volume. this is where the applications geometry goes.
    SoSeparator* clientVolume = windowKit->getClientVolumeSeparator();
    SoSeparator* geometry = readFile("arlib.iv",comm->workingDirectory);
    // show error if loading geometry went wrong
    if( geometry == NULL )
        printf("ARLibrary: couldn't load geometry !!\n");
    // add geometry to the child volume of the window
    else
        clientVolume->addChild( geometry );

	//---- find Separator and set the callbacks of the marked books
	SoSeparator	* pMarkedBooks	= (SoSeparator *)SoNode::getByName("MarkedBooks");
	if (pMarkedBooks == NULL)	printf("ARLibrary: error in geometry file!!\n");
	else
	{
		int				numChildren	= pMarkedBooks->getNumChildren();
		SoStationKit	* pMBook;

		while (numChildren--)
		{
			pMBook	= (SoStationKit *)pMarkedBooks->getChild(numChildren);

			pMBook->addEventCallback(cbMarkedBookSeen, (void *)0);
			pMBook->addTimeoutCallback(cbMarkedBookTimeout, (void *)0);
		}
	}

	// set all books and shelves to invisible
	clearall();

	// now we'll try to set up the library info on the PIP. if the pip file has not been loaded yet, it will fail gracefully
	SoText3* libnamefield = (SoText3*)SoNode::getByName("libinfo_name");
	SoInfo* libname = (SoInfo*)SoNode::getByName("libraryinfo_name");
	SoText3* libnumbooksfield = (SoText3*)SoNode::getByName("libinfo_numbooks");
	SoInfo* libnumbooks = (SoInfo*)SoNode::getByName("libraryinfo_numbooks");

	if(libnamefield!=NULL)
		libnamefield->string.setValue(libname->string.getValue());

	if(libnumbooksfield!=NULL)
		libnumbooksfield->string.setValue(libnumbooks->string.getValue());

    // finally return the new windowkit
    return windowKit;

}


int SoLibKit::getNumWindows()
{
	// overwritten
	return 1;
}


