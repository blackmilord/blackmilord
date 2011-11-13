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

#include "PlainTextEditorUndoStack.h"
#include "MainWindow.h"
#include <QDebug>
#include <PlainTextEditor.h>
#include <QtGui/qstyleoption.h>

PlainTextEditorUndoStack::PlainTextEditorUndoStack(PlainTextEditor *editor) :
    m_editor(editor),
    m_position(0),
    m_maxStackSize(100)
{
    connect(m_editor->asObject(), SIGNAL(cursorPositionChanged()),
        this, SLOT(cursorPositionChanged()));
    connect(m_editor->asObject(), SIGNAL(selectionChanged()),
        this, SLOT(selectionChanged()));
    connect(m_editor->asObject(), SIGNAL(contentsChange(int, int, int)),
        this, SLOT(contentsChange(int, int, int)));

    addNewUndoCommand();
}

PlainTextEditorUndoStack::~PlainTextEditorUndoStack()
{
    clear();
    //clear adds one empty undo command, it needs to be removed.
    delete popUndoCommand();
}

void PlainTextEditorUndoStack::clear()
{
    clearRedoCommands();
    clearUndoCommands();
    addNewUndoCommand();
}

void PlainTextEditorUndoStack::clearUndoCommands()
{
    foreach(RedoUndoCommand* command, m_undoCommands) {
        delete command;
    }
    m_undoCommands.clear();
    emit canUndo(false);
}

void PlainTextEditorUndoStack::clearRedoCommands()
{
    foreach(RedoUndoCommand* command, m_redoCommands) {
        delete command;
    }
    m_redoCommands.clear();
    emit canRedo(false);
}

void PlainTextEditorUndoStack::undo()
{
    Q_ASSERT(canUndo());

    RedoUndoCommand* command = popUndoCommand();
    if (command->m_type == RedoUndoCommand::UNKNOWN) {
        delete command;
        Q_ASSERT(canUndo());
        command = popUndoCommand();
    }
    disconnect(m_editor->asObject(), SIGNAL(contentsChange(int, int, int)),
        this, SLOT(contentsChange(int, int, int)));

    switch (command->m_type) {
        case RedoUndoCommand::INSERT:
        {
            QTextCursor cursor = m_editor->textCursor();
            cursor.setPosition(command->m_position);
            cursor.setPosition(command->m_position + command->m_textAdded->size(), QTextCursor::KeepAnchor);
            cursor.removeSelectedText();
            break;
        }
        case RedoUndoCommand::REPLACE:
        {
            //qDebug() << "undo replace" << *command->m_textAdded << "->" << *command->m_textRemoved;
            QTextCursor cursor = m_editor->textCursor();
            cursor.setPosition(command->m_position);
            cursor.setPosition(command->m_position + command->m_textAdded->size(), QTextCursor::KeepAnchor);
            cursor.insertText(*command->m_textRemoved);
            break;
        }
        case RedoUndoCommand::DELETE:
        {
            //qDebug() << "undo remove" << *command->m_textRemoved;
            QTextCursor cursor = m_editor->textCursor();
            cursor.setPosition(command->m_position);
            cursor.insertText(*command->m_textRemoved);
            break;
        }
        default:
            Q_ASSERT(false);
    }

    connect(m_editor->asObject(), SIGNAL(contentsChange(int, int, int)),
        this, SLOT(contentsChange(int, int, int)));

    addNewRedoCommand(command);
    addNewUndoCommand();
}

bool PlainTextEditorUndoStack::canUndo()
{
    return m_undoCommands.size() > 1 ||
           (!m_undoCommands.isEmpty() && m_undoCommands.first()->m_type != RedoUndoCommand::UNKNOWN);
}

void PlainTextEditorUndoStack::redo()
{
    Q_ASSERT(canRedo());

    RedoUndoCommand* command = popRedoCommand();
    Q_ASSERT(command->m_type != RedoUndoCommand::UNKNOWN);

    disconnect(m_editor->asObject(), SIGNAL(contentsChange(int, int, int)),
        this, SLOT(contentsChange(int, int, int)));

    switch (command->m_type) {
        case RedoUndoCommand::INSERT:
        {
            QTextCursor cursor = m_editor->textCursor();
            cursor.setPosition(command->m_position);
            cursor.insertText(*command->m_textAdded);
            break;
        }
        case RedoUndoCommand::REPLACE:
        {
            //qDebug() << "undo replace" << *command->m_textAdded << "->" << *command->m_textRemoved;
            QTextCursor cursor = m_editor->textCursor();
            cursor.setPosition(command->m_position);
            cursor.setPosition(command->m_position + command->m_textRemoved->size(), QTextCursor::KeepAnchor);
            cursor.insertText(*command->m_textAdded);
            break;
        }
        case RedoUndoCommand::DELETE:
        {
            //qDebug() << "undo remove" << *command->m_textRemoved;
            QTextCursor cursor = m_editor->textCursor();
            cursor.setPosition(command->m_position);
            cursor.setPosition(command->m_position + command->m_textRemoved->size(), QTextCursor::KeepAnchor);
            cursor.removeSelectedText();
            break;
        }
        default:
            Q_ASSERT(false);
    }

    connect(m_editor->asObject(), SIGNAL(contentsChange(int, int, int)),
        this, SLOT(contentsChange(int, int, int)));

    addNewUndoCommand(command);
}

bool PlainTextEditorUndoStack::canRedo()
{
    return m_redoCommands.size() > 1 ||
           (!m_redoCommands.isEmpty() && m_redoCommands.first()->m_type != RedoUndoCommand::UNKNOWN);
}

void PlainTextEditorUndoStack::contentsChange(int position, int charsRemoved, int charsAdded)
{
    //qDebug() << position << charsRemoved << charsAdded << m_position;

    clearRedoCommands();

    const QString &text = m_editor->toPlainText();

    if (charsAdded == 1 && charsRemoved == 0) {       //char added
        Q_ASSERT(position == m_position);
        //qt clames it added 1 char when document became empty
        if (text.isEmpty() && position == 0) {
            return;
        }
        adding(position, text.at(position));
    }
    else if (charsAdded > 1 && charsRemoved == 0) {   //pasting text
        Q_ASSERT(position == m_position);
        insert(position, text.mid(position, charsAdded));
    }
    else if (charsAdded > 0 && charsRemoved > 0) {
        if (m_selection.isNull()) {                   //no selection
            if (position == 0) {
                //when inserting at position 0 then qt handle it in different way
                insert(position, text.mid(position, charsAdded - charsRemoved));
            }
            else {
                Q_ASSERT(false);
            }
        }
        else {
            replace(position, m_selection, text.mid(position, charsAdded));
        }
    }
    else if (charsAdded == 0 && charsRemoved == 1) {  //char remoed
        if (position == m_position - 1) {   //backspace button
            deleting(position, m_before, true);
        }
        else if (position == m_position) {  //delete button
            deleting(position, m_after, false);
        }
        else {
            Q_ASSERT(false);
        }
    }
}

void PlainTextEditorUndoStack::cursorPositionChanged()
{
    //qDebug() << "cursor position change";
    QTextCursor cursor =  m_editor->textCursor();
    m_position = cursor.position();
    if (!cursor.atEnd()) {
        m_after = m_editor->toPlainText().at(m_position);
    }
    else {
        m_after = QChar();
    }
    if (!cursor.atStart()) {
        m_before = m_editor->toPlainText().at(m_position - 1);
    }
    else {
        m_before = QChar();
    }
}

void PlainTextEditorUndoStack::selectionChanged()
{
    //qDebug() << "selection change";
    QTextCursor cursor =  m_editor->textCursor();
    if (cursor.hasSelection()) {
        m_selection = cursor.selectedText();
    }
    else {
        m_selection = QString();
    }
}

//queue mehtods

void PlainTextEditorUndoStack::addNewUndoCommand(RedoUndoCommand* command)
{
    if (!m_undoCommands.isEmpty() && command != NULL && m_undoCommands.first()->m_type == RedoUndoCommand::UNKNOWN) {
        delete m_undoCommands.takeFirst();
    }
    if (NULL == command) {
        command = new RedoUndoCommand();
    }
    Q_ASSERT(m_undoCommands.isEmpty() || m_undoCommands.first()->m_type != RedoUndoCommand::UNKNOWN);
    m_undoCommands.push_front(command);
    if (m_undoCommands.size() > m_maxStackSize) {
        delete m_undoCommands.takeLast();
    }
    emit canUndo(canUndo());
}

PlainTextEditorUndoStack::RedoUndoCommand* PlainTextEditorUndoStack::popUndoCommand()
{
    RedoUndoCommand *result = m_undoCommands.takeFirst();
    emit canUndo(canUndo());
    return result;
}

void PlainTextEditorUndoStack::addNewRedoCommand(RedoUndoCommand* command)
{
    if (NULL == command) {
        command = new RedoUndoCommand();
    }
    m_redoCommands.push_front(command);
    if (m_redoCommands.size() > m_maxStackSize) {
        delete m_redoCommands.takeLast();
    }
    emit canRedo(canRedo());
}

PlainTextEditorUndoStack::RedoUndoCommand* PlainTextEditorUndoStack::popRedoCommand()
{
    RedoUndoCommand *result = m_redoCommands.takeFirst();
    emit canRedo(canRedo());
    return result;
}

void PlainTextEditorUndoStack::commit()
{
    //qDebug() << "commit";
    if (m_undoCommands.first()->m_type != RedoUndoCommand::UNKNOWN) {
        addNewUndoCommand();
    }
}

void PlainTextEditorUndoStack::adding(int position, const QChar &charAdded)
{
    //qDebug() << "adding" << charAdded;

    RedoUndoCommand *command = m_undoCommands.first();

    if ((command->m_type != RedoUndoCommand::UNKNOWN && command->m_type != RedoUndoCommand::INSERT) ||
        (command->m_type == RedoUndoCommand::INSERT && command->m_position + command->m_textAdded->size() != position))
    {
        commit();
        command = m_undoCommands.first();
    }

    if (command->m_type == RedoUndoCommand::UNKNOWN) {
        command->m_type = RedoUndoCommand::INSERT;
        command->m_position = position;
    }

    command->m_textAdded->append(charAdded);

    if (charAdded == '\n') {
        commit();
    }
}

void PlainTextEditorUndoStack::deleting(int position, const QChar &charDeleted, bool backSpace)
{
    //qDebug() << "deleting" << charDeleted;

    RedoUndoCommand *command = m_undoCommands.first();

    if ((command->m_type != RedoUndoCommand::UNKNOWN && command->m_type != RedoUndoCommand::DELETE) ||
        (command->m_type == RedoUndoCommand::DELETE && backSpace && command->m_position - command->m_textRemoved->size() != position))
    {
        commit();
        command = m_undoCommands.first();
    }

    if (command->m_type == RedoUndoCommand::UNKNOWN) {
        command->m_type = RedoUndoCommand::DELETE;
    }

    command->m_position = position;
    command->m_textRemoved->prepend(charDeleted);
}

void PlainTextEditorUndoStack::insert(int position, const QString &textAdded)
{
    //qDebug() << "insert" << textAdded;

    commit();

    RedoUndoCommand *command = m_undoCommands.first();
    command->m_type = RedoUndoCommand::INSERT;
    command->m_position = position;
    command->m_textAdded->append(textAdded);

    commit();
}

void PlainTextEditorUndoStack::replace(int position, const QString &textRemoved, const QString &textAdded)
{
    //qDebug() << "replace" << textRemoved << "->" << textAdded;

    commit();

    RedoUndoCommand *command = m_undoCommands.first();
    command->m_type = RedoUndoCommand::REPLACE;
    command->m_position = position;
    command->m_textAdded->append(textAdded);
    command->m_textRemoved->append(textRemoved);

    commit();
}

void PlainTextEditorUndoStack::remove(int position, const QString &textRemoved)
{
    //qDebug() << "remove" << textRemoved;

    commit();

    RedoUndoCommand *command = m_undoCommands.first();
    command->m_type = RedoUndoCommand::DELETE;
    command->m_position = position;
    command->m_textRemoved->append(textRemoved);

    commit();
}
