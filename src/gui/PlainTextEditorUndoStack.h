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

#ifndef BLACK_MILORD_PLAIN_TEXT_EDITOR_UNDO_STACK_H
#define BLACK_MILORD_PLAIN_TEXT_EDITOR_UNDO_STACK_H

#include <QObject>

class PlainTextEditor;
class QTextCursor;

class PlainTextEditorUndoStack : public QObject
{
    Q_OBJECT

    class RedoUndoCommand
    {
    public:
        enum Type
        {
            UNKNOWN,
            INSERT,
            DELETE,
            REPLACE
        };

        RedoUndoCommand() :
            m_commited(false),
            m_position(0),
            m_textAdded(new QString("")),
            m_textRemoved(new QString("")),
            m_type(UNKNOWN)
        {
        }

        ~RedoUndoCommand()
        {
            delete m_textAdded;
            delete m_textRemoved;
        }
        bool m_commited;
        int m_position;
        QString *m_textAdded;
        QString *m_textRemoved;
        Type m_type;
    };

public:
    explicit PlainTextEditorUndoStack(PlainTextEditor *editor);
    virtual ~PlainTextEditorUndoStack();

    void clear();
    void undo();
    bool canUndo();
    void redo();
    bool canRedo();

private:
    PlainTextEditor *m_editor;
    QChar m_before;
    QChar m_after;
    int m_position;
    QString m_selection;

    QList<RedoUndoCommand*> m_undoCommands;
    QList<RedoUndoCommand*> m_redoCommands;

    void addNewUndoCommand(RedoUndoCommand* command = NULL);
    RedoUndoCommand* popUndoCommand();
    void addNewRedoCommand(RedoUndoCommand* command = NULL);
    RedoUndoCommand* popRedoCommand();
    void commit();
    void adding(int position, const QChar &charAdded);
    void insert(int position, const QString &textAdded);
    void replace(int position, const QString &textRemoved, const QString &textAdded);
    void remove(int position, const QString &textRemoved);

private slots:
    void cursorPositionChanged();
    void selectionChanged();
    void contentsChange(int position, int charsRemoved, int charsAdded);

signals:
    void canUndo(bool);
    void canRedo(bool);
};

#endif /* BLACK_MILORD_PLAIN_TEXT_EDITOR_UNDO_STACK_H */