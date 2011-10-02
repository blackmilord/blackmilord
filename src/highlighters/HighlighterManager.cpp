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

#include "HighlighterManager.h"
#include <QDebug>
#include <QPlainTextEdit>
#include <QApplication>
#include <QTextBlock>

#include <Book.h>
#include "AbstractHighlighter.h"
#include "HighlighterHTMLTags.h"
#include "HighlighterSpellcheck.h"
#include "HighlightEvent.h"
#include "HighlighterThread.h"

HighlighterManager::HighlighterManager(QPlainTextEdit *editor) :
    m_editor(editor),
    m_highlighterThread(new HighlighterThread())
{
    m_highlighterThread->start();
    m_highlighters.push_back(new HighlighterHTMLTags());
    m_highlighters.push_back(new HighlighterSpellcheck());
    connect(&Preferences::instance(), SIGNAL(settingsChanged()),
            this, SLOT(applySettings()));
}

HighlighterManager::~HighlighterManager()
{
    foreach(AbstractHighlighter* highlighter, m_highlighters) {
        delete highlighter;
    }
}

void HighlighterManager::applySettings()
{
    foreach(AbstractHighlighter* highlighter, m_highlighters) {
        highlighter->applySettings();
        
    }
    if (Book::instance().isFileOpened()) {
        //TODO: rehighlight();
    }
}

void HighlighterManager::highlightBlock(int blockIndex)
{
    QApplication::postEvent(m_highlighterThread, new HighlightEvent(
        blockIndex, m_editor->document()->findBlockByNumber(blockIndex).text()));
}

void HighlighterManager::customEvent(QEvent *event)
{
    event->ignore();
    return;
    /*
    if (event->type() == m_highlighEventType) {
        event->accept();
        int blockIndex = static_cast<HighlightEvent*>(event)->getBlockIndex();
        QTextBlock block = document()->findBlockByNumber(blockIndex);
        block.setUserState(0);
        qDebug() << "event for " << blockIndex;
    }
    else {
        event->ignore();
    }
    */
}

void HighlighterManager::highlightBlockPrivate(int /*blockIndex*/)
{
    return;
/*
    QVector<AbstractHighlighter::FormatList> results;
    foreach(AbstractHighlighter* highlighter, m_highlighters) {
        results.push_back(highlighter->highlightBlock(text));
    }

    QSet<int> steps;
    steps << 0 << text.length();
    foreach(const AbstractHighlighter::FormatList &formats, results) {
        foreach(const AbstractHighlighter::CharFormat &format, formats) {
            steps << format.m_start << format.m_start + format.m_count;            
        }
    }
    QList<int> stepList = steps.toList();
    qSort(stepList);

    int start = 0;
    int end = stepList.takeFirst();
    while (!stepList.isEmpty()) {
        start = end;
        end = stepList.takeFirst();
        QTextCharFormat newFormat;
        bool newFormatSet = false;
        foreach(const AbstractHighlighter::FormatList &formats, results) {
            foreach(const AbstractHighlighter::CharFormat &format, formats) {
                if (format.m_start <= start && format.m_start + format.m_count >= end) {
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
        setFormat(start, end, newFormat);
    }
*/
}

HighlighterManager* HighlighterManagerFactory::m_instance = NULL;

HighlighterManager& HighlighterManagerFactory::instance()
{
    Q_ASSERT(m_instance != NULL);
    return *m_instance;
}

void HighlighterManagerFactory::createInstance(QPlainTextEdit *editor)
{
    Q_ASSERT(m_instance == NULL);
    m_instance = new HighlighterManager(editor);
}
