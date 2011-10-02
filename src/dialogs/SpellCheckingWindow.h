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

#ifndef OPEN_EBOOK_EDITOR_SPELL_CHECKING_WINDOW_H
#define	OPEN_EBOOK_EDITOR_SPELL_CHECKING_WINDOW_H

#include <QDialog>
#include <QTextBoundaryFinder>
#include <QStringList>
#include <QString>

class QTextEdit;
class QStringListModel;
class QListView;
class QComboBox;
class QShowEvent;
class QPushButton;

class SpellCheckingWindow : public QDialog
{
    Q_OBJECT
public:
    SpellCheckingWindow(QWidget *parent = 0);
    ~SpellCheckingWindow();

private:
    void loadLanguages();

    QTextEdit *m_textContext;
    QStringListModel *m_suggestions;
    QListView *m_suggestionsListView;
    QComboBox *m_language;
    QTextBoundaryFinder *m_finder;
    QPushButton *m_ignoreOnceButton;
    QPushButton *m_ignoreAllButton;
    QPushButton *m_addToDictionaryButton;
    QPushButton *m_changeButton;
    QPushButton *m_changeAllButton;

    QString m_currentWord;
    QString m_undoEditText;
    int m_wordEndPos;
    int m_wordStartPos;
    int m_sentenceStartPos;
    int m_sentenceEndPos;
    bool m_editMode;
    int m_findNextEventType;

private slots:
    void ignoreOnce();
    void ignoreAll();
    void addToDictionary();
    void change();
    void changeAll();
    void editorTextChanged();
    void textContextChanged();
    void changeLanguage(int index);

protected:
    void showEvent(QShowEvent *event);
    void customEvent(QEvent *event);
    void findNextWord();
    void fillWindow();
    void applyEditMode();
};

#endif /* OPEN_EBOOK_EDITOR_SPELL_CHECKING_WINDOW_H */
