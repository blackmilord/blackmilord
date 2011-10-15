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

#ifndef OPEN_EBOOK_EDITOR_HIGHLIGHT_EVENT_RESPONSE_H
#define OPEN_EBOOK_EDITOR_HIGHLIGHT_EVENT_RESPONSE_H

#include <QEvent>
#include <QString>
#include <QVector>
#include <QSharedPointer>

#include "AbstractHighlighter.h"

class HighlightEventResponse :
    public QEvent
{
public:
    HighlightEventResponse(int blockIndex);
    virtual ~HighlightEventResponse();
    int getBlockIndex() const;
    AbstractHighlighter::MultiFormatListPtr getResults() const;
    static QEvent::Type getType() { return m_type; }
private:
    int m_blockIndex;
    QSharedPointer<QVector<AbstractHighlighter::FormatList> > m_results;
protected:
    static QEvent::Type m_type;
};

#endif /* OPEN_EBOOK_EDITOR_HIGHLIGHT_EVENT_RESPONSE_H */
