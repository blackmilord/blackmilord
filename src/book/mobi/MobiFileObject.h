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

#ifndef BLACK_MILORD_MOBIFILE_H
#define BLACK_MILORD_MOBIFILE_H

#include <QObject>
#include <QList>
#include <QVector>
#include <QPair>
#include <QStringList>
#include <QList>
#include <QByteArray>

#include "DatabaseHeader.h"
#include "PalmDOCHeader.h"
#include "MOBIHeader.h"
#include "EXTHHeader.h"
#include <AbstractBook.h>

class QDataStream;
class QByteArray;
class WriteState;

class MobiFileObject : public QObject, public AbstractBook
{
    Q_OBJECT
public:
    bool openFile(const QString &fileName);
    bool saveFile(const QString &fileName);
    bool newFile();

    const DatabaseHeader& getDatabaseHeader() const;
    const PalmDOCHeader& getPalmDOCHeader() const;
    const MOBIHeader& getMOBIHeader() const;
    const EXTHHeader& getEXTHHeader() const;

    bool hasEXTH() const;

private:
    DatabaseHeader m_databaseHeader;
    PalmDOCHeader m_palmDOCHeader;
    MOBIHeader m_MOBIHeader;
    EXTHHeader m_EXTHHeader;

    MobiFileObject();
    virtual ~MobiFileObject();

    QString textToWrite() const;
    QList<QByteArray> prepareTextRecords() const;
    quint32 encodedTextSize() const;

    bool readTextRecords(QDataStream &data);
    bool readImageRecords(QDataStream &data);

    bool hasOverlaps() const;
    int trailingEntriesCount() const;

    friend class AbstractBookFactory;
    friend class HeaderDiff;
};

#endif /* BLACK_MILORD_MOBIFILE_H */

