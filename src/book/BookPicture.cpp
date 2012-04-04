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
#include <QBuffer>

BookPicture::BookPicture(const QByteArray &pictureData) :
    m_original(NULL),
    m_current(new QByteArray(pictureData))
{
}

BookPicture::~BookPicture()
{
}


QPixmap BookPicture::getOriginalPicture() const
{
    Q_ASSERT(NULL != m_current.data());
    QPixmap image;
    image.loadFromData(m_original.isNull() ? *m_current : *m_original);
    return image;
}

QPixmap BookPicture::getCurrentPicture() const
{
    Q_ASSERT(NULL != m_current.data());
    QPixmap image;
    image.loadFromData(*m_current);
    return image;
}

QByteArray BookPicture::getOriginalPictureData() const
{
    return m_original.isNull() ? *m_current : *m_original;
}

QByteArray BookPicture::getCurrentPictureData() const
{
    return *m_current;
}


void BookPicture::setPicture(const QPixmap &picture)
{
    if (m_original.isNull()) {
        m_original = m_current;
    }
    m_current = QSharedPointer<QByteArray>(new QByteArray());
    QBuffer buffer(m_current.data());
    buffer.open(QIODevice::WriteOnly);
    //TODO: check if smaller than 63kb
    picture.save(&buffer, "JPG");
}

void BookPicture::setPicture(const QByteArray &pictureData)
{
    if (m_original.isNull()) {
        m_original = m_current;
    }
    m_current = QSharedPointer<QByteArray>(new QByteArray(pictureData));
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
