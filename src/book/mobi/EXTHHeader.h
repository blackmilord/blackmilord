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

#ifndef BLACK_MILORD_EXTH_HEADER_H
#define BLACK_MILORD_EXTH_HEADER_H

class QString;
class QDataStream;

#include <QtGlobal>
#include <QVector>
#include <QString>
#include "EXTHHeaderEntry.h"

class EXTHHeader
{
    static const QString EXTH_MOBI_HEADER_INDENTIFIER;
    static const int EXTH_MOBI_HEADER_INDENTIFIER_SIZE = 4;
public:

    enum ExthRecordType {
        EXTH_RECORD_TYPE_AUTHOR = 100,
        EXTH_RECORD_TYPE_PUBLISHER = 101,
        EXTH_RECORD_TYPE_DESCRIPTION = 103,
        EXTH_RECORD_TYPE_ISBN = 104,
        EXTH_RECORD_TYPE_SUBJECT = 105
    };

    EXTHHeader();
    ~EXTHHeader();
    bool read(QDataStream &data);
    bool write(QDataStream &data);
    void initForWrite();

    void print() const;
    QString why() const;
    quint32 size() const;

    QString getIdentifier() const;

    quint32 getHeaderLength() const;

    quint32 getRecordCount() const;

    void setAuthor(const QString &author);
    QString getAuthor() const;

    void setPublisher(const QString &publisher);
    QString getPublisher() const;

    void setDescription(const QString &description);
    QString getDescription() const;

    void setSubject(const QString &subject);
    QString getSubject() const;

    void setIsbn(const QString &ISBN);
    QString getIsbn() const;

private:
    typedef QVector<EXTHHeaderEntry> RecordList;
    typedef RecordList::iterator RecordListIterator;
    typedef RecordList::const_iterator RecordListConstIterator;

    char m_identifier[EXTH_MOBI_HEADER_INDENTIFIER_SIZE+1];
    quint32 m_headerLength;
    quint32 m_recordCount;
    RecordList m_records;

    QString m_why;

    quint32 recalculateSize() const;
    bool hasRecord(quint32 type) const;
    EXTHHeaderEntry getRecord(quint32 type) const;
    EXTHHeaderEntry createRecord(quint32 type) const;
    void storeRecord(EXTHHeaderEntry &entry);
    void removeRecord(quint32 type);

};

#endif /* BLACK_MILORD_EXTH_HEADER_H */
