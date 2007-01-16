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
    connect(actionLocal_TestSource, SIGNAL (activated()), 
            this, SLOT(readFileTest()));

    qmutex = new QMutex();
    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(updateEventsEdit()));

    otthread = new OpentrackerThread(this);

    registerCallbacks();

    updateConfigFileEdit("clientLocal.xml");
     
    connect(this, SIGNAL(fileNameSignal(const QString&)),
            otthread, SLOT(setConfigurationFile(const QString&)));

    connect(this, SIGNAL(fileNameSignal(const QString&)),
            this, SLOT(updateConfigFileEdit(const QString&)));

    connect(configPushButton, SIGNAL(pressed()),
            this, SLOT(setupConfigFromEdit()));

    connect(eventPushButton, SIGNAL(pressed()),
            this, SLOT(createEventFromEdit()));

    timer->start();

    otthread->start();
}

CbCfMainWindow::~CbCfMainWindow()
{
    timer->stop();

    delete timer;
    delete qmutex;

    usleep(2000);
}

void CbCfMainWindow::readFileA()
{
    emit fileNameSignal("clientA.xml");
}

void CbCfMainWindow::readFileB()
{
    emit fileNameSignal("clientB.xml");
}

void CbCfMainWindow::readFileTest()
{
    emit fileNameSignal("clientLocal.xml");
}

void CbCfMainWindow::registerCallbacks()
{
    otthread->getCallbackModule()->
        setCallback( "clientAInput", CbCfMainWindow::clientACB, this );
    otthread->getCallbackModule()->
        setCallback( "clientBInput", CbCfMainWindow::clientBCB, this );
    otthread->getCallbackModule()->
        setCallback( "clientTestInput", CbCfMainWindow::clientTestCB, this );
    otthread->getCallbackModule()->
        setGlobalCallback(CbCfMainWindow::globalClientCB, this);
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
    if (!otthread->getCallforwardModule()->callForward("clientAInput", event))
    {
        outEventsEdit->append("callForward to 'clientAInput' failed");
    }
    else
    {
        outEventsEdit->append("callForward to 'clientAInput' succeeded");
    }

    if (!otthread->getCallforwardModule()->callForward("clientBInput", event))
    {
        outEventsEdit->append("callForward to 'clientBInput' failed");
    }
    else
    {
        outEventsEdit->append("callForward to 'clientBInput' succeeded");
    }


    if (!otthread->getCallforwardModule()->callForward("clientLocalInput", event))
    {
        outEventsEdit->append("callForward to 'clientLocalInput' failed");
    }
    else
    {
        outEventsEdit->append("callForward to 'clientLocalInput' succeeded");
    }
                                                  
}

void CbCfMainWindow::updateEventsEdit()
{
    qmutex->lock();
    while (not messagequeue.empty())
    {
        eventsEdit->append(messagequeue.dequeue());
    }
    qmutex->unlock();
}

void CbCfMainWindow::clientACB( ot::CallbackNode & node,  ot::Event & event, void * data )
{
    using namespace ot;

    CbCfMainWindow *self = (CbCfMainWindow*)(data);

    double diff = (OSUtils::currentTime() - event.time ) / 1000;

    self->qmutex->lock();

    self->messagequeue.append("CbCfMainWindow::clientACB");
    self->messagequeue.append(event.getPrintOut().c_str());
    //cout << node.getName() << " time diff " << diff << endl;
    //self->textCursor.movePosition(QTextCursor::End);

    self->qmutex->unlock();
}

void CbCfMainWindow::clientBCB( ot::CallbackNode & node, 
                                ot::Event & event, void * data )
{
    using namespace ot;

    CbCfMainWindow *self = (CbCfMainWindow*)(data);

    double diff = (OSUtils::currentTime() - event.time ) / 1000;

    self->qmutex->lock();

    self->messagequeue.append("CbCfMainWindow::clientBCB");
    self->messagequeue.append(event.getPrintOut().c_str());
    //cout << node.getName() << " time diff " << diff << endl;
    //self->textCursor.movePosition(QTextCursor::End);

    self->qmutex->unlock();
}

void CbCfMainWindow::clientTestCB( ot::CallbackNode & node, 
                                ot::Event & event, void * data )
{
    using namespace ot;

    CbCfMainWindow *self = (CbCfMainWindow*)(data);

    double diff = (OSUtils::currentTime() - event.time ) / 1000;

    self->qmutex->lock();
   
    self->messagequeue.append("CbCfMainWindow::clientTestCB");
    self->messagequeue.append(event.getPrintOut().c_str());
    //cout << node.getName() << " time diff " << diff << endl;
    //QTextCursor cursor = self->eventsEdit->textCursor();
    //cursor.movePosition(QTextCursor::End);
    //self->eventsEdit->setTextCursor(self->textCursor);

    self->qmutex->unlock();
}

void CbCfMainWindow::globalClientCB( ot::CallbackNode & node,  
                                     ot::Event & event, void * data )
{
    using namespace std;

    CbCfMainWindow *self = (CbCfMainWindow*)(data);

    self->qmutex->lock();

    self->messagequeue.append("CbCfMainWindow::globalClientCB");
    //cout << "This is the global callback function." << endl;

    self->qmutex->unlock();
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
