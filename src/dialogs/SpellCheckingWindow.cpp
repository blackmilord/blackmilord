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

#include "SpellCheckingWindow.h"
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTextEdit>
#include <QStringListModel>
#include <QListView>
#include <QComboBox>
#include <QRegExp>
#include <QShowEvent>
#include <QDebug>
#include <QMessageBox>
#include <QCoreApplication>

#include <Dictionary.h>
#include <AspellWrapper.h>
#include <Preferences.h>
#include <Book.h>

namespace {
    const QString BUTTON_LABEL_IGNORE_ONCE(QObject::tr("Ignore Once"));
    const QString BUTTON_LABEL_UNDO_EDIT(QObject::tr("Undo Edit"));
}

SpellCheckingWindow::SpellCheckingWindow(QWidget *parent) :
    QDialog(parent),
    m_finder(NULL)
{
    m_findNextEventType = QEvent::registerEventType();

    QVBoxLayout *layout = new QVBoxLayout();
    QLabel *notInDictionaryLabel = new QLabel(tr("Not in dictionary:"));
    QLabel *suggestionsLabel = new QLabel(tr("Suggestions:"));
    QLabel *languageLabel = new QLabel(tr("Language:"));
    m_ignoreOnceButton = new QPushButton(BUTTON_LABEL_IGNORE_ONCE);
    m_ignoreAllButton = new QPushButton(tr("Ignore All"));
    m_addToDictionaryButton = new QPushButton(tr("Add To Dictionary"));
    m_changeButton = new QPushButton(tr("Change"));
    m_changeAllButton = new QPushButton(tr("Change All"));
    QPushButton *cancel = new QPushButton(tr("Cancel"));
    m_textContext = new QTextEdit();
    m_textContext->setFixedSize(300,100);

    QHBoxLayout *notFoundLayout = new QHBoxLayout();
    QVBoxLayout *notFoundButtonsLayout = new QVBoxLayout();
    notFoundButtonsLayout->addWidget(m_ignoreOnceButton);
    notFoundButtonsLayout->addWidget(m_ignoreAllButton);
    notFoundButtonsLayout->addWidget(m_addToDictionaryButton);
    notFoundButtonsLayout->addStretch(1);
    notFoundLayout->addWidget(m_textContext, 0);
    notFoundLayout->addLayout(notFoundButtonsLayout, 1);

    QHBoxLayout *suggestionLayout = new QHBoxLayout();
    QVBoxLayout *suggestionButtonsLayout = new QVBoxLayout();
    suggestionButtonsLayout->addWidget(m_changeButton);
    suggestionButtonsLayout->addWidget(m_changeAllButton);
    suggestionButtonsLayout->addStretch(1);
    m_suggestions = new QStringListModel();
    m_suggestionsListView = new QListView;
    m_suggestionsListView->setFixedSize(300,100);
    m_suggestionsListView->setModel(m_suggestions);
    suggestionLayout->addWidget(m_suggestionsListView, 0);
    suggestionLayout->addLayout(suggestionButtonsLayout, 1);

    m_language = new QComboBox();
    QHBoxLayout *languageLayout = new QHBoxLayout();
    languageLayout->addWidget(languageLabel);
    languageLayout->addWidget(m_language);
    languageLayout->addStretch(1);

    QHBoxLayout *buttonsLayout = new QHBoxLayout();
    buttonsLayout->addStretch(1);
    buttonsLayout->addWidget(cancel);

    layout->addWidget(notInDictionaryLabel);
    layout->addLayout(notFoundLayout);
    layout->addWidget(suggestionsLabel);
    layout->addLayout(suggestionLayout);
    layout->addLayout(languageLayout);
    layout->addLayout(buttonsLayout);

    connect(cancel, SIGNAL(released()), this, SLOT(close()));
    connect(m_ignoreOnceButton, SIGNAL(released()), this, SLOT(ignoreOnce()));
    connect(m_ignoreAllButton, SIGNAL(released()), this, SLOT(ignoreAll()));
    connect(m_addToDictionaryButton, SIGNAL(released()), this, SLOT(addToDictionary()));
    connect(m_changeButton, SIGNAL(released()), this, SLOT(change()));
    connect(m_changeAllButton, SIGNAL(released()), this, SLOT(changeAll()));
    connect(m_textContext, SIGNAL(textChanged()), this, SLOT(textContextChanged()));
    connect(m_language, SIGNAL(currentIndexChanged(int)), this, SLOT(changeLanguage(int)));
    connect(&Book::instance(), SIGNAL(textChanged()), this, SLOT(editorTextChanged()));

    setLayout(layout);
    setWindowTitle(tr("Spelling"));
    setWindowIcon(QIcon(":/resource/icon/menu_check_spelling.png"));
}

SpellCheckingWindow::~SpellCheckingWindow()
{
}

void SpellCheckingWindow::findNextWord()
{
    int startPos;
    int endPos;
    bool insideTag = false;
    bool resetToBegin = false;
    QString word;

    do {
        do {
            m_finder->setPosition(Book::instance().getCursorPosition());

            //find beginning of the word
            startPos = m_finder->toNextBoundary();
            //check if finished
            if (startPos == -1) {
                if (resetToBegin) {
                    QMessageBox::information(this, tr("Finish"), tr("Checking document has finished."));
                    close();
                    return;
                }
                if (QMessageBox::No == QMessageBox::information(this, tr("Finish"), tr("Checking document has finished.\nDo you want to start from the beginning?"), QMessageBox::Yes, QMessageBox::No)) {
                    close();
                    return;
                }
                Book::instance().setCursorPosition(0);
                insideTag = false;
                resetToBegin = true;
                continue;
            }
            //if not start word then seek backward to find beginning
            if (m_finder->boundaryReasons() != QTextBoundaryFinder::StartWord) {
                //should not happen to return -1
                startPos = m_finder->toPreviousBoundary();
            }
            //should not happen to return -1
            endPos = m_finder->toNextBoundary();

            //move cursor to current word
            Book::instance().setCursorPosition(endPos);

            word = m_finder->string().mid(startPos, endPos - startPos);
            //qDebug() << word;

            if (word == "<") {
                insideTag = true;
            }
            else if (word == ">") {
                insideTag = false;
            }
        } while (insideTag || Dictionary::skipSpellCheck(word));
    } while (ASpellWrapper::instance().checkWord(word));
    m_currentWord = word;
    qDebug() << "not found word" << word;
    fillWindow();
}

void SpellCheckingWindow::loadLanguages()
{
    const QList<QPair<QString, QString> > &lang =
            ASpellWrapper::instance().installedDictionaries();
    if (lang.size() == m_language->count()) {
        return;
    }
    QList<QPair<QString, QString> >::const_iterator it = lang.begin();
    QString propertiesDictionary = Preferences::instance().getValue(
            Preferences::PROP_ASPELL_DICTIONARY, "").toString();
    m_language->clear();
    for (; it != lang.end(); ++it) {
        QString display = it->first;
        QVariant userData = it->second;
        m_language->addItem(display, userData);
        if (it == lang.begin() || it->second == propertiesDictionary)
        {
            m_language->setCurrentIndex(m_language->count()-1);
        }
    }
}

void SpellCheckingWindow::fillWindow()
{
    //Q_ASSERT(m_finder->boundaryReasons() == QTextBoundaryFinder::EndWord);
    m_wordEndPos = m_finder->position();
    m_finder->toPreviousBoundary();
    m_wordStartPos = m_finder->position();
    QTextBoundaryFinder sentence(QTextBoundaryFinder::Sentence, m_finder->string());
    sentence.setPosition(m_wordEndPos);
    sentence.toPreviousBoundary();
    m_sentenceStartPos = sentence.position();
    sentence.toNextBoundary();
    m_sentenceEndPos = sentence.position();

    while (m_finder->string().at(m_sentenceEndPos - 1).isSpace() &&
           m_sentenceEndPos-- > m_sentenceStartPos) ; //do nothing

    m_undoEditText = sentence.string().mid(m_sentenceStartPos,
            m_sentenceEndPos-m_sentenceStartPos);
    m_undoEditText.insert(m_wordEndPos - m_sentenceStartPos, "</font>");
    m_undoEditText.insert(m_wordStartPos - m_sentenceStartPos, "<font color=\"red\">");
    m_textContext->blockSignals(true);
    m_textContext->setHtml(m_undoEditText);
    m_textContext->blockSignals(false);

    m_suggestions->setStringList(ASpellWrapper::instance().hints(m_currentWord));
    m_editMode = false;
    applyEditMode();
}

void SpellCheckingWindow::applyEditMode()
{
    m_ignoreAllButton->setEnabled(!m_editMode);
    m_addToDictionaryButton->setEnabled(!m_editMode);
    m_changeAllButton->setEnabled(!m_editMode);
    m_suggestionsListView->setEnabled(!m_editMode);
    if (m_editMode) {
        m_ignoreOnceButton->setText(BUTTON_LABEL_UNDO_EDIT);
    }
    else {
        m_ignoreOnceButton->setText(BUTTON_LABEL_IGNORE_ONCE);
    }
}

void SpellCheckingWindow::ignoreOnce()
{
    if (m_editMode) {
        m_textContext->setHtml(m_undoEditText);
        QTextCursor cursor = m_textContext->textCursor();
        cursor.setPosition(0);
        m_textContext->setTextCursor(cursor);
        m_editMode = false;
        applyEditMode();
    }
    else {
        findNextWord();
    }
}

void SpellCheckingWindow::ignoreAll()
{
    ASpellWrapper::instance().addWordToSessionDictionary(m_currentWord);
    findNextWord();
}

void SpellCheckingWindow::addToDictionary()
{
    ASpellWrapper::instance().addWordToPersonalDictionary(m_currentWord);
    findNextWord();
}

void SpellCheckingWindow::change()
{
    if (m_editMode) {
        qDebug() << Book::instance().getText().mid(m_sentenceStartPos, m_sentenceEndPos - m_sentenceStartPos) <<
                    "replaced by" <<
                    m_textContext->toPlainText();
        Book::instance().setText(
                Book::instance().getText().replace(m_sentenceStartPos,
                        m_sentenceEndPos - m_sentenceStartPos,
                        m_textContext->toPlainText()));
    }
    else {
        QModelIndexList indexes =
                m_suggestionsListView->selectionModel()->selectedIndexes();
        if (indexes.size() != 1) {
            return;
        }
        QString replacement = m_suggestions->data(indexes.first(),
                Qt:: DisplayRole).toString();

        qDebug() << Book::instance().getText().mid(m_wordStartPos, m_wordEndPos - m_wordStartPos) <<
                    "replaced by" <<
                    replacement;
        Book::instance().setText(
                Book::instance().getText().replace(m_wordStartPos,
                        m_wordEndPos - m_wordStartPos,
                        replacement));

    }
    findNextWord();
}

void SpellCheckingWindow::changeAll()
{
    Q_ASSERT(!m_editMode);
    QString replacement;

    QModelIndexList indexes =
            m_suggestionsListView->selectionModel()->selectedIndexes();
    if (indexes.size() != 1) {
        return;
    }
    replacement = m_suggestions->data(indexes.first(),
            Qt::DisplayRole).toString();

    int position = Book::instance().getCursorPosition() - m_currentWord.length();

    Book::instance().setText(
        Book::instance().getText().left(position) +
        Book::instance().getText().mid(position).replace(m_currentWord, replacement));

    Book::instance().setCursorPosition(position);

    findNextWord();
}

void SpellCheckingWindow::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);
    loadLanguages();

    delete m_finder;
    m_finder = new QTextBoundaryFinder(QTextBoundaryFinder::Word, Book::instance().getText());

    m_textContext->blockSignals(true);
    m_textContext->clear();
    m_textContext->blockSignals(false);
    m_suggestions->setStringList(QStringList());
    QCoreApplication::postEvent(this, new QEvent(static_cast<QEvent::Type>(m_findNextEventType)));

    QDialog::showEvent(event);;
}

void SpellCheckingWindow::customEvent(QEvent *event)
{
    if (event->type() == m_findNextEventType) {
        findNextWord();
        event->accept();
    }
    else {
        event->ignore();
    }
}

void SpellCheckingWindow::editorTextChanged()
{
    if (isVisible()) {
        qDebug() << "editor text changed, recreating QTextBoundaryFinder";
        delete m_finder;
        m_finder = new QTextBoundaryFinder(QTextBoundaryFinder::Word,
                Book::instance().getText());
    }
}

void SpellCheckingWindow::textContextChanged()
{
    if (m_editMode) {
        return;
    }
    m_textContext->blockSignals(true);
    QTextCursor cursor = m_textContext->textCursor();
    int position = cursor.position();
    m_textContext->setTextColor(Qt::black);
    m_textContext->setPlainText(m_textContext->toPlainText());
    cursor.setPosition(position);
    m_textContext->setTextCursor(cursor);
    m_textContext->blockSignals(false);
    m_editMode = true;
    applyEditMode();
}

void SpellCheckingWindow::changeLanguage(int index)
{
    ASpellWrapper::instance().changeLanguage(
            m_language->itemData(index, Qt::UserRole).toString());
}
