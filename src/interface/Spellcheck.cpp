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

#include "Spellcheck.h"
#include <QDebug>
#include <QDir>
#include <QPluginLoader>
#include <QApplication>
#include "PluginSpellcheck.h"

Spellcheck::Spellcheck() :
    m_instance(NULL)
{
    QDir pluginsDir(qApp->applicationDirPath());
    PluginSpellcheck *spellcheck;
    foreach(QString fileName, pluginsDir.entryList(QStringList("libspellcheck_*"), QDir::Files))
    {
        QPluginLoader pluginLoader(pluginsDir.absoluteFilePath(fileName));
        QObject *plugin = pluginLoader.instance();
        if (plugin)
        {
            spellcheck = qobject_cast<PluginSpellcheck*>(plugin);
            if (spellcheck)
            {
                m_instance = spellcheck;
                qDebug() << "Loading spellcheck from " << pluginsDir.absolutePath() << fileName << "OK";
                break;
            }
            else
            {
                qDebug() << "Loading spellcheck from " << pluginsDir.absolutePath() << fileName <<
                            "FAILED (cannot cast to PluginSpellcheck object)";
            }
        }
        else
        {
            qDebug() << "Loading spellcheck from " << pluginsDir.absolutePath() << fileName <<
                        "FAILED" << pluginLoader.errorString();
        }
    }
}

Spellcheck::~Spellcheck()
{
}

Spellcheck& Spellcheck::instance()
{
    static Spellcheck instance;
    return instance;
}


bool Spellcheck::isLoaded() const
{
    return NULL != m_instance && m_instance->isLoaded();
}

bool Spellcheck::checkWord(const QString &word) const
{
    Q_ASSERT(isLoaded());
    if (1 == word.length())
    {
        if (0 == word.compare(".") ||
            0 == word.compare(",") ||
            0 == word.compare("!") ||
            0 == word.compare("?") ||
            0 == word.compare(" ") ||
            0 == word.compare("<") ||
            0 == word.compare(">"))
        {
            return true;
        }
    }
    bool ok = false;
    word.toInt(&ok, 10);
    if (ok) {
        return true;
    }
    return m_instance->checkWord(word);
}

bool Spellcheck::addWordToSessionDictionary(const QString &word)
{
    Q_ASSERT(isLoaded());
    return m_instance->addWordToSessionDictionary(word);
}

bool Spellcheck::addWordToPersonalDictionary(const QString &word)
{
    Q_ASSERT(isLoaded());
    return m_instance->addWordToPersonalDictionary(word);
}

QStringList Spellcheck::hints(const QString &word) const
{
    Q_ASSERT(isLoaded());
    return m_instance->hints(word);
}

QString Spellcheck::language() const
{
    Q_ASSERT(isLoaded());
    return m_instance->language();
}

QList<QPair<QString, QString> > Spellcheck::availableLanguages() const
{
    Q_ASSERT(isLoaded());
    return m_instance->availableLanguages();
}

void Spellcheck::changeLanguage(const QString &code)
{
    Q_ASSERT(isLoaded());
    m_instance->changeLanguage(code);
}
