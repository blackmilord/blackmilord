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

#ifndef BLACK_MILORD_HIGHLIGHTER_HTML_H
#define BLACK_MILORD_HIGHLIGHTER_HTML_H

#include <QObject>
#include <QColor>

#include <PluginHighlighter.h>

class QString;
class QCheckBox;
class QPushButton;

class HighlighterHTMLTags :
    public QObject,
    public PluginHighlighter
{
    Q_OBJECT
public:
    HighlighterHTMLTags();
    virtual ~HighlighterHTMLTags();

    FormatListPtr highlightBlock(const QString &text);

    QLayout* configurationLayout();
    void resetConfigurationLayout();
    void saveSettings();

    void applySettings();

    QString guid() const;
    QString name() const;

private slots:
    void updateConfigurationLayoutState();
    void pickColor();

private:
    void updateConfigurationLayoutColors();

    //internal configuration objects
    bool m_hasForegroundNormal;
    bool m_hasBackgroundNormal;
    bool m_hasForegroundInvalid;
    bool m_hasBackgroundInvalid;
    QColor m_foregroundNormal;
    QColor m_backgroundNormal;
    QColor m_foregroundInvalid;
    QColor m_backgroundInvalid;

    //configuration layout objects
    QCheckBox *m_enableCB;
    QCheckBox *m_colorForegroundNormalCB;
    QCheckBox *m_colorBackgroundNormalCB;
    QCheckBox *m_colorForegroundInvalidCB;
    QCheckBox *m_colorBackgroundInvalidCB;
    QPushButton *m_settingColorForegroundNormalPB;
    QPushButton *m_settingColorBackgroundNormalPB;
    QPushButton *m_settingColorForegroundInvalidPB;
    QPushButton *m_settingColorBackgroundInvalidPB;
};

#endif /* BLACK_MILORD_HIGHLIGHTER_HTML_H */
