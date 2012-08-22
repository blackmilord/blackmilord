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
#include <QFileInfo>
#include <Version.h>

namespace {
    const QString PROP_VERSION                      = "core/version";
    const QString PROP_MAKE_BACKUP_BEFORE_OVERWRITE = "core/backup_before_overwrite";
    const QString PROP_ASPELL_DICTIONARY            = "core/aspell_language";
    const QString PROP_LAST_USED_DIRECTORY          = "core/last_dir";
    const QString PROP_WINDOW_WIDTH                 = "window/width";
    const QString PROP_WINDOW_HEIGHT                = "window/height";
    const QString PROP_WINDOW_POSITION_X            = "window/x";
    const QString PROP_WINDOW_POSITION_Y            = "window/y";
    const QString PROP_WINDOW_MAXIMIZED             = "window/maximized";
    const QString PROP_HIGHLIGHTER_SPELLCHECK       = "editor/highlighter_spellchecking";
    const QString PROP_HIGHLIGHTER_HTML_TAGS        = "editor/highlighter_htmltags";
    const QString PROP_EDITOR_FONT_FAMILY           = "editor/font_editor_family";
    const QString PROP_EDITOR_FONT_SIZE             = "editor/font_editor_size";
}

double Preferences::getVersion() const
{
    Q_ASSERT(m_threadGuard == QThread::currentThread());
    return m_settings->value(PROP_VERSION, 0.0).toDouble();
}

void Preferences::setMakeBackupBeforeOverwrite(bool makeBackup)
{
    Q_ASSERT(m_threadGuard == QThread::currentThread());
    m_settings->setValue(PROP_MAKE_BACKUP_BEFORE_OVERWRITE, makeBackup);
}

bool Preferences::getMakeBackupBeforeOverwrite() const
{
    Q_ASSERT(m_threadGuard == QThread::currentThread());
    return m_settings->value(PROP_MAKE_BACKUP_BEFORE_OVERWRITE, true).toBool();
}

void Preferences::setAspellDictionary(const QString &dictionary)
{
    Q_ASSERT(m_threadGuard == QThread::currentThread());
    m_settings->setValue(PROP_ASPELL_DICTIONARY, dictionary);
}

QString Preferences::getAspellDictionary() const
{
    Q_ASSERT(m_threadGuard == QThread::currentThread());
    return m_settings->value(PROP_ASPELL_DICTIONARY, "").toString();
}

void Preferences::setWindowWidth(int width)
{
    Q_ASSERT(m_threadGuard == QThread::currentThread());
    m_settings->setValue(PROP_WINDOW_WIDTH, width);
    //No emit needed
}

int Preferences::getWindowWidth(int defWidth) const
{
    Q_ASSERT(m_threadGuard == QThread::currentThread());
    return m_settings->value(PROP_WINDOW_WIDTH, defWidth).toInt();
}

void Preferences::setWindowHeight(int height)
{
    Q_ASSERT(m_threadGuard == QThread::currentThread());
    m_settings->setValue(PROP_WINDOW_HEIGHT, height);
    //No emit needed
}

int Preferences::getWindowHeight(int defHeight) const
{
    Q_ASSERT(m_threadGuard == QThread::currentThread());
    return m_settings->value(PROP_WINDOW_HEIGHT, defHeight).toInt();
}

void Preferences::setWindowPositionX(int x)
{
    Q_ASSERT(m_threadGuard == QThread::currentThread());
    m_settings->setValue(PROP_WINDOW_POSITION_X, x);
    //No emit needed
}

int Preferences::getWindowPositionX(int defX) const
{
    Q_ASSERT(m_threadGuard == QThread::currentThread());
    return m_settings->value(PROP_WINDOW_POSITION_X, defX).toInt();
}

void Preferences::setWindowPositionY(int y)
{
    Q_ASSERT(m_threadGuard == QThread::currentThread());
    m_settings->setValue(PROP_WINDOW_POSITION_Y, y);
    //No emit needed
}

int Preferences::getWindowPositionY(int defY) const
{
    Q_ASSERT(m_threadGuard == QThread::currentThread());
    return m_settings->value(PROP_WINDOW_POSITION_Y, defY).toInt();
}

void Preferences::setWindowMaximized(bool maximized)
{
    Q_ASSERT(m_threadGuard == QThread::currentThread());
    m_settings->setValue(PROP_WINDOW_MAXIMIZED, maximized);
    //No emit needed
}

bool Preferences::getWindowMaximized() const
{
    Q_ASSERT(m_threadGuard == QThread::currentThread());
    return m_settings->value(PROP_WINDOW_MAXIMIZED, 0).toBool();
}

void Preferences::setLastUsedDirectory(const QString &dir)
{
    Q_ASSERT(m_threadGuard == QThread::currentThread());
    m_settings->setValue(PROP_LAST_USED_DIRECTORY,
        QFileInfo(dir).absoluteDir().absolutePath());
}

QString Preferences::getLastUsedDirectory() const
{
    Q_ASSERT(m_threadGuard == QThread::currentThread());
    return m_settings->value(PROP_LAST_USED_DIRECTORY, QString()).toString();
}

void Preferences::setEditorFontSize(int size)
{
    Q_ASSERT(m_threadGuard == QThread::currentThread());
    m_settings->setValue(PROP_EDITOR_FONT_SIZE, size);
    emit settingsChanged();
}

int Preferences::getEditorFontSize() const
{
    Q_ASSERT(m_threadGuard == QThread::currentThread());
    return m_settings->value(PROP_EDITOR_FONT_SIZE, 12).toInt();
}

void Preferences::setEditorFontFamily(const QString &family)
{
    Q_ASSERT(m_threadGuard == QThread::currentThread());
    m_settings->setValue(PROP_EDITOR_FONT_FAMILY, family);
    emit settingsChanged();
}

QString Preferences::getEditorFontFamily() const
{
    Q_ASSERT(m_threadGuard == QThread::currentThread());
    return m_settings->value(PROP_EDITOR_FONT_FAMILY, "Arial").toString();
}

QFont Preferences::getEditorFont() const
{
    Q_ASSERT(m_threadGuard == QThread::currentThread());
    QFont defaultFontEditor;
    defaultFontEditor.setFamily(getEditorFontFamily());
    defaultFontEditor.setPointSize(getEditorFontSize());
    return defaultFontEditor;
}

QVariant Preferences::getHighlighterValue(const QString &guid, const QString &key, const QVariant defValue)
{
    Q_ASSERT(m_threadGuard == QThread::currentThread());
    return m_settings->value(guid + "/" + key, defValue);
}

void Preferences::setHighlighterValue(const QString &guid, const QString &key, const QVariant &value)
{
    Q_ASSERT(m_threadGuard == QThread::currentThread());
    m_settings->setValue(guid + "/" + key, value);
}

void Preferences::createDefaultConfig()
{
    setMakeBackupBeforeOverwrite(true);
    setWindowMaximized(false);
    setEditorFontFamily("Arial");
    setEditorFontSize(12);
}

Preferences::Preferences() :
    m_threadGuard(QThread::currentThread())
{
    QString configFile;
#if (defined Q_WS_X11 || defined Q_WS_MAC)
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
    //TODO: Version check and conversions.
    m_settings->setValue(PROP_VERSION, BLACK_MILORD_VERSION);
}

Preferences::~Preferences()
{
    delete m_settings;
}
