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

#ifndef BLACK_MILORD_DATABASE_RECORD_INFO_ENTRY_H
#define BLACK_MILORD_DATABASE_RECORD_INFO_ENTRY_H

#include <QtGlobal>

class DatabaseRecordInfoEntry
{
public:
    DatabaseRecordInfoEntry();
    DatabaseRecordInfoEntry next();
    void print() const;

    quint32 m_recordDataOffset;
    //Bit field.
    //The least significant four bits are used to represent the category values.
    //These are the categories used to split the databases for viewing on the screen.
    //A few of the 16 categories are pre-defined but the user can add their own.
    //There is an undefined category for use if the user or programmer hasn't set this.
    //0x10 (16 decimal) Secret record bit.
    //0x20 (32 decimal) Record in use (busy bit).
    //0x40 (64 decimal) Dirty record bit.
    //0x80 (128, unsigned decimal) Delete record on next HotSync.
    quint8  m_recordAttributes;
    //3 bytes
    quint32 m_uniqueID;

    quint32 m_length;
};


#endif /* BLACK_MILORD_DATABASE_RECORD_INFO_ENTRY_H */
