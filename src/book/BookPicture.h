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

#ifndef BLACK_MILORD_BOOK_PICTURE_H
#define BLACK_MILORD_BOOK_PICTURE_H

#include <QString>
#include <QSharedPointer>
#include <QByteArray>

class QPixmap;

class BookPicture
{
public:
    explicit BookPicture(const QByteArray &pictureData);
    virtual ~BookPicture();
    QPixmap getOriginalPicture() const;
    QPixmap getCurrentPicture() const;
    QByteArray getOriginalPictureData() const;
    QByteArray getCurrentPictureData() const;
    void setPicture(const QPixmap &picture);
    void setPicture(const QByteArray &pictureData);
    bool isModified() const;
    QString getHtmlIndex() const;
    void setHtmlIndex(const QString &htmlIndex);
    QString getFilePath() const;
    void setFilePath(const QString &filePath);

private:
    QSharedPointer<QByteArray> m_original;
    QSharedPointer<QByteArray> m_current;
    QString m_htmlIndex;
    QString m_filePath;
};

#endif /* BLACK_MILORD_BOOK_PICTURE_H */