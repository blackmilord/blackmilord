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
#include <QDebug>
#include <PlainTextEditor.h>
#include <QtGui/qstyleoption.h>

PlainTextEditorUndoStack::PlainTextEditorUndoStack(PlainTextEditor *editor) :
    m_editor(editor),
    m_position(0)
{
    connect(m_editor, SIGNAL(cursorPositionChanged()),
        this, SLOT(cursorPositionChanged()));
    connect(m_editor, SIGNAL(selectionChanged()),
        this, SLOT(selectionChanged()));
    connect(m_editor->document(), SIGNAL(contentsChange(int, int, int)),
        this, SLOT(contentsChange(int, int, int)));

    addNewUndoCommand();
}

PlainTextEditorUndoStack::~PlainTextEditorUndoStack()
{
}

void PlainTextEditorUndoStack::clear()
{
    foreach(RedoUndoCommand* command, m_undoCommands) {
        delete command;
    }
    m_undoCommands.clear();
    addNewUndoCommand();
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

    disconnect(m_editor->document(), SIGNAL(contentsChange(int, int, int)),
        this, SLOT(contentsChange(int, int, int)));

    switch (command->m_type) {
        case RedoUndoCommand::INSERT:
        {
            //qDebug() << "undo insert" << command->m_position << command->m_position + command->m_textAdded->size();
            QTextCursor cursor(m_editor->document());
            cursor.setPosition(command->m_position);
            cursor.setPosition(command->m_position + command->m_textAdded->size(), QTextCursor::KeepAnchor);
            cursor.removeSelectedText();
            break;
        }
        default:
            Q_ASSERT(false);
    }

    connect(m_editor->document(), SIGNAL(contentsChange(int, int, int)),
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
}

bool PlainTextEditorUndoStack::canRedo()
{
    return m_redoCommands.size() > 1 ||
           (!m_redoCommands.isEmpty() && m_redoCommands.first()->m_type != RedoUndoCommand::UNKNOWN);
}

void PlainTextEditorUndoStack::contentsChange(int position, int charsRemoved, int charsAdded)
{
    Q_UNUSED(position);
    Q_UNUSED(charsAdded);
    Q_UNUSED(charsRemoved);

    if (charsAdded == 1 && charsRemoved == 0) {       //char added
        Q_ASSERT(position == m_position);
        insert(position, m_editor->toPlainText().at(position));

    }
    else if (charsAdded == 0 && charsRemoved == 1) {  //char remoed
        if (position == m_position - 1) {   //backspace button
            qDebug() << "backspace, deleted" << m_before ;
        }
        else if (position == m_position) {  //delete button
            qDebug() << "delete, deleted" << m_after;
        }
        else {
            Q_ASSERT(false);
        }
    }

    qDebug() << "Contents change";
}

void PlainTextEditorUndoStack::cursorPositionChanged()
{
    //commit();
    qDebug() << "cursor position change";
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
    qDebug() << "selection change";
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
    if (NULL == command) {
        command = new RedoUndoCommand();
    }
    m_undoCommands.push_front(command);
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
    qDebug() << "commit";
    m_undoCommands.first()->m_commited = true;
    addNewUndoCommand();
}

void PlainTextEditorUndoStack::insert(int position, QChar charAdded)
{
    qDebug() << "inserting" << charAdded;

    if (m_undoCommands.first()->m_type == RedoUndoCommand::DELETE ||
        m_undoCommands.first()->m_type == RedoUndoCommand::REPLACE)
    {
        commit();
    }
    if (m_undoCommands.first()->m_type == RedoUndoCommand::UNKNOWN) {
        m_undoCommands.first()->m_type = RedoUndoCommand::INSERT;
        m_undoCommands.first()->m_position = position;
    }

    m_undoCommands.first()->m_textAdded->append(charAdded);

    if (charAdded == '\n') {
        commit();
    }
}
