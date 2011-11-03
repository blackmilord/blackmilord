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
#include <QDebug>
#include <QMenu>
#include <QLayout>

#include <AspellWrapper.h>
#include <HighlighterManager.h>
#include <Book.h>
#include <qtextcursor.h>
#include <qtextdocument.h>

PlainTextEditor::PlainTextEditor(QWidget * parent) :
    QPlainTextEdit(parent),
    m_undoStack(this)
{
    HighlighterManagerFactory::createInstance(this);
    connect(&Preferences::instance(), SIGNAL(settingsChanged()), SLOT(applySettings()));
    connect(document(), SIGNAL(contentsChange(int, int, int)), SLOT(contentsChangeSlot(int, int, int)));
    connect(document(), SIGNAL(contentsChanged()), SLOT(contentsChangedSlot()));
    connect(&m_undoStack, SIGNAL(canUndo(bool)), SLOT(canUndoSlot(bool)));
    connect(&m_undoStack, SIGNAL(canRedo(bool)), SLOT(canRedoSlot(bool)));
    setUndoRedoEnabled(false);
    installEventFilter(this);
}

PlainTextEditor::~PlainTextEditor()
{
    //this is the place where instance should be deleted, but
    //qt handle it itself when instance is set as a child of other QObject.
}

PlainTextEditor& PlainTextEditor::instance()
{
    static PlainTextEditor *instance = new PlainTextEditor();
    return *instance;
}

int PlainTextEditor::firstVisibleBlock() const
{
    return QPlainTextEdit::firstVisibleBlock().blockNumber();
}

bool PlainTextEditor::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == this) {
        if (event->type() == QEvent::KeyPress) {
            QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
            if (keyEvent->matches(QKeySequence::Undo)) {
                if (m_undoStack.canUndo()) {
                    undo();
                }
                return true;
            }
            else if (keyEvent->matches(QKeySequence::Redo)) {
                if (m_undoStack.canRedo()) {
                    redo();
                }
                return true;
            }
        }
        return false;
    }
    return QPlainTextEdit::eventFilter(watched, event);
}

void PlainTextEditor::contextMenuEvent(QContextMenuEvent * event)
{
    QMenu *menu = new QMenu();

    QAction *action = menu->addAction(tr("Undo"), this, SLOT(undo()), QKeySequence::Undo);
    action->setEnabled(m_undoStack.canUndo());
    action = menu->addAction(tr("Redo"), this, SLOT(redo()), QKeySequence::Redo);
    action->setEnabled(m_undoStack.canRedo());

    menu->addSeparator();
    menu->addAction(tr("Copy"), this, SLOT(copy()), QKeySequence::Copy);
    menu->addAction(tr("Cut"), this, SLOT(cut()), QKeySequence::Cut);
    menu->addAction(tr("Paste"), this, SLOT(paste()), QKeySequence::Paste);

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

void PlainTextEditor::applySettings()
{
    setFont(Preferences::instance().getEditorFont());
}

void PlainTextEditor::redo()
{
    m_undoStack.redo();
}

void PlainTextEditor::undo()
{
    m_undoStack.undo();
}

void PlainTextEditor::clearRedoUndoHistory()
{
    m_undoStack.clear();
}

void PlainTextEditor::contentsChangedSlot()
{
    emit contentsChanged();
    if (toPlainText().isEmpty()) {
        HighlighterManagerFactory::instance().cancelHighlighting();
    }
}

void PlainTextEditor::contentsChangeSlot(int position, int charsRemoved, int charsAdded)
{
    emit contentsChange(position, charsRemoved, charsAdded);
    if (Book::instance().isFileOpened()) {
        HighlighterManagerFactory::instance().registerBlockToHighlight(position, position + charsAdded, true);
    }
    else {
        //Opening in progress, use rehighlight instead
        //rehighlight has set whole document to be rehighlighted with low priority
        HighlighterManagerFactory::instance().rehighlight();
    }
}

void PlainTextEditor::applyHintSlot(QAction *action)
{
    if (!action->data().isNull()) {
        QTextCursor cursor = textCursor();
        cursor.select(QTextCursor::WordUnderCursor);
        cursor.insertText(action->text());
    }
}

void PlainTextEditor::canUndoSlot(bool value)
{
    emit canUndo(value);
}

void PlainTextEditor::canRedoSlot(bool value)
{
    emit canRedo(value);
}

void PlainTextEditor::connect(const char *signal, const QObject *receiver,
        const char *method, Qt::ConnectionType type)
{
    QWidget::connect(this, signal, receiver, method, type);
}

void PlainTextEditor::connect(const QObject *sender, const char *signal,
        const char *method, Qt::ConnectionType type)
{
    QWidget::connect(sender, signal, this, method, type);
}

void PlainTextEditor::disconnect(const char *signal, const QObject *receiver, const char *method)
{
    QWidget::disconnect(this, signal, receiver, method);
}

void PlainTextEditor::disconnect(const QObject *sender, const char *signal, const char *method)
{
    QWidget::disconnect(sender, signal, this, method);
}

void PlainTextEditor::replace(int position, int length, const QString &after)
{
    Q_ASSERT(position >= 0);
    Q_ASSERT(position + length <= toPlainText().length());

    QTextCursor cursor = textCursor();
    cursor.setPosition(position);
    cursor.setPosition(position + length, QTextCursor::KeepAnchor);
    cursor.insertText(after);
}

int PlainTextEditor::getCursorPosition() const
{
    return textCursor().position();
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

int PlainTextEditor::getSelectionStart() const
{
    Q_ASSERT(hasSelection());
    return textCursor().selectionStart();
}

int PlainTextEditor::getSelectionEnd() const
{
    Q_ASSERT(hasSelection());
    return textCursor().selectionEnd();
}

bool PlainTextEditor::hasSelection() const
{
    return textCursor().hasSelection();
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
    textCursor().clearSelection();
}

QString PlainTextEditor::toPlainText() const
{
    return QPlainTextEdit::toPlainText();
}

void PlainTextEditor::setPlainText(const QString &text)
{
    QTextCursor cursor = textCursor();
    cursor.select(QTextCursor::Document);
    cursor.insertText(text);
}

QTextBlock PlainTextEditor::findBlockByNumber(int blockNumber) const
{
    return document()->findBlockByNumber(blockNumber);
}

QTextBlock PlainTextEditor::findBlock(int pos) const
{
    return document()->findBlock(pos);
}

void PlainTextEditor::setTextCursor(const QTextCursor& cursor)
{
    QPlainTextEdit::setTextCursor(cursor);
}

QTextCursor PlainTextEditor::textCursorForBlock(int blockNumber) const
{
    Q_ASSERT(blockNumber < blockCount());
    return QTextCursor(document()->findBlockByNumber(blockNumber));
}

QTextCursor PlainTextEditor::textCursor() const
{
    return QPlainTextEdit::textCursor();
}

int PlainTextEditor::blockCount() const
{
    return QPlainTextEdit::blockCount();
}

bool PlainTextEditor::blockSignals(bool block)
{
    document()->blockSignals(block);
    return QObject::blockSignals(block);
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

void PlainTextEditor::addToLayout(QLayout *layout)
{
    layout->addWidget(this);
}
