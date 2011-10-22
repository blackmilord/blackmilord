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

#ifndef BLACK_MILORD_PREFERENCES_H
#define BLACK_MILORD_PREFERENCES_H

#include <QObject>
#include <QSettings>
#include <QMap>
#include <QFont>

class QVariant;
class QString;
class QThread;

class Preferences : public QObject
{
    Q_OBJECT
    Preferences();
public:
    enum PropertyName {
        PROP_SETTINGS_VERSION,
        PROP_WINDOW_WIDTH,
        PROP_WINDOW_HEIGHT,
        PROP_WINDOW_POSITION_X,
        PROP_WINDOW_POSITION_Y,
        PROP_WINDOW_MAXIMIZED,
        PROP_LAST_USED_DIRECTORY,
        PROP_EDITOR_FONT_FAMILY,
        PROP_EDITOR_FONT_SIZE,
        PROP_ASPELL_DICTIONARY,
        PROP_MAKE_BACKUP_BEFORE_OVERWRITE,
        PROP_HIGHLIGHTER_HTML_TAGS,
        PROP_HIGHLIGHTER_SPELLCHECK,
        PROPERTY_NAME_SIZE //keep it last
    };

    ~Preferences();
    static Preferences& instance();
    void setValue(PropertyName key, const QVariant &value);
    QVariant getValue(PropertyName key, const QVariant &defaultValue);
    void saveLastUsedDirectory(const QString &dir);
    QFont getDefaultFontEditor();
private:
    void createDefaultConfig();
    void initPropertiesMap();

    QThread *m_threadGuard;
    QMap<PropertyName, QString> m_propertyMap;
    QSettings *m_settings;
    QString m_configFile;
    QFont m_defaultFontEditor;
signals:
    void settingsChanged();
};

#endif /* BLACK_MILORD_PREFERENCES_H */