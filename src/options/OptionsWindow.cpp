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

#include "OptionsWindow.h"
#include <QDebug>
#include <QPushButton>
#include <QStackedWidget>
#include <QListWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include "MainPage.h"
#include "EditorPage.h"
#include "HighlighterPage.h"

OptionsWindow::~OptionsWindow()
{
}

OptionsWindow::OptionsWindow(QWidget *parent) :
    QDialog(parent),
    m_contentsWidget(new QListWidget()),
    m_pagesWidget(new QStackedWidget()),
    m_mainPage(new MainPage()),
    m_editorPage(new EditorPage()),
    m_highlighterPage(new HighlighterPage())
{
    m_contentsWidget->setViewMode(QListView::IconMode);
    m_contentsWidget->setIconSize(QSize(72, 72));
    m_contentsWidget->setMovement(QListView::Static);
    m_contentsWidget->setFixedWidth(104);
    m_contentsWidget->setSpacing(4);

    m_mainPage->registerPage(m_contentsWidget, m_pagesWidget);
    m_editorPage->registerPage(m_contentsWidget, m_pagesWidget);
    m_highlighterPage->registerPage(m_contentsWidget, m_pagesWidget);

    QPushButton *cancelButton = new QPushButton(tr("Cancel"), this);
    QPushButton *applyButton = new QPushButton(tr("Apply"), this);
    QPushButton *okButton = new QPushButton(tr("OK"), this);

    connect(cancelButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(applyButton, SIGNAL(clicked()), this, SLOT(apply()));
    connect(okButton, SIGNAL(clicked()), this, SLOT(ok()));

    m_contentsWidget->setCurrentRow(0);
    connect(m_contentsWidget, SIGNAL(currentItemChanged(QListWidgetItem *, QListWidgetItem *)),
	    this, SLOT(changePage(QListWidgetItem *, QListWidgetItem*)));

    QHBoxLayout *viewLayout = new QHBoxLayout();
    viewLayout->addWidget(m_contentsWidget, 0);
    viewLayout->addWidget(m_pagesWidget, 1);

    QHBoxLayout *buttonsLayout = new QHBoxLayout();
    buttonsLayout->addStretch(1);
    buttonsLayout->addWidget(okButton);
    buttonsLayout->addWidget(applyButton);
    buttonsLayout->addWidget(cancelButton);

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addLayout(viewLayout, 1);
    mainLayout->addSpacing(12);
    mainLayout->addLayout(buttonsLayout, 0);
    setLayout(mainLayout);
    setWindowTitle((tr("Settings")));
    setWindowIcon(QIcon(":/resource/icon/menu_settings.png"));
    resize(700, 450);
}

void OptionsWindow::changePage(QListWidgetItem *current, QListWidgetItem *previous)
{
    if (!current) {
        current = previous;
    }
    m_pagesWidget->setCurrentIndex(m_contentsWidget->row(current));
}

void OptionsWindow::ok()
{
    apply();
    close();
}

void OptionsWindow::apply()
{
    for (int i = 0; i < m_pagesWidget->count(); ++i) {
        IPageWidget *widget = dynamic_cast<IPageWidget*>(m_pagesWidget->widget(i));
        widget->apply();
    }
}
