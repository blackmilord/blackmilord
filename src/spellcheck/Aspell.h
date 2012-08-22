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

#ifndef BLACK_MILORD_ASPELL_WRAPPER_H
#define BLACK_MILORD_ASPELL_WRAPPER_H

#include <QGlobalStatic>
#ifdef Q_WS_WIN
#  include <windef.h>
#endif
#include <aspell.h>
#include <QMutex>

#include <ISpellcheck.h>

class ASpell : public QObject, public ISpellcheck
{
    Q_OBJECT

    friend class ISpellcheck;

    typedef unsigned int (*aspell_error_number_fun)(const struct AspellCanHaveError*);
    typedef const char* (*aspell_error_message_fun)(const struct AspellCanHaveError*);
    typedef struct AspellConfig* (*new_aspell_config_fun)();
    typedef void (*delete_aspell_config_fun)(struct AspellConfig*);
    typedef int (*aspell_config_replace_fun)(struct AspellConfig*, const char *, const char *);
    typedef struct AspellCanHaveError* (*new_aspell_speller_fun)(struct AspellConfig*);
    typedef struct AspellSpeller* (*to_aspell_speller_fun)(struct AspellCanHaveError*);
    typedef void (*delete_aspell_speller_fun)(struct AspellSpeller*);
    typedef int (*aspell_speller_check_fun)(struct AspellSpeller*, const char*, int);
    typedef void (*delete_aspell_string_enumeration_fun)(struct AspellStringEnumeration*);
    typedef const char* (*aspell_string_enumeration_next_fun)(struct AspellStringEnumeration*);
    typedef struct AspellStringEnumeration* (*aspell_word_list_elements_fun)(const struct AspellWordList*);
    typedef const struct AspellWordList* (*aspell_speller_suggest_fun)(struct AspellSpeller*, const char*, int);
    typedef struct AspellDictInfoList* (*get_aspell_dict_info_list_fun)(struct AspellConfig*);
    typedef struct AspellDictInfoEnumeration* (*aspell_dict_info_list_elements_fun)(const struct AspellDictInfoList*);
    typedef const struct AspellDictInfo* (*aspell_dict_info_enumeration_next_fun)(struct AspellDictInfoEnumeration*);
    typedef void (*delete_aspell_dict_info_enumeration_fun)(struct AspellDictInfoEnumeration*);
    typedef int (*aspell_speller_add_to_personal_fun)(struct AspellSpeller*, const char*, int);
    typedef int (*aspell_speller_add_to_session_fun)(struct AspellSpeller*, const char*, int);
    typedef const char* (*aspell_speller_error_message_fun)(const struct AspellSpeller*);
    typedef int (*aspell_speller_save_all_word_lists_fun)(struct AspellSpeller*);
    typedef int (*aspell_speller_clear_session_fun)(struct AspellSpeller*);

private:
    ASpell();
    ~ASpell();
    bool loadLibrary();
    bool closeLibrary();

public:
    bool isLoaded() const;
    bool checkWord(const QString &word) const;
    bool addWordToSessionDictionary(const QString &word);
    bool addWordToPersonalDictionary(const QString &word);
    QStringList hints(const QString &word) const;
    QString language() const;
    QList<QPair<QString, QString> > availableLanguages() const;

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
    aspell_error_number_fun m_aspell_error_number;
    aspell_error_message_fun m_aspell_error_message;
    new_aspell_config_fun m_new_aspell_config;
    delete_aspell_config_fun m_delete_aspell_config;
    aspell_config_replace_fun m_aspell_config_replace;
    new_aspell_speller_fun m_new_aspell_speller;
    to_aspell_speller_fun m_to_aspell_speller;
    delete_aspell_speller_fun m_delete_aspell_speller;
    aspell_speller_check_fun m_aspell_speller_check;
    delete_aspell_string_enumeration_fun m_delete_aspell_string_enumeration;
    aspell_string_enumeration_next_fun m_aspell_string_enumeration_next;
    aspell_word_list_elements_fun m_aspell_word_list_elements;
    aspell_speller_suggest_fun m_aspell_speller_suggest;
    get_aspell_dict_info_list_fun m_get_aspell_dict_info_list;
    aspell_dict_info_list_elements_fun m_aspell_dict_info_list_elements;
    aspell_dict_info_enumeration_next_fun m_aspell_dict_info_enumeration_next;
    delete_aspell_dict_info_enumeration_fun m_delete_aspell_dict_info_enumeration;
    aspell_speller_add_to_personal_fun m_aspell_speller_add_to_personal;
    aspell_speller_add_to_session_fun m_aspell_speller_add_to_session;
    aspell_speller_error_message_fun m_aspell_speller_error_message;
    aspell_speller_save_all_word_lists_fun m_aspell_speller_save_all_word_lists;
    aspell_speller_clear_session_fun m_aspell_speller_clear_session;

public slots:
    void changeLanguage(const QString &code);
};

#endif /* BLACK_MILORD_ASPELL_WRAPPER_H */
