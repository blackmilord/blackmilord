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

#include <QObject>

class MainWindow;

class BlackMilordTests: public QObject
{
    Q_OBJECT
private:
    MainWindow *m_mainWindow;
private slots:
    void initTestCase();        //will be called before the first testfunction is executed.
    void cleanupTestCase();     //will be called after the last testfunction was executed.
    void init();        //will be called before each testfunction is executed.
    void cleanup();     //will be called after every testfunction.

    void check_PlainTextEditor_setTextgetText();
    void check_PlainTextEditor_typing();
    void check_PlainTextEditor_firstVisibleBlock();
    void check_PlainTextEditor_getCursorPosition();
    void check_PlainTextEditor_setCursorPosition();
    void check_PlainTextEditor_selection();
};
