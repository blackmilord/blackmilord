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

#ifndef OPEN_EBOOK_EDITOR_HIGHLIGHTER_MANAGER_H
#define OPEN_EBOOK_EDITOR_HIGHLIGHTER_MANAGER_H

#include <QObject>
#include <QSharedPointer>
#include <QList>
#include <QEvent>
#include "AbstractHighlighter.h"

class QPlainTextEdit;
class HighlighterThread;

class HighlighterManager :
    public QObject
{
    Q_OBJECT

    explicit HighlighterManager(QPlainTextEdit *editor);
    virtual ~HighlighterManager();
public:
    void registerBlockHighlight(int start, int end, bool important);
    void startBlockHighlight();
    QVector<AbstractHighlighter*> getHighlighters() const { return m_highlighters; }

protected:
    void highlightBlockPrivate(int blockIndex, const AbstractHighlighter::MultiFormatList &formatting);
    void customEvent(QEvent *event);

private:
    QList<int> m_queue;
    bool m_inProgress;
    QVector<AbstractHighlighter*> m_highlighters;
    QPlainTextEdit *m_editor;
    HighlighterThread *m_highlighterThread;

private slots:
    void applySettings();

private:
    friend class HighlighterManagerFactory;
};


class HighlighterManagerFactory
{
public:
    static HighlighterManager& instance();
    static void createInstance(QPlainTextEdit *editor);
private:
    static HighlighterManager* m_instance;
};

#endif
