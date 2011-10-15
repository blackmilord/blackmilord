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
#include <QMutexLocker>

#include <Preferences.h>
#include <DeviceConfiguration.h>

HighlighterHTMLTags::HighlighterHTMLTags()
{
}

HighlighterHTMLTags::~HighlighterHTMLTags()
{
    QMutexLocker lock(&m_mutex);
}

QVector<AbstractHighlighter::CharFormat> HighlighterHTMLTags::highlightBlock(const QString &text)
{
    QMutexLocker lock(&m_mutex);
    QVector<AbstractHighlighter::CharFormat> result;
    if (!Preferences::instance().getValue(Preferences::PROP_HIGHLIGHTER_HTML_TAGS, false).toBool()) {
        return result;
    }
    QTextCharFormat htmlFormat;
    //TODO: add to setting this
    htmlFormat.setForeground(Qt::white);
    htmlFormat.setBackground(QBrush(QColor(0xB3,0xC6,0xF2)));
    QTextCharFormat htmlInvalidFormat;
    //TODO: add to setting this
    htmlInvalidFormat.setForeground(Qt::white);
    htmlInvalidFormat.setBackground(QBrush(QColor(0xFF,0x00,0x00)));
    QTextCharFormat htmlFormatWithSpaces;
    //TODO: add to setting this
    htmlFormatWithSpaces.setForeground(Qt::white);
    htmlFormatWithSpaces.setBackground(QBrush(QColor(0x7F,0x00,0x00)));

    QRegExp rx("<(\\s*)/?(\\s*)([A-Za-z0-9]*)(\\s)*/?(\\s)*>");
    rx.setMinimal(true);

    QStringList validTags = DeviceConfiguration::instance().getValidHTMLTags();
    int index = text.indexOf(rx);
    while (index >= 0) {
        int length = rx.matchedLength();
        if (validTags.contains(rx.capturedTexts().at(3), Qt::CaseInsensitive)) {
            if (rx.capturedTexts().at(1).isEmpty() &&
                rx.capturedTexts().at(2).isEmpty() &&
                rx.capturedTexts().at(4).isEmpty() &&
                rx.capturedTexts().at(5).isEmpty())
            {
                result.push_back(AbstractHighlighter::CharFormat(index, length, htmlFormat));
            }
            else {
                result.push_back(AbstractHighlighter::CharFormat(index, length, htmlFormatWithSpaces));
            }
        }
        else {
            result.push_back(AbstractHighlighter::CharFormat(index, length, htmlInvalidFormat));
        }
        index = text.indexOf(rx, index + length);
    }
    return result;
}

QString HighlighterHTMLTags::getOptionCheckBoxCaption() const
{
    QMutexLocker lock(&m_mutex);
    return QObject::tr("Highlight HTML tags");
}

Preferences::PropertyName HighlighterHTMLTags::getPropertyName() const
{
    QMutexLocker lock(&m_mutex);
    return Preferences::PROP_HIGHLIGHTER_HTML_TAGS;
}

void HighlighterHTMLTags::applySettings()
{
}