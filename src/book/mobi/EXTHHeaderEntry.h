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

#ifndef BLACK_MILORD_EXTH_HEADER_ENTRY_H
#define BLACK_MILORD_EXTH_HEADER_ENTRY_H

#include <QtGlobal>

class EXTHHeaderEntry
{
public:
    EXTHHeaderEntry();
    EXTHHeaderEntry(const EXTHHeaderEntry &entry);
    EXTHHeaderEntry& operator=(const EXTHHeaderEntry &entry);
    ~EXTHHeaderEntry();
    bool read(QDataStream &data);
    bool write(QDataStream &data);
    void print() const;

    quint32 getType() const;
    void setType(quint32 type);

    quint32 getLength() const;
    void setLength(quint32 length);

    void setValue(const QString &value);
    QString getValue() const;

private:
    quint32 m_recordType;
    quint32 m_recordLength;
    char *m_data;
};

#endif /* BLACK_MILORD_EXTH_HEADER_ENTRY_H */
