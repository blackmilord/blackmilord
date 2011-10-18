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

#ifndef BLACK_MILORD_PALM_DOC_HEADER_H
#define BLACK_MILORD_PALM_DOC_HEADER_H

#include <QtGlobal>
#include <QString>

class QDataStream;

class PalmDOCHeader
{
public:
    enum Compression {
        COMPRESSION_NONE = 1,
        COMPRESSION_PALMDOC = 2,
        COMPRESSION_HUFF_CDIC = 17480
    };

    enum Encryption {
        ENCRIPTION_NONE = 0,
        ENCRIPTION_OLD_MOBIPOCKET = 1,
        ENCRIPTION_MOBIPOCKET = 2
    };

    static const int MAX_RECORD_SIZE = 4096;

    PalmDOCHeader();
    bool read(QDataStream &data);
    bool write(QDataStream &data);
    void initForWrite();
    void print() const;
    QString why() const;
    quint32 size() const;

    void setTextLength(quint32 textLength);
    quint32 getTextLength() const;
    void setTextRecordCount(quint16 textRecordCount);
    quint16 getTextRecordCount() const;
    quint16 getCompression() const;
    quint16 getEncryption() const;
    quint16 getMaxRecordSize() const;

private:
    //1 = no compression
    //2 = PalmDOC compression
    //17480 = HUFF/CDIC compression
    quint16 m_compression;
    //Uncompressed length of the entire text of the book
    quint32 m_textLength;
    //number of PDB records used for the text of the book.
    quint16 m_textRecordCount;
    //Maximum size of each record containing text, always 4096
    quint16 m_maxRecordSize;
    //0 = no encryption
    //1 = Old Mobipocket Encryption
    //2 = Mobipocket Encryption
    quint16 m_encryption;

    QString m_why;
};

#endif /* BLACK_MILORD_PALM_DOC_HEADER_H */
