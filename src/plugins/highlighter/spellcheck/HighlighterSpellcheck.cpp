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

#include "HighlighterSpellcheck.h"
#include <QDebug>
#include <QString>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QtPlugin>

#include <Spellcheck.h>
#include <Preferences.h>

namespace {
    const QString GUID = "B70AD074-B926-437f-9720-B6CDF0E422AB";
    const QString PROP_ENABLED = "enabled";
}

Q_EXPORT_PLUGIN2(highlighter_spellcheck, HighlighterSpellcheck)

HighlighterSpellcheck::HighlighterSpellcheck() :
    PluginHighlighter(Preferences::instance().getHighlighterValue(GUID, PROP_ENABLED, true).toBool() &&
                      Spellcheck::instance().isLoaded())
{
}

HighlighterSpellcheck::~HighlighterSpellcheck()
{
}

PluginHighlighter::FormatListPtr HighlighterSpellcheck::highlightBlock(const QString &text)
{
    FormatListPtr result(new FormatList());
    QTextCharFormat errorFormat;
    errorFormat.setFontUnderline(true);
    errorFormat.setUnderlineStyle(QTextCharFormat::SpellCheckUnderline);
    errorFormat.setUnderlineColor(QColor(255, 0, 0));

    QTextBoundaryFinder finder(QTextBoundaryFinder::Word, text);
    int startPos = 0;
    int endPos = 0;
    bool insideTag = false;
    finder.toStart();

    while (!finder.boundaryReasons().testFlag(QTextBoundaryFinder::StartWord)) {
        startPos = finder.toNextBoundary();
        if (startPos == -1) {
            return result;
        }
    }
    if (!finder.boundaryReasons().testFlag(QTextBoundaryFinder::StartWord)) {
        return result;
    }

    endPos = finder.toNextBoundary();
    if (endPos == -1) {
        return result;
    }

    while (true) {
        QString word = text.mid(startPos, endPos - startPos);
        if (word == "<") {
            insideTag = true;
        }
        else if (word == ">") {
            insideTag = false;
        }
        if (!insideTag) {
            /* if (word == " ") {
                //double spaces
                result.push_back(AbstractHighlighter::CharFormat(startPos, endPos, errorFormat));
            } else */
            if (!Spellcheck::instance().checkWord(word)) {
                result->push_back(PluginHighlighter::CharFormat(startPos, endPos, errorFormat));
            }
        }
        if (finder.boundaryReasons() & QTextBoundaryFinder::StartWord) {
            startPos = endPos;
            endPos = finder.toNextBoundary();
        }
        else {
            startPos = finder.toNextBoundary();
            endPos = finder.toNextBoundary();
        }
        if (startPos == -1 || endPos == -1) {
            break;
        }
    }
    return result;
}

QString HighlighterSpellcheck::name() const
{
    return QObject::tr("Spellcheck");
}

QLayout* HighlighterSpellcheck::configurationLayout()
{
    QVBoxLayout *layout = new QVBoxLayout();
    m_enableCB = new QCheckBox(QObject::tr("Enabled"));
    layout->addWidget(m_enableCB);
    resetConfigurationLayout();
    return layout;
}

void HighlighterSpellcheck::resetConfigurationLayout()
{
    m_enableCB->setChecked(m_enabled);
    m_enableCB->setEnabled(Spellcheck::instance().isLoaded());
}

void HighlighterSpellcheck::saveSettings()
{
    Preferences::instance().setHighlighterValue(GUID, PROP_ENABLED, m_enableCB->isChecked());
}

QString HighlighterSpellcheck::guid() const
{
    return GUID;
}

void HighlighterSpellcheck::applySettings()
{
    m_enabled = Preferences::instance().getHighlighterValue(GUID, PROP_ENABLED, true).toBool() &&
                Spellcheck::instance().isLoaded();
}
