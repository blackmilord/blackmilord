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

#include "HighlightEventResponse.h"

QEvent::Type HighlightEventResponse::m_type =
    static_cast<QEvent::Type>(QEvent::registerEventType());

HighlightEventResponse::HighlightEventResponse(int blockIndex) :
    QEvent(m_type),
    m_blockIndex(blockIndex),
    m_results(new QVector<AbstractHighlighter::FormatList>())
{
}

HighlightEventResponse::~HighlightEventResponse()
{
}

AbstractHighlighter::MultiFormatListPtr HighlightEventResponse::getResults() const
{
    return m_results;
}

int HighlightEventResponse::getBlockIndex() const
{
    return m_blockIndex;
}