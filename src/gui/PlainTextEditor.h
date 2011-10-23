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

#ifndef BLACK_MILORD_PLAIN_TEXT_EDITOR_H
#define BLACK_MILORD_PLAIN_TEXT_EDITOR_H

#include <QPlainTextEdit>
#include <QTextBlockUserData>
#include "PlainTextEditorUndoStack.h"

class PlainTextEditor :
    public QPlainTextEdit
{
    Q_OBJECT
public:
    explicit PlainTextEditor(QWidget * parent = 0);
    virtual ~PlainTextEditor();

    int firstVisibleBlock() const;

protected:
    bool eventFilter(QObject *watched, QEvent *event);
    void contextMenuEvent(QContextMenuEvent * event);

private:
    PlainTextEditorUndoStack m_undoStack;

signals:
    void canUndo(bool);
    void canRedo(bool);

public slots:
    void redo();
    void undo();
    void clearRedoUndoHistory();

private slots:
    void contentsChanged();
    void contentsChange(int position, int charsRemoved, int charsAdded);
    void applyHint(QAction *action);
    void canUndoSlot(bool value);
    void canRedoSlot(bool value);
};

class BlockData :
    public QTextBlockUserData
{
public:
    BlockData() :
        m_needRehighlight(false)
    {
    }

    ~BlockData()
    {
    }

    bool needRehighlight()
    {
        return m_needRehighlight;
    }

    void setNeedRehighlight(bool needRehighlight)
    {
        m_needRehighlight = needRehighlight;
    }

private:
    bool m_needRehighlight;
};


#endif /* BLACK_MILORD_PLAIN_TEXT_EDITOR_H */