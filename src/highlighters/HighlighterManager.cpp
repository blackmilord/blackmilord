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
#include <QApplication>
#include <QTextBlock>

#include <PlainTextEditor.h>
#include <Gui.h>
#include <event/HighlightBlockEvent.h>
#include <event/HighlightBlockEventResponse.h>
#include <event/HighlightersApplySettingsEvent.h>
#include "AbstractHighlighter.h"
#include "HighlighterHTMLTags.h"
#include "HighlighterSpellcheck.h"
#include "HighlighterThread.h"

HighlighterManager::HighlighterManager() :
    m_inProgress(0),
    m_highlighterThread(new HighlighterThread(this))
{
    m_highlighters.push_back(new HighlighterHTMLTags());
    m_highlighters.push_back(new HighlighterSpellcheck());
    connect(&Preferences::instance(), SIGNAL(settingsChanged()),
            this, SLOT(applySettings()));
    m_highlighterThread->start();
}

HighlighterManager::~HighlighterManager()
{
    m_highlighterThread->quit();
    m_highlighterThread->wait();
    foreach(AbstractHighlighter* highlighter, m_highlighters) {
        delete highlighter;
    }
}

HighlighterManager& HighlighterManager::instance()
{
    static HighlighterManager instance;
    return instance;
}

void HighlighterManager::applySettings()
{
    if (0 == m_inProgress) {
        foreach(AbstractHighlighter* highlighter, m_highlighters) {
            highlighter->applySettings();
        }
        rehighlight();
    }
    else {
        QApplication::postEvent(this, new HighlightersApplySettingsEvent());
    }
}

void HighlighterManager::registerBlockToHighlight(const QTextBlock &block, bool important)
{
    if (important) {
        int firstVisible = Gui::plainTextEditor()->firstVisibleBlock();
        int lastVisible = Gui::plainTextEditor()->lastVisibleBlock();
        int current = block.blockNumber();
        if (current < firstVisible || current > lastVisible) {
            important = false;
        }
    }
    if (!important) {
        BlockData *data = dynamic_cast<BlockData*>(block.userData());
        if (data == NULL){
            data = new BlockData();
            const_cast<QTextBlock&>(block).setUserData(data);
        }
        if(data->highlightingDone()) {
            return;
        }
        data->setHighlightingDone(true);
    }
    int priority = important ? Qt::HighEventPriority : Qt::NormalEventPriority;
    ++m_inProgress;
    QApplication::postEvent(m_highlighterThread->getWorker(),
        new HighlightBlockEvent(block.blockNumber(), block.text()), priority);
}

void HighlighterManager::cancelHighlighting()
{
    qDebug() << "HighlighterManager::cancelHighlighting() is not implemented";

    //TODO: unless we know amount of removed events we cannot fix m_inProgress variable
    //QApplication::removePostedEvents(m_highlighterThread->getWorker(), HighlightBlockEvent::getType());
}

void HighlighterManager::customEvent(QEvent *event)
{
    if (event->type() == HighlightBlockEventResponse::getType()) {
        event->accept();
        HighlightBlockEventResponse *responseEvent =
                dynamic_cast<HighlightBlockEventResponse*>(event);
        highlightBlock(responseEvent->getBlockIndex(), *responseEvent->getResults());
        --m_inProgress;
    }
    else if (event->type() == HighlightersApplySettingsEvent::getType()) {
        event->accept();
        applySettings();
    }
    else {
        event->ignore();
    }
}

void HighlighterManager::rehighlight()
{
    qDebug() << "HighlighterManager::rehighlight() is not implemented";
    //TODO: implement
}

void HighlighterManager::highlightBlock(int blockIndex, const AbstractHighlighter::MultiFormatList &formatting)
{
    //TODO: TextEditor should provide API for highlighting,
    //TODO: Blocking signal should be removed
    QSet<int> steps;
    const QTextBlock &block = Gui::plainTextEditor()->findBlockByNumber(blockIndex);
    bool signalsBlockedEditor = Gui::plainTextEditor()->blockSignals(true);
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
    bool newFormatSet = false;
    while (!stepList.isEmpty()) {
        start = end;
        end = stepList.takeFirst();
        QTextCharFormat newFormat;
        newFormatSet = false;
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
    Gui::plainTextEditor()->blockSignals(signalsBlockedEditor);
}

QVector<AbstractHighlighter*> HighlighterManager::getHighlighters() const
{
    return m_highlighters;
}
