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

#ifndef BLACK_MILORD_PLUGIN_HIGHLIGHTER_H
#define BLACK_MILORD_PLUGIN_HIGHLIGHTER_H

#include <QTextCharFormat>

#include "Plugin.h"

class PluginHighlighter : public Plugin
{
public:

    struct CharFormat
    {
        CharFormat()
        {
            Q_ASSERT(false);
        }

        CharFormat(int start, int end, const QTextCharFormat &format) :
            m_format(format),
            m_start(start),
            m_end(end)
        {
        }

        QTextCharFormat m_format;
        int m_start;
        int m_end;
    };

    typedef QVector<CharFormat> FormatList;
    typedef QSharedPointer<FormatList> FormatListPtr;

    explicit PluginHighlighter(bool enabled = false) : Plugin(enabled)
    {
    }

    virtual ~PluginHighlighter()
    {
    }

    /**
     * This function is called every time a line of text needs highlighting.
     * WARNING: This function is called from non-main thread.
     *          It should base only on internal settings.
     *          This function should not call anything.
     *          It is quaranted @see applySettins() is not called
     *          during execution of this function.
     * @param text Text to highlight.
     * @return List of text formatting.
     */
    virtual FormatListPtr highlightBlock(const QString &text) = 0;
};

Q_DECLARE_INTERFACE(PluginHighlighter, "org.blackmilord.Plugin.Highlighter/1.0");

#endif /* BLACK_MILORD_PLUGIN_HIGHLIGHTER_H */
