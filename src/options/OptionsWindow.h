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

#ifndef BLACK_MILORD_OPTIONS_WINDOW_H
#define BLACK_MILORD_OPTIONS_WINDOW_H

#include <QDialog>
#include <QWidget>

class QListWidget;
class QListWidgetItem;
class QStackedWidget;

class EditorPage;
class MainPage;

class OptionsWindow : public QDialog
{
    Q_OBJECT

public:
    explicit OptionsWindow(QWidget *parent = 0);
    ~OptionsWindow();

private:
    QListWidget *m_contentsWidget;
    QStackedWidget *m_pagesWidget;
    EditorPage *m_editorPage;
    MainPage *m_mainPage;

private slots:
    void changePage(QListWidgetItem *current, QListWidgetItem *previous);
    void apply();
    void ok();
};

#endif /* BLACK_MILORD_OPTIONS_WINDOW_H */
