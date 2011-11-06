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

#ifndef BLACK_MILORD_BOOK_H
#define BLACK_MILORD_BOOK_H

#include <QDateTime>
#include <QList>
#include <MobiFileObject.h>
#include "MetadataEnum.h"
#include "BookPicture.h"

class QString;
class QVariant;

class Book : public QObject
{
    Q_OBJECT
    Book();
    ~Book();

public:
    static Book& instance();

    //file management
    bool openFile(const QString &fileName);
    bool saveFile();
    bool closeFile();
    bool newFile();
    bool isFileOpened();

    //text accessors
    QString getText() const;
    void setText(const QString& text);
    //pictures accessors
    int getPicturesCount() const;
    void addPicture(const BookPicture & picture);
    BookPicture getPicture(int index) const;

    //book metadata
    QVariant getMetadata(MetaData metadata) const;

    QString getFileName() const;
    void setFileName(const QString fileName);

    QString getWhy() const;

public slots:
    void setMetadata(MetaData metadata, const QVariant &data);
    void setWhy(const QString &why);

signals:
    void fileLoaded();
    void fileSaved();
    void fileClosed();
    void fileCreated();
    void textChanged();
    void metadataChanged();

private:
    void reset();

    //book metadata
    QList<BookPicture> m_pictures;
    QString m_author;
    QString m_publisher;
    QString m_description;
    QString m_isbn;
    QString m_subject;
    quint16 m_version;
    QDateTime m_creationDate;
    QDateTime m_modificationDate;
    QDateTime m_lastBackupDate;
    quint32 m_modificationNumber;

    //internal metadata
    bool m_fileOpened;
    QString m_fileName;
    QString m_why;
};

#endif /* BLACK_MILORD_BOOK_H */
