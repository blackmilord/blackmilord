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

#include <QDebug>
#include <QDataStream>
#include <QIODevice>
#include <QString>
#include <QDateTime>
#include <QDate>
#include <string.h>

#include "DatabaseHeader.h"
#include "DatabaseRecordInfoEntry.h"

const QString DatabaseHeader::TYPE_BOOK = QString("BOOK");
const QString DatabaseHeader::CREATOR_MOBI = QString("MOBI");


DatabaseHeader::DatabaseHeader()
{
    memset(m_databaseName, 0, DATABASE_NAME_LENGTH);
    memset(m_type, 0, TYPE_LENGTH + 1);
    memset(m_creator, 0, CREATOR_LENGTH + 1);
}

DatabaseHeader::~DatabaseHeader()
{
}

bool DatabaseHeader::read(QDataStream &data)
{
    data.readRawData(m_databaseName, DATABASE_NAME_LENGTH);
    data >> m_attributes;
    data >> m_version;
    data >> m_creationDate;
    data >> m_modificationDate;
    data >> m_lastBackupDate;
    data >> m_modificationNumber;
    data >> m_appInfoID;
    data >> m_sortInfoID;
    data.readRawData(m_type, TYPE_LENGTH);
    data.readRawData(m_creator, CREATOR_LENGTH);
    data >> m_uniqueIDseed;
    data >> m_nextRecordListID;
    data >> m_numberOfRecords;

    quint8 tmp8;
    for (int i=0; i<m_numberOfRecords; ++i) {
        DatabaseRecordInfoEntry entry;
        data >> entry.m_recordDataOffset;
        data >> entry.m_recordAttributes;
        data >> tmp8;
        entry.m_uniqueID = tmp8 << 16;
        data >> tmp8;
        entry.m_uniqueID += tmp8 << 8;
        data >> tmp8;
        entry.m_uniqueID += tmp8;
        m_recordInfoEntries.push_back(entry);
        //entry.print();
    }
    quint16 tmp16;
    data >> tmp16;
    if (tmp16 != 0) {
        m_why = QObject::tr("Invalid header.");
        return false;
    }
    if (m_type != TYPE_BOOK || m_creator != CREATOR_MOBI) {
        m_why = QObject::tr("This is not a Mobipocket document.");
        return false;
    }
    //TODO: it's not correct, but this is not supported ATM
    if (m_appInfoID != 0 || m_sortInfoID != 0) {
        m_why = QObject::tr("Invalid database header.\nappInfoID or sortInfoID is set.\nIt is not supported yet.\nPlease submit yout file to fix it.");
        return false;
    }
    print();
    return true;
}

bool DatabaseHeader::write(QDataStream& data)
{
    data.writeRawData(m_databaseName, DATABASE_NAME_LENGTH);
    data << m_attributes;
    data << m_version;
    data << m_creationDate;
    data << m_modificationDate;
    data << m_lastBackupDate;
    data << m_modificationNumber;
    data << m_appInfoID;
    data << m_sortInfoID;
    data.writeRawData(m_type, TYPE_LENGTH);
    data.writeRawData(m_creator, CREATOR_LENGTH);
    data << m_uniqueIDseed;
    data << m_nextRecordListID;
    data << m_numberOfRecords;
    for (int i=0; i<m_numberOfRecords; ++i) {
        data << m_recordInfoEntries[i].m_recordDataOffset;
        data << m_recordInfoEntries[i].m_recordAttributes;
        data << static_cast<quint8>(m_recordInfoEntries[i].m_uniqueID >> 16 & 0xFF);
        data << static_cast<quint8>(m_recordInfoEntries[i].m_uniqueID >> 8 & 0xFF);
        data << static_cast<quint8>(m_recordInfoEntries[i].m_uniqueID & 0xFF);
    }
    data << static_cast<quint16>(0); //Gap to data.
    return true;
}

bool DatabaseHeader::updateRecordInfoEntries(QDataStream &data)
{
    qint64 pos = data.device()->pos();
    data.device()->seek(78);
    for (int i=0; i<m_numberOfRecords; ++i) {
        data << m_recordInfoEntries[i].m_recordDataOffset;
        data << m_recordInfoEntries[i].m_recordAttributes;
        data << static_cast<quint8>(m_recordInfoEntries[i].m_uniqueID >> 16 & 0xFF);
        data << static_cast<quint8>(m_recordInfoEntries[i].m_uniqueID >> 8 & 0xFF);
        data << static_cast<quint8>(m_recordInfoEntries[i].m_uniqueID & 0xFF);
    }
    data.device()->seek(pos);
    return true;
}

void DatabaseHeader::initForWrite()
{
    memset(m_databaseName, 0, DATABASE_NAME_LENGTH);
    m_attributes = 0;
    m_version = 0;
    m_creationDate = 0;
    m_modificationDate = 0;
    m_lastBackupDate = 0;
    m_modificationNumber = 0;
    m_appInfoID = 0;
    m_sortInfoID = 0;
    Q_ASSERT(TYPE_BOOK.toAscii().size() == 4);
    Q_ASSERT(CREATOR_MOBI.toAscii().size() == 4);
    memcpy(m_type, TYPE_BOOK.toAscii().data(), TYPE_BOOK.toAscii().size());
    memcpy(m_creator, CREATOR_MOBI.toAscii().data(), CREATOR_MOBI.toAscii().size());
    m_uniqueIDseed = 0; //TODO: make random?
    m_nextRecordListID = 0;
    m_numberOfRecords = 0;
    m_recordInfoEntries.clear();
}

quint32 DatabaseHeader::getRecordOffset(int recordIndex) const
{
    Q_ASSERT(recordIndex < m_recordInfoEntries.size());
    return m_recordInfoEntries[recordIndex].m_recordDataOffset;
}

void DatabaseHeader::setRecordOffset(int recordIndex, quint32 recordOffset)
{
    m_recordInfoEntries[recordIndex].m_recordDataOffset = recordOffset;
}

quint32 DatabaseHeader::getRecordLength(int recordIndex) const
{
    Q_ASSERT(recordIndex + 1 < m_recordInfoEntries.size());
    return m_recordInfoEntries[recordIndex + 1].m_recordDataOffset -
            m_recordInfoEntries[recordIndex].m_recordDataOffset;
}



QString DatabaseHeader::getDatabaseName() const
{
    return QString(m_databaseName);
}

void DatabaseHeader::setDatabaseName(const QString &databaseName)
{
    QByteArray data = databaseName.toAscii().left(31);
    memset(m_databaseName, 0, DATABASE_NAME_LENGTH);
    memcpy(m_databaseName, data.data(), data.size());
}

quint16 DatabaseHeader::getAttributes() const
{
    return m_attributes;
}

quint16 DatabaseHeader::getVersion() const
{
    return m_version;
}

QDateTime DatabaseHeader::getCreationDate() const
{
    //If the time has the top bit set, it's an unsigned 32-bit number counting from 1st Jan 1904
    //If the time has the top bit clear, it's a signed 32-bit number counting from 1st Jan 1970.
    int year = m_creationDate & 0x80000000 ? 1904 : 1970;
    return QDateTime(QDate(year, 1, 1)).addSecs(m_creationDate);
}

void DatabaseHeader::setCreationDate(const QDateTime &date)
{
    QDateTime refDate(QDate(1970, 1, 1));
    m_creationDate = refDate.secsTo(date);
}

QDateTime DatabaseHeader::getModificationDate() const
{
    //If the time has the top bit set, it's an unsigned 32-bit number counting from 1st Jan 1904
    //If the time has the top bit clear, it's a signed 32-bit number counting from 1st Jan 1970.
    int year = m_modificationDate & 0x80000000 ? 1904 : 1970;
    return QDateTime(QDate(year, 1, 1)).addSecs(m_modificationDate);
}

void DatabaseHeader::setModificationDate(const QDateTime &date)
{
    QDateTime refDate(QDate(1970, 1, 1));
    m_modificationDate = refDate.secsTo(date);
}

QDateTime DatabaseHeader::getLastBackupDate() const
{
    //If the time has the top bit set, it's an unsigned 32-bit number counting from 1st Jan 1904
    //If the time has the top bit clear, it's a signed 32-bit number counting from 1st Jan 1970.
    int year = m_lastBackupDate & 0x80000000 ? 1904 : 1970;
    return QDateTime(QDate(year, 1, 1)).addSecs(m_lastBackupDate);
}

void DatabaseHeader::setLastBackupDate(const QDateTime &date)
{
    QDateTime refDate(QDate(1970, 1, 1));
    m_lastBackupDate = refDate.secsTo(date);
}

quint32 DatabaseHeader::getModificationNumber() const
{
    return m_modificationNumber;
}

void DatabaseHeader::setModificationNumber(quint32 modificationNumber)
{
    m_modificationNumber = modificationNumber;
}

quint32 DatabaseHeader::getAppInfoID() const
{
    return m_appInfoID;
}

quint32 DatabaseHeader::getSortInfoID() const
{
    return m_sortInfoID;
}

QString DatabaseHeader::getType() const
{
    return QString(m_type);
}

QString DatabaseHeader::getCreator() const
{
    return QString(m_creator);
}

quint32 DatabaseHeader::getUniqueIDseed() const
{
    return m_uniqueIDseed;
}

quint32 DatabaseHeader::getNextRecordListID() const
{
    return m_nextRecordListID;
}

quint16 DatabaseHeader::getNumberOfRecords() const
{
    Q_ASSERT(m_numberOfRecords == m_recordInfoEntries.size());
    return m_numberOfRecords;
}

void DatabaseHeader::setNumberOfRecords(quint16 numberOfRecords)
{
    m_numberOfRecords = numberOfRecords;
    m_recordInfoEntries.clear();
    DatabaseRecordInfoEntry entry;
    for (quint16 i=0; i<m_numberOfRecords; ++i) {
        m_recordInfoEntries.push_back(entry);
        entry = entry.next();
    }
}

void DatabaseHeader::print() const
{
    qDebug() << "*** Database header ***" << "\n"
             << "databaseName" << m_databaseName << "\n"
             << "attributes" << m_attributes << "\n"
             << "version" << m_version << "\n"
             << "creationDate" << m_creationDate << getCreationDate() << "\n"
             << "modificationDate" << m_modificationDate << getModificationDate() << "\n"
             << "lastBackupDate" << m_lastBackupDate << getLastBackupDate() << "\n"
             << "modificationNumber" << m_modificationNumber << "\n"
             << "appInfoID" << m_appInfoID << "\n"
             << "sortInfoID" << m_sortInfoID << "\n"
             << "type" << m_type << "\n"
             << "creator" << m_creator << "\n"
             << "uniqueIDseed" << m_uniqueIDseed << "\n"
             << "nextRecordListID" << m_nextRecordListID << "\n"
             << "numberOfRecords" << m_numberOfRecords;
}

QString DatabaseHeader::why() const
{
    return m_why;
}

quint32 DatabaseHeader::size() const
{
    return 80 + 8 * m_numberOfRecords;
}