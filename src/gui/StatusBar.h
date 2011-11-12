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

#ifndef BLACK_MILORD_STATUS_BAR_H
#define BLACK_MILORD_STATUS_BAR_H

#include <QStatusBar>

class QLabel;
class QString;

class StatusBar :
    public QStatusBar
{
public:
    explicit StatusBar(QWidget * parent = 0);
    virtual ~StatusBar();

    void setStatusBarDocLength(const QString &length);

public slots:
    void showMessage(const QString &message, int timeout = 2000);

private:
    QLabel *m_statusBarDocLength;

};

#endif /* BLACK_MILORD_STATUS_BAR_H */
