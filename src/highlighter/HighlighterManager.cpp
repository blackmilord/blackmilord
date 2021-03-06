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
#include <QDir>
#include <QPluginLoader>

#include <PlainTextEditor.h>
#include <BlockData.h>
#include <Gui.h>
#include <qvector.h>
#include "HighlightBlockEvent.h"
#include "HighlightBlockEventResponse.h"
#include "HighlightersApplySettingsEvent.h"
#include "HighlighterThread.h"

HighlighterManager* HighlighterManager::m_instance = NULL;

HighlighterManager::HighlighterManager(QTextDocument *document) :
    QSyntaxHighlighter(document),
    m_inProgress(0),
    m_highlighterThread(new HighlighterThread(this)),
    m_preparedFormatting(NULL)
{
    m_highlighterThread->start();

    QDir pluginsDir(qApp->applicationDirPath());
    PluginHighlighter *highlighter;
    foreach(QString fileName, pluginsDir.entryList(QStringList("libhighlighter_*"), QDir::Files))
    {
        QPluginLoader pluginLoader(pluginsDir.absoluteFilePath(fileName));
        QObject *plugin = pluginLoader.instance();
        if (plugin)
        {
            highlighter = qobject_cast<PluginHighlighter*>(plugin);
            if (highlighter)
            {
                m_highlighters.push_back(highlighter);
                highlighter->applySettings();
                qDebug() << "Loading highlighter from " << pluginsDir.absolutePath() << fileName << "OK";
            }
            else
            {
                qDebug() << "Loading highlighter from " << pluginsDir.absolutePath() << fileName << "FAILED";
            }
        }
        else
        {
            qDebug() << "Loading highlighter from " << pluginsDir.absolutePath() << fileName << "FAILED";
        }
    }
}

HighlighterManager::~HighlighterManager()
{
    cancelHighlighting();
    m_highlighterThread->quit();
    m_highlighterThread->wait();
}

void HighlighterManager::createInstance(QTextDocument *document)
{
    delete m_instance;
    m_instance = new HighlighterManager(document);
}

HighlighterManager& HighlighterManager::instance()
{
    Q_ASSERT(NULL != m_instance);
    return *m_instance;
}

void HighlighterManager::applySettings()
{
    if (0 == m_inProgress) {
        foreach(PluginHighlighter* highlighter, m_highlighters) {
            highlighter->applySettings();
        }
        rehighlight();
    }
    else {
        cancelHighlighting();
        QApplication::postEvent(this, new HighlightersApplySettingsEvent());
    }
}

void HighlighterManager::saveSettings()
{
    foreach(PluginHighlighter* highlighter, m_highlighters) {
        highlighter->saveSettings();
    }
}

void HighlighterManager::registerBlockToHighlight(const QTextBlock &block, bool invalidate)
{
    int blockNumber = block.blockNumber();
    if (blockNumber < Gui::plainTextEditor()->firstVisibleBlock() ||
        blockNumber > Gui::plainTextEditor()->lastVisibleBlock())
    {
        invalidateBlock(block);
        return;
    }
    BlockData *data = dynamic_cast<BlockData*>(block.userData());
    if (NULL == data){
        data = new BlockData();
        const_cast<QTextBlock&>(block).setUserData(data);
    }
    else if (data->highlightingDone() && !invalidate) {
        return;
    }
    data->setHighlightingDone(true);
    QApplication::postEvent(m_highlighterThread->getWorker(),
        new HighlightBlockEvent(block.blockNumber(), block.text()));
}

void HighlighterManager::invalidateBlock(const QTextBlock &block)
{
    BlockData *data = dynamic_cast<BlockData*>(block.userData());
    if (data == NULL){
        //No need to do anything
        //Block is invalid by default
        return;
    }
    data->setHighlightingDone(false);
}

void HighlighterManager::cancelHighlighting()
{
    QApplication::removePostedEvents(m_highlighterThread->getWorker(), HighlightBlockEvent::getType());
}

void HighlighterManager::customEvent(QEvent *event)
{
    if (event->type() == HighlightBlockEventResponse::getType()) {
        event->accept();
        Q_ASSERT(m_preparedFormatting == NULL);
        m_preparedFormatting = dynamic_cast<HighlightBlockEventResponse*>(event);
        QSyntaxHighlighter::rehighlightBlock(
            Gui::plainTextEditor()->findBlockByNumber(m_preparedFormatting->getBlockIndex()));
        m_preparedFormatting = NULL;
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
    cancelHighlighting();
    int blockCount = Gui::plainTextEditor()->blockCount();
    int firstVisible = Gui::plainTextEditor()->firstVisibleBlock();
    int lastVisible = Gui::plainTextEditor()->lastVisibleBlock();

    for (int i = 0; i < firstVisible; ++i) {
        const QTextBlock &block = Gui::plainTextEditor()->findBlockByNumber(i);
        invalidateBlock(block);
    }
    for (int i = lastVisible + 1; i < blockCount; ++i) {
        const QTextBlock &block = Gui::plainTextEditor()->findBlockByNumber(i);
        invalidateBlock(block);
    }

    for (int i = firstVisible; i <= lastVisible; ++i) {
        registerBlockToHighlight(Gui::plainTextEditor()->findBlockByNumber(i), true);
    }
}

QVector<PluginHighlighter*> HighlighterManager::getHighlighters() const
{
    return m_highlighters;
}

void HighlighterManager::highlightBlock(const QString &text)
{
    if (m_preparedFormatting == NULL ||
        m_preparedFormatting->getBlockLength() != text.length() ||
        m_preparedFormatting->getBlockIndex() != QSyntaxHighlighter::currentBlock().blockNumber())
    {
        registerBlockToHighlight(QSyntaxHighlighter::currentBlock(), true);
        return;
    }
    //apply formatting
    foreach(const PluginHighlighter::CharFormat &format, *m_preparedFormatting->getResults().data()) {
        setFormat(format.m_start, format.m_end - format.m_start, format.m_format);
    }
}