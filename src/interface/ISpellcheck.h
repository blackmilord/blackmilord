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

#ifndef BLACK_MILORD_I_DEVICE_CONFIGURATION_H
#define BLACK_MILORD_I_DEVICE_CONFIGURATION_H

#include <QStringList>
#include <QString>
#include <QList>
#include <QPair>

class ISpellcheck
{
public:

    virtual ~ISpellcheck() {}

    static ISpellcheck& instance();

    virtual bool isLoaded() const = 0;
    virtual bool checkWord(const QString &word) const = 0;
    virtual bool addWordToSessionDictionary(const QString &word) = 0;
    virtual bool addWordToPersonalDictionary(const QString &word) = 0;
    virtual QStringList hints(const QString &word) const = 0;
    virtual QString language() const = 0;
    virtual QList<QPair<QString, QString> > availableLanguages() const = 0;

public slots:
    virtual void changeLanguage(const QString &code) = 0;

protected:
    virtual bool skipSpellCheck(const QString &word) const;
    static QStringList initWordsToSkipOnSpellCheck();
    static QStringList m_wordsToSkipOnSpellCheck;
};


#endif /* BLACK_MILORD_DEVICE_CONFIGURATION_H */