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

#include "XMLElement.h"
#include <QTextStream>
#include <QDebug>

const char* XMLElement::BOOK_INDEX_ATTRIBUTE = "recindex";

XMLElement::XMLElement(const QString &name) :
    m_name(name),
    m_startPos(-1),
    m_endPos(-1)
{
}

XMLElement::~XMLElement()
{
}

QString XMLElement::formatElement() const
{
    QString result;
    QTextStream str(&result);
    str << "<" << m_name;
    foreach(QString key, m_attributes.keys()) {
        str << " " <<  key << "=\"" << m_attributes.value(key) << "\"";
    }
    str << ">";
    return result;
}
