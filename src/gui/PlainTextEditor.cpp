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
#include <Spellcheck.h>
#include <HighlighterManager.h>
#include <Book.h>
#include <Preferences.h>

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

//Cursor

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

//Selection

int PlainTextEditor::getSelectionStart() const
{
    return textCursor().selectionStart();
}

int PlainTextEditor::getSelectionEnd() const
{
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
    QTextCursor cursor = textCursor();
    cursor.clearSelection();
    setTextCursor(cursor);
}

QString PlainTextEditor::getSelectedText() const
{
    return textCursor().selectedText();
}

//Text handling

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

void PlainTextEditor::replace(int position, int length, const QString &after)
{
    Q_ASSERT(position >= 0);

    QTextCursor cursor = textCursor();
    cursor.setPosition(position);
    cursor.setPosition(position + length, QTextCursor::KeepAnchor);
    cursor.insertText(after);
}

//Blocks handling

int PlainTextEditor::firstVisibleBlock() const
{
    return QPlainTextEdit::firstVisibleBlock().blockNumber();
}

int PlainTextEditor::lastVisibleBlock() const
{
    return QPlainTextEdit::cursorForPosition(
        QPoint(viewport()->width(), viewport()->height())).blockNumber();
}

QTextBlock PlainTextEditor::findBlockByNumber(int blockNumber) const
{
    return document()->findBlockByNumber(blockNumber);
}

int PlainTextEditor::blockCount() const
{
    return QPlainTextEdit::blockCount();
}

//XML utils

XMLElement PlainTextEditor::findXMLElement(const QString &element, int from) const
{
    XMLElement result(element);

    QString pattern = "<\\s*\\b" + QRegExp::escape(element) + "\\b";
    QRegExp rx(pattern);
    rx.setMinimal(true);
    rx.setCaseSensitivity(Qt::CaseInsensitive);

    const QString text = toPlainText();

    int start = text.indexOf(rx, from);
    int end = text.indexOf(">", start + 1);
    if (-1 != start && -1 != end)
    {
        QString elementCopy = text.mid(start + 1 + element.length(),
                                       end - (start + 1 + element.length())).trimmed();

        QRegExp attrPattern("([a-z]+)\\s*=\\s*((?:\"[^\"]*\")|(?:'[^']*'))");
        QMap<QString, QString> attributes;
        int attrPos = 0;
        do
        {
            attrPos = elementCopy.indexOf(attrPattern, attrPos);
            if (attrPos == -1) {
                break;
            }
            QString attrName = attrPattern.cap(1);
            QString attrValue = attrPattern.cap(2).mid(1);
            attrValue = attrValue.left(attrValue.length() - 1);
            attributes.insert(attrName, attrValue);
            attrPos += attrPattern.matchedLength();
        } while (attrPos != -1);

        result.setStartPos(start);
        result.setEndPos(end + 1);
        result.setAttributes(attributes);

//        qDebug() << "position of "
//                 << result.elementName()
//                 << " "
//                 << result.startPos()
//                 << " "
//                 << result.endPos()
//                 << " "
//                 << result.attributes();
    }
    return result;
}

//Utils

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

bool PlainTextEditor::canRedo() const
{
    return document()->isRedoAvailable();
}
bool PlainTextEditor::canUndo() const
{
    return document()->isUndoAvailable();
}

QWidget* PlainTextEditor::asWidget()
{
    return static_cast<QWidget*>(this);
}

QObject* PlainTextEditor::asObject()
{
    return static_cast<QObject*>(this);
}

//Reimplemented virtual functions

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
        if (Spellcheck::instance().isLoaded()) {
            cursor = cursorForPosition(event->pos());
            setTextCursor(cursor);
            cursor.select(QTextCursor::WordUnderCursor);
            if (!Spellcheck::instance().checkWord(cursor.selectedText())) {
                const QStringList &hints = Spellcheck::instance().hints(cursor.selectedText());
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

//Public slots

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

//Private slots

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
