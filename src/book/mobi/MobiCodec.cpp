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

#include "MobiCodec.h"
#include <QDebug>
#include <QByteArray>

QByteArray MobiCodec::DecodePalmDoc(const QByteArray &compressed)
{
    QByteArray result;
    QByteArray::ConstIterator it = compressed.begin();
    for (; it != compressed.end(); ++it) {
        if (static_cast<quint8>(*it) == 0) {
            result.push_back(*it);
        }
        else if (static_cast<quint8>(*it) >= 0x01 &&
                 static_cast<quint8>(*it) <= 0x08)
        {
            int count = static_cast<quint8>(*it);
            for (int i = 0; i < count; ++i) {
                ++it;
                Q_ASSERT(it != compressed.end());
                result.push_back(*it);
            }
        }
        else if (static_cast<quint8>(*it) >= 0x09 &&
                 static_cast<quint8>(*it) <= 0x7F)
        {
            result.push_back(*it);
        }
        else if (static_cast<quint8>(*it) >= 0x80 &&
                 static_cast<quint8>(*it) <= 0xBF)
        {
            quint16 data = static_cast<quint8>(*it) * 256;
            ++it;
            Q_ASSERT(it != compressed.end());
            data += static_cast<quint8>(*it);
            int size = 3 + (data & 0x07);
            int distance = (data / 8) & 0x07FF;
            QByteArray replace = result.mid(result.size() - distance, size);
            while (replace.size() < size) {
                replace.append(replace);
            }
            replace.truncate(size);
            result.append(replace);
        }
        else { //if (static_cast<quint8>(*it) >= 0xC0)
            result.push_back(' ');
            result.push_back(static_cast<quint8>(*it) ^ 0x80);
        }
    }
    return result;
}