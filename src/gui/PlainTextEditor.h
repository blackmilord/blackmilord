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

class QLayout;

class PlainTextEditor :
    protected QPlainTextEdit
{
    Q_OBJECT

    explicit PlainTextEditor(QWidget * parent = 0);
    virtual ~PlainTextEditor();

public:
    static PlainTextEditor& instance();

    int firstVisibleBlock() const;

    void replace(int position, int length, const QString &after);
    int getCursorPosition() const;
    void setCursorPosition(int position);
    void setCursorPositionToStart();
    void setCursorPositionToEnd();
    int getSelectionStart() const;
    int getSelectionEnd() const;
    bool hasSelection() const;
    void setSelection(int selectionStart, int selectionEnd);
    void clearSelection();

    QString toPlainText() const;
    void setPlainText(const QString &text);

    QTextBlock findBlockByNumber(int blockNumber) const;
    QTextBlock findBlock(int pos) const;
    void setTextCursor(const QTextCursor &cursor);
    QTextCursor textCursor() const;
    QTextCursor textCursorForBlock(int blockNumber = 0) const;
    int blockCount() const;
    bool blockSignals(bool block);
    void setFocus();

    void setEnabled(bool enabled);
    bool isEnabled() const;

    void setModified(bool modified);
    bool isModified() const;

    void addToLayout(QLayout *layout);
    void connect(const char *signal, const QObject *receiver,
            const char *method, Qt::ConnectionType type = Qt::AutoConnection);
    void connect(const QObject *sender, const char *signal,
            const char *method, Qt::ConnectionType type = Qt::AutoConnection);
    void disconnect(const char *signal, const QObject *receiver, const char *method);
    void disconnect(const QObject *sender, const char *signal, const char *method);

protected:
    bool eventFilter(QObject *watched, QEvent *event);
    void contextMenuEvent(QContextMenuEvent * event);

private:
    PlainTextEditorUndoStack m_undoStack;

signals:
    void canUndo(bool);
    void canRedo(bool);
    void contentsChange(int, int, int);
    void contentsChanged();

public slots:
    void applySettings();
    void redo();
    void undo();
    void clearRedoUndoHistory();

private slots:
    void contentsChangedSlot();
    void contentsChangeSlot(int position, int charsRemoved, int charsAdded);
    void applyHintSlot(QAction *action);
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