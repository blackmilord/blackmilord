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

#include "BookPicture.h"
#include <QPixmap>

BookPicture::BookPicture(const QPixmap &picture) :
    m_original(NULL),
    m_current(new QPixmap(picture))
{
}

BookPicture::~BookPicture()
{
}


const QPixmap* BookPicture::getOriginalPicture() const
{
    return m_original.isNull() ? m_current.data() : m_original.data();
}

const QPixmap* BookPicture::getCurrentPicture() const
{
    return m_current.data();
}

void BookPicture::setPicture(const QPixmap &picture)
{
    if (m_original.isNull()) {
        m_original = m_current;
    }
    m_current = QSharedPointer<QPixmap>(new QPixmap(picture));
}

bool BookPicture::isModified() const
{
    //This only tells setPicture was called. Images may be exactly the same.
    //For more reasons this should be checked in setPicture if picture is still the same and ignore storing.
    return m_original.data() != m_current.data();
}

QString BookPicture::getHtmlIndex() const
{
    return m_htmlIndex;
}

void BookPicture::setHtmlIndex(const QString &htmlIndex)
{
    m_htmlIndex = htmlIndex;
}

QString BookPicture::getFilePath() const
{
    return m_filePath;
}

void BookPicture::setFilePath(const QString &filePath)
{
    m_filePath = filePath;
}
