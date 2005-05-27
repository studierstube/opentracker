/****************************************************************************
** Form implementation generated from reading ui file '.\settings.ui'
**
** Created: Thu Aug 5 01:29:06 2004
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#include "settings.h"

#include <qvariant.h>
#include <qcombobox.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include "settings.ui.h"

/* 
 *  Constructs a Settings which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
Settings::Settings( QWidget* parent,  const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "Settings" );
    resize( 259, 163 ); 
    setCaption( trUtf8( "SimVoleon settings" ) );

    TextLabel1 = new QLabel( this, "TextLabel1" );
    TextLabel1->setGeometry( QRect( 11, 41, 89, 21 ) ); 
    TextLabel1->setText( trUtf8( "Mode:" ) );
    TextLabel1->setAlignment( int( QLabel::AlignVCenter | QLabel::AlignRight ) );

    TextLabel3 = new QLabel( this, "TextLabel3" );
    TextLabel3->setGeometry( QRect( 10, 100, 91, 21 ) ); 
    TextLabel3->setText( trUtf8( "Color map type:" ) );
    TextLabel3->setAlignment( int( QLabel::AlignVCenter | QLabel::AlignRight ) );

    TextLabel4 = new QLabel( this, "TextLabel4" );
    TextLabel4->setGeometry( QRect( 10, 130, 91, 21 ) ); 
    TextLabel4->setText( trUtf8( "Axis:" ) );
    TextLabel4->setAlignment( int( QLabel::AlignVCenter | QLabel::AlignRight ) );

    TextLabel2 = new QLabel( this, "TextLabel2" );
    TextLabel2->setGeometry( QRect( 11, 68, 89, 21 ) ); 
    TextLabel2->setText( trUtf8( " Transfer function:" ) );
    TextLabel2->setAlignment( int( QLabel::AlignVCenter | QLabel::AlignRight ) );

    TextLabel5 = new QLabel( this, "TextLabel5" );
    TextLabel5->setGeometry( QRect( 10, 10, 91, 21 ) ); 
    TextLabel5->setText( trUtf8( "File:" ) );
    TextLabel5->setAlignment( int( QLabel::AlignVCenter | QLabel::AlignRight ) );

    ComboBox1 = new QComboBox( FALSE, this, "ComboBox1" );
    ComboBox1->insertItem( trUtf8( "Volume" ) );
    ComboBox1->insertItem( trUtf8( "Orthoslice" ) );
    ComboBox1->insertItem( trUtf8( "Oblique slice" ) );
    ComboBox1->setGeometry( QRect( 110, 40, 141, 21 ) ); 
    ComboBox1->setCurrentItem( 1 );

    ComboBox2 = new QComboBox( FALSE, this, "ComboBox2" );
    ComboBox2->insertItem( trUtf8( "None" ) );
    ComboBox2->insertItem( trUtf8( "Gray" ) );
    ComboBox2->insertItem( trUtf8( "Temperature" ) );
    ComboBox2->insertItem( trUtf8( "Physics" ) );
    ComboBox2->insertItem( trUtf8( "Standard" ) );
    ComboBox2->insertItem( trUtf8( "Glow" ) );
    ComboBox2->insertItem( trUtf8( "Blue/Red" ) );
    ComboBox2->insertItem( trUtf8( "Seismic" ) );
    ComboBox2->setGeometry( QRect( 110, 70, 141, 21 ) ); 
    ComboBox2->setCurrentItem( 3 );

    ComboBox3 = new QComboBox( FALSE, this, "ComboBox3" );
    ComboBox3->insertItem( trUtf8( "Alpha" ) );
    ComboBox3->insertItem( trUtf8( "Lum_alpha" ) );
    ComboBox3->insertItem( trUtf8( "RGBA" ) );
    ComboBox3->setGeometry( QRect( 110, 100, 141, 21 ) ); 

    ComboBox4 = new QComboBox( FALSE, this, "ComboBox4" );
    ComboBox4->insertItem( trUtf8( "X" ) );
    ComboBox4->insertItem( trUtf8( "Y" ) );
    ComboBox4->insertItem( trUtf8( "Z" ) );
    ComboBox4->setGeometry( QRect( 110, 130, 141, 21 ) ); 
    ComboBox4->setCurrentItem( 2 );

    ComboBox5 = new QComboBox( FALSE, this, "ComboBox5" );
    ComboBox5->insertItem( trUtf8( "3dhead" ) );
    ComboBox5->insertItem( trUtf8( "bentum" ) );
    ComboBox5->insertItem( trUtf8( "colt" ) );
    ComboBox5->insertItem( trUtf8( "cthead" ) );
    ComboBox5->insertItem( trUtf8( "engine" ) );
    ComboBox5->insertItem( trUtf8( "hplogo" ) );
    ComboBox5->insertItem( trUtf8( "lobster" ) );
    ComboBox5->insertItem( trUtf8( "spine" ) );
    ComboBox5->insertItem( trUtf8( "syn_64" ) );
    ComboBox5->insertItem( trUtf8( "trabecular" ) );
    ComboBox5->setGeometry( QRect( 110, 10, 141, 21 ) ); 
    ComboBox5->setCurrentItem( 3 );

    // signals and slots connections
    connect( ComboBox1, SIGNAL( activated(int) ), this, SLOT( ModeActivated(int) ) );
    connect( ComboBox2, SIGNAL( activated(int) ), this, SLOT( TransferFunctionActivated(int) ) );
    connect( ComboBox3, SIGNAL( activated(int) ), this, SLOT( ColorMapActivated(int) ) );
    connect( ComboBox4, SIGNAL( activated(int) ), this, SLOT( AxisActivated(int) ) );
    connect( ComboBox5, SIGNAL( activated(const QString&) ), this, SLOT( FileActivated(const QString&) ) );
}

/*  
 *  Destroys the object and frees any allocated resources
 */
Settings::~Settings()
{
    // no need to delete child widgets, Qt does it all for us
}

