/************************************************************************
 *                                                                      *
 * Author: Lukasz Marek <lukasz.m.luki@gmail.com>                       *
 *                                                                      *
 * This file is part of BlackMilord.                                    *
 *                                                                      *
 * BlackMilord is free software: you can redistribute it and/or modify  *
 * it under the terms of the GNU General Public License as published by *
 * the Free Software Foundation, either version 3 of the License, or    *
 * (at your option) any later version.                                  *
 *                                                                      *
 * BlackMilord is distributed in the hope that it will be useful,       *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of       *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the         *
 * GNU General Public License for more details.                         *
 *                                                                      *
 * You should have received a copy of the GNU General Public License    *
 * along with BlackMilord. If not, see http://www.gnu.org/licenses/     *
 *                                                                      *
 ************************************************************************/

#include "HighlighterThread.h"
#include <QDebug>
#include <QEvent>
#include <QApplication>
#include <QtCore/qmetatype.h>

#include <event/HighlightBlockEvent.h>
#include <event/HighlightBlockEventResponse.h>
#include "HighlighterManager.h"
#include "AbstractHighlighter.h"

HighlighterThread::HighlighterThread(QObject * parent) :
    QThread(parent),
    m_guard(0),
    m_worker(NULL)
{
}

HighlighterThread::~HighlighterThread()
{
    delete m_worker;
}

void HighlighterThread::run()
{
    m_worker = new HighlighterWorker(NULL);
    m_guard = 1;
    exec();
}

HighlighterWorker* HighlighterThread::getWorker()
{
    while(m_guard != 1) {
        msleep(1);
    }
    Q_ASSERT(m_worker != NULL);
    return m_worker;
}

HighlighterWorker::HighlighterWorker(QObject *parent) :
    QObject(parent)
{

}

HighlighterWorker::~HighlighterWorker()
{
}


void HighlighterWorker::customEvent(QEvent *event)
{
    if (event->type() == HighlightBlockEvent::getType()) {
        event->accept();
        HighlightBlockEvent *highlightEvent = dynamic_cast<HighlightBlockEvent*>(event);
        QVector<AbstractHighlighter*> highlighters =
            HighlighterManager::instance().getHighlighters();

        HighlightBlockEventResponse *responseEvent =
            new HighlightBlockEventResponse(highlightEvent->getBlockIndex());

        foreach(AbstractHighlighter* highlighter, highlighters) {
            if (highlighter->isEnabled()) {
                responseEvent->getResults()->push_back(
                    highlighter->highlightBlock(highlightEvent->getText()));
            }
        }

        QApplication::postEvent(&HighlighterManager::instance(), responseEvent);

    }
    else {
        event->ignore();
    }
}
