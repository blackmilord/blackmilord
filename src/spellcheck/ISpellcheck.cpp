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

#include <ISpellcheck.h>
#include "Aspell.h"

QStringList ISpellcheck::m_wordsToSkipOnSpellCheck = initWordsToSkipOnSpellCheck();

ISpellcheck& ISpellcheck::instance()
{
    static ASpell instance;
    return instance;
}

bool ISpellcheck::skipSpellCheck(const QString &word) const
{
    if (m_wordsToSkipOnSpellCheck.contains(word)) {
        return true;
    }
    bool ok = false;
    word.toInt(&ok, 10);
    return ok;
}

QStringList ISpellcheck::initWordsToSkipOnSpellCheck()
{
    QStringList result;
    result.push_back("\"");
    result.push_back(".");
    result.push_back(",");
    result.push_back("!");
    result.push_back("?");
    result.push_back(" ");
    result.push_back("<");
    result.push_back(">");
    return result;
}