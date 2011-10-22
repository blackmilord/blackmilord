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

#include <Preferences.h>
#include <DeviceConfiguration.h>

HighlighterHTMLTags::HighlighterHTMLTags()
{
    applySettings();
}

HighlighterHTMLTags::~HighlighterHTMLTags()
{
}

QVector<AbstractHighlighter::CharFormat> HighlighterHTMLTags::highlightBlock(const QString &text)
{
    QVector<AbstractHighlighter::CharFormat> result;
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

    QStringList validTags = DeviceConfiguration::instance().getValidHTMLTags();
    int index = text.indexOf(rx);
    while (index >= 0) {
        int length = rx.matchedLength();
        if (validTags.contains(rx.capturedTexts().at(1), Qt::CaseInsensitive)) {
            result.push_back(AbstractHighlighter::CharFormat(index, length, htmlFormat));
        }
        else {
            qDebug() << rx.capturedTexts().at(1);
            result.push_back(AbstractHighlighter::CharFormat(index, length, htmlInvalidFormat));
        }
        index = text.indexOf(rx, index + length);
    }
    return result;
}

QString HighlighterHTMLTags::getOptionCheckBoxCaption() const
{
    return QObject::tr("Highlight HTML tags");
}

Preferences::PropertyName HighlighterHTMLTags::getPropertyName() const
{
    return Preferences::PROP_HIGHLIGHTER_HTML_TAGS;
}

void HighlighterHTMLTags::applySettings()
{
    m_enabled = Preferences::instance().getValue(Preferences::PROP_HIGHLIGHTER_HTML_TAGS, false).toBool();
}