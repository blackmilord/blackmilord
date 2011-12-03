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

#include "HighlighterPage.h"
#include <QListWidgetItem>
#include <QListWidget>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QDebug>

#include "Preferences.h"
#include <HighlighterManager.h>
#include <AbstractHighlighter.h>

HighlighterPage::HighlighterPage(QWidget *parent) :
    QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout();

    const QVector<AbstractHighlighter*> &highlighters =
            HighlighterManager::instance().getHighlighters();
    foreach(AbstractHighlighter* highlighter, highlighters) {
        QGroupBox *groupBox = new QGroupBox(highlighter->name());
        groupBox->setLayout(highlighter->configurationLayout());
        mainLayout->addWidget(groupBox);
    }

    mainLayout->addStretch(1);
    setLayout(mainLayout);
}

HighlighterPage::~HighlighterPage()
{
}

void HighlighterPage::registerPage(QListWidget *contentsWidget, QStackedWidget *pagesWidget)
{
    pagesWidget->addWidget(this);
    QListWidgetItem *editorButton = new QListWidgetItem(contentsWidget);
    editorButton->setIcon(QIcon(":/resource/icon/settings_highlighters.png"));
    editorButton->setText(tr("Highlighters"));
    editorButton->setTextAlignment(Qt::AlignHCenter);
    editorButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
}

void HighlighterPage::apply()
{
    HighlighterManager::instance().saveSettings();
    HighlighterManager::instance().applySettings();
}

void HighlighterPage::showEvent(QShowEvent *event)
{
    const QVector<AbstractHighlighter*> &highlighters =
            HighlighterManager::instance().getHighlighters();
    foreach(AbstractHighlighter* highlighter, highlighters) {
        highlighter->resetConfigurationLayout();
    }
    QWidget::showEvent(event);
}
