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

#include "HowToUseAspellWindow.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QString>

namespace {
#ifdef Q_WS_X11
    const QString labelText(QObject::tr(
        "You can consider install Aspell program.<br/>"
        "This will allow you to perform spell checking.<br/>"
        "You can install Aspell from Linux's software repository.<br/>"
        "Keep in mind you need to install core program and language pack(s).<br/><br/>"
    ));
#elif defined Q_WS_WIN
    const QString labelText(QObject::tr(
        "You can consider install Aspell program.<br/>"
        "This will allow you to perform spell checking.<br/>"
        "You can download Aspell Windows port from here: <a href=\"http://aspell.net/win32/\">http://aspell.net/win32/</a>.<br/>"
        "Keep in mind you need to install core program and language pack(s).<br/><br/>"
    ));
#endif
}

HowToUseAspellWindow::HowToUseAspellWindow(QWidget *parent) :
    QDialog(parent)
{
    QVBoxLayout *layout = new QVBoxLayout();
    QLabel *label = new QLabel();
    label->setText(labelText);
    label->setOpenExternalLinks(true);
    QPushButton *button = new QPushButton("&OK");
    button->setDefault(true);
    connect(button, SIGNAL(released()), this, SLOT(close()));
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch(1);
    buttonLayout->addWidget(button);
    buttonLayout->addStretch(1);
    layout->addWidget(label);
    layout->addLayout(buttonLayout);
    setLayout(layout);
    setWindowTitle(tr("How to use Aspell..."));
}

HowToUseAspellWindow::~HowToUseAspellWindow()
{
}
