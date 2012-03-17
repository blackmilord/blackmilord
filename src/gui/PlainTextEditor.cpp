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

#include "PlainTextEditor.h"
#include <QtGlobal>
#include <QDebug>
#include <QMenu>
#include <QLayout>
#include <QApplication>
#include <QTextCursor>
#include <QTextDocument>

#include "Gui.h"
#include "StatusBar.h"
#include <AspellWrapper.h>
#include <HighlighterManager.h>
#include <Book.h>

PlainTextEditor::PlainTextEditor(QWidget * parent) :
    QPlainTextEdit(parent)
{
    connect(&Preferences::instance(), SIGNAL(settingsChanged()), this, SLOT(applySettings()));
    connect(document(), SIGNAL(contentsChange(int, int, int)), this, SLOT(contentsChangeSlot(int, int, int)));
    connect(document(), SIGNAL(contentsChanged()), this, SLOT(contentsChangedSlot()));
    connect(this, SIGNAL(updateRequest(const QRect &, int)), this, SLOT(updateRequestSlot(const QRect &, int)));
    setUndoRedoEnabled(true);
    applySettings();
    HighlighterManager::createInstance(document());
}

PlainTextEditor::~PlainTextEditor()
{
}

void PlainTextEditor::contextMenuEvent(QContextMenuEvent * event)
{
    QMenu *menu = new QMenu();

    QAction *action = menu->addAction(tr("Undo"), this, SLOT(undo()), QKeySequence::Undo);
    action->setEnabled(canUndo());
    action = menu->addAction(tr("Redo"), this, SLOT(redo()), QKeySequence::Redo);
    action->setEnabled(canRedo());

    menu->addSeparator();
    action = menu->addAction(tr("Copy"), this, SLOT(copy()), QKeySequence::Copy);
    action->setEnabled(hasSelection());
    action = menu->addAction(tr("Cut"), this, SLOT(cut()), QKeySequence::Cut);
    action->setEnabled(hasSelection());
    action = menu->addAction(tr("Paste"), this, SLOT(paste()), QKeySequence::Paste);
    action->setEnabled(canPaste());

    menu->addSeparator();
    menu->addAction(tr("Select All"), this, SLOT(selectAll()), QKeySequence::SelectAll);

    QTextCursor cursor = textCursor();
    if (!cursor.hasSelection()) {
        if (ASpellWrapper::instance().isLoaded()) {
            cursor = cursorForPosition(event->pos());
            setTextCursor(cursor);
            cursor.select(QTextCursor::WordUnderCursor);
            if (!ASpellWrapper::instance().checkWord(cursor.selectedText())) {
                const QStringList &hints = ASpellWrapper::instance().hints(cursor.selectedText());
                if (hints.size() > 0) {
                    connect(menu, SIGNAL(triggered(QAction*)), SLOT(applyHintSlot(QAction *)));
                    menu->addSeparator();
                    int size = hints.size() < 10 ? hints.size() : 10;
                    for (int i = 0; i < size; ++i) {
                        QAction *action = menu->addAction(hints[i]);
                        action->setData(true);
                    }
                }
            }
        }
    }
    menu->exec(event->globalPos());
    delete menu;
}

void PlainTextEditor::resizeEvent(QResizeEvent *event)
{
    QPlainTextEdit::resizeEvent(event);
    int firstVisibleBlockNumber = firstVisibleBlock();
    int lastVisibleBlockNumber = lastVisibleBlock();
    for (int blockNumber = firstVisibleBlockNumber; blockNumber <= lastVisibleBlockNumber; ++blockNumber ) {
        HighlighterManager::instance().registerBlockToHighlight(findBlockByNumber(blockNumber), false);
    }
}

void PlainTextEditor::updateRequestSlot(const QRect &rect, int dy)
{
    Q_UNUSED(rect);
    if (dy != 0) {
        int firstVisibleBlockNumber = firstVisibleBlock();
        int lastVisibleBlockNumber = lastVisibleBlock();
        for (int blockNumber = firstVisibleBlockNumber; blockNumber <= lastVisibleBlockNumber; ++blockNumber ) {
            HighlighterManager::instance().registerBlockToHighlight(findBlockByNumber(blockNumber), false);
        }
    }
}

void PlainTextEditor::applySettings()
{
    setFont(Preferences::instance().getEditorFont());
}

void PlainTextEditor::redo()
{
    QPlainTextEdit::redo();
}

void PlainTextEditor::undo()
{
    QPlainTextEdit::undo();
}

void PlainTextEditor::clearRedoUndoHistory()
{
#if (QT_VERSION < QT_VERSION_CHECK(4, 7, 0))
    //At this moment function is called only after closing and openning document.
    //It may require to store more information like cursor position.
    QPlainTextEdit::setPlainText(QPlainTextEdit::toPlainText());
#else
    document()->clearUndoRedoStacks();
#endif
}

void PlainTextEditor::contentsChangedSlot()
{
    Gui::statusBar()->setStatusBarDocLength(QString::number(toPlainText().size()));
    emit contentsChanged();
}

void PlainTextEditor::contentsChangeSlot(int position, int charsRemoved, int charsAdded)
{
    emit contentsChange(position, charsRemoved, charsAdded);
}

void PlainTextEditor::applyHintSlot(QAction *action)
{
    if (!action->data().isNull()) {
        QTextCursor cursor = textCursor();
        cursor.select(QTextCursor::WordUnderCursor);
        cursor.insertText(action->text());
    }
}

void PlainTextEditor::replace(int position, int length, const QString &after)
{
    Q_ASSERT(position >= 0);

    QTextCursor cursor = textCursor();
    cursor.setPosition(position);
    cursor.setPosition(position + length, QTextCursor::KeepAnchor);
    cursor.insertText(after);
}

void PlainTextEditor::setCursorPosition(int position)
{
    if (getCursorPosition() != position) {
        QTextCursor cursor = textCursor();
        cursor.setPosition(position);
        setTextCursor(cursor);
    }
}

void PlainTextEditor::setCursorPositionToStart()
{
    QTextCursor cursor = textCursor();
    cursor.movePosition(QTextCursor::Start);
    setTextCursor(cursor);
}

void PlainTextEditor::setCursorPositionToEnd()
{
    QTextCursor cursor = textCursor();
    cursor.movePosition(QTextCursor::End);
    setTextCursor(cursor);
}

void PlainTextEditor::setSelection(int selectionStart, int selectionEnd)
{
    QTextCursor cursor = textCursor();
    int oldSelectionStart = cursor.selectionStart();
    int oldSelectionEnd = cursor.selectionEnd();
    Q_ASSERT(selectionStart >= 0 && selectionEnd >= 0);
    if (oldSelectionStart != selectionStart || oldSelectionEnd != selectionEnd) {
        cursor.setPosition(selectionStart);
        cursor.setPosition(selectionEnd, QTextCursor::KeepAnchor);
        setTextCursor(cursor);
    }
}

void PlainTextEditor::clearSelection()
{
    QTextCursor cursor = textCursor();
    cursor.clearSelection();
    setTextCursor(cursor);
}

void PlainTextEditor::setPlainText(const QString &text)
{
    QTextCursor cursor = textCursor();
    cursor.select(QTextCursor::Document);
    cursor.insertText(text);
}

void PlainTextEditor::setFocus()
{
    QWidget::setFocus();
}

void PlainTextEditor::setEnabled(bool enabled)
{
    QWidget::setEnabled(enabled);
}

bool PlainTextEditor::isEnabled() const
{
    return QWidget::isEnabled();
}

void PlainTextEditor::setModified(bool modified)
{
    document()->setModified(modified);
}

bool PlainTextEditor::isModified() const
{
    return document()->isModified();
}

QWidget* PlainTextEditor::asWidget()
{
    return static_cast<QWidget*>(this);
}

QObject* PlainTextEditor::asObject()
{
    return static_cast<QObject*>(this);
}

