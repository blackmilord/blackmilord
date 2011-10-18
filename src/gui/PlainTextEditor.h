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

class PlainTextEditor :
    public QPlainTextEdit
{
    Q_OBJECT
public:
    explicit PlainTextEditor(QWidget * parent = 0);
    ~PlainTextEditor();

protected:
    void contextMenuEvent(QContextMenuEvent * event);

public slots:
    void redo();
    void undo();

private slots:
    void contentsChange(int position, int charsRemoved, int charsAdded);
    void applyHint(QAction *action);
};

#endif /* BLACK_MILORD_PLAIN_TEXT_EDITOR_H */