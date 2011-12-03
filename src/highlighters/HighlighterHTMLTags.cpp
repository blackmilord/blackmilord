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
#include <QVBoxLayout>
#include <QCheckBox>

#include <Preferences.h>
#include <DeviceConfiguration.h>

namespace {
    const QString PROP_ENABLED = "enabled";
}

HighlighterHTMLTags::HighlighterHTMLTags() :
    AbstractHighlighter(Preferences::instance().getHighlighterValue(guid(), PROP_ENABLED, true).toBool())
{
}

HighlighterHTMLTags::~HighlighterHTMLTags()
{
}

AbstractHighlighter::FormatListPtr HighlighterHTMLTags::highlightBlock(const QString &text)
{
    FormatListPtr result(new FormatList());
    QTextCharFormat htmlFormat;
    //TODO: add to setting this
    htmlFormat.setForeground(Qt::white);
    htmlFormat.setBackground(QBrush(QColor(0xB3,0xC6,0xF2)));
    QTextCharFormat htmlInvalidFormat;
    //TODO: add to setting this
    htmlInvalidFormat.setForeground(Qt::white);
    htmlInvalidFormat.setBackground(QBrush(QColor(0xFF,0x00,0x00)));

    QRegExp rx("<\\s*/?\\s*([^\\s>/]*)\\s*([A-Za-z]+\\s*=\\s*\"[ A-Za-z0-9]*\"\\s*)*/?\\s*>");
    rx.setMinimal(true);

    const QStringList &validTags = DeviceConfiguration::instance().getValidHTMLTags();
    int index = text.indexOf(rx);
    while (index >= 0) {
        int length = rx.matchedLength();
        if (validTags.contains(rx.capturedTexts().at(1), Qt::CaseInsensitive)) {
            result->push_back(AbstractHighlighter::CharFormat(index, index + length, htmlFormat));
        }
        else {
            qDebug() << rx.capturedTexts().at(1);
            result->push_back(AbstractHighlighter::CharFormat(index, index + length, htmlInvalidFormat));
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
    QVBoxLayout *layout = new QVBoxLayout();
    m_enableCB = new QCheckBox(QObject::tr("Enabled"));
    layout->addWidget(m_enableCB);
    resetConfigurationLayout();
    return layout;
}

void HighlighterHTMLTags::resetConfigurationLayout()
{
    m_enableCB->setChecked(m_enabled);
}

void HighlighterHTMLTags::saveSettings()
{
    Preferences::instance().setHighlighterValue(guid(), PROP_ENABLED, m_enableCB->isChecked());
}

QString HighlighterHTMLTags::guid() const
{
    return "12E0DE8E-A82E-4674-9C08-B42FBD292509";
}

void HighlighterHTMLTags::applySettings()
{
    m_enabled = Preferences::instance().getHighlighterValue(guid(), PROP_ENABLED, true).toBool();
}
