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

#ifndef BLACK_MILORD_ASPELL_H
#define BLACK_MILORD_ASPELL_H

#include <QObject>
#ifdef Q_WS_WIN
#  include <windef.h>
#endif
#include <aspell.h>

#include <QMutex>
#include <PluginSpellcheck.h>

class ASpell :
    public QObject,
    public PluginSpellcheck
{
    Q_OBJECT
    Q_INTERFACES(PluginSpellcheck)

public:
    ASpell();
    ~ASpell();
    bool isLoaded() const;
    bool checkWord(const QString &word) const;
    bool addWordToSessionDictionary(const QString &word);
    bool addWordToPersonalDictionary(const QString &word);
    QStringList hints(const QString &word) const;
    QString language() const;
    QList<QPair<QString, QString> > availableLanguages() const;
    void changeLanguage(const QString &code);

    QLayout* configurationLayout();
    void resetConfigurationLayout();
    void saveSettings();
    void applySettings();
    QString guid() const;
    QString name() const;

private:
    mutable QMutex m_mutex;
    AspellConfig* m_spellConfig;
    AspellSpeller* m_spellChecker;
    QString m_language;

#if (defined Q_WS_X11 || defined Q_WS_MAC)
    void *m_handle;
#elif defined Q_WS_WIN
    HMODULE m_handle;
#endif
};

#endif /* BLACK_MILORD_ASPELL_H */
