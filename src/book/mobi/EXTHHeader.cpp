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

#include "EXTHHeader.h"
#include <QDebug>
#include <QString>
#include <QDataStream>

const QString EXTHHeader::EXTH_MOBI_HEADER_INDENTIFIER("EXTH");

EXTHHeader::EXTHHeader()
{
    memset(m_identifier, 0, EXTH_MOBI_HEADER_INDENTIFIER_SIZE+1);
}

EXTHHeader::~EXTHHeader()
{
}

void EXTHHeader::initForWrite()
{
    memcpy(m_identifier, EXTH_MOBI_HEADER_INDENTIFIER.toAscii().data(), EXTH_MOBI_HEADER_INDENTIFIER.toAscii().size());
    m_recordCount = 0;
    m_records.clear();
    m_headerLength = recalculateSize();
}

bool EXTHHeader::read(QDataStream &data)
{
    data.readRawData(m_identifier, EXTH_MOBI_HEADER_INDENTIFIER_SIZE);
    if (EXTH_MOBI_HEADER_INDENTIFIER != m_identifier) {
        m_why = QObject::tr("Missing EXTH MOBI file header.");
        return false;
    }
    data >> m_headerLength;
    data >> m_recordCount;

    for (quint32 i=0; i<m_recordCount; ++i) {
        EXTHHeaderEntry entry;
        entry.read(data);
        m_records.push_back(entry);
    }

    print();

    return true;
}

bool EXTHHeader::write(QDataStream &data)
{
    quint32 written = data.device()->pos();
    data.writeRawData(m_identifier, EXTH_MOBI_HEADER_INDENTIFIER_SIZE);
    data << m_headerLength;
    data << m_records.size();
    for (int i = 0; i < m_records.size(); ++i) {
        m_records[i].write(data);
    }
    written = data.device()->pos() - written;
    while (written++ < m_headerLength) {
        data << static_cast<quint8>(0);
    }
    return true;
}

void EXTHHeader::print() const
{
    qDebug() << "*** EXTH header ***" << "\n"
             << "headerLength" << m_headerLength << "\n"
             << "recordCount" << m_recordCount << "\n";
}

QString EXTHHeader::why() const
{
    return m_why;
}

quint32 EXTHHeader::size() const
{
    return m_headerLength;
}

quint32 EXTHHeader::recalculateSize() const
{
    quint32 headerLength = 12;
    for (int i = 0; i < m_records.size(); ++i) {
        headerLength += m_records[i].getLength();
    }
    while (headerLength % 4) {
        ++headerLength;
    }
    return headerLength;
}

bool EXTHHeader::hasRecord(quint32 type) const
{
    RecordListConstIterator it = m_records.begin();
    for(; it != m_records.end(); ++it) {
        if (it->getType() == type) {
            return true;
        }
    }
    return false;
}

EXTHHeaderEntry EXTHHeader::getRecord(quint32 type) const
{
    RecordListConstIterator it = m_records.begin();
    for(; it != m_records.end(); ++it) {
        if (it->getType() == type) {
            return *it;
        }
    }
    Q_ASSERT(false);
    return EXTHHeaderEntry();
}

EXTHHeaderEntry EXTHHeader::createRecord(quint32 type) const
{
    EXTHHeaderEntry result;
    result.setType(type);
    return result;
}

void EXTHHeader::storeRecord(EXTHHeaderEntry &entry)
{
    removeRecord(entry.getType());
    m_records.push_back(entry);
    m_recordCount = m_records.size();
}

void EXTHHeader::removeRecord(quint32 type)
{
    RecordListIterator it = m_records.begin();
    for(; it != m_records.end(); ++it) {
        if (it->getType() == type) {
            m_records.erase(it);
            m_recordCount = m_records.size();
            return;
        }
    }
}

QString EXTHHeader::getIdentifier() const
{
    return QString(m_identifier);
}

quint32 EXTHHeader::getHeaderLength() const
{
    return m_headerLength;
}

quint32 EXTHHeader::getRecordCount() const
{
    return m_recordCount;
}

void EXTHHeader::setAuthor(const QString &author)
{
    if (author.isEmpty()) {
        removeRecord(EXTH_RECORD_TYPE_AUTHOR);
        return;
    }
    EXTHHeaderEntry entry = createRecord(EXTH_RECORD_TYPE_AUTHOR);
    entry.setValue(author);
    storeRecord(entry);
}

QString EXTHHeader::getAuthor() const
{
    if (hasRecord(EXTH_RECORD_TYPE_AUTHOR)) {
        return getRecord(EXTH_RECORD_TYPE_AUTHOR).getValue();
    }
    return QString();
}

void EXTHHeader::setPublisher(const QString &publisher)
{
    if (publisher.isEmpty()) {
        removeRecord(EXTH_RECORD_TYPE_PUBLISHER);
        return;
    }
    EXTHHeaderEntry entry = createRecord(EXTH_RECORD_TYPE_PUBLISHER);
    entry.setValue(publisher);
    storeRecord(entry);
}

QString EXTHHeader::getPublisher() const
{
    if (hasRecord(EXTH_RECORD_TYPE_PUBLISHER)) {
        return getRecord(EXTH_RECORD_TYPE_PUBLISHER).getValue();
    }
    return QString();
}

void EXTHHeader::setDescription(const QString &description)
{
    if (description.isEmpty()) {
        removeRecord(EXTH_RECORD_TYPE_DESCRIPTION);
        return;
    }
    EXTHHeaderEntry entry = createRecord(EXTH_RECORD_TYPE_DESCRIPTION);
    entry.setValue(description);
    storeRecord(entry);
}

QString EXTHHeader::getDescription() const
{
    if (hasRecord(EXTH_RECORD_TYPE_DESCRIPTION)) {
        return getRecord(EXTH_RECORD_TYPE_DESCRIPTION).getValue();
    }
    return QString();
}

void EXTHHeader::setSubject(const QString &subject)
{
    if (subject.isEmpty()) {
        removeRecord(EXTH_RECORD_TYPE_SUBJECT);
        return;
    }
    EXTHHeaderEntry entry = createRecord(EXTH_RECORD_TYPE_SUBJECT);
    entry.setValue(subject);
    storeRecord(entry);
}

QString EXTHHeader::getSubject() const
{
    if (hasRecord(EXTH_RECORD_TYPE_SUBJECT)) {
        return getRecord(EXTH_RECORD_TYPE_SUBJECT).getValue();
    }
    return QString();
}

void EXTHHeader::setIsbn(const QString &ISBN)
{
    if (ISBN.isEmpty()) {
        removeRecord(EXTH_RECORD_TYPE_ISBN);
        return;
    }
    EXTHHeaderEntry entry = createRecord(EXTH_RECORD_TYPE_ISBN);
    entry.setValue(ISBN);
    storeRecord(entry);
}

QString EXTHHeader::getIsbn() const
{
    if (hasRecord(EXTH_RECORD_TYPE_ISBN)) {
        return getRecord(EXTH_RECORD_TYPE_ISBN).getValue();
    }
    return QString();
}
