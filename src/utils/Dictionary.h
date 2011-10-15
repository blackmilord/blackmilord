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

#ifndef OPEN_EBOOK_EDITOR_EXTRA_RULES_H
#define OPEN_EBOOK_EDITOR_EXTRA_RULES_H

#include <QMap>
#include <MetadataEnum.h>

class QStringList;
class QString;

class Dictionary
{
public:
    static const QStringList& wordsToSkipOnSpellCheck();
    static const QMap<QString, QString>& languageCodes();
    static bool skipSpellCheck(const QString &word);
    static QString bookMetaDataLabel(MetaData metadata);

private:
    static QStringList m_wordsToSkipOnSpellCheck;
    static QMap<QString, QString> m_languageCodes;

    static QStringList initWordsToSkipOnSpellCheck();
    static QMap<QString, QString> initLanguageCodes();
};

#endif /* OPEN_EBOOK_EDITOR_EXTRA_RULES_H */