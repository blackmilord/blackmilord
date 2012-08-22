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

#ifndef BLACK_MILORD_MOBI_HEADER_H
#define BLACK_MILORD_MOBI_HEADER_H

#include <QString>
#include <QtGlobal>

class QDataStream;

class MOBIHeader
{
    static const QString MOBI_HEADER_INDENTIFIER;
    static const int MOBI_HEADER_INDENTIFIER_SIZE = 4;
public:

    enum mobiType {
        MOBI_TYPE_MOBIPOCKET_BOOK = 2,
        MOBI_TYPE_PALMDOC_BOOK = 3,
        MOBI_TYPE_AUDIO = 4,
        MOBI_TYPE_WORD = 514,
        MOBI_TYPE_TEXT = 517,
        MOBI_TYPE_HTML = 518
    };

    enum textEncoding {
        ENCODING_CP1252 = 1252,
        ENCODING_UTF_8 = 65001
    };

    MOBIHeader();
    virtual ~MOBIHeader();

    void initForWrite();
    bool read(QDataStream &data);
    bool write(QDataStream &data);
    void print() const;
    QString why() const;
    quint32 size() const;

    QString getIdentifier() const;

    quint32 getHeaderLength() const;

    quint32 getMobiType() const;

    quint32 getTextEncoding() const;

    quint32 getUniqueID() const;

    quint32 getFileVersion() const;

    quint32 getOrtographicIndex() const;

    quint32 getInflectionIndex() const;

    quint32 getIndexNames() const;

    quint32 getIndexKeys() const;

    quint32 getExtraIndex0() const;

    quint32 getExtraIndex1() const;

    quint32 getExtraIndex2() const;

    quint32 getExtraIndex3() const;

    quint32 getExtraIndex4() const;

    quint32 getExtraIndex5() const;

    quint32 getFirstNonTextRecordIndex() const;
    void setFirstNonTextRecordIndex(quint32 firstNonTextRecordIndex);

    quint32 getFullNameOffset() const;
    void setFullNameOffset(quint32 fullNameOffset);

    quint32 getFullNameLength() const;
    void setFullNameLength(quint32 fullNameLength);

    quint32 getLocale() const;

    quint32 getInputLanguage() const;

    quint32 getOutputLanguage() const;

    quint32 getMinVersion() const;

    quint32 getFirstImageRecordIndex() const;
    void setFirstImageRecordIndex(quint32 firstImageRecordIndex);

    quint32 getHuffmanRecordOffset() const;

    quint32 getHuffmanRecordCount() const;

    quint32 getHuffmanTableOffset() const;

    quint32 getHuffmanTableLength() const;

    quint32 getEXTHflags() const;

    quint32 getDRMOffset() const;

    quint32 getDRMCount() const;

    quint32 getDRMSize() const;

    quint32 getDRMFlags() const;

    quint16 getFirstContentRecord() const;

    quint16 getLastContentRecord() const;
    void setLastContentRecord(quint16 lastContentRecord);

    quint32 getFcisRecordIndex() const;
    void setFcisRecordIndex(quint32 fcisRecordIndex);

    quint32 getFcisRecordCount();

    quint32 getFlisRecordIndex() const;
    void setFlisRecordIndex(quint32 flisRecordIndex);

    quint32 getFlisRecordCount();

    quint32 getExtraDataFlags() const;

    quint32 getIndexRecordOffset() const;

protected:
    void initForRead();

private:
    char m_identifier[MOBI_HEADER_INDENTIFIER_SIZE+1];
    quint32 m_headerLength;
    quint32 m_mobiType;
    quint32 m_textEncoding;
    quint32 m_uniqueID;
    quint32 m_fileVersion;
    quint32 m_ortographicIndex;
    quint32 m_inflectionIndex;
    quint32 m_indexNames;
    quint32 m_indexKeys;
    quint32 m_extraIndex0;
    quint32 m_extraIndex1;
    quint32 m_extraIndex2;
    quint32 m_extraIndex3;
    quint32 m_extraIndex4;
    quint32 m_extraIndex5;
    quint32 m_firstNonTextRecordIndex;
    quint32 m_fullNameOffset;
    quint32 m_fullNameLength;
    quint32 m_locale;
    quint32 m_inputLanguage;
    quint32 m_outputLanguage;
    quint32 m_minVersion;
    quint32 m_firstImageRecordIndex;
    quint32 m_huffmanRecordOffset;
    quint32 m_huffmanRecordCount;
    quint32 m_huffmanTableOffset;
    quint32 m_huffmanTableLength;
    quint32 m_EXTHflags;
    quint32 m_DRMOffset;
    quint32 m_DRMCount;
    quint32 m_DRMSize;
    quint32 m_DRMFlags;
    quint32 m_unknown1;  //use 0x00000000 on save
    quint32 m_unknown2;  //use 0x00000000 on save
    quint32 m_unknown3;  //use 0x00000000 on save
    quint16 m_firstContentRecord;
    quint16 m_lastContentRecord;
    quint32 m_unknown4;  //use 0x00000001 on save
    quint32 m_fcisRecordIndex;
    quint32 m_fcisRecordCount;
    quint32 m_flisRecordIndex;
    quint32 m_flisRecordCount;
    quint32 m_unknown5;  //use 0x00000000 on save
    quint32 m_unknown6;  //use 0x00000000 on save
    quint32 m_unknown7;  //use 0xFFFFFFFF on save
    quint32 m_unknown8;  //use 0x00000000 on save
    quint32 m_unknown9;  //use 0xFFFFFFFF on save
    quint32 m_unknown10; //use 0xFFFFFFFF on save
    quint32 m_extraDataFlags;
    quint32 m_indexRecordOffset;

    QString m_why;
};

#endif /* BLACK_MILORD_MOBI_HEADER_H */
