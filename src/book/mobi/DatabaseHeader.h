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

#ifndef OPEN_EBOOK_EDITOR_DATABASE_HEADER_H
#define	OPEN_EBOOK_EDITOR_DATABASE_HEADER_H

#include <QVector>

class QDataStream;
class QString;
class DatabaseRecordInfoEntry;
class QDateTime;

class DatabaseHeader
{
private:
    static const int DATABASE_NAME_LENGTH = 32;
    static const int TYPE_LENGTH = 4;
    static const int CREATOR_LENGTH = 4;
    static const QString TYPE_BOOK;
    static const QString CREATOR_MOBI;
public:
    DatabaseHeader();
    virtual ~DatabaseHeader();
    bool read(QDataStream &data);
    bool write(QDataStream &data);
    bool updateRecordInfoEntries(QDataStream &data);
    void initForWrite();
    quint32 getRecordOffset(int recordIndex) const;
    void setRecordOffset(int recordIndex, quint32 recordOffset);
    quint32 getRecordLength(int recordIndex) const;

    QString getDatabaseName() const;
    void setDatabaseName(const QString &databaseName);

    quint16 getAttributes() const;

    quint16 getVersion() const;

    QDateTime getCreationDate() const;
    void setCreationDate(const QDateTime &date);

    QDateTime getModificationDate() const;
    void setModificationDate(const QDateTime &date);

    QDateTime getLastBackupDate() const;
    void setLastBackupDate(const QDateTime &date);

    quint32 getModificationNumber() const;
    void setModificationNumber(quint32 modificationNumber);

    quint32 getAppInfoID() const;

    quint32 getSortInfoID() const;

    QString getType() const;

    QString getCreator() const;

    quint32 getUniqueIDseed() const;

    quint32 getNextRecordListID() const;

    quint16 getNumberOfRecords() const;
    void setNumberOfRecords(quint16 numberOfRecords);

    void print() const;
    QString why() const;
    quint32 size() const;

private:
    char m_databaseName[DATABASE_NAME_LENGTH];
    //Bit field
    //0x0002 Read-Only
    //0x0004 Dirty AppInfoArea
    //0x0008 Backup this database (i.e. no conduit exists)
    //0x0010 (16 decimal) Okay to install newer over existing copy, if present on PalmPilot
    //0x0020 (32 decimal) Force the PalmPilot to reset after this database is installed
    //0x0040 (64 decimal) Don't allow copy of file to be beamed to other Pilot.
    quint16 m_attributes;
    quint16 m_version;
    //No. of seconds since start of January 1, 1904.
    quint32 m_creationDate;
    //No. of seconds since start of January 1, 1904.
    quint32 m_modificationDate;
    //No. of seconds since start of January 1, 1904.
    quint32 m_lastBackupDate;
    quint32 m_modificationNumber;
    //offset to start of Application Info (if present) or null
    quint32 m_appInfoID;
    //offset to start of Sort Info (if present) or null
    quint32 m_sortInfoID;
    char m_type[TYPE_LENGTH+1];
    char m_creator[CREATOR_LENGTH+1];
    //	used internally to identify record
    quint32 m_uniqueIDseed;
    //Only used when in-memory on Palm OS. Always set to zero in stored files.
    quint32 m_nextRecordListID;
    quint16 m_numberOfRecords;
    QVector<DatabaseRecordInfoEntry> m_recordInfoEntries;

    QString m_why;
};

#endif /* OPEN_EBOOK_EDITOR_DATABASE_HEADER_H */
