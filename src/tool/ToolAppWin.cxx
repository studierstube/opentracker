 /* ========================================================================
  * Copyright (C) 2001  Vienna University of Technology
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the GNU Lesser General Public
  * License as published by the Free Software Foundation; either
  * version 2.1 of the License, or (at your option) any later version.
  *
  * This library is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  * Lesser General Public License for more details.
  *
  * You should have received a copy of the GNU Lesser General Public
  * License along with this library; if not, write to the Free Software
  * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
  *
  * For further information please contact Gerhard Reitmayr under
  * <reitmayr@ims.tuwien.ac.at> or write to Gerhard Reitmayr,
  * Vienna University of Technology, Favoritenstr. 9-11/188, A1040 Vienna,
  * Austria.
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** source file for the main application window class of the OpenTracker tool
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/tool/ToolAppWin.cxx,v 1.1 2001/11/21 11:46:05 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

#include "ToolAppWin.h"

#include <qimage.h>
#include <qpixmap.h>
#include <qtoolbar.h>
#include <qtoolbutton.h>
#include <qpopupmenu.h>
#include <qmenubar.h>
#include <qkeycode.h>
#include <qmultilineedit.h>
#include <qfile.h>
#include <qfiledialog.h>
#include <qstatusbar.h>
#include <qmessagebox.h>
#include <qapplication.h>
#include <qaccel.h>
#include <qtextstream.h>
#include <qpainter.h>
#include <qpaintdevicemetrics.h>
#include <qwhatsthis.h>
#include "ToolForm.h"
#include <qtable.h>
#include <qheader.h>
#include <qtimer.h>
#include <qlabel.h>

#include <OpenTracker.h>
#include <common/ConsoleModule.h>
#include "ToolIOModule.h"

#include "fileopen.xpm"

const char * fileOpenText = "<img source=\"fileopen\"> "
"Click this button to open a <em>new file</em>. <br><br>"
"You can also select the <b>Open command</b> from the File menu.";

ToolAppWin::ToolAppWin()
    : QMainWindow( 0, "OpenTracker Tool", WDestructiveClose )
{
    int id;

    QPixmap openIcon;

    fileTools = new QToolBar( this, "file operations" );
    fileTools->setLabel( tr( "File Operations" ) );

    openIcon = QPixmap( fileopen );
    QToolButton * fileOpen
	= new QToolButton( openIcon, "Open File", QString::null,
			   this, SLOT(load()), fileTools, "open file" );

    (void)QWhatsThis::whatsThisButton( fileTools );

    QWhatsThis::add( fileOpen, fileOpenText );

    QPopupMenu * file = new QPopupMenu( this );
    menuBar()->insertItem( "&File", file );

    id = file->insertItem( openIcon, "&Open",
			   this, SLOT(load()), CTRL+Key_O );
    file->setWhatsThis( id, fileOpenText );
    file->insertItem( "&Reload", this, SLOT(reload()), CTRL+SHIFT+Key_O );
    file->insertSeparator();
    file->insertItem( "&Quit", qApp, SLOT( closeAllWindows() ), CTRL+Key_Q );

    controlTools = new QToolBar( this, "control operations" );
    controlTools->setLabel( tr("Control Operations"));

    QPopupMenu * control = new QPopupMenu( this );
    menuBar()->insertItem( "&Control", control );
    control->insertItem("&Start", this, SLOT(start()), CTRL+Key_S );
    control->insertItem("&Pause", this, SLOT(pause()), CTRL+Key_P );
    control->insertItem("S&top", this, SLOT(stop()), CTRL+Key_T );
    control->insertItem("&Reset", this, SLOT(reload()), CTRL+Key_R );

    QPopupMenu * help = new QPopupMenu( this );
    menuBar()->insertSeparator();
    menuBar()->insertItem( "&Help", help );

    help->insertItem( "&About", this, SLOT(about()), Key_F1 );
    help->insertSeparator();
    help->insertItem( "What's &This", this, SLOT(whatsThis()), SHIFT+Key_F1 );

    // setup contents
    win = new ToolForm( this );
    QHeader * top = win->output->horizontalHeader();
    top->setLabel(0, "position");
    top->setLabel(1, "orientation");
    top->setLabel(2, "button");
    top->setLabel(3, "confidence");
    top->setLabel(4, "time");
    top->setLabel(5, "lifetime");
    win->output->setLeftMargin(100);
    win->output->setColumnStretchable( 0, TRUE ); 
    win->output->setColumnStretchable( 1, TRUE );
    //win->output->setColumnStretchable( 2, TRUE );
    //win->output->setColumnStretchable( 3, TRUE );
    win->output->setColumnStretchable( 4, TRUE );
    win->output->setColumnStretchable( 5, TRUE );
    win->output->setColumnMovingEnabled( TRUE );
    win->output->setSelectionMode( QTable::NoSelection );

    setCentralWidget( win );

    framerate = new QLabel( statusBar());
    framerate->setText("Framerate 0");
    statusBar()->addWidget( framerate, 0, TRUE );

    statusBar()->message( "Ready", 2000 );

    timer = new QTimer( this );
    connect( timer, SIGNAL(timeout()), SLOT(loop()) );
}


ToolAppWin::~ToolAppWin()
{
    delete win;
}


void ToolAppWin::load()
{
    QString fn = QFileDialog::getOpenFileName( QString::null, QString::null,
					       this);
    if ( !fn.isEmpty() )
	    load( fn );
    else
	    statusBar()->message( "Loading aborted", 2000 );
}


void ToolAppWin::load( const char *fileName )
{
    filename = fileName;
    reload();

    QString s;
    s.sprintf( "Loaded document %s", fileName );
    statusBar()->message( s, 2000 );
}

void ToolAppWin::reload()
{
    /// @todo clean up any old context first !!!!
    context = new Context( 1 );
    ConsoleModule * mod = (ConsoleModule *) context->getModule("ConsoleConfig");
    context->removeModule( *mod );
    context->removeFactory( *mod );
    delete mod;

    ToolIOModule * tool = new ToolIOModule( win->output );
    context->addModule( "ConsoleConfig", *tool );
    context->addFactory( *tool );

    context->parseConfiguration( filename.latin1() );
}

void ToolAppWin::closeEvent( QCloseEvent* ce )
{
    stop();
    ce->accept();
}

void ToolAppWin::about()
{
    QMessageBox::about( this, "OpenTracker Tool 0.1",
			"The OpenTracker Tool is a simple GUI application "
			"to execute a configuration.\n"
            "Use it to experiment and deal with servers.\n\n"
            "Developed by Gerhard Reitmayr\n"
            "at Vienna University of Technology, Austria");
}

void ToolAppWin::start()
{
    context->start();
    timer->start( 0, FALSE );
    statusBar()->message( "Processing started..." );
    startTime = OSUtils::currentTime();
    frames = 0;
}
    
void ToolAppWin::stop()
{
    if( timer->isActive())
    {
        timer->stop();
        context->close();
        statusBar()->message( "Processing stopped..." );
    }
}

void ToolAppWin::pause()
{
   if( timer->isActive())
   {
       timer->stop();
       statusBar()->message( "Processing paused..." );
   }
   else
   {
       timer->start( 0 , FALSE );
       statusBar()->message( "Processing restarted..." );
       startTime = OSUtils::currentTime();
       frames = 0;
   }
}

void ToolAppWin::loop()
{
    context->pushStates();
    context->pullStates();
    if(context->stop() == 1)
        stop();
    frames ++;
    QString s;
    s.sprintf("Framerate %lf", frames*1000/(OSUtils::currentTime() - startTime));
    framerate->setText( s );
}
