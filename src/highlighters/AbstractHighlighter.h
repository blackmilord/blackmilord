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

#ifndef BLACK_MILORD_ABSTRACT_HIGHLIGHTER_H
#define BLACK_MILORD_ABSTRACT_HIGHLIGHTER_H

#include <QObject>
#include <QTextCharFormat>
#include <QMutex>
#include <Preferences.h>

class QString;
class QLayout;

class AbstractHighlighter
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

    explicit AbstractHighlighter(bool enabled = false);
    virtual ~AbstractHighlighter();

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

    /**
     * This funcion provides inteface to configure a highlighter.
     * @return layout with configuration dialog
     */
    virtual QLayout* configurationLayout() = 0;

    /**
     * Reset layout to current settins;
     */
    virtual void resetConfigurationLayout() = 0;

    /**
     * This function saves configuration provided
     * by user in @see configurationLayout() in application preferences.
     * This function should not modify internal settins, but store it in application preferences.
     * Internal settins are updated by @see applySettings() function.
     * It guaranties thread-save behaviour.
     */
    virtual void saveSettings() = 0;

    /**
     * This function is called when highlighter should reread configuration
     * from application preferences. It is guaranted it it's not called during @see highlightBlock() execution.
     */
    virtual void applySettings() = 0;

    virtual bool isEnabled();

    /**
     * Function provides unique highlighter's GUID.
     * @return highlighter's GUID.
     */
    virtual QString guid() const = 0;

    /**
     * Function provides highlighter's name.
     * @return highlighter's name.
     */
    virtual QString name() const = 0;

protected:
    bool m_enabled;
};

#endif /* BLACK_MILORD_ABSTRACT_HIGHLIGHTER_H */
