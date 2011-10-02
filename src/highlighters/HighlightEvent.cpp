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

#include "HighlightEvent.h"

QEvent::Type HighlightEvent::m_type =
    static_cast<QEvent::Type>(QEvent::registerEventType());

HighlightEvent::HighlightEvent(int blockIndex, const QString &text) :
    QEvent(m_type),
    m_blockIndex(blockIndex),
    m_text(text)
{
}

HighlightEvent::~HighlightEvent()
{
}

QString HighlightEvent::getText() const
{
    return m_text;
}

int HighlightEvent::getBlockIndex() const
{
    return m_blockIndex;
}