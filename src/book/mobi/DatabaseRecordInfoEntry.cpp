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

#include "DatabaseRecordInfoEntry.h"
#include <QDebug>

DatabaseRecordInfoEntry::DatabaseRecordInfoEntry() :
    m_recordDataOffset(0),
    m_recordAttributes(0),
    m_uniqueID(0),
    m_length(0)
{
}

DatabaseRecordInfoEntry DatabaseRecordInfoEntry::next()
{
    //Mobipocket reader adds 2 on this field for consecutive records
    DatabaseRecordInfoEntry result;
    result.m_uniqueID = 2 + m_uniqueID;
    return result;
}

void DatabaseRecordInfoEntry::print() const
{
    qDebug() << "DataOffset" << m_recordDataOffset
             << "Attributes" << m_recordAttributes
             << "uniqueID" << m_uniqueID
             << "length" << m_length;
}