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
/** header file for the main application window class of the OpenTracker tool
  *
  * @author Gerhard Reitmayr
  *
  * $Id$
  * @file                                                                   */
 /* ======================================================================= */


#ifndef _TOOLAPIWIN_H
#define _TOOLAPIWIN_H

#include <qmainwindow.h>

class ToolForm;
class QToolBar;
class QPopupMenu;
class QTimer;
class QLabel;
class Context;

/**
 * The main application window of the OpenTracker tool. This class currently
 * handles almost everything, besides the OpenTracker details. 
 * @author Gerhard Reitmayr
 * @ingroup tool
 */
class ToolAppWin: public QMainWindow
{
    Q_OBJECT
public:
    ToolAppWin();
    ~ToolAppWin();
    
protected:
    void closeEvent( QCloseEvent* );

private slots:
    void load();
    void load( const char *fileName );
    void reload();

    void start();
    void pause();
    void stop();
    void loop();

    void about();

private:
    QToolBar *fileTools, *controlTools;
    ToolForm *win;
    QTimer *timer;
    QString filename;
    QLabel * framerate;
    double startTime;
    long int frames;

protected:
    Context * context;
};

#endif
