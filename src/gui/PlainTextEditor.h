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

    //Cursor
    int getCursorPosition() const;
    void setCursorPosition(int position);
    void setCursorPositionToStart();
    void setCursorPositionToEnd();

    //Selection
    int getSelectionStart() const;
    int getSelectionEnd() const;
    bool hasSelection() const;
    void setSelection(int selectionStart, int selectionEnd);
    void clearSelection();
    QString getSelectedText() const;

    //Text handling
    QString toPlainText() const;
    void setPlainText(const QString &text);
    void replace(int position, int length, const QString &after);

    //Blocks handling
    int firstVisibleBlock() const;
    int lastVisibleBlock() const;
    QTextBlock findBlockByNumber(int blockNumber) const;
    int blockCount() const;

    //Utils
    void setFocus();

    void setEnabled(bool enabled);
    bool isEnabled() const;

    void setModified(bool modified);
    bool isModified() const;

    bool canRedo() const;
    bool canUndo() const;

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