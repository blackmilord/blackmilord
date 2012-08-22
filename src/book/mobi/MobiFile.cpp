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

#include "MobiFile.h"
#include <QDebug>
#include <QFile>
#include <QDataStream>
#include <QTextCodec>
#include <QRegExp>
#include <QString>
#include <QStringList>
#include <QIODevice>
#include <QTemporaryFile>
#include <QDateTime>
#include <QPixmap>

#include <Preferences.h>
#include <Book.h>
#include <Dictionary.h>
#include <Formatting.h>
#include "DatabaseRecordInfoEntry.h"
#include "MobiCodec.h"

MobiFile::MobiFile()
{
}

MobiFile::~MobiFile()
{
}

bool MobiFile::openFile(const QString &fileName)
{
    qDebug() << "loading" << fileName;
    QFile file(fileName);
    if (!file.exists() || !file.open(QIODevice::ReadOnly))
    {
        Book::instance().setWhy(tr("File cannot be opened."));
        return false;
    }
    QDataStream data(&file);

    //read Database header
    if (!m_databaseHeader.read(data)) {
        Book::instance().setWhy(m_databaseHeader.why());
        file.close();
        return false;
    }

    //seek to first record
    data.device()->seek(m_databaseHeader.getRecordOffset(0));

    //read Palm DOC header
    if (!m_palmDOCHeader.read(data)) {
        Book::instance().setWhy(m_palmDOCHeader.why());
        file.close();
        return false;
    }

    //read MOBI header
    if (!m_MOBIHeader.read(data)) {
        Book::instance().setWhy(m_MOBIHeader.why());
        file.close();
        return false;
    }

    //read EXTH header if exists
    if (hasEXTH()) {
        if (!m_EXTHHeader.read(data)) {
            Book::instance().setWhy(m_EXTHHeader.why());
            file.close();
            return false;
        }
    }

    if (!readTextRecords(data)) {
        file.close();
        return false;
    }

    if (!readImageRecords(data)) {
        file.close();
        return false;
    }

    file.close();

    Book::instance().setMetadata(METADATA_CREATION_DATE, m_databaseHeader.getCreationDate());
    Book::instance().setMetadata(METADATA_MODIFICATION_DATE, m_databaseHeader.getModificationDate());
    Book::instance().setMetadata(METADATA_LAST_BACKUP_DATE, m_databaseHeader.getLastBackupDate());
    Book::instance().setMetadata(METADATA_MODIFICATION_NUMBER, m_databaseHeader.getModificationNumber());
    Book::instance().setMetadata(METADATA_VERSION, m_databaseHeader.getVersion());
    Book::instance().setMetadata(METADATA_AUTHOR, m_EXTHHeader.getAuthor());
    Book::instance().setMetadata(METADATA_ISBN, m_EXTHHeader.getIsbn());
    Book::instance().setMetadata(METADATA_PUBLISHER, m_EXTHHeader.getPublisher());
    Book::instance().setMetadata(METADATA_SUBJECT, m_EXTHHeader.getSubject());
    Book::instance().setMetadata(METADATA_DESCRIPTION, m_EXTHHeader.getDescription());

    return true;
}

bool MobiFile::newFile()
{
    return true;
}

bool MobiFile::canOpenFile()
{
    return true;
}

bool MobiFile::canSaveFile()
{
    return true;
}

bool MobiFile::saveFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadWrite)) {
        Book::instance().setWhy(tr("Cannot open output file for writing.\nFile is not saved!"));
        return false;
    }
    file.resize(0);
    QDataStream data(&file);
    bool writeOk = false;

    do {
        //Prepare data
        m_databaseHeader.initForWrite();
        m_palmDOCHeader.initForWrite();
        m_MOBIHeader.initForWrite();
        m_EXTHHeader.initForWrite();

        QList<QByteArray> textRecords = prepareTextRecords();
        int currentRecordNumber = 0;
        QString fullName = Book::instance().getMetadata(METADATA_SUBJECT).toString();
        int fullNameSize = fullName.toUtf8().size();

        m_databaseHeader.setDatabaseName(fullName);
        m_databaseHeader.setModificationDate(QDateTime::currentDateTime());
        m_databaseHeader.setLastBackupDate(Book::instance().getMetadata(METADATA_LAST_BACKUP_DATE).toDateTime());
        m_databaseHeader.setCreationDate(Book::instance().getMetadata(METADATA_CREATION_DATE).toDateTime());
        m_databaseHeader.setModificationNumber(Book::instance().getMetadata(METADATA_MODIFICATION_NUMBER).toUInt() + 1);
        m_databaseHeader.setNumberOfRecords(2 + textRecords.size()); //header record + text records + EOF record

        m_palmDOCHeader.setTextLength(encodedTextSize());
        m_palmDOCHeader.setTextRecordCount(textRecords.size());

        m_EXTHHeader.setAuthor(Book::instance().getMetadata(METADATA_AUTHOR).toString());
        m_EXTHHeader.setIsbn(Book::instance().getMetadata(METADATA_ISBN).toString());
        m_EXTHHeader.setPublisher(Book::instance().getMetadata(METADATA_PUBLISHER).toString());
        m_EXTHHeader.setSubject(Book::instance().getMetadata(METADATA_SUBJECT).toString());
        m_EXTHHeader.setDescription(Book::instance().getMetadata(METADATA_DESCRIPTION).toString());

        m_MOBIHeader.setFullNameOffset(
                m_palmDOCHeader.size() +
                m_MOBIHeader.size() +
                m_EXTHHeader.size());
        m_MOBIHeader.setFullNameLength(fullNameSize);
        m_MOBIHeader.setFirstImageRecordIndex(1 + textRecords.size());
        m_MOBIHeader.setFirstNonTextRecordIndex(1 + textRecords.size());
        m_MOBIHeader.setLastContentRecord(1 + textRecords.size() + Book::instance().getPicturesCount());

        //Write Database header
        if (!m_databaseHeader.write(data)) {
            Book::instance().setWhy(m_databaseHeader.why());
            break;
        }

        //Write header record
        m_databaseHeader.setRecordOffset(currentRecordNumber++, data.device()->pos());

        //header record: Palm DOC header
        if (!m_palmDOCHeader.write(data)) {
            Book::instance().setWhy(m_palmDOCHeader.why());
            break;
        }

        //header record: MOBI header
        if (!m_MOBIHeader.write(data)) {
            Book::instance().setWhy(m_MOBIHeader.why());
            break;
        }

        //header record: EXTH header
        if (hasEXTH()) {
            if (!m_EXTHHeader.write(data)) {
                Book::instance().setWhy(m_EXTHHeader.why());
                break;
            }
        }

        //header record: full name at the end
        data.writeRawData(fullName.toUtf8().data(), fullNameSize);
        data << static_cast<qint8>(0);
        data << static_cast<qint8>(0);
        fullNameSize += 2;
        int toWrite = (4 - (fullNameSize % 4)) % 4;
        for (int i = 0; i < toWrite; ++i) {
            data << static_cast<qint8>(0);
        }

        //here some unknown data may appear in most mobi files

        //Write text
        QByteArray newData;
        qint64 offset = data.device()->pos();
        QList<QByteArray>::ConstIterator it = textRecords.begin();
        for (; it != textRecords.end(); ++it) {
            m_databaseHeader.setRecordOffset(currentRecordNumber++, offset);
            offset += it->size();
            newData.append(*it);
        }
        data.writeRawData(newData.data(), newData.size());

        //Write EOF record
        m_databaseHeader.setRecordOffset(currentRecordNumber++, data.device()->pos());
        data << static_cast<qint8>(233);
        data << static_cast<qint8>(142);
        data << static_cast<qint8>(13);
        data << static_cast<qint8>(10);

        if (!m_databaseHeader.updateRecordInfoEntries(data)) {
            Book::instance().setWhy(m_databaseHeader.why());
            break;
        }
        writeOk = true;
    } while (0);

    if (!writeOk) {
        file.resize(0);
    }
    file.close();
    return writeOk;
}

QString MobiFile::textToWrite() const
{
    return Book::instance().getText().remove("\n");
}

quint32 MobiFile::encodedTextSize() const
{
    QTextCodec* codec = NULL;
    if (m_MOBIHeader.getTextEncoding() == MOBIHeader::ENCODING_UTF_8) {
        codec = QTextCodec::codecForName("UTF-8");
    }
    else if (m_MOBIHeader.getTextEncoding() == MOBIHeader::ENCODING_CP1252) {
        codec = QTextCodec::codecForName("Windows-1252");
    }
    if (NULL == codec) {
        return 0;
    }
    QByteArray rawData;
    QTextStream stream(&rawData);
    stream.setCodec(codec);
    stream << textToWrite();
    stream.flush();
    return rawData.size();
}

QList<QByteArray> MobiFile::prepareTextRecords() const
{
    QList<QByteArray> textRecords;
    QString text = textToWrite();

    QTextCodec* codec = NULL;
    if (m_MOBIHeader.getTextEncoding() == 65001) {
        codec = QTextCodec::codecForName("UTF-8");
    }
    else if (m_MOBIHeader.getTextEncoding() == 1252) {
        codec = QTextCodec::codecForName("Windows-1252");
    }
    if (NULL == codec) {
        return textRecords;
    }
    QByteArray overlap;
    int textIndex = 0;

    while(textIndex < text.size()) {
        QByteArray record;
        QTextStream stream(&record);
        stream.setCodec(codec);
        while (record.size() + overlap.size() < 4096 && textIndex < text.size()) {
            stream << text.at(textIndex++);
            stream.flush();
        }
        // record cannot be initialized with overlap content
        // stream's << operator is resetting it for some reason.
        if (overlap.size() > 0) {
            record.insert(0, overlap);
        }
        //next record has to copy overlapped bytes
        if (textIndex == text.size() && record.size() <= 4096) {
            record.append(static_cast<char>(0));
            overlap.clear();
        }
        else {
            overlap = record.right(record.size() - 4096);
            record.append(static_cast<char>(record.size() - 4096));
        }
        textRecords.push_back(record);
    }
    return textRecords;
}

bool MobiFile::readImageRecords(QDataStream &data)
{
    bool loaded;
    int count = 0;
    int record = m_MOBIHeader.getFirstImageRecordIndex();
    do {
        if (m_databaseHeader.getNumberOfRecords() <= record) {
            break;
        }
        data.device()->seek(m_databaseHeader.getRecordOffset(record));
        quint32 length = m_databaseHeader.getRecordLength(record);
        QByteArray imageData = data.device()->read(length);
        if (imageData.size() != static_cast<int>(length)) {
            return false;
        }
        //validate image
        loaded = QPixmap().loadFromData(imageData);
        if (loaded) {
            Book::instance().addPicture(BookPicture(imageData));
            ++count;
        }
        ++record;
    } while (loaded);
    qDebug() << "loaded" << count << "images.";
    return true;
}

bool MobiFile::readTextRecords(QDataStream &data)
{
    QTextCodec* codec = NULL;
    if (m_MOBIHeader.getTextEncoding() == 65001) {
        codec = QTextCodec::codecForName("UTF-8");
    }
    else if (m_MOBIHeader.getTextEncoding() == 1252) {
        codec = QTextCodec::codecForName("Windows-1252");
    }
    if (NULL == codec) {
        Book::instance().setWhy(tr("Not supported encoding."));
        return false;
    }
    quint32 length = 0;
    qint64 overlap = 0;
    QByteArray rawData;
    rawData.reserve(m_palmDOCHeader.getTextLength());
    for (quint16 i = 1; i <= m_palmDOCHeader.getTextRecordCount(); ++i) {
        //qDebug() << "loading record" << i;
        length = m_databaseHeader.getRecordLength(i);
        data.device()->seek(m_databaseHeader.getRecordOffset(i));

        QByteArray newData = data.device()->read(length);

        //remove trailing entries
        for (int i = 0; i < trailingEntriesCount(); ++i ) {
            int bytesRead = 0;
            int dataSize = 0;
            quint8 value = 0;
            while (0 == (0x80 & value)) {
                value = static_cast<quint8>(newData.right(1).at(0));
                newData.chop(1);
                dataSize *= 128;
                dataSize += (0x7F & value);
                ++bytesRead;
            }
            newData.chop(dataSize - bytesRead);
        }
        if (hasOverlaps()) {
            overlap = static_cast<quint8>(newData.right(1).at(0));
            overlap &= 0x03;
            newData.chop(1 + overlap);
        }

        if (m_palmDOCHeader.getCompression() == PalmDOCHeader::COMPRESSION_NONE) {
            rawData.append(newData);
        }
        else if (m_palmDOCHeader.getCompression() == PalmDOCHeader::COMPRESSION_PALMDOC) {
            rawData.append(MobiCodec::DecodePalmDoc(newData));
        }
        else {
            Book::instance().setWhy(tr("Not supported compression."));
            return false;
        }
    }
    QTextStream text(rawData);
    text.setCodec(codec);
    Book::instance().setText(Formatting::formatHTMLContent(text.readAll()));
    return true;
}

const DatabaseHeader& MobiFile::getDatabaseHeader() const
{
    return m_databaseHeader;
}

const PalmDOCHeader& MobiFile::getPalmDOCHeader() const
{
    return m_palmDOCHeader;
}

const MOBIHeader& MobiFile::getMOBIHeader() const
{
    return m_MOBIHeader;
}

const EXTHHeader& MobiFile::getEXTHHeader() const
{
    return m_EXTHHeader;
}

bool MobiFile::hasEXTH() const
{
    return (m_MOBIHeader.getEXTHflags() & 0x40) != 0;
}

bool MobiFile::hasOverlaps() const
{
    return (m_MOBIHeader.getExtraDataFlags() & 0x01) != 0;
}

int MobiFile::trailingEntriesCount() const
{
    int count = 0;
    for (int i = 1; i < 16; ++i) {
        if (m_MOBIHeader.getExtraDataFlags() & (1 << i)) {
            ++count;
        }
    }
    return count;
}
