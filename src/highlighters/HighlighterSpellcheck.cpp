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
#include <Preferences.h>
#include <AspellWrapper.h>
#include <Dictionary.h>

HighlighterSpellcheck::HighlighterSpellcheck()
{
}

HighlighterSpellcheck::~HighlighterSpellcheck()
{
}

QVector<AbstractHighlighter::CharFormat> HighlighterSpellcheck::highlightBlock(const QString &text)
{
    QVector<AbstractHighlighter::CharFormat> result;
    if (!Preferences::instance().getValue(Preferences::PROP_HIGHLIGHTER_SPELLCHECK, false).toBool()) {
        return result;
    }
    if (!ASpellWrapper::instance().isLoaded()) {
        return result;
    }
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
                result.push_back(AbstractHighlighter::CharFormat(startPos, endPos - startPos, errorFormat));
            } else */
            if (!Dictionary::skipSpellCheck(word)) {
                if (!ASpellWrapper::instance().checkWord(word)) {
                    result.push_back(AbstractHighlighter::CharFormat(startPos, endPos - startPos, errorFormat));
                }
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

QString HighlighterSpellcheck::getOptionCheckBoxCaption() const
{
    return QObject::tr("Highlight spelling errors");
}

Preferences::PropertyName HighlighterSpellcheck::getPropertyName() const
{
    return Preferences::PROP_HIGHLIGHTER_SPELLCHECK;
}

void HighlighterSpellcheck::applySettings()
{
}
