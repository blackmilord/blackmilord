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
#include <QThread>
#include <QSettings>
#include <QFile>
#include <QDir>
#include <QVariant>
#include <QString>
#include <QFont>
#include <Version.h>

void Preferences::saveLastUsedDirectory(const QString &dir)
{
    Q_ASSERT(m_threadGuard == QThread::currentThread());
    setValue(PROP_LAST_USED_DIRECTORY, dir.left(
            dir.lastIndexOf(QDir::separator())));
}

void Preferences::setValue(PropertyName key, const QVariant &value)
{
    Q_ASSERT(m_threadGuard == QThread::currentThread());
    m_settings->setValue(m_propertyMap[key], value);
    emit settingsChanged();
}

QVariant Preferences::getValue(PropertyName key, const QVariant &defaultValue)
{
    Q_ASSERT(m_threadGuard == QThread::currentThread());
    return m_settings->value(m_propertyMap[key], defaultValue);
}

QFont Preferences::getEditorFont()
{
    Q_ASSERT(m_threadGuard == QThread::currentThread());
    QFont defaultFontEditor;
    defaultFontEditor.setFamily(
            getValue(PROP_EDITOR_FONT_FAMILY, "Arial").toString());
    defaultFontEditor.setPointSize(
            getValue(PROP_EDITOR_FONT_SIZE, 12).toInt());
    return defaultFontEditor;
}

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
    setValue(PROP_EDITOR_FONT_FAMILY, "Arial");
    setValue(PROP_EDITOR_FONT_SIZE, 12);
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
    m_propertyMap[PROP_HIGHLIGHTER_SPELLCHECK]       = "editor/highlighter_spellchecking";
    m_propertyMap[PROP_HIGHLIGHTER_HTML_TAGS]        = "editor/highlighter_htmltags";
    m_propertyMap[PROP_EDITOR_FONT_FAMILY]           = "editor/font_editor_family";
    m_propertyMap[PROP_EDITOR_FONT_SIZE]             = "editor/font_editor_size";
    Q_ASSERT(m_propertyMap.size() == PROPERTY_NAME_SIZE);
}

Preferences::Preferences() :
    m_threadGuard(QThread::currentThread())
{
    initPropertiesMap();
    QString configFile;
#ifdef Q_WS_X11
    configFile = getenv("HOME");
    configFile.append( "/.blackmilord/config.ini");
#elif defined Q_WS_WIN
    configFile = getenv("APPDATA");
    configFile.append( "\\BlackMilord\\config.ini");
#endif
    qDebug() << "Using config file" << configFile;
    bool needDefault = !QFile::exists(configFile);
    m_settings = new QSettings(configFile, QSettings::IniFormat);
    if (needDefault) {
        createDefaultConfig();
    }
    setValue(PROP_SETTINGS_VERSION, BLACK_MILORD_VERSION);
}

Preferences::~Preferences()
{
    delete m_settings;
}
