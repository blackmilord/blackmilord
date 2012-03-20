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

#include "MOBIHeader.h"
#include <QDebug>
#include <QString>
#include <QDataStream>

const QString MOBIHeader::MOBI_HEADER_INDENTIFIER("MOBI");

MOBIHeader::MOBIHeader()
{
    memset(m_identifier, 0, MOBI_HEADER_INDENTIFIER_SIZE+1);
}

MOBIHeader::~MOBIHeader()
{
}

void MOBIHeader::initForWrite()
{
    memset(m_identifier, 0, MOBI_HEADER_INDENTIFIER_SIZE+1);
    memcpy(m_identifier, MOBI_HEADER_INDENTIFIER.toAscii().data(), MOBI_HEADER_INDENTIFIER.toAscii().size());
    m_headerLength = 232;
    m_mobiType = MOBI_TYPE_HTML;
    m_textEncoding = ENCODING_UTF_8;
    m_uniqueID = 0;                     //TODO: random?
    m_fileVersion = 5;
    m_ortographicIndex = 0xFFFFFFFF;    //no index
    m_inflectionIndex = 0xFFFFFFFF;     //no index
    m_indexNames = 0xFFFFFFFF;          //no index
    m_indexKeys = 0xFFFFFFFF;           //no index
    m_extraIndex0 = 0xFFFFFFFF;         //no index
    m_extraIndex1 = 0xFFFFFFFF;         //no index
    m_extraIndex2 = 0xFFFFFFFF;         //no index
    m_extraIndex3 = 0xFFFFFFFF;         //no index
    m_extraIndex4 = 0xFFFFFFFF;         //no index
    m_extraIndex5 = 0xFFFFFFFF;         //no index
    m_firstNonTextRecordIndex = 0;      //IMPORTANT: to be set outside
    m_fullNameOffset = 0;               //IMPORTANT: to be set outside
    m_fullNameLength = 0;               //IMPORTANT: to be set outside
    m_locale = 0;
    m_inputLanguage = 0;
    m_outputLanguage = 0;
    m_minVersion = 5;
    m_firstImageRecordIndex = 0;        //IMPORTANT: to be set outside
    m_huffmanRecordOffset = 0;          //no Huffman
    m_huffmanRecordCount = 0;           //no Huffman
    m_huffmanTableOffset = 0;           //no Huffman
    m_huffmanTableLength = 0;           //no Huffman
    m_EXTHflags = 0x40;                 //EXTH header
    m_DRMOffset = 0xFFFFFFFF;           //no DRM
    m_DRMCount = 0xFFFFFFFF;            //no DRM
    m_DRMSize = 0;                      //no DRM
    m_DRMFlags = 0;                     //no DRM
    m_extraDataFlags = 0x01;            //multicharacter overlaps
    m_indexRecordOffset = 0xFFFFFFFF;   //no index record
}

bool MOBIHeader::read(QDataStream &data)
{
    qint64 recordOffset = data.device()->pos();
    data.readRawData(m_identifier, MOBI_HEADER_INDENTIFIER_SIZE);
    data >> m_headerLength;
    data >> m_mobiType;
    data >> m_textEncoding;
    data >> m_uniqueID;
    data >> m_fileVersion;
    data >> m_ortographicIndex;
    data >> m_inflectionIndex;
    data >> m_indexNames;
    data >> m_indexKeys;
    data >> m_extraIndex0;
    data >> m_extraIndex1;
    data >> m_extraIndex2;
    data >> m_extraIndex3;
    data >> m_extraIndex4;
    data >> m_extraIndex5;
    data >> m_firstNonTextRecordIndex;
    data >> m_fullNameOffset;
    data >> m_fullNameLength;
    data >> m_locale;
    data >> m_inputLanguage;
    data >> m_outputLanguage;
    data >> m_minVersion;
    data >> m_firstImageRecordIndex;
    data >> m_huffmanRecordOffset;
    data >> m_huffmanRecordCount;
    data >> m_huffmanTableOffset;
    data >> m_huffmanTableLength;
    data >> m_EXTHflags;
    //Unknown data here, skip it
    data.device()->seek(data.device()->pos() + 32);
    data >> m_DRMOffset;
    data >> m_DRMCount;
    data >> m_DRMSize;
    data >> m_DRMFlags;

    if (m_fileVersion >= 5 && m_headerLength == 228) {
        data.device()->seek(recordOffset + m_headerLength - 2);
        data >> m_extraDataFlags;
        m_indexRecordOffset = 0xFFFFFFFF;
    }
    else if (m_fileVersion >= 5 && m_headerLength == 232) {
        data.device()->seek(recordOffset + m_headerLength - 6);
        data >> m_extraDataFlags;
        data >> m_indexRecordOffset;
    }
    else {
        data.device()->seek(recordOffset + m_headerLength);
        m_extraDataFlags = 0;
        m_indexRecordOffset = 0xFFFFFFFF;
    }

#ifndef QT_NO_DEBUG_OUTPUT
    print();
#endif

    if (MOBI_HEADER_INDENTIFIER != m_identifier) {
        m_why = QObject::tr("Missing MOBI file header.");
        return false;
    }
    if (m_textEncoding != 65001 && m_textEncoding != 1252) {
        m_why = QObject::tr("Not supported encoding.");
        return false;
    }
    if (m_DRMOffset != 0xFFFFFFFF || m_DRMCount != 0xFFFFFFFF) {
        m_why = QObject::tr("Document is DRM'd.");
        return false;
    }
    if (m_huffmanRecordCount != 0 || m_huffmanRecordOffset != 0) {
        m_why = QObject::tr("Huffman encoding is not supported.");
        return false;
    }

    return true;
}

bool MOBIHeader::write(QDataStream& data)
{
    data.writeRawData(m_identifier, MOBI_HEADER_INDENTIFIER_SIZE);
    data << m_headerLength;
    data << m_mobiType;
    data << m_textEncoding;
    data << m_uniqueID ;
    data << m_fileVersion;
    data << m_ortographicIndex;
    data << m_inflectionIndex;
    data << m_indexNames;
    data << m_indexKeys;
    data << m_extraIndex0;
    data << m_extraIndex1;
    data << m_extraIndex2;
    data << m_extraIndex3;
    data << m_extraIndex4;
    data << m_extraIndex5;
    data << m_firstNonTextRecordIndex;
    data << m_fullNameOffset;
    data << m_fullNameLength;
    data << m_locale;
    data << m_inputLanguage;
    data << m_outputLanguage;
    data << m_minVersion;
    data << m_firstImageRecordIndex;
    data << m_huffmanRecordOffset;
    data << m_huffmanRecordCount;
    data << m_huffmanTableOffset;
    data << m_huffmanTableLength;
    data << m_EXTHflags;
    //Unknown data here, 32 bytes
    for (int i=0; i<32; ++i) {
        data << static_cast<quint8>(0);
    }
    data << m_DRMOffset;
    data << m_DRMCount;
    data << m_DRMSize;
    data << m_DRMFlags;
    //Unknown data here, 62 bytes
    for (int i=0; i<62; ++i) {
        data << static_cast<quint8>(0);
    }
    data << m_extraDataFlags;
    data << m_indexRecordOffset;
    return true;
}

void MOBIHeader::print() const
{
    qDebug() << "*** MOBI header ***" << "\n"
             << "MOBI header length" << m_headerLength << "\n"
             << "mobiType" << m_mobiType << "\n"
             << "textEncoding" << m_textEncoding << "\n"
             << "uniqueID" << m_uniqueID  << "\n"
             << "fileVersion" << m_fileVersion << "\n"
             << "firstNonTextRecordIndex" << m_firstNonTextRecordIndex << "\n"
             << "fullNameOffset" << m_fullNameOffset << "\n"
             << "fullNameLength" << m_fullNameLength << "\n"
             << "locale" << m_locale << "\n"
             << "inputLanguage" << m_inputLanguage << "\n"
             << "outputLanguage" << m_outputLanguage << "\n"
             << "minVersion" << m_minVersion << "\n"
             << "firstImageRecordIndex" << m_firstImageRecordIndex << "\n"
             << "huffmanRecordOffset" << m_huffmanRecordOffset << "\n"
             << "huffmanRecordCount" << m_huffmanRecordCount << "\n"
             << "huffmanTableOffset" << m_huffmanTableOffset << "\n"
             << "huffmanTableLength" << m_huffmanTableLength << "\n"
             << "EXTHflags" << m_EXTHflags << "\n"
             << "DRMOffset" << m_DRMOffset << "\n"
             << "DRMCount" << m_DRMCount << "\n"
             << "DRMSize" << m_DRMSize << "\n"
             << "DRMFlags" << m_DRMFlags << "\n"
             << "extraDataFlags" << m_extraDataFlags << "\n"
             << "indexRecordOffset" << m_indexRecordOffset;
}

QString MOBIHeader::why() const
{
    return m_why;
}

quint32 MOBIHeader::size() const
{
    return m_headerLength;
}

QString MOBIHeader::getIdentifier() const
{
    return QString(m_identifier);
}

quint32 MOBIHeader::getHeaderLength() const
{
    return m_headerLength;
}

quint32 MOBIHeader::getMobiType() const
{
    return m_mobiType;
}

quint32 MOBIHeader::getTextEncoding() const
{
    return m_textEncoding;
}

quint32 MOBIHeader::getUniqueID() const
{
    return m_uniqueID;
}

quint32 MOBIHeader::getFileVersion() const
{
    return m_fileVersion;
}

quint32 MOBIHeader::getOrtographicIndex() const
{
    return m_ortographicIndex;
}

quint32 MOBIHeader::getInflectionIndex() const
{
    return m_inflectionIndex;
}

quint32 MOBIHeader::getIndexNames() const
{
    return m_indexNames;
}

quint32 MOBIHeader::getIndexKeys() const
{
    return m_indexKeys;
}

quint32 MOBIHeader::getExtraIndex0() const
{
    return m_extraIndex0;
}

quint32 MOBIHeader::getExtraIndex1() const
{
    return m_extraIndex1;
}

quint32 MOBIHeader::getExtraIndex2() const
{
    return m_extraIndex2;
}

quint32 MOBIHeader::getExtraIndex3() const
{
    return m_extraIndex3;
}

quint32 MOBIHeader::getExtraIndex4() const
{
    return m_extraIndex4;
}

quint32 MOBIHeader::getExtraIndex5() const
{
    return m_extraIndex5;
}

quint32 MOBIHeader::getFirstNonTextRecordIndex() const
{
    return m_firstNonTextRecordIndex;
}
void MOBIHeader::setFirstNonTextRecordIndex(quint32 firstNonTextRecordIndex)
{
    m_firstNonTextRecordIndex = firstNonTextRecordIndex;
}

quint32 MOBIHeader::getFullNameOffset() const
{
    return m_fullNameOffset;
}

void MOBIHeader::setFullNameOffset(quint32 fullNameOffset)
{
    m_fullNameOffset = fullNameOffset;
}

quint32 MOBIHeader::getFullNameLength() const
{
    return m_fullNameLength;
}

void MOBIHeader::setFullNameLength(quint32 fullNameLength)
{
    m_fullNameLength = fullNameLength;
}

quint32 MOBIHeader::getLocale() const
{
    return m_locale;
}

quint32 MOBIHeader::getInputLanguage() const
{
    return m_inputLanguage;
}

quint32 MOBIHeader::getOutputLanguage() const
{
    return m_outputLanguage;
}

quint32 MOBIHeader::getMinVersion() const
{
    return m_minVersion;
}

quint32 MOBIHeader::getFirstImageRecordIndex() const
{
    return m_firstImageRecordIndex;
}

void MOBIHeader::setFirstImageRecordIndex(quint32 firstImageRecordIndex)
{
    m_firstImageRecordIndex = firstImageRecordIndex;
}

quint32 MOBIHeader::getHuffmanRecordOffset() const
{
    return m_huffmanRecordOffset;
}

quint32 MOBIHeader::getHuffmanRecordCount() const
{
    return m_huffmanRecordCount;
}

quint32 MOBIHeader::getHuffmanTableOffset() const
{
    return m_huffmanTableOffset;
}

quint32 MOBIHeader::getHuffmanTableLength() const
{
    return m_huffmanTableLength;
}

quint32 MOBIHeader::getEXTHflags() const
{
    return m_EXTHflags;
}

quint32 MOBIHeader::getDRMOffset() const
{
    return m_DRMOffset;
}

quint32 MOBIHeader::getDRMCount() const
{
    return m_DRMCount;
}

quint32 MOBIHeader::getDRMSize() const
{
    return m_DRMSize;
}

quint32 MOBIHeader::getDRMFlags() const
{
    return m_DRMFlags;
}

quint16 MOBIHeader::getExtraDataFlags() const
{
    return m_extraDataFlags;
}

quint32 MOBIHeader::getIndexRecordOffset() const
{
    return m_indexRecordOffset;
}