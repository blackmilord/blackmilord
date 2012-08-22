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

#include <PluginHighlighter.h>
#include "HighlightBlockEvent.h"
#include "HighlightBlockEventResponse.h"
#include "HighlighterManager.h"

HighlighterThread::HighlighterThread(QObject * parent) :
    QThread(parent),
    m_guard(0),
    m_worker(NULL)
{
}

HighlighterThread::~HighlighterThread()
{
    m_guard = 0;
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
        //get request event
        HighlightBlockEvent *highlightEvent = dynamic_cast<HighlightBlockEvent*>(event);
        const QString &text = highlightEvent->getText();

        //create response event
        HighlightBlockEventResponse *responseEvent =
            new HighlightBlockEventResponse(highlightEvent->getBlockIndex(), text.length());
        PluginHighlighter::FormatListPtr result = responseEvent->getResults();

        //get highlighters
        QVector<PluginHighlighter*> highlighters =
            HighlighterManager::instance().getHighlighters();

        //call all highlighters
        QVector<PluginHighlighter::FormatListPtr> results;
        foreach(PluginHighlighter* highlighter, highlighters) {
            if (highlighter->isEnabled()) {
                results.push_back(highlighter->highlightBlock(text));
            }
        }

        //merge results to single list

        //calculate all steps
        QSet<int> steps;
        steps << 0 << text.length();
        foreach(const PluginHighlighter::FormatListPtr &formats, results) {
            foreach(const PluginHighlighter::CharFormat &format, *formats.data()) {
                steps << format.m_start << format.m_end;
            }
        }
        QList<int> stepList = steps.toList();
        qSort(stepList);

        //store formating for each range when not default
        int start = 0;
        int end = stepList.takeFirst();
        bool newFormatSet = false;
        QTextCharFormat lastStoredFormat;
        int lastStoredEnd = -1;
        while (!stepList.isEmpty()) {
            start = end;
            end = stepList.takeFirst();
            QTextCharFormat newFormat;
            newFormatSet = false;
            foreach(const PluginHighlighter::FormatListPtr &formats, results) {
                foreach(const PluginHighlighter::CharFormat &format, *formats.data()) {
                    if (format.m_start <= start && format.m_end >= end) {
                        if (!newFormatSet) {
                            newFormat = format.m_format;
                            newFormatSet = true;
                        }
                        else {
                            QMap<int, QVariant> props = format.m_format.properties();
                            QMap<int, QVariant>::const_iterator prop = props.begin();
                            for(; prop != props.end(); ++prop) {
                                newFormat.setProperty(prop.key(), prop.value());
                            }
                        }
                    }
                }
            }
            if (newFormatSet) {
                if (newFormat == lastStoredFormat && lastStoredEnd == start) {
                    //resize last block for optimalization
                    result->last().m_end = end;
                    lastStoredEnd = end;
                }
                else {
                    lastStoredEnd = end;
                    lastStoredFormat = newFormat;
                    result->push_back(PluginHighlighter::CharFormat(start, end, newFormat));
                }
            }
        }
        QApplication::postEvent(&HighlighterManager::instance(), responseEvent);
    }
    else {
        event->ignore();
    }
}
