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

#include "event/HighlightEvent.h"
#include "event/HighlightEventResponse.h"
#include "HighlighterManager.h"
#include "AbstractHighlighter.h"

HighlighterThread::HighlighterThread(QObject * parent) :
    QThread(parent)
{
}

HighlighterThread::~HighlighterThread()
{
}

void HighlighterThread::highlightBlock()
{
}

void HighlighterThread::customEvent(QEvent *event)
{
    if (event->type() == HighlightEvent::getType()) {
        event->accept();
        HighlightEvent *highlightEvent = static_cast<HighlightEvent*>(event);
        QVector<AbstractHighlighter*> highlighters =
            HighlighterManagerFactory::instance().getHighlighters();

        HighlightEventResponse *responseEvent =
            new HighlightEventResponse(highlightEvent->getBlockIndex());

        foreach(AbstractHighlighter* highlighter, highlighters) {
            responseEvent->getResults()->push_back(
                highlighter->highlightBlock(highlightEvent->getText()));
        }

        QApplication::postEvent(parent(), responseEvent);
        
    }
    else {
        event->ignore();
    }
}
