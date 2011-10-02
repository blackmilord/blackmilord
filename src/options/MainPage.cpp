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

#include "MainPage.h"
#include <QDebug>
#include <QListWidgetItem>
#include <QListWidget>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QComboBox>
#include <QLabel>

#include <AspellWrapper.h>
#include <Preferences.h>

MainPage::MainPage(QWidget *parent) : 
    QWidget(parent)
{
    QGridLayout *optionsLayout = new QGridLayout();
    m_makeBackupOverwrite = new QCheckBox(tr("Make a copy before overwriting original file."));
    
    m_makeBackupOverwrite->setChecked(Preferences::instance().getValue(
            Preferences::PROP_MAKE_BACKUP_BEFORE_OVERWRITE, true).toBool());

    optionsLayout->addWidget(m_makeBackupOverwrite, 0, 0);

    if (ASpellWrapper::instance().isLoaded()) {
        m_language = new QComboBox();
        loadLanguages();
        selectLanguageFromPreference();
        optionsLayout->addWidget(new QLabel(tr("Spelling check language")), 1, 0);
        optionsLayout->addWidget(m_language, 1, 1);
    }

    optionsLayout->setColumnMinimumWidth(0, 75);
    optionsLayout->setColumnStretch(0, 0);
    optionsLayout->setColumnStretch(1, 1);

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addLayout(optionsLayout);
    mainLayout->addStretch(1);
    setLayout(mainLayout);
}

void MainPage::registerPage(QListWidget *contentsWidget, QStackedWidget *pagesWidget)
{
    pagesWidget->addWidget(this);
    QListWidgetItem *mainButton = new QListWidgetItem(contentsWidget);
    mainButton->setText(tr("Main"));
    mainButton->setTextAlignment(Qt::AlignHCenter);
    mainButton->setFlags(Qt::ItemIsEnabled);
    mainButton->setIcon(QIcon(":/icon/settings_general.png"));
}

void MainPage::apply()
{
    Preferences::instance().setValue(
            Preferences::PROP_MAKE_BACKUP_BEFORE_OVERWRITE, 
            m_makeBackupOverwrite->isChecked());
    if (ASpellWrapper::instance().isLoaded()) {
        ASpellWrapper::instance().changeLanguage(
                m_language->itemData(m_language->currentIndex(), Qt::UserRole).toString());
    }
}

void MainPage::loadLanguages()
{
    const QList<QPair<QString, QString> > &lang =
            ASpellWrapper::instance().installedDictionaries();
    QList<QPair<QString, QString> >::const_iterator it = lang.begin();
    m_language->clear();
    for (; it != lang.end(); ++it) {
        QString display = it->first;
        QVariant userData = it->second;
        m_language->addItem(display, userData);
    }
}

void MainPage::selectLanguageFromPreference()
{
    QString language = Preferences::instance().getValue(
            Preferences::PROP_ASPELL_DICTIONARY, "").toString();
    m_language->setCurrentIndex(0);
    for (int i = 0; i < m_language->count(); ++i) {
        if (language == m_language->itemData(i, Qt::UserRole).toString()) {
            m_language->setCurrentIndex(i);
            return;
        }
    }
}

void MainPage::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);
    selectLanguageFromPreference();
}