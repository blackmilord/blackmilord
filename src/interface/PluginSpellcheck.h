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

#ifndef BLACK_MILORD_PLUGIN_SPELLCHECK_H
#define BLACK_MILORD_PLUGIN_SPELLCHECK_H

#include <QPair>
#include <QStringList>
#include <QString>
#include "Plugin.h"

class PluginSpellcheck : public Plugin
{
public:

    explicit PluginSpellcheck(bool enabled = false) : Plugin(enabled)
    {
    }

    virtual ~PluginSpellcheck()
    {
    }

    virtual bool isLoaded() const = 0;
    virtual bool checkWord(const QString &word) const = 0;
    virtual bool addWordToSessionDictionary(const QString &word) = 0;
    virtual bool addWordToPersonalDictionary(const QString &word) = 0;
    virtual QStringList hints(const QString &word) const = 0;
    virtual QString language() const = 0;
    virtual QList<QPair<QString, QString> > availableLanguages() const = 0;
    virtual void changeLanguage(const QString &code) = 0;
};

Q_DECLARE_INTERFACE(PluginSpellcheck, "org.blackmilord.Plugin.Spellcheck/1.0");

#endif /* BLACK_MILORD_PLUGIN_SPELLCHECK_H */
