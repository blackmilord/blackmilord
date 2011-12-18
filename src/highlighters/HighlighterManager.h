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

#ifndef BLACK_MILORD_HIGHLIGHTER_MANAGER_H
#define BLACK_MILORD_HIGHLIGHTER_MANAGER_H

#include <QObject>
#include <QSharedPointer>
#include <QList>
#include <QEvent>
#include <QAtomicInt>
#include <QSyntaxHighlighter>
#include "AbstractHighlighter.h"

class QTextBlock;
class PlainTextEditor;
class HighlighterThread;
class HighlightBlockEventResponse;
class QTextDocument;

class HighlighterManager :
    public QSyntaxHighlighter
{
    friend class PlainTextEditor;

    Q_OBJECT

    HighlighterManager(QTextDocument *document);
    virtual ~HighlighterManager();
    static void createInstance(QTextDocument *document);

public:
    static HighlighterManager& instance();
    void registerBlockToHighlight(const QTextBlock &block, bool invalidate);
    void invalidateBlock(const QTextBlock &block);
    void cancelHighlighting();
    void rehighlight();
    QVector<AbstractHighlighter*> getHighlighters() const;

protected:
    void customEvent(QEvent *event);
    void highlightBlock(const QString &text);

private:
    static HighlighterManager *m_instance;
    QAtomicInt m_inProgress;
    QVector<AbstractHighlighter*> m_highlighters;
    HighlighterThread *m_highlighterThread;

    HighlightBlockEventResponse *m_preparedFormatting;

    void applySettings();
    void saveSettings();

    friend class HighlighterPage;
    friend class HighlightBlockEvent;
    friend class HighlightBlockEventResponse;
};

#endif /* BLACK_MILORD_HIGHLIGHTER_MANAGER_H */
