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

#include "Preferences.h"
#include <QDebug>
#include <QSettings>
#include <QFile>
#include <QDir>
#include <QMutexLocker>

#include <Version.h>

void Preferences::saveLastDir(const QString &dir)
{
    setValue(PROP_LAST_USED_DIRECTORY, dir.left(
            dir.lastIndexOf(QDir::separator())));
}

void Preferences::setValue(PropertyName key, const QVariant &value)
{
    {
        QMutexLocker lock(&m_mutex);
        m_settings->setValue(m_propertyMap[key], value);
#ifdef QT_GUI_LIB
        if (key == PROP_EDITOR_FONT_FAMILY || key == PROP_EDITOR_FONT_SIZE) {
            m_defaultFontEditor.setFamily(
                    getValue(PROP_EDITOR_FONT_FAMILY, "Arial").toString());
            m_defaultFontEditor.setPointSize(
                    getValue(PROP_EDITOR_FONT_SIZE, 12).toInt());
        }
#endif
    }
    emit settingsChanged();
}

QVariant Preferences::getValue(PropertyName key, const QVariant &defaultValue)
{
    QMutexLocker lock(&m_mutex);
    return m_settings->value(m_propertyMap[key], defaultValue);
}

#ifdef QT_GUI_LIB
QFont Preferences::getDefaultFontEditor()
{
    QMutexLocker lock(&m_mutex);
    return m_defaultFontEditor;
}
#endif

Preferences& Preferences::instance()
{
    static Preferences instance;
    return instance;
}

void Preferences::createDefaultConfig()
{
    setValue(PROP_HIGHLIGHTER_SPELLCHECK, false);
    setValue(PROP_HIGHLIGHTER_HTML_TAGS, false);
    setValue(PROP_MAKE_BACKUP_BEFORE_OVERWRITE, true);
    setValue(PROP_WINDOW_MAXIMIZED, false);
}

void Preferences::initPropertiesMap()
{
    m_propertyMap[PROP_SETTINGS_VERSION]             = "core/version";
    m_propertyMap[PROP_MAKE_BACKUP_BEFORE_OVERWRITE] = "core/backup_before_overwrite";
    m_propertyMap[PROP_ASPELL_DICTIONARY]            = "core/aspell_language";
    m_propertyMap[PROP_LAST_USED_DIRECTORY]          = "core/last_dir";
    m_propertyMap[PROP_WINDOW_WIDTH]                 = "window/width";
    m_propertyMap[PROP_WINDOW_HEIGHT]                = "window/height";
    m_propertyMap[PROP_WINDOW_POSITION_X]            = "window/x";
    m_propertyMap[PROP_WINDOW_POSITION_Y]            = "window/y";
    m_propertyMap[PROP_WINDOW_MAXIMIZED]             = "window/maximized";
    m_propertyMap[PROP_HIGHLIGHTER_SPELLCHECK]       = "editor/highlighter/spellchecking";
    m_propertyMap[PROP_HIGHLIGHTER_HTML_TAGS]        = "editor/highlighter/htmltags";
    m_propertyMap[PROP_EDITOR_FONT_FAMILY]           = "editor/font_editor_family";
    m_propertyMap[PROP_EDITOR_FONT_SIZE]             = "editor/font_editor_size";
    Q_ASSERT(m_propertyMap.size() == PROPERTY_NAME_SIZE);
}

Preferences::Preferences()
{
    initPropertiesMap();
    qDebug() << "Loading preferences...";
#ifdef Q_WS_X11
    m_configFile = getenv("HOME");
    m_configFile.append( "/.blackmilord/config.ini");
#elif defined Q_WS_WIN
    m_configFile = getenv("USERPROFILE");
    m_configFile.append( "\\BlackMilord\\config.ini");
#endif
    qDebug() << "using config file" << m_configFile;
    bool needDefault = !QFile::exists(m_configFile);
    m_settings = new QSettings(m_configFile, QSettings::IniFormat);
    if (needDefault) {
        createDefaultConfig();
    }
#ifdef QT_GUI_LIB
    m_defaultFontEditor.setFamily(
            getValue(PROP_EDITOR_FONT_FAMILY, "Arial").toString());
    m_defaultFontEditor.setPointSize(
            getValue(PROP_EDITOR_FONT_SIZE, 12).toInt());
#endif
    setValue(PROP_SETTINGS_VERSION, OPEN_EBOOK_EDITOR_VERSION);
}

Preferences::~Preferences()
{
    delete m_settings;
}
