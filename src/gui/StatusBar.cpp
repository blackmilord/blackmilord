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

#include "StatusBar.h"
#include <QLabel>

StatusBar::StatusBar(QWidget *parent) :
    QStatusBar(parent),
    m_statusBarDocLength(new QLabel())
{
    addPermanentWidget(m_statusBarDocLength);
    m_statusBarDocLength->setText("0");
    m_statusBarDocLength->setMinimumWidth(30);
    m_statusBarDocLength->setAlignment(Qt::AlignRight);
}

StatusBar::~StatusBar()
{
}

void StatusBar::setStatusBarDocLength(const QString &length)
{
    m_statusBarDocLength->setText(length);
}

void StatusBar::showMessage(const QString &message, int timeout)
{
    QStatusBar::showMessage(message, timeout);
}