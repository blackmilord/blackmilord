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

class QFont;
class QVariant;
class QString;
class QThread;

class Preferences : public QObject
{
    Q_OBJECT
    Preferences();
    ~Preferences();

public:

    static Preferences& instance();

    double getVersion() const;

    void setMakeBackupBeforeOverwrite(bool makeBackup);
    bool getMakeBackupBeforeOverwrite() const;

    void setAspellDictionary(const QString &dictionary);
    QString getAspellDictionary() const;

    void setWindowWidth(int width);
    int getWindowWidth(int defWidth = 0) const;

    void setWindowHeight(int height);
    int getWindowHeight(int defHeight = 0) const;

    void setWindowPositionX(int x);
    int getWindowPositionX(int defX = 0) const;

    void setWindowPositionY(int y);
    int getWindowPositionY(int defY = 0) const;

    void setWindowMaximized(bool maximized);
    bool getWindowMaximized() const;

    void setLastUsedDirectory(const QString &dir);
    QString getLastUsedDirectory() const;

    void setEditorFontSize(int size);
    int getEditorFontSize() const;

    void setEditorFontFamily(const QString &family);
    QString getEditorFontFamily() const;
    QFont getEditorFont() const;

    QVariant getHighlighterValue(const QString &guid, const QString &key, const QVariant defValue);
    void setHighlighterValue(const QString &guid, const QString &key, const QVariant &value);

private:
    void createDefaultConfig();

    QThread *m_threadGuard;
    QSettings *m_settings;

signals:
    void settingsChanged();
};

#endif /* BLACK_MILORD_PREFERENCES_H */