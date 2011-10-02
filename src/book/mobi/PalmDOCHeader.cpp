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

#include "PalmDOCHeader.h"
#include <QDebug>

PalmDOCHeader::PalmDOCHeader()
    : m_compression(COMPRESSION_NONE)
    , m_textLength(0)
    , m_textRecordCount(0)
    , m_maxRecordSize(MAX_RECORD_SIZE)
    , m_encryption(ENCRIPTION_NONE)
{
}

void PalmDOCHeader::initForWrite()
{
    m_compression = COMPRESSION_NONE;
    m_textLength = 0;
    m_textRecordCount = 0;
    m_maxRecordSize = MAX_RECORD_SIZE;
    m_encryption = ENCRIPTION_NONE;
}

bool PalmDOCHeader::read(QDataStream &data)
{
    quint16 tmp16;
    data >> m_compression;
    data >> tmp16;
    if (tmp16 != 0) {
        m_why = QObject::tr("Invalid header.");
        return false;
    }
    data >> m_textLength;
    data >> m_textRecordCount;
    data >> m_maxRecordSize;
    data >> m_encryption;
    data >> tmp16;

    print();

    if (COMPRESSION_NONE != m_compression && COMPRESSION_PALMDOC != m_compression) {
        m_why = QObject::tr("Compressed files are not fully supported yet.");
        return false;
    }

    if (ENCRIPTION_NONE != m_encryption) {
        m_why = QObject::tr("Encrypted files are not supported.");
        return false;
    }

    return true;
}

bool PalmDOCHeader::write(QDataStream& data)
{
    data << m_compression;
    data << static_cast<quint16>(0);
    data << m_textLength;
    data << m_textRecordCount;
    data << m_maxRecordSize;
    data << m_encryption;
    data << static_cast<quint16>(0);
    return true;
}

void PalmDOCHeader::print() const
{
    qDebug() << "*** PalmDoc header ***" << "\n"
             << "compression" << m_compression << "\n"
             << "textLength" << m_textLength << "\n"
             << "textRecordCount" << m_textRecordCount << "\n"
             << "maxRecordSize" << m_maxRecordSize << "\n"
             << "encryption" << m_encryption;
}

QString PalmDOCHeader::why() const
{
    return m_why;
}

quint32 PalmDOCHeader::size() const
{
    return 16;
}

void PalmDOCHeader::setTextLength(quint32 textLength)
{
    m_textLength = textLength;
}

quint32 PalmDOCHeader::getTextLength() const
{
    return m_textLength;
}

void PalmDOCHeader::setTextRecordCount(quint16 textRecordCount)
{
    m_textRecordCount = textRecordCount;
}

quint16 PalmDOCHeader::getTextRecordCount() const
{
    return m_textRecordCount;
}

quint16 PalmDOCHeader::getCompression() const
{
    return m_compression;
}

quint16 PalmDOCHeader::getEncryption() const
{
    return m_encryption;
}

quint16 PalmDOCHeader::getMaxRecordSize() const
{
    return m_maxRecordSize;
}