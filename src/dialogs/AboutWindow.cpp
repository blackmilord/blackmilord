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

#include "AboutWindow.h"
#include "SpellCheckingWindow.h"
#include "PlainTextEditorUndoStack.h"
#include <QTabWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTextBrowser>
#include <QFile>
#include <QPushButton>

AboutWindow::AboutWindow(QWidget *parent) :
    QDialog(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QTabWidget *tabWidget = new QTabWidget(this);
    tabWidget->setFocusPolicy(Qt::NoFocus);
    tabWidget->addTab(new QTextBrowser(), tr("About"));
    tabWidget->addTab(new QTextBrowser(), tr("Authors"));
    tabWidget->addTab(new QTextBrowser(), tr("Licence"));
    static_cast<QTextBrowser*>(tabWidget->widget(0))->setReadOnly(true);
    static_cast<QTextBrowser*>(tabWidget->widget(1))->setReadOnly(true);
    static_cast<QTextBrowser*>(tabWidget->widget(2))->setReadOnly(true);
    QFile fileAbout(":/resource/about.html");
    if (fileAbout.open(QIODevice::ReadOnly | QIODevice::Text)) {
        static_cast<QTextBrowser*>(tabWidget->widget(0))->setHtml(fileAbout.readAll());
        static_cast<QTextBrowser*>(tabWidget->widget(0))->setOpenExternalLinks(true);
        fileAbout.close();
    }
    QFile fileAuthors(":/resource/authors.html");
    if (fileAuthors.open(QIODevice::ReadOnly | QIODevice::Text)) {
        static_cast<QTextBrowser*>(tabWidget->widget(1))->setHtml(fileAuthors.readAll());
        static_cast<QTextBrowser*>(tabWidget->widget(1))->setOpenExternalLinks(true);
        fileAuthors.close();
    }
    QFile fileLicence(":/resource/licence.html");
    if (fileLicence.open(QIODevice::ReadOnly | QIODevice::Text)) {
        static_cast<QTextBrowser*>(tabWidget->widget(2))->setHtml(fileLicence.readAll());
        static_cast<QTextBrowser*>(tabWidget->widget(2))->setOpenExternalLinks(true);
        fileLicence.close();
    }

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *closeButton = new QPushButton(tr("Close"));
    buttonLayout->addStretch(1);
    buttonLayout->addWidget(closeButton);

    mainLayout->addWidget(tabWidget, 1);
    mainLayout->addLayout(buttonLayout);

    setLayout(mainLayout);
    setFixedSize(400, 400);
    setWindowTitle(tr("About BlackMilord"));
}

AboutWindow::~AboutWindow()
{
}
