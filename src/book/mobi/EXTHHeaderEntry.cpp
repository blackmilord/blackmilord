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

#include "EXTHHeaderEntry.h"
#include <QDataStream>
#include <QDebug>

EXTHHeaderEntry::EXTHHeaderEntry() :
    m_recordType(0),
    m_recordLength(8),
    m_data(NULL)
{
}

EXTHHeaderEntry::EXTHHeaderEntry(const EXTHHeaderEntry &entry) :
    m_recordType(entry.m_recordType),
    m_recordLength(entry.m_recordLength)
{
    m_data = new char[m_recordLength-8];
    memcpy(m_data, entry.m_data, m_recordLength-8);
}

EXTHHeaderEntry& EXTHHeaderEntry::operator=(const EXTHHeaderEntry &entry)
{
    m_recordType = entry.m_recordType;
    m_recordLength = entry.m_recordLength;
    m_data = new char[m_recordLength-8];
    memcpy(m_data, entry.m_data, m_recordLength-8);
    return *this;
}

EXTHHeaderEntry::~EXTHHeaderEntry()
{
    delete [] m_data;
}

bool EXTHHeaderEntry::read(QDataStream &data)
{
    data >> m_recordType;
    data >> m_recordLength;
    m_data = new char[m_recordLength-8];
    data.readRawData(m_data, m_recordLength-8);

    print();

    return true;
}

bool EXTHHeaderEntry::write(QDataStream &data)
{
    data << m_recordType;
    data << m_recordLength;
    data.writeRawData(m_data, m_recordLength-8);
    return true;
}

void EXTHHeaderEntry::print() const
{
    qDebug() << "recordType" << m_recordType << "\n"
             << "recordLength" << m_recordLength << "\n";
}

quint32 EXTHHeaderEntry::getType() const
{
    return m_recordType;
}

void EXTHHeaderEntry::setType(quint32 type)
{
    m_recordType = type;
}

quint32 EXTHHeaderEntry::getLength() const
{
    return m_recordLength;
}

void EXTHHeaderEntry::setLength(quint32 length)
{
    m_recordLength = length;
}

void EXTHHeaderEntry::setValue(const QString &value)
{
    Q_ASSERT(m_data == NULL);
    m_data = new char[value.toAscii().size()];
    memcpy(m_data, value.toAscii().data(), value.toAscii().size());
    m_recordLength = 8 + value.toAscii().size();
}

QString EXTHHeaderEntry::getValue() const
{
    return QString(QByteArray(m_data, m_recordLength - 8));
}