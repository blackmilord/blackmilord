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

#ifndef BLACK_MILORD_FIND_REPLACE_WINDOW_H
#define BLACK_MILORD_FIND_REPLACE_WINDOW_H

#include <QDialog>

#include "SpellCheckingWindow.h"

class QComboBox;
class QCheckBox;
class QShowEvent;

class FindReplaceWindow : public QDialog
{
    Q_OBJECT
public:
    FindReplaceWindow(QWidget *parent = 0);
    ~FindReplaceWindow();
private:
    void saveValues();
    QComboBox *m_findWhat;
    QComboBox *m_replaceWith;
    QCheckBox *m_regExp;
    QCheckBox *m_backward;
    QCheckBox *m_wordsOnly;
    QCheckBox *m_caseSensitive;

private slots:
    void find();
    void replace();
    void replaceAll();
    void checkboxChanged();

protected:
    void showEvent(QShowEvent *event);
};

#endif /* BLACK_MILORD_FIND_REPLACE_WINDOW_H */
