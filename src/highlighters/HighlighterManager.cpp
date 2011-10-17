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

#include "AbstractHighlighter.h"
#include "HighlighterHTMLTags.h"
#include "HighlighterSpellcheck.h"
#include "HighlighterThread.h"
#include "event/HighlightEvent.h"
#include "event/HighlightEventResponse.h"

HighlighterManager::HighlighterManager(QPlainTextEdit *editor) :
    m_inProgress(false),
    m_editor(editor),
    m_highlighterThread(new HighlighterThread(this))
{
    m_highlighterThread->start();
    m_highlighters.push_back(new HighlighterHTMLTags());
    m_highlighters.push_back(new HighlighterSpellcheck());
    connect(&Preferences::instance(), SIGNAL(settingsChanged()),
            this, SLOT(applySettings()));
}

HighlighterManager::~HighlighterManager()
{
    m_highlighterThread->quit();
    m_highlighterThread->wait();
    foreach(AbstractHighlighter* highlighter, m_highlighters) {
        delete highlighter;
    }
}

void HighlighterManager::applySettings()
{
    foreach(AbstractHighlighter* highlighter, m_highlighters) {
        highlighter->applySettings();
    }
    rehighlight();
}

void HighlighterManager::startBlockHighlight()
{
    if (m_queue.isEmpty() || m_inProgress) {
        return;
    }
    m_inProgress = true;
    int blockIndex = m_queue.takeFirst();
    QApplication::postEvent(m_highlighterThread, new HighlightEvent(
        blockIndex, m_editor->document()->findBlockByNumber(blockIndex).text()));
}

void HighlighterManager::registerBlockHighlight(int start, int end, bool important)
{
    if (m_editor->toPlainText().length() < end) {
        end = m_editor->toPlainText().length();
    }
    const QTextBlock &firstBlock = m_editor->document()->findBlock(start);
    const QTextBlock &lastBlock = m_editor->document()->findBlock(end);
    qDebug() << start << end << m_editor->toPlainText().length() << firstBlock.blockNumber() << lastBlock.blockNumber();
    for (int i = firstBlock.blockNumber(); i <= lastBlock.blockNumber(); ++i) {
        if (important) {
            m_queue.push_back(i);
        }
        else {
            m_queue.push_back(i);
        }
    }
    startBlockHighlight();
}

void HighlighterManager::cancelBlockHighlight()
{
    m_queue.clear();
}

void HighlighterManager::customEvent(QEvent *event)
{
    if (event->type() == HighlightEventResponse::getType()) {
        event->accept();
        HighlightEventResponse *responseEvent =
                static_cast<HighlightEventResponse*>(event);
        highlightBlock(responseEvent->getBlockIndex(), *responseEvent->getResults());
        m_inProgress = false;
        startBlockHighlight();
    }
    else {
        event->ignore();
    }
}

void HighlighterManager::rehighlight()
{
    //TODO: implement
}

void HighlighterManager::highlightBlock(int blockIndex, const AbstractHighlighter::MultiFormatList &formatting)
{
    QSet<int> steps;
    const QTextBlock &block = m_editor->document()->findBlockByNumber(blockIndex);
    bool signalsBlockedEditor = m_editor->blockSignals(true);
    bool signalsBlockedDocument = m_editor->document()->blockSignals(true);
    steps << 0 << block.text().length();
    foreach(const AbstractHighlighter::FormatList &formats, formatting) {
        foreach(const AbstractHighlighter::CharFormat &format, formats) {
            steps << format.m_start << format.m_start + format.m_count;
        }
    }
    QList<int> stepList = steps.toList();
    qSort(stepList);

    QTextCursor cursor(block);
    int cursorPositionOffset = cursor.position();
    int start = 0;
    int end = stepList.takeFirst();
    while (!stepList.isEmpty()) {
        start = end;
        end = stepList.takeFirst();
        QTextCharFormat newFormat;
        bool newFormatSet = false;
        foreach(const AbstractHighlighter::FormatList &formats, formatting) {
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
        cursor.setPosition(cursorPositionOffset + start);
        cursor.setPosition(cursorPositionOffset + end, QTextCursor::KeepAnchor);
        cursor.setCharFormat(newFormat);
    }
    QTextCharFormat defaultFormat;
    defaultFormat.setFont(Preferences::instance().getDefaultFontEditor());
    cursor.setCharFormat(defaultFormat);
    m_editor->blockSignals(signalsBlockedEditor);
    m_editor->document()->blockSignals(signalsBlockedDocument);
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
