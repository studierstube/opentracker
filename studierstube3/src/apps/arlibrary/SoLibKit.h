 /* ======================================================================= */
/** header file for SoLibKit
  *
  * @author Harald Piringer, Eike Umlauf
  *
  * $Id: SoLibKit.h 3426 2004-07-14 12:11:27Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef __SOLIBKIT_H_
#define __SOLIBKIT_H_

#include <stbapi/widgets/So3DButton.h>
#include <stbapi/workspace/SoContextKit.h>
#include <SoLists.h>
#include "ARlibrary.h"
#include "SoBookKit.h"

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

/**
 * SoLibKit contains all functionality for the ARLibrary - Studierstube application.
 *
 * Very briefly, the ARLibrary is a database system for libraries with the special
 * feature that books can easily be located since their position is represented
 * in an augmented reality environment. Check out the webpage for further details.
 *
 * @author Harald Piringer and Eike Umlauf.
 * @ingroup arlibrary
 */
class ARLIBRARY_API SoLibKit
:
    public SoContextKit
{
    SO_KIT_HEADER(SoLibKit);

public:
    /// static class initialization.
    static void initClass();
    /// constructor.
    SoLibKit();
    /** override this to create new context of a specific type */
    SoContextKit* factory();
    /// destructor.
    ~SoLibKit();
	/// number of windows.
	int getNumWindows();

private:

	/**
	 * Converts the character to lower case.
	 *
	 * If the character passed as parameter is Uppercase, the respective
	 * lower case letter is returned. Otherwise, the input is returned.
	 *
	 * @param c The character to be converted.
	 *
	 * @return The converted letter.
	 */
	static char	toLowercase(const char & c);

	/**
	 * Converts a whole string to lower case.
	 *
	 * The retuned string is the same word as the string passed as
	 * parameter, but all letters are converted to lower case.
	 *
	 * @param s The string to be converted.
	 *
	 * @return The converted string.
	 */
	static SbString	toLowercase(const SbString & s);

	/**
	 * Tests, if the second string is part of the first string.
	 *
	 * @param s1 the 'outer' string that possibly contains the other one.
	 * @param s2 the 'inner' string that is possibly part of the other one.
	 *
	 * @return true if s2 is part of s1, false otherwise.
	 */
	static bool	StringContainsString(const SbString & s1, const SbString & s2);

	/**
	 * Returns the node representing a cretain book.
	 *
	 * Performs a search action starting from the 'AllBooks' separator and
	 * returns the requested book, if available.
	 *
	 * @param index The index of the requested book.
	 *
	 * @return A pointer to the respective SoBookKit, or NULL if the book wasn't found.
	 */
	static SoBookKit	* getBook(const int & index);

	/**
	 * Sets the visibility state of the shelf containing the specified book.
	 *
	 * @param pBook A pointer to the book whose shelf is to be enabled/disabled.
	 * @param on Set this to true to cause the shelf to be displayed, false to make it invisible.
	 */
	static void	setStateOfBookshelf(const SoBookKit	* pBook, const bool & on);

	/**
	 * Sets the visibility state of the specified book.
	 *
	 * @param pBook A pointer to the book which is to be displayed/hidden.
	 * @param on Set this to true to cause the book to be displayed, false to make it invisible.
	 */
	static void	setStateOfBook(const SoBookKit	* pBook, const bool & on);


	/**
	 * Registers the callback functions needed for a certain tab within the listbox.
	 *
	 * A listbox contains five tabs beeing ordinary buttons. The callback function
	 * menuSelection() gets called in order to react when such a button gets pressed.
	 * This method is used to enable this for a certain sheet.
	 *
	 * @param sheet The number of the sheet.
	 */
	static void registerTabCBs(int sheet);

	/**
	 * @name Geometry construction methods
	 */
	//@{

	/**
	 * Build the inventor subtree for the introductionary screen.
	 *
	 * The splash screen comprises the logo, the names of the
	 * authors and stuff like that.
	 *
	 * @return The separator being root of the subtree.
	 */
	static SoSeparator* buildsplash();

	/**
	 * Builds the inventor subtree for the options panel.
	 *
	 * Builds a sheet comprising six buttons to select on of the
	 * various search modes (by title/ by author/ ...)
	 *
	 * @return The separator being root of the subtree.
	 */
	static SoSeparator* buildcriteria();

	/**
	 * Builds the inventor subtree for both input modes (keyboard and grafitti).
	 *
	 * There are to ways to enter text (for example being the title you are looking for):
	 * A virtual keyboard and two grafitti pads (one for numbers and one for letters).
	 * The whole inventor subtree is constructed in this method.
	 *
	 * @return The separator being root of the subtree.
	 */
	SoSeparator* buildkeyboard();

	/**
	 * Builds the info panel showing detailed information about a book.
	 *
	 * When a specific book is selected, detailed information is shown. Besides
	 * this is when the 'augmented part' is performed: When you look around in
	 * the library, the book and its shelf gets highlighted.
	 *
	 * @return The separator being root of the subtree.
	 */
	static SoSeparator* buildinfopanel();

	/**
	 * Builds the listbox showing the titles of the currently selected books.
	 *
	 * This gets shown after selecting books via select by name/author... to
	 * resolve ambiguities. All books matching the current query are listed
	 * and you select one by simply presseing the according button.
	 *
	 * @return The separator being root of the subtree.
	 */
	static SoSeparator* buildlistbox();


	/**
	 * Builds up the whole pipsheet for the arlibrary application.
	 *
	 * All GUI-parts constructed in the previous methods are subtrees
	 * of a switch node ("pipswitch"). This and everything else that's
	 * necessary in order for the GUI to work properly is set/registered/
	 * buildt in this method.
	 *
	 * @param pip The separator where the inventor subtree of the whole gui is added to.
	 * @param app Not used in this method.
	 */
	void buildGUI(SoSeparator* pip, SoBaseKit* app);

    /** create new pip sheet.
     * @param uid user assigned to sheet */
    SoNode* createPipSheet( int uid );

    /** create window and content.
     * @param index index of window */
    SoWindowKit* createWindowGeometry( int index );

	//@}

	/**
	 * @name Callbacks
	 */
	//@{

	/**
	 * Gets triggered when we spotted a book that has a marker attached to it.
	 */
	static void cbMarkedBookSeen(void *userData, int stationId, SoStationKit *stationKit, SoHandle3DEventAction *action);

	/**
	 * Gets triggered, when a yet visible marked book isn't within sight any more.
	 */
	static void cbMarkedBookTimeout(void *userData, SoStationKit *stationKit);

	/**
	 * Gets triggered when a gesture has been recognized.
	 *
	 * The gesture indicates which gesture has been recognized: numbers from
	 * 2000 upwards are letters (2000 being 'a') and from 1000 upwards being
	 * digits (1000 being '0').
	 *
	 * @param gesture The code of the recognized gesture.
	 */
	static void cbGesture(void* userData, int gesture, SoGestureKit* gestureKit);

	/**
	 * Gets triggerd when the user clicks the up/down buttons within a listbox.
	 */
	static void cbScrollList(void * data, So3DButton* btn);

	/**
	 * Handles all user input except listbox.
	 */
	static void cbMenuSelection(void * data, So3DButton* btn);

	/**
	 * Handles events of the buttons in the listbox: causes a specified book
	 * to be selected.
	 */
	static void cbSelectBook(void * data, So3DButton* btn);

	/**
	 * Handles keyboard / gesture kit switching.
	 */
	static void cbSwitchPic(void * data, So3DButton* btn);

	//@}

//---- member variables
private:
	static SoNodeList	m_SelectedNodes;	///< contains the nodes of all books that have been chosen by a certain query.

public:

	/**
	 * @name Selection methods
	 * The following methods insert the nodes for all books into the m_SelectedNodes list, that match the respective query.
	 */
	//@{

	/**
	 * Select all books whose 'title'-field contains the specified string.
	 *
	 * @param title A (part of) the requested title.
	 */
	static void	preselectByTitle(const SbString & title);

	/**
	 * Select all books whose 'isbn'-field matches the specified string.
	 *
	 * @param isbn The exact ISBN number to be looked for.
	 */
	static void	preselectByISBN(const SbString & isbn);

	/**
	 * Select all books whose 'author'-field contains the specified string.
	 *
	 * @param author A (part of) the requested author.
	 */
	static void	preselectByAuthor(const SbString & author);

	/**
	 * Selects all books whose keywords match one (or all) of the specified keywords.
	 *
	 * 'Matching' means that one of the specifed keywords must be part of one of
	 * the keywords in the 'keywords'-field.
	 *
	 * @param	keywords An arbitrary set of keywords.
	 * @param	all If true, only books are accepted where all specified keywords are contained,
	 *			otherwise a single match suffices.
	 */
	static void	preselectByKeywords(const SoMFString & keywords, const bool & all);

	/**
	 * Selects all books whose 'year'-field is between the two specified values.
	 *
	 * @param startyear The first year to be taken into account.
	 * @param endyear The last year to be taken into account.
	 */
	static void	preselectByYear(const int & startyear, const int & endyear);

	//@}

	/**
	 * Clears all currently selected nodes and erases listbox.
	 */
	static void	clearSelection();

	/**
	 * Sets visibility of selected nodes to false.
	 */
	static void deactivateSelection();


	/**
	 * Clears visibility for all books and shelves.
	 */
	static void clearall();
};


#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif


