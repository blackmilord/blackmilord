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

#ifndef BLACK_MILORD_ABSTRACT_BOOK_H
#define BLACK_MILORD_ABSTRACT_BOOK_H

#include <QSharedPointer>

class QString;

class AbstractBook
{
public:
    enum Type {
        BOOK_TYPE_MOBI,
        BOOK_TYPE_HTML,
        BOOK_TYPE_PDF
    };

    AbstractBook();
    virtual ~AbstractBook();
    virtual bool openFile(const QString &fileName) = 0;
    virtual bool saveFile(const QString &fileName) = 0;
    virtual bool newFile() = 0;
    virtual bool canOpenFile() = 0;
    virtual bool canSaveFile() = 0;
};

typedef QSharedPointer<AbstractBook> AbstractBookPtr;

class AbstractBookFactory
{
public:
    static AbstractBookPtr getObject(AbstractBook::Type type = AbstractBook::BOOK_TYPE_MOBI);
};

#endif /* BLACK_MILORD_ABSTRACT_BOOK_H */