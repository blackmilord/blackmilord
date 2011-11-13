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

#include "EditorPage.h"
#include <QListWidgetItem>
#include <QListWidget>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QFontComboBox>
#include <QLabel>
#include <QCheckBox>
#include <QDebug>

#include "Preferences.h"
#include <AspellWrapper.h>
#include <HighlighterManager.h>
#include <AbstractHighlighter.h>

EditorPage::EditorPage(QWidget *parent) :
    QWidget(parent)
{
    QGridLayout *optionsLayout = new QGridLayout();
    QLabel *fontLabel = new QLabel(tr("Font family: "));
    m_font = new QFontComboBox();
    QLabel *fontSizeLabel = new QLabel(tr("Font size: "));
    m_fontSize = new QComboBox();

    int verticalIndex = 0;
    optionsLayout->addWidget(fontLabel, verticalIndex, 0);
    optionsLayout->addWidget(m_font, verticalIndex++, 1);
    optionsLayout->addWidget(fontSizeLabel, verticalIndex, 0);
    optionsLayout->addWidget(m_fontSize, verticalIndex++, 1);

    const QVector<AbstractHighlighter*> &highlighters =
            HighlighterManager::instance().getHighlighters();
    for(int i = 0; i < highlighters.size(); ++i) {
        QCheckBox *check = new QCheckBox(highlighters[i]->getOptionCheckBoxCaption(), this);
        optionsLayout->addWidget(check, verticalIndex++, 0);
        check->setChecked(Preferences::instance().getValue(
            highlighters[i]->getPropertyName(), false).toBool());
        m_highlighterCheckBox.push_back(check);
    }

    optionsLayout->setColumnMinimumWidth(0, 75);
    optionsLayout->setColumnStretch(0, 0);
    optionsLayout->setColumnStretch(1, 1);

    connect(m_font, SIGNAL(currentIndexChanged(const QString&)),
            this, SLOT(fontFamilyChanged(const QString&)));

    m_font->setCurrentFont(Preferences::instance().getEditorFont());
    fontFamilyChanged(m_font->currentText());

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addLayout(optionsLayout);
    mainLayout->addStretch(1);
    setLayout(mainLayout);
}

void EditorPage::registerPage(QListWidget *contentsWidget, QStackedWidget *pagesWidget)
{
    pagesWidget->addWidget(this);
    QListWidgetItem *editorButton = new QListWidgetItem(contentsWidget);
    editorButton->setText(tr("Editor"));
    editorButton->setTextAlignment(Qt::AlignHCenter);
    editorButton->setFlags(Qt::ItemIsEnabled);
    editorButton->setIcon(QIcon(":/resource/icon/settings_editor.png"));
}

void EditorPage::apply()
{
    Preferences::instance().setValue(
            Preferences::PROP_EDITOR_FONT_FAMILY, m_font->currentText());
    Preferences::instance().setValue(
            Preferences::PROP_EDITOR_FONT_SIZE, m_fontSize->currentText());
    const QVector<AbstractHighlighter*> &highlighters =
            HighlighterManager::instance().getHighlighters();
    for(int i = 0; i < highlighters.size(); ++i) {
        Preferences::instance().setValue(
                highlighters[i]->getPropertyName(), m_highlighterCheckBox[i]->isChecked());
    }
}

void EditorPage::fontFamilyChanged(const QString& fontName)
{
    QFontDatabase database;
    m_fontSize->clear();
    foreach (int points, database.pointSizes(fontName)) {
        m_fontSize->addItem(QString::number(points));
        if (Preferences::instance().getValue(Preferences::PROP_EDITOR_FONT_SIZE, -1).toInt() == points) {
            m_fontSize->setCurrentIndex(m_fontSize->count() - 1);
        }
    }
}

void EditorPage::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);
}