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
#include <AbstractHighlighter.h>

class QLayout;

class PlainTextEditor :
    protected QPlainTextEdit
{
    friend class HighlighterManager;
    Q_OBJECT
public:
    explicit PlainTextEditor(QWidget * parent = 0);
    virtual ~PlainTextEditor();

    inline int firstVisibleBlock() const
    {
        return QPlainTextEdit::firstVisibleBlock().blockNumber();
    }
    inline int lastVisibleBlock() const
    {
        return QPlainTextEdit::cursorForPosition(
            QPoint(viewport()->width(), viewport()->height())).blockNumber();
    }

    void replace(int position, int length, const QString &after);

    inline int getCursorPosition() const
    {
        return textCursor().position();
    }
    void setCursorPosition(int position);
    void setCursorPositionToStart();
    void setCursorPositionToEnd();

    inline int getSelectionStart() const
    {
        return textCursor().selectionStart();
    }
    inline int getSelectionEnd() const
    {
        return textCursor().selectionEnd();
    }
    inline bool hasSelection() const
    {
        return textCursor().hasSelection();
    }
    void setSelection(int selectionStart, int selectionEnd);
    void clearSelection();
    inline QString getSelectedText() const
    {
        return textCursor().selectedText();
    }

    inline QString toPlainText() const
    {
        return QPlainTextEdit::toPlainText();
    }
    void setPlainText(const QString &text);

    inline QTextBlock findBlockByNumber(int blockNumber) const
    {
        return document()->findBlockByNumber(blockNumber);
    }
    inline QTextBlock findBlock(int pos) const
    {
        return document()->findBlock(pos);
    }

    inline int blockCount() const
    {
        return QPlainTextEdit::blockCount();
    }

    void setFocus();

    void setEnabled(bool enabled);
    bool isEnabled() const;

    void setModified(bool modified);
    bool isModified() const;

    inline bool canRedo()
    {
        return document()->isRedoAvailable();
    }
    inline bool canUndo()
    {
        return document()->isUndoAvailable();
    }

    QWidget* asWidget();
    QObject* asObject();

protected:
    void contextMenuEvent(QContextMenuEvent * event);
    void resizeEvent(QResizeEvent *event);

signals:
    void contentsChange(int, int, int);
    void contentsChanged();

public slots:
    void applySettings();
    void redo();
    void undo();
    void clearRedoUndoHistory();

private slots:
    void updateRequestSlot(const QRect &rect, int dy);
    void contentsChangedSlot();
    void contentsChangeSlot(int position, int charsRemoved, int charsAdded);
    void applyHintSlot(QAction *action);
};

class BlockData :
    public QTextBlockUserData
{
public:
    BlockData() :
        m_highlightingDone(false)
    {
    }

    ~BlockData()
    {
    }

    bool highlightingDone()
    {
        return m_highlightingDone;
    }

    void setHighlightingDone(bool highlightingDone)
    {
        m_highlightingDone = highlightingDone;
    }

private:
    bool m_highlightingDone;
};


#endif /* BLACK_MILORD_PLAIN_TEXT_EDITOR_H */