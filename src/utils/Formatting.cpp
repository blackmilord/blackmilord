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

#include "Formatting.h"
#include <QString>
#include <QRegExp>

QString Formatting::formatHTMLContent(const QString &text)
{
    //uncomment for debugging and testing
    //return text;
    return QString(text).\
           remove("\n").
           remove("\r").
           replace(QRegExp("( ){2,}"), " ").
           replace(QRegExp(">\\s+<"), "><").
           replace(QRegExp("(\\s*<br\\s*/?>\\s*)"), "<br/>").
           replace("</h1>", "</h1>\n", Qt::CaseInsensitive).
           replace("</h2>", "</h2>\n", Qt::CaseInsensitive).
           replace("</h3>", "</h3>\n", Qt::CaseInsensitive).
           replace("</h4>", "</h4>\n", Qt::CaseInsensitive).
           replace("</h5>", "</h5>\n", Qt::CaseInsensitive).
           replace("</p>", "</p>\n", Qt::CaseInsensitive).
           replace("<p>", "\n<p>", Qt::CaseInsensitive).
           replace("<br/>", "<br/>\n", Qt::CaseInsensitive).
           replace("<html>", "<html>\n", Qt::CaseInsensitive).
           replace("<body>", "<body>\n", Qt::CaseInsensitive).
           replace("</body>", "</body>\n", Qt::CaseInsensitive).
           replace("\n\n", "\n");
}
