/* ========================================================================
 * Copyright (c) 2006,
 * Institute for Computer Graphics and Vision
 * Graz University of Technology
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *
 * Neither the name of the Graz University of Technology nor the names of
 * its contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * ========================================================================
 * PROJECT: OpenTracker
 * ======================================================================== */
/** main window of the Callback/Callforward program
 *
 * @author Alexander Bornik
 *
 * $Id$
 * @file                                                                   */
/* ======================================================================= */

#include <QtGui>
#include "cbcfmainwindow.h"


CbCfMainWindow::CbCfMainWindow( QWidget * parent, 
                                Qt::WindowFlags flags)
    : QMainWindow(parent, flags),
      otthread(NULL)
{
    setupUi(this);
    connect(actionClient_A, SIGNAL (activated()), this, SLOT(readFileA()));
    connect(actionClient_B, SIGNAL (activated()), this, SLOT(readFileB()));

    otthread = new OpentrackerThread(this);

    updateConfigFileEdit("clientA.xml");
    
    otthread->getCallbackModule()->
        setCallback( "clientA", CbCfMainWindow::clientACB, this );
    otthread->getCallbackModule()->
        setCallback( "clientB", CbCfMainWindow::clientBCB, this );
    otthread->getCallbackModule()->
        setGlobalCallback(CbCfMainWindow::globalClientCB, this);
    
    connect(this, SIGNAL(fileNameSignal(const QString&)),
            otthread, SLOT(setConfigurationFile(const QString&)));

    connect(this, SIGNAL(fileNameSignal(const QString&)),
            this, SLOT(updateConfigFileEdit(const QString&)));

    connect(configPushButton, SIGNAL(pressed()),
            this, SLOT(setupConfigFromEdit()));

    connect(eventPushButton, SIGNAL(pressed()),
            this, SLOT(createEventFromEdit()));

    otthread->start();
}

void CbCfMainWindow::readFileA()
{
    emit fileNameSignal("clientA.xml");
}

void CbCfMainWindow::readFileB()
{
    emit fileNameSignal("clientB.xml");
}

void CbCfMainWindow::updateConfigFileEdit(const QString &fileName)
{
    QFile file(fileName);

    if (file.open(QFile::ReadOnly | QFile::Text))
    {
        configEdit->setPlainText(file.readAll());   
    }
}

void CbCfMainWindow::setupConfigFromEdit()
{
    otthread->setConfigurationString(configEdit->toPlainText());
}

void CbCfMainWindow::createEventFromEdit()
{
    using namespace ot;
    Event event;
    event.timeStamp();
    event.getPosition()[0] = 1.0;
    otthread->getCallforwardModule()->callForward("clientAInput", event);
                                                  
}

void CbCfMainWindow::clientACB( ot::CallbackNode & node,  ot::Event & event, void * data )
{
    using namespace ot;
    double diff = (OSUtils::currentTime() - event.time ) / 1000;
    cout << node.getName() << " time diff " << diff << endl;

}

void CbCfMainWindow::clientBCB( ot::CallbackNode & node, 
                                ot::Event & event, void * data )
{
    using namespace ot;
    double diff = (OSUtils::currentTime() - event.time ) / 1000;
    cout << node.getName() << " time diff " << diff << endl;

}

void CbCfMainWindow::globalClientCB( ot::CallbackNode & node,  
                                     ot::Event & event, void * data )
{
    using namespace std;

    cout << "This is the global callback function." << endl;
}

/* 
 * ------------------------------------------------------------
 *   End of cbcfmainwindow.cpp
 * ------------------------------------------------------------
 *   Automatic Emacs configuration follows.
 *   Local Variables:
 *   mode:c++
 *   c-basic-offset: 4
 *   eval: (c-set-offset 'substatement-open 0)
 *   eval: (c-set-offset 'case-label '+)
 *   eval: (c-set-offset 'statement 'c-lineup-runin-statements)
 *   eval: (setq indent-tabs-mode nil)
 *   End:
 * ------------------------------------------------------------ 
 */
