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

#include "HighlighterHTMLTags.h"
#include <QDebug>
#include <QString>
#include <QRegExp>
#include <QBrush>
#include <QColor>
#include <QGridLayout>
#include <QCheckBox>
#include <QColorDialog>
#include <QPushButton>
#include <QtPlugin>

#include <Preferences.h>
#include <DeviceConfiguration.h>

namespace {
    const QString GUID = "12E0DE8E-A82E-4674-9C08-B42FBD292509";
    const QString PROP_ENABLED = "enabled";
    const QString PROP_NORMAL_FOREGROUND = "normal_foreground_color";
    const QString PROP_NORMAL_BACKGROUND = "normal_background_color";
    const QString PROP_INVALID_FOREGROUND = "invalid_foreground_color";
    const QString PROP_INVALID_BACKGROUND = "invalid_background_color";
    const QString PROP_NORMAL_FOREGROUND_ENABLED = "normal_foreground_color_enabled";
    const QString PROP_NORMAL_BACKGROUND_ENABLED = "normal_background_color_enabled";
    const QString PROP_INVALID_FOREGROUND_ENABLED = "invalid_foreground_color_enabled";
    const QString PROP_INVALID_BACKGROUND_ENABLED = "invalid_background_color_enabled";
}

Q_EXPORT_PLUGIN2(highlighter_html, HighlighterHTMLTags)

HighlighterHTMLTags::HighlighterHTMLTags() :
    PluginHighlighter(Preferences::instance().getHighlighterValue(GUID, PROP_ENABLED, true).toBool())
{
}

HighlighterHTMLTags::~HighlighterHTMLTags()
{
}

PluginHighlighter::FormatListPtr HighlighterHTMLTags::highlightBlock(const QString &text)
{
    FormatListPtr result(new FormatList());
    QTextCharFormat htmlFormat;
    if (m_hasForegroundNormal) {
        htmlFormat.setForeground(m_foregroundNormal);
    }
    if (m_hasBackgroundNormal) {
        htmlFormat.setBackground(QBrush(m_backgroundNormal));
    }
    QTextCharFormat htmlInvalidFormat;
    if (m_hasForegroundInvalid) {
        htmlInvalidFormat.setForeground(m_foregroundInvalid);
    }
    if (m_hasBackgroundInvalid) {
        htmlInvalidFormat.setBackground(QBrush(m_backgroundInvalid));
    }

    QRegExp rx("<\\s*/?\\s*([^\\s>/]*)\\s*([A-Za-z]+\\s*=\\s*\"[ A-Za-z0-9]*\"\\s*)*/?\\s*>");
    rx.setMinimal(true);

    const QStringList &validTags = DeviceConfiguration::instance().getValidHTMLTags();
    int index = text.indexOf(rx);
    while (index >= 0) {
        int length = rx.matchedLength();
        if (validTags.contains(rx.capturedTexts().at(1), Qt::CaseInsensitive)) {
            result->push_back(PluginHighlighter::CharFormat(index, index + length, htmlFormat));
        }
        else {
            qDebug() << rx.capturedTexts().at(1);
            result->push_back(PluginHighlighter::CharFormat(index, index + length, htmlInvalidFormat));
        }
        index = text.indexOf(rx, index + length);
    }
    return result;
}

QString HighlighterHTMLTags::name() const
{
    return QObject::tr("HTML tags");
}

QLayout* HighlighterHTMLTags::configurationLayout()
{
    QGridLayout *layout = new QGridLayout();

    layout->addWidget(m_enableCB =
        new QCheckBox(QObject::tr("Enabled")), 0, 0);

    layout->addWidget(m_colorForegroundNormalCB =
        new QCheckBox(QObject::tr("HTML tag font color")), 1, 0);
    layout->addWidget(m_colorBackgroundNormalCB =
        new QCheckBox(QObject::tr("HTML tag font background color")), 2, 0);
    layout->addWidget(m_colorForegroundInvalidCB =
        new QCheckBox(QObject::tr("Invalid HTML tag font color")), 3, 0);
    layout->addWidget(m_colorBackgroundInvalidCB =
        new QCheckBox(QObject::tr("Invalid HTML tag font background color")), 4, 0);

    layout->addWidget(m_settingColorForegroundNormalPB =
        new QPushButton(), 1, 1);
    layout->addWidget(m_settingColorBackgroundNormalPB =
        new QPushButton(), 2, 1);
    layout->addWidget(m_settingColorForegroundInvalidPB =
        new QPushButton(), 3, 1);
    layout->addWidget(m_settingColorBackgroundInvalidPB =
        new QPushButton(), 4, 1);

    connect(m_enableCB, SIGNAL(released()), this, SLOT(updateConfigurationLayoutState()));
    connect(m_colorForegroundNormalCB, SIGNAL(released()), this, SLOT(updateConfigurationLayoutState()));
    connect(m_colorBackgroundNormalCB, SIGNAL(released()), this, SLOT(updateConfigurationLayoutState()));
    connect(m_colorForegroundInvalidCB, SIGNAL(released()), this, SLOT(updateConfigurationLayoutState()));
    connect(m_colorBackgroundInvalidCB, SIGNAL(released()), this, SLOT(updateConfigurationLayoutState()));

    connect(m_settingColorForegroundNormalPB, SIGNAL(pressed()), this, SLOT(pickColor()));
    connect(m_settingColorBackgroundNormalPB, SIGNAL(pressed()), this, SLOT(pickColor()));
    connect(m_settingColorForegroundInvalidPB, SIGNAL(pressed()), this, SLOT(pickColor()));
    connect(m_settingColorBackgroundInvalidPB, SIGNAL(pressed()), this, SLOT(pickColor()));

    resetConfigurationLayout();
    return layout;
}

void HighlighterHTMLTags::pickColor()
{
    QColor &color =
        sender() == m_settingColorForegroundNormalPB ? m_foregroundNormal :
        sender() == m_settingColorBackgroundNormalPB ? m_backgroundNormal :
        sender() == m_settingColorForegroundInvalidPB ? m_foregroundInvalid : m_foregroundInvalid;
    const QColor &newColor = QColorDialog::getColor(color, NULL);
    if (newColor.isValid()) {
        color = newColor;
        updateConfigurationLayoutColors();
    }
}

void HighlighterHTMLTags::updateConfigurationLayoutState()
{
    m_colorForegroundNormalCB->setEnabled(m_enableCB->isChecked());
    m_colorBackgroundNormalCB->setEnabled(m_enableCB->isChecked());
    m_colorForegroundInvalidCB->setEnabled(m_enableCB->isChecked());
    m_colorBackgroundInvalidCB->setEnabled(m_enableCB->isChecked());
    m_settingColorForegroundNormalPB->setVisible(m_enableCB->isChecked() && m_colorForegroundNormalCB->isChecked());
    m_settingColorBackgroundNormalPB->setVisible(m_enableCB->isChecked() && m_colorBackgroundNormalCB->isChecked());
    m_settingColorForegroundInvalidPB->setVisible(m_enableCB->isChecked() && m_colorForegroundInvalidCB->isChecked());
    m_settingColorBackgroundInvalidPB->setVisible(m_enableCB->isChecked() && m_colorBackgroundInvalidCB->isChecked());
}

void HighlighterHTMLTags::resetConfigurationLayout()
{
    m_enableCB->setChecked(m_enabled);
    m_colorForegroundNormalCB->setChecked(m_hasForegroundNormal);
    m_colorBackgroundNormalCB->setChecked(m_hasBackgroundNormal);
    m_colorForegroundInvalidCB->setChecked(m_hasForegroundInvalid);
    m_colorBackgroundInvalidCB->setChecked(m_hasBackgroundInvalid);
    updateConfigurationLayoutColors();
    updateConfigurationLayoutState();
}

void HighlighterHTMLTags::updateConfigurationLayoutColors()
{
    m_settingColorForegroundNormalPB->setStyleSheet(
        "QPushButton{background-color:" + m_foregroundNormal.name() + ";}");
    m_settingColorBackgroundNormalPB->setStyleSheet(
        "QPushButton{background-color:" + m_backgroundNormal.name() + ";}");
    m_settingColorForegroundInvalidPB->setStyleSheet(
        "QPushButton{background-color:" + m_foregroundInvalid.name() + ";}");
    m_settingColorBackgroundInvalidPB->setStyleSheet(
        "QPushButton{background-color:" + m_backgroundInvalid.name() + ";}");
}

void HighlighterHTMLTags::saveSettings()
{
    Preferences::instance().setHighlighterValue(GUID, PROP_ENABLED, m_enableCB->isChecked());
    Preferences::instance().setHighlighterValue(GUID, PROP_INVALID_BACKGROUND_ENABLED, m_colorBackgroundInvalidCB->isChecked());
    Preferences::instance().setHighlighterValue(GUID, PROP_INVALID_FOREGROUND_ENABLED, m_colorForegroundInvalidCB->isChecked());
    Preferences::instance().setHighlighterValue(GUID, PROP_NORMAL_BACKGROUND_ENABLED, m_colorBackgroundNormalCB->isChecked());
    Preferences::instance().setHighlighterValue(GUID, PROP_NORMAL_FOREGROUND_ENABLED, m_colorForegroundNormalCB->isChecked());
    Preferences::instance().setHighlighterValue(GUID, PROP_NORMAL_FOREGROUND, m_foregroundNormal.name());
    Preferences::instance().setHighlighterValue(GUID, PROP_NORMAL_BACKGROUND, m_backgroundNormal.name());
    Preferences::instance().setHighlighterValue(GUID, PROP_INVALID_FOREGROUND, m_foregroundInvalid.name());
    Preferences::instance().setHighlighterValue(GUID, PROP_INVALID_BACKGROUND, m_backgroundInvalid.name());
}

QString HighlighterHTMLTags::guid() const
{
    return GUID;
}

void HighlighterHTMLTags::applySettings()
{
    m_enabled = Preferences::instance().getHighlighterValue(GUID, PROP_ENABLED, true).toBool();
    m_hasForegroundNormal = Preferences::instance().getHighlighterValue(GUID, PROP_NORMAL_FOREGROUND_ENABLED, true).toBool();
    m_hasBackgroundNormal = Preferences::instance().getHighlighterValue(GUID, PROP_NORMAL_BACKGROUND_ENABLED, false).toBool();
    m_hasForegroundInvalid = Preferences::instance().getHighlighterValue(GUID, PROP_INVALID_FOREGROUND_ENABLED, true).toBool();
    m_hasBackgroundInvalid = Preferences::instance().getHighlighterValue(GUID, PROP_INVALID_BACKGROUND_ENABLED, false).toBool();
    m_foregroundNormal = QColor(Preferences::instance().getHighlighterValue(GUID, PROP_NORMAL_FOREGROUND, "#0000FF").toString());
    m_backgroundNormal = QColor(Preferences::instance().getHighlighterValue(GUID, PROP_NORMAL_BACKGROUND, "#FFFFFF").toString());
    m_foregroundInvalid = QColor(Preferences::instance().getHighlighterValue(GUID, PROP_INVALID_FOREGROUND, "#FF0000").toString());
    m_backgroundInvalid = QColor(Preferences::instance().getHighlighterValue(GUID, PROP_INVALID_BACKGROUND, "#FFFFFF").toString());
}
