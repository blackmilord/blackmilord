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

#ifndef BLACK_MILORD_XML_ELEMENT_H
#define BLACK_MILORD_XML_ELEMENT_H

#include <QString>
#include <QMap>

class XMLElement
{
public:
    static const char *BOOK_INDEX_ATTRIBUTE;

    explicit XMLElement(const QString &name = QString());
    virtual ~XMLElement();

    inline QString elementName() const { return m_name; }
    inline int startPos() const { return m_startPos; }
    inline void setStartPos(int startPos) { m_startPos = startPos; }
    inline int endPos() const { return m_endPos; }
    inline void setEndPos(int endPos) { m_endPos = endPos; }

    inline QMap<QString, QString> attributes() const { return m_attributes; }
    inline void setAttributes(const QMap<QString, QString> &attributes) { m_attributes = attributes; }
    inline bool hasAttribute(const QString &name) const { return m_attributes.contains(name); }
    inline void setAttribute(const QString &name, const QString &value) { m_attributes.insert(name, value); }
    inline QString attribute(const QString &name) const { return hasAttribute(name) ? m_attributes.value(name) : QString(); }


    QString formatElement() const;

private:
    QString m_name;
    QMap<QString, QString> m_attributes;
    int m_startPos;
    int m_endPos;
};

#endif