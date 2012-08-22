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

#ifndef BLACK_MILORD_IPREFERENCES_H
#define BLACK_MILORD_IPREFERENCES_H

#include <QObject>
#include <QSettings>
#include <QMap>

class QFont;
class QVariant;
class QString;
class QThread;

class IPreferences : public QObject
{
    Q_OBJECT
public:

    virtual ~IPreferences() {}

    static IPreferences& instance();

    virtual double getVersion() const = 0;

    virtual void setMakeBackupBeforeOverwrite(bool makeBackup) = 0;
    virtual bool getMakeBackupBeforeOverwrite() const = 0;

    virtual void setAspellDictionary(const QString &dictionary) = 0;
    virtual QString getAspellDictionary() const = 0;

    virtual void setWindowWidth(int width) = 0;
    virtual int getWindowWidth(int defWidth = 0) const = 0;

    virtual void setWindowHeight(int height) = 0;
    virtual int getWindowHeight(int defHeight = 0) const = 0;

    virtual void setWindowPositionX(int x) = 0;
    virtual int getWindowPositionX(int defX = 0) const = 0;

    virtual void setWindowPositionY(int y) = 0;
    virtual int getWindowPositionY(int defY = 0) const = 0;

    virtual void setWindowMaximized(bool maximized) = 0;
    virtual bool getWindowMaximized() const = 0;

    virtual void setLastUsedDirectory(const QString &dir) = 0;
    virtual QString getLastUsedDirectory() const = 0;

    virtual void setEditorFontSize(int size) = 0;
    virtual int getEditorFontSize() const = 0;

    virtual void setEditorFontFamily(const QString &family) = 0;
    virtual QString getEditorFontFamily() const = 0;
    virtual QFont getEditorFont() const = 0;

    virtual QVariant getHighlighterValue(const QString &guid, const QString &key, const QVariant defValue) = 0;
    virtual void setHighlighterValue(const QString &guid, const QString &key, const QVariant &value) = 0;

signals:
    void settingsChanged();
};

#endif /* BLACK_MILORD_IPREFERENCES_H */