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

#ifndef OPEN_EBOOK_EDITOR_ABSTRACT_HIGHLIGHTER_H
#define OPEN_EBOOK_EDITOR_ABSTRACT_HIGHLIGHTER_H

#include <QObject>
#include <Preferences.h>
#include <QTextCharFormat>

class QString;

class AbstractHighlighter
{
public:

    struct CharFormat
    {
        CharFormat() :
            m_start(0),
            m_count(0)
        {
        }

        CharFormat(int start, int count, const QTextCharFormat &format) :
            m_format(format),
            m_start(start),
            m_count(count)
        {
        }

        QTextCharFormat m_format;
        int m_start;
        int m_count;
    };

    typedef QVector<CharFormat> FormatList;
    typedef FormatList::const_iterator FormatListIterator;

    AbstractHighlighter();
    virtual ~AbstractHighlighter();

    virtual FormatList highlightBlock(const QString &text) = 0;
    virtual QString getOptionCheckBoxCaption() const = 0;
    virtual Preferences::PropertyName getPropertyName() const = 0;
    virtual void applySettings() = 0;
};

#endif