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

#include "DeviceConfiguration.h"
#include <QDebug>
#include <QFile>

DeviceConfiguration& DeviceConfiguration::instance()
{
    static DeviceConfiguration instance;
    return instance;
}

DeviceConfiguration::DeviceConfiguration()
{
    QFile file(":/kindle3/HtmlValidation.def");
     if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
         qDebug() << "Cannot open configuration file";
         return;
     }

     while (!file.atEnd()) {
         QString line(file.readLine());
         //TODO: it should be real XML parser.
         //TODO: every element should also has information about valid attributes.
         line.remove("<element>").remove("</element>").remove("\r").remove("\n");
         m_validHTMLTags.push_back(line);
     }
}

DeviceConfiguration::~DeviceConfiguration()
{
}

QStringList DeviceConfiguration::getValidHTMLTags() const
{
    return m_validHTMLTags;
}
