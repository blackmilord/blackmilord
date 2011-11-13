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

#ifndef BLACK_MILORD_HIGHLIGHTER_THREAD_H
#define BLACK_MILORD_HIGHLIGHTER_THREAD_H

#include <QThread>

class HighlighterWorker :
    public QObject
{
    explicit HighlighterWorker(QObject *parent = 0);
    virtual ~HighlighterWorker();
protected:
    void customEvent(QEvent *event);

    friend class HighlighterThread;
};

class HighlighterThread :
    public QThread
{
    Q_OBJECT
public:
    explicit HighlighterThread(QObject *parent = 0);
    virtual ~HighlighterThread();
    HighlighterWorker* getWorker();
protected:
    void run();
private:
    QAtomicInt m_guard;
    HighlighterWorker *m_worker;
};

#endif /* BLACK_MILORD_HIGHLIGHTER_THREAD_H */