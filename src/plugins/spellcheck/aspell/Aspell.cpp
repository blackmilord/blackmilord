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

#include "Aspell.h"
#if (defined Q_WS_X11 || defined Q_WS_MAC)
#  include <dlfcn.h>
#elif defined Q_WS_WIN
#  include <windows.h>
#  include <winbase.h>
#endif
#include <QDebug>
#include <QtPlugin>
#include <QMutexLocker>
#include <Preferences.h>
#include <Dictionary.h>

Q_EXPORT_PLUGIN2(spellcheck_aspell, ASpell)

ASpell::ASpell() :
    m_mutex(QMutex::Recursive),
    m_spellConfig(NULL),
    m_spellChecker(NULL)
{
    m_spellConfig = new_aspell_config();
    if (!m_spellConfig) {
        return;
    }
    changeLanguage(Preferences::instance().getAspellDictionary());
    qDebug() << "Aspell is loaded";
}

ASpell::~ASpell()
{
    if (m_spellChecker) {
        delete_aspell_speller(m_spellChecker);
    }
    if (m_spellConfig) {
        delete_aspell_config(m_spellConfig);
    }
}

void ASpell::changeLanguage(const QString &code)
{
    QMutexLocker lock(&m_mutex);
    if (code == m_language) {
        return;
    }
    if (m_spellChecker) {
        delete_aspell_speller(m_spellChecker);
        m_spellChecker = NULL;
    }
    if (!aspell_config_replace(m_spellConfig, "lang", code.toUtf8().constData()))
    {
        qDebug() << "Cannot set language";
        return;
    }
    if (!aspell_config_replace(m_spellConfig, "encoding", "utf-8"))
    {
        qDebug() << "Cannot set encoding";
        return;
    }

    AspellCanHaveError *possibleErr = new_aspell_speller(m_spellConfig);
    if (aspell_error_number(possibleErr) != 0) {
        qDebug() << aspell_error_message(possibleErr);
        m_language.clear();
    }
    else {
        m_spellChecker = to_aspell_speller(possibleErr);
        m_language = code;
        Preferences::instance().setAspellDictionary(m_language);
    }
}

bool ASpell::isLoaded() const
{
    QMutexLocker lock(&m_mutex);
    return m_spellConfig != NULL;
}

bool ASpell::checkWord(const QString &word) const
{
    QMutexLocker lock(&m_mutex);
    if (!isLoaded()) {
        return true;
    }
    return 1 ==  aspell_speller_check(m_spellChecker, word.toUtf8().constData(), -1);
}

bool ASpell::addWordToSessionDictionary(const QString &word)
{
    qDebug() << "Adding word to session dictionary" << word;
    QMutexLocker lock(&m_mutex);
    return 0 != aspell_speller_add_to_session(m_spellChecker, word.toUtf8().constData(), -1);
}

bool ASpell::addWordToPersonalDictionary(const QString &word)
{
    qDebug() << "Adding word to personal dictionary" << word;
    QMutexLocker lock(&m_mutex);
    if (0 != aspell_speller_add_to_personal(m_spellChecker, word.toUtf8().constData(), -1)) {
        return 0 != aspell_speller_save_all_word_lists(m_spellChecker);
    }
    return false;
}

QStringList ASpell::hints(const QString &word) const
{
    QStringList result;
    QMutexLocker lock(&m_mutex);
    const AspellWordList *suggestions = aspell_speller_suggest(m_spellChecker, word.toUtf8().constData(), -1);
    AspellStringEnumeration *elements = aspell_word_list_elements(suggestions);
    const char *hint;
    while ((hint = aspell_string_enumeration_next(elements)) != NULL) {
        result.push_back(QString::fromUtf8(hint));
    }
    delete_aspell_string_enumeration(elements);
    return result;
}

QString ASpell::language() const
{
    QMutexLocker lock(&m_mutex);
    return m_language;
}

QList<QPair<QString, QString> > ASpell::availableLanguages() const
{
    QList<QPair<QString, QString> > result;
    QMutexLocker lock(&m_mutex);
    if (!isLoaded()) {
        qDebug() << "Cannot obtain dictionary list";
        return result;
    }
    AspellDictInfoList *dictionaryList =
            get_aspell_dict_info_list(m_spellConfig);
    if (!dictionaryList) {
        qDebug() << "Cannot obtain dictionary list";
        return result;
    }
    AspellDictInfoEnumeration *enumerator =
            aspell_dict_info_list_elements(dictionaryList);
    if (!enumerator) {
        qDebug() << "Cannot obtain dictionary list";
        return result;
    }

    QStringList buffer;
    const AspellDictInfo *entry = NULL;
    while ((entry = aspell_dict_info_enumeration_next(enumerator)) != 0)  {
        QString code = entry->code;
        QString readable;
        QStringList splitted = code.split("_");
        if (splitted.size() > 1) {
            readable = Dictionary::languageCode(splitted[0]) + " (" + splitted[1] + ")";
        }
        else {
            readable = Dictionary::languageCode(code);
        }
        if (!buffer.contains(readable)) {
            result.push_back(qMakePair<QString, QString>(readable, code));
            buffer.push_back(readable);
        }
    }
    delete_aspell_dict_info_enumeration(enumerator);
    return result;
}


QLayout* ASpell::configurationLayout()
{
    return NULL;
}

void ASpell::resetConfigurationLayout()
{

}

void ASpell::saveSettings()
{

}

void ASpell::applySettings()
{

}

QString ASpell::guid() const
{
    return "AAA";
}

QString ASpell::name() const
{
    return tr("Aspell");
}
