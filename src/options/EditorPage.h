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

#ifndef BLACK_MILORD_EDITOR_PAGE_H
#define BLACK_MILORD_EDITOR_PAGE_H

#include <QWidget>
#include <QVector>
#include "IPageWidget.h"

class QListWidget;
class QStackedWidget;
class QFontComboBox;
class QComboBox;
class QCheckBox;

class EditorPage : public QWidget, public IPageWidget
{
    Q_OBJECT
public:
    explicit EditorPage(QWidget *parent = 0);
    virtual ~EditorPage();
    void registerPage(QListWidget *contentsWidget, QStackedWidget *pagesWidget);
    void apply();
private:
     QFontComboBox *m_font;
     QComboBox *m_fontSize;

     void showEvent(QShowEvent *event);

private slots:
    void fontFamilyChanged(const QString &fontName);
};


#endif /* BLACK_MILORD_EDITOR_PAGE_H */