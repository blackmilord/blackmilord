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

#include "AspellWrapper.h"
#ifdef Q_WS_X11
#  include <dlfcn.h>
#elif defined Q_WS_WIN
#  include <windows.h>
#  include <winbase.h>
#endif
#include <QDebug>
#include <QMutexLocker>
#include <Preferences.h>
#include <Dictionary.h>

ASpellWrapper::ASpellWrapper() :
    m_mutex(QMutex::Recursive),
    m_spellConfig(NULL),
    m_spellChecker(NULL),
    m_aspell_error_number(NULL),
    m_aspell_error_message(NULL),
    m_new_aspell_config(NULL),
    m_delete_aspell_config(NULL),
    m_aspell_config_replace(NULL),
    m_new_aspell_speller(NULL),
    m_to_aspell_speller(NULL),
    m_delete_aspell_speller(NULL),
    m_aspell_speller_check(NULL),
    m_delete_aspell_string_enumeration(NULL),
    m_aspell_string_enumeration_next(NULL),
    m_aspell_word_list_elements(NULL),
    m_aspell_speller_suggest(NULL),
    m_get_aspell_dict_info_list(NULL),
    m_aspell_dict_info_list_elements(NULL),
    m_aspell_dict_info_enumeration_next(NULL),
    m_delete_aspell_dict_info_enumeration(NULL),
    m_aspell_speller_add_to_personal(NULL),
    m_aspell_speller_add_to_session(NULL),
    m_aspell_speller_error_message(NULL),
    m_aspell_speller_save_all_word_lists(NULL),
    m_aspell_speller_clear_session(NULL)

{
    if (!loadLibrary())
    {
        qDebug() << "No aspell library found";
        return;
    }
    m_spellConfig = (*m_new_aspell_config)();
    if (!m_spellConfig) {
        return;
    }
    changeLanguage(Preferences::instance().getAspellDictionary());
    qDebug() << "Aspell is loaded";
}

ASpellWrapper::~ASpellWrapper()
{
    if (m_spellChecker) {
        (*m_delete_aspell_speller)(m_spellChecker);
    }
    if (m_spellConfig) {
        (*m_delete_aspell_config)(m_spellConfig);
    }
    closeLibrary();
}

ASpellWrapper& ASpellWrapper::instance()
{
    static ASpellWrapper instance;
    return instance;
}

void ASpellWrapper::changeLanguage(const QString &code)
{
    QMutexLocker lock(&m_mutex);
    if (code == m_language) {
        return;
    }
    if (m_spellChecker) {
        (*m_delete_aspell_speller)(m_spellChecker);
        m_spellChecker = NULL;
    }
    if (!(*m_aspell_config_replace)(m_spellConfig, "lang", code.toUtf8().constData()))
    {
        qDebug() << "Cannot set language";
        return;
    }
    if (!(*m_aspell_config_replace)(m_spellConfig, "encoding", "utf-8"))
    {
        qDebug() << "Cannot set encoding";
        return;
    }

    AspellCanHaveError *possibleErr = (*m_new_aspell_speller)(m_spellConfig);
    if ((*m_aspell_error_number)(possibleErr) != 0) {
        qDebug() << (*m_aspell_error_message)(possibleErr);
        m_language.clear();
    }
    else {
        m_spellChecker = (*m_to_aspell_speller)(possibleErr);
        m_language = code;
        Preferences::instance().setAspellDictionary(m_language);
    }
}

#ifdef Q_WS_X11
bool ASpellWrapper::loadLibrary()
{
    m_handle = dlopen("libaspell.so", RTLD_NOW);
    if (!m_handle) {
        return false;
    }
    QMutexLocker lock(&m_mutex);
    m_aspell_error_number                 = reinterpret_cast<aspell_error_number_fun>(dlsym(m_handle, "aspell_error_number"));
    m_aspell_error_message                = reinterpret_cast<aspell_error_message_fun>(dlsym(m_handle, "aspell_error_message"));
    m_new_aspell_config                   = reinterpret_cast<new_aspell_config_fun>(dlsym(m_handle, "new_aspell_config"));
    m_delete_aspell_config                = reinterpret_cast<delete_aspell_config_fun>(dlsym(m_handle, "delete_aspell_config"));
    m_aspell_config_replace               = reinterpret_cast<aspell_config_replace_fun>(dlsym(m_handle, "aspell_config_replace"));
    m_new_aspell_speller                  = reinterpret_cast<new_aspell_speller_fun>(dlsym(m_handle, "new_aspell_speller"));
    m_to_aspell_speller                   = reinterpret_cast<to_aspell_speller_fun>(dlsym(m_handle, "to_aspell_speller"));
    m_delete_aspell_speller               = reinterpret_cast<delete_aspell_speller_fun>(dlsym(m_handle, "delete_aspell_speller"));
    m_aspell_speller_check                = reinterpret_cast<aspell_speller_check_fun>(dlsym(m_handle, "aspell_speller_check"));
    m_delete_aspell_string_enumeration    = reinterpret_cast<delete_aspell_string_enumeration_fun>(dlsym(m_handle, "delete_aspell_string_enumeration"));
    m_aspell_string_enumeration_next      = reinterpret_cast<aspell_string_enumeration_next_fun>(dlsym(m_handle, "aspell_string_enumeration_next"));
    m_aspell_word_list_elements           = reinterpret_cast<aspell_word_list_elements_fun>(dlsym(m_handle, "aspell_word_list_elements"));
    m_aspell_speller_suggest              = reinterpret_cast<aspell_speller_suggest_fun>(dlsym(m_handle, "aspell_speller_suggest"));
    m_get_aspell_dict_info_list           = reinterpret_cast<get_aspell_dict_info_list_fun>(dlsym(m_handle, "get_aspell_dict_info_list"));
    m_aspell_dict_info_list_elements      = reinterpret_cast<aspell_dict_info_list_elements_fun>(dlsym(m_handle, "aspell_dict_info_list_elements"));
    m_aspell_dict_info_enumeration_next   = reinterpret_cast<aspell_dict_info_enumeration_next_fun>(dlsym(m_handle, "aspell_dict_info_enumeration_next"));
    m_delete_aspell_dict_info_enumeration = reinterpret_cast<delete_aspell_dict_info_enumeration_fun>(dlsym(m_handle, "delete_aspell_dict_info_enumeration"));
    m_aspell_speller_add_to_personal      = reinterpret_cast<aspell_speller_add_to_personal_fun>(dlsym(m_handle, "aspell_speller_add_to_personal"));
    m_aspell_speller_add_to_session       = reinterpret_cast<aspell_speller_add_to_session_fun>(dlsym(m_handle, "aspell_speller_add_to_session"));
    m_aspell_speller_error_message        = reinterpret_cast<aspell_speller_error_message_fun>(dlsym(m_handle, "aspell_speller_error_message"));
    m_aspell_speller_save_all_word_lists  = reinterpret_cast<aspell_speller_save_all_word_lists_fun>(dlsym(m_handle, "aspell_speller_save_all_word_lists"));
    m_aspell_speller_clear_session        = reinterpret_cast<aspell_speller_clear_session_fun>(dlsym(m_handle, "aspell_speller_clear_session"));
    return isLoaded();
}

bool ASpellWrapper::closeLibrary()
{
    QMutexLocker lock(&m_mutex);
    m_aspell_error_number = NULL;
    m_aspell_error_message = NULL;
    m_new_aspell_config = NULL;
    m_delete_aspell_config = NULL;
    m_aspell_config_replace = NULL;
    m_new_aspell_speller = NULL;
    m_to_aspell_speller = NULL;
    m_delete_aspell_speller = NULL;
    m_aspell_speller_check = NULL;
    m_delete_aspell_string_enumeration = NULL;
    m_aspell_string_enumeration_next = NULL;
    m_aspell_word_list_elements = NULL;
    m_aspell_speller_suggest = NULL;
    m_get_aspell_dict_info_list = NULL;
    m_aspell_dict_info_list_elements = NULL;
    m_aspell_dict_info_enumeration_next = NULL;
    m_delete_aspell_dict_info_enumeration = NULL;
    m_aspell_speller_add_to_personal = NULL;
    m_aspell_speller_add_to_session = NULL;
    m_aspell_speller_error_message = NULL;
    m_aspell_speller_save_all_word_lists = NULL;
    m_aspell_speller_clear_session = NULL;
    if (m_handle) {
        if (0 == dlclose(m_handle)) {
            return true;
        }
    }
    return false;
}
#else
bool ASpellWrapper::loadLibrary()
{
    const wchar_t *lib = L"C:\\Program Files\\Aspell\\bin\\aspell-15.dll";
    m_handle = LoadLibrary(lib);
    if (!m_handle) {
        return false;
    }
    QMutexLocker lock(&m_mutex);
    m_aspell_error_number                 = reinterpret_cast<aspell_error_number_fun>(GetProcAddress(m_handle, "aspell_error_number"));
    m_aspell_error_message                = reinterpret_cast<aspell_error_message_fun>(GetProcAddress(m_handle, "aspell_error_message"));
    m_new_aspell_config                   = reinterpret_cast<new_aspell_config_fun>(GetProcAddress(m_handle, "new_aspell_config"));
    m_delete_aspell_config                = reinterpret_cast<delete_aspell_config_fun>(GetProcAddress(m_handle, "delete_aspell_config"));
    m_aspell_config_replace               = reinterpret_cast<aspell_config_replace_fun>(GetProcAddress(m_handle, "aspell_config_replace"));
    m_new_aspell_speller                  = reinterpret_cast<new_aspell_speller_fun>(GetProcAddress(m_handle, "new_aspell_speller"));
    m_to_aspell_speller                   = reinterpret_cast<to_aspell_speller_fun>(GetProcAddress(m_handle, "to_aspell_speller"));
    m_delete_aspell_speller               = reinterpret_cast<delete_aspell_speller_fun>(GetProcAddress(m_handle, "delete_aspell_speller"));
    m_aspell_speller_check                = reinterpret_cast<aspell_speller_check_fun>(GetProcAddress(m_handle, "aspell_speller_check"));
    m_delete_aspell_string_enumeration    = reinterpret_cast<delete_aspell_string_enumeration_fun>(GetProcAddress(m_handle, "delete_aspell_string_enumeration"));
    m_aspell_string_enumeration_next      = reinterpret_cast<aspell_string_enumeration_next_fun>(GetProcAddress(m_handle, "aspell_string_enumeration_next"));
    m_aspell_word_list_elements           = reinterpret_cast<aspell_word_list_elements_fun>(GetProcAddress(m_handle, "aspell_word_list_elements"));
    m_aspell_speller_suggest              = reinterpret_cast<aspell_speller_suggest_fun>(GetProcAddress(m_handle, "aspell_speller_suggest"));
    m_get_aspell_dict_info_list           = reinterpret_cast<get_aspell_dict_info_list_fun>(GetProcAddress(m_handle, "get_aspell_dict_info_list"));
    m_aspell_dict_info_list_elements      = reinterpret_cast<aspell_dict_info_list_elements_fun>(GetProcAddress(m_handle, "aspell_dict_info_list_elements"));
    m_aspell_dict_info_enumeration_next   = reinterpret_cast<aspell_dict_info_enumeration_next_fun>(GetProcAddress(m_handle, "aspell_dict_info_enumeration_next"));
    m_delete_aspell_dict_info_enumeration = reinterpret_cast<delete_aspell_dict_info_enumeration_fun>(GetProcAddress(m_handle, "delete_aspell_dict_info_enumeration"));
    m_aspell_speller_add_to_personal      = reinterpret_cast<aspell_speller_add_to_personal_fun>(GetProcAddress(m_handle, "aspell_speller_add_to_personal"));
    m_aspell_speller_add_to_session       = reinterpret_cast<aspell_speller_add_to_session_fun>(GetProcAddress(m_handle, "aspell_speller_add_to_session"));
    m_aspell_speller_error_message        = reinterpret_cast<aspell_speller_error_message_fun>(GetProcAddress(m_handle, "aspell_speller_error_message"));
    m_aspell_speller_save_all_word_lists  = reinterpret_cast<aspell_speller_save_all_word_lists_fun>(GetProcAddress(m_handle, "aspell_speller_save_all_word_lists"));
    m_aspell_speller_clear_session        = reinterpret_cast<aspell_speller_clear_session_fun>(GetProcAddress(m_handle, "aspell_speller_clear_session"));
    return isLoaded();
}

bool ASpellWrapper::closeLibrary()
{
    QMutexLocker lock(&m_mutex);
    m_aspell_error_number = NULL;
    m_aspell_error_message = NULL;
    m_new_aspell_config = NULL;
    m_delete_aspell_config = NULL;
    m_aspell_config_replace = NULL;
    m_new_aspell_speller = NULL;
    m_to_aspell_speller = NULL;
    m_delete_aspell_speller = NULL;
    m_aspell_speller_check = NULL;
    m_delete_aspell_string_enumeration = NULL;
    m_aspell_string_enumeration_next = NULL;
    m_aspell_word_list_elements = NULL;
    m_aspell_speller_suggest = NULL;
    m_get_aspell_dict_info_list = NULL;
    m_aspell_dict_info_list_elements = NULL;
    m_aspell_dict_info_enumeration_next = NULL;
    m_delete_aspell_dict_info_enumeration = NULL;
    m_aspell_speller_add_to_personal = NULL;
    m_aspell_speller_add_to_session = NULL;
    m_aspell_speller_error_message = NULL;
    m_aspell_speller_save_all_word_lists = NULL;
    m_aspell_speller_clear_session = NULL;
    if (m_handle) {
        if (FreeLibrary(m_handle)) {
            return true;
        }
    }
    return false;
}
#endif

bool ASpellWrapper::isLoaded() const
{
    QMutexLocker lock(&m_mutex);
    return  m_aspell_error_number != NULL &&
            m_aspell_error_message != NULL &&
            m_new_aspell_config != NULL &&
            m_delete_aspell_config != NULL &&
            m_aspell_config_replace != NULL &&
            m_new_aspell_speller != NULL &&
            m_to_aspell_speller != NULL &&
            m_delete_aspell_speller != NULL &&
            m_aspell_speller_check != NULL &&
            m_delete_aspell_string_enumeration != NULL &&
            m_aspell_string_enumeration_next != NULL &&
            m_aspell_word_list_elements != NULL &&
            m_aspell_speller_suggest != NULL &&
            m_get_aspell_dict_info_list != NULL &&
            m_aspell_dict_info_list_elements != NULL &&
            m_aspell_dict_info_enumeration_next != NULL &&
            m_delete_aspell_dict_info_enumeration != NULL &&
            m_aspell_speller_add_to_personal != NULL &&
            m_aspell_speller_add_to_session != NULL &&
            m_aspell_speller_error_message != NULL &&
            m_aspell_speller_save_all_word_lists != NULL &&
            m_aspell_speller_clear_session != NULL;
}

bool ASpellWrapper::checkWord(const QString &word) const
{
    QMutexLocker lock(&m_mutex);
    return 1 == (*m_aspell_speller_check)(m_spellChecker, word.toUtf8().constData(), -1);
}

bool ASpellWrapper::addWordToSessionDictionary(const QString &word)
{
    qDebug() << "adding word to session dict" << word;
    QMutexLocker lock(&m_mutex);
    return 0 != (*m_aspell_speller_add_to_session)(m_spellChecker, word.toUtf8().constData(), -1);
}

bool ASpellWrapper::addWordToPersonalDictionary(const QString &word)
{
    qDebug() << "adding word to personal dict" << word;
    QMutexLocker lock(&m_mutex);
    if (0 != (*m_aspell_speller_add_to_personal)(m_spellChecker, word.toUtf8().constData(), -1)) {
        return 0 != (*m_aspell_speller_save_all_word_lists)(m_spellChecker);
    }
    return false;
}

QStringList ASpellWrapper::hints(const QString &word) const
{
    QStringList result;
    QMutexLocker lock(&m_mutex);
    const AspellWordList *suggestions = (*m_aspell_speller_suggest)(m_spellChecker, word.toUtf8().constData(), -1);
    AspellStringEnumeration *elements = (*m_aspell_word_list_elements)(suggestions);
    const char *hint;
    while ((hint = (*m_aspell_string_enumeration_next)(elements)) != NULL) {
        result.push_back(QString::fromUtf8(hint));
    }
    (*m_delete_aspell_string_enumeration)(elements);
    return result;
}

QString ASpellWrapper::language() const
{
    QMutexLocker lock(&m_mutex);
    return m_language;
}

QList<QPair<QString, QString> > ASpellWrapper::installedDictionaries() const
{
    QList<QPair<QString, QString> > result;
    QMutexLocker lock(&m_mutex);
    if (!isLoaded()) {
        qDebug() << "Cannot obtain dictionary list";
        return result;
    }
    AspellDictInfoList *dictionaryList =
            (*m_get_aspell_dict_info_list)(m_spellConfig);
    if (!dictionaryList) {
        qDebug() << "Cannot obtain dictionary list";
        return result;
    }
    AspellDictInfoEnumeration *enumerator =
            (*m_aspell_dict_info_list_elements)(dictionaryList);
    if (!enumerator) {
        qDebug() << "Cannot obtain dictionary list";
        return result;
    }

    QStringList buffer;
    const AspellDictInfo *entry = NULL;
    while ((entry = (*m_aspell_dict_info_enumeration_next)(enumerator)) != 0)  {
        QString code = entry->code;
        QString readable;
        QStringList splitted = code.split("_");
        if (splitted.size() > 1) {
            readable = Dictionary::languageCodes()[splitted[0]] + " (" + splitted[1] + ")";
        }
        else {
            readable = Dictionary::languageCodes()[code];
        }
        if (!buffer.contains(readable)) {
            result.push_back(qMakePair<QString, QString>(readable, code));
            buffer.push_back(readable);
        }
    }
    (*m_delete_aspell_dict_info_enumeration)(enumerator);
    return result;
}
