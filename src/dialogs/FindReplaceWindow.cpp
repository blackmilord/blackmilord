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

#include "FindReplaceWindow.h"
#include <QDebug>
#include <QComboBox>
#include <QCheckBox>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QRegExp>
#include <QTextCursor>
#include <QShowEvent>
#include <QCompleter>
#include <QMessageBox>

#include <Book.h>

FindReplaceWindow::FindReplaceWindow(QWidget *parent) :
    QDialog(parent)
{
    QHBoxLayout *layout = new QHBoxLayout();

    QVBoxLayout *labelsLayout = new QVBoxLayout();
    QLabel *findLabel = new QLabel(tr("Find what:"));
    QLabel *replaceLabel = new QLabel(tr("Replace with:"));
    labelsLayout->addWidget(findLabel);
    labelsLayout->addWidget(replaceLabel);
    labelsLayout->addStretch(1);

    QVBoxLayout *controlsLayout = new QVBoxLayout();
    m_findWhat = new QComboBox();
    m_findWhat->setEditable(true);
    m_findWhat->setMinimumWidth(200);
    m_findWhat->setMaxCount(10);
    m_findWhat->completer()->setCaseSensitivity(Qt::CaseSensitive);
    m_replaceWith = new QComboBox();
    m_replaceWith->setEditable(true);
    m_replaceWith->setMinimumWidth(200);
    m_replaceWith->setMaxCount(10);
    m_replaceWith->completer()->setCaseSensitivity(Qt::CaseSensitive);
    findLabel->setFixedHeight(m_findWhat->height());
    replaceLabel->setFixedHeight(m_replaceWith->height());
    m_regExp = new QCheckBox(tr("Regular expression"));
    m_regExp->setFocusPolicy(Qt::TabFocus);
    m_backward = new QCheckBox(tr("Search backward"));
    m_backward->setFocusPolicy(Qt::TabFocus);
    m_wordsOnly = new QCheckBox(tr("Match words only"));
    m_wordsOnly->setFocusPolicy(Qt::TabFocus);
    m_caseSensitive = new QCheckBox(tr("Case sensitive"));
    m_caseSensitive->setFocusPolicy(Qt::TabFocus);
    controlsLayout->addWidget(m_findWhat);
    controlsLayout->addWidget(m_replaceWith);
    controlsLayout->addWidget(m_regExp);
    controlsLayout->addWidget(m_backward);
    controlsLayout->addWidget(m_wordsOnly);
    controlsLayout->addWidget(m_caseSensitive);
    controlsLayout->addStretch(1);

    QVBoxLayout *buttonsLayout = new QVBoxLayout();
    QPushButton *findNextButton = new QPushButton(tr("Fin&d next"));
    QPushButton *replaceButton = new QPushButton(tr("&Replace"));
    QPushButton *replaceAllButton = new QPushButton(tr("Repl&ace All"));
    QPushButton *closeButton = new QPushButton(tr("Close"));
    buttonsLayout->addWidget(findNextButton);
    buttonsLayout->addWidget(replaceButton);
    buttonsLayout->addWidget(replaceAllButton);
    buttonsLayout->addStretch(1);
    buttonsLayout->addWidget(closeButton);

    layout->addLayout(labelsLayout, 0);
    layout->addLayout(controlsLayout, 1);
    layout->addLayout(buttonsLayout, 0);

    connect(findNextButton, SIGNAL(released()), this, SLOT(find()));
    connect(replaceButton, SIGNAL(released()), this, SLOT(replace()));
    connect(replaceAllButton, SIGNAL(released()), this, SLOT(replaceAll()));
    connect(closeButton, SIGNAL(released()), this, SLOT(close()));

    connect(m_regExp, SIGNAL(toggled(bool)), this, SLOT(checkboxChanged()));
    connect(m_backward, SIGNAL(toggled(bool)), this, SLOT(checkboxChanged()));
    connect(m_wordsOnly, SIGNAL(toggled(bool)), this, SLOT(checkboxChanged()));
    connect(m_caseSensitive, SIGNAL(toggled(bool)), this, SLOT(checkboxChanged()));

    setLayout(layout);
    setWindowTitle(tr("Find and Replace"));
    setWindowIcon(QIcon(":/icon/menu_find_and_replace.png"));
}

void FindReplaceWindow::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);
    m_findWhat->clearEditText();
    m_replaceWith->clearEditText();
    QDialog::showEvent(event);;
}

FindReplaceWindow::~FindReplaceWindow()
{
}

void FindReplaceWindow::find()
{
    saveValues();

    QString pattern = m_findWhat->currentText();
    const QString &text = Book::instance().getText();
    int from = Book::instance().getCursorPosition();
    int position = -1;
    int length = 0;

    qDebug() << "looking for" << pattern;

    if (!m_regExp->isChecked()) {
        pattern = QRegExp::escape(pattern);
    }
    if (m_wordsOnly->isChecked()) {
        pattern = "\\b" + pattern + "\\b";
    }

    QRegExp rx(pattern);
    rx.setMinimal(true);
    rx.setCaseSensitivity(m_caseSensitive->isChecked() ? Qt::CaseSensitive : Qt::CaseInsensitive);

    if (m_backward->isChecked()) {
        if (Book::instance().hasSelection()) {
            from = Book::instance().getSelectionStart();
        }
        position = text.lastIndexOf(rx, from - 1);
    }
    else {
        if (Book::instance().hasSelection()) {
            from = Book::instance().getSelectionEnd();
        }
        position = text.indexOf(rx, from);
    }
    length = rx.matchedLength();

    if (position != -1) {
        Book::instance().setSelection(position, position + length);
    }
    else {
        if (Book::instance().hasSelection()) {
            Book::instance().setCursorPosition(Book::instance().getSelectionStart());
        }
        Book::instance().clearSelection();
    }
}

void FindReplaceWindow::replace()
{
    saveValues();

    if (Book::instance().hasSelection()) {
        if (m_backward->isChecked()) {
            Book::instance().setCursorPosition(Book::instance().getSelectionEnd());
        }
        else {
            Book::instance().setCursorPosition(Book::instance().getSelectionStart());
        }
    }

    find();

    if (!Book::instance().hasSelection()) {
        Book::instance().setCursorPosition(0);
        find();
        if (!Book::instance().hasSelection()) {
            QMessageBox::information(this, QObject::tr("Text not found"), QObject::tr("There is no more text to replace."));
            return;
        }
    }
    
    Book::instance().setText(
        Book::instance().getText().replace(
            Book::instance().getSelectionStart(),
            Book::instance().getSelectionEnd() - Book::instance().getSelectionStart(),
            m_replaceWith->currentText()));

    Book::instance().setCursorPosition(Book::instance().getSelectionEnd());

    find();
}

void FindReplaceWindow::replaceAll()
{
/*
    saveValues();
    if (m_findWhat->currentText() == m_replaceWith->currentText()) {
        return;
    }

    QTextCursor cursor = m_editor->textCursor();
    cursor.setPosition(0);
    m_editor->setTextCursor(cursor);

    find();
    while (m_editor->textCursor().hasSelection()) {
        m_editor->textCursor().insertText(m_replaceWith->currentText());
        find();
    }
*/
}

void FindReplaceWindow::checkboxChanged()
{
    if (m_regExp->isChecked()) {
        m_wordsOnly->setChecked(false);
    }
    m_wordsOnly->setEnabled(!m_regExp->isChecked());
}

void FindReplaceWindow::saveValues()
{
    bool alreadyOnList = false;
    if (!m_findWhat->currentText().isEmpty()) {
        for (int i=0; i<m_findWhat->count(); ++i) {
            if (m_findWhat->itemText(i) == m_findWhat->currentText()) {
                alreadyOnList = true;
                break;
            }
        }
        if (!alreadyOnList) {
            m_findWhat->insertItem(0, m_findWhat->currentText());
        }
    }
    alreadyOnList = false;
    if (!m_replaceWith->currentText().isEmpty()) {
        for (int i=0; i<m_replaceWith->count(); ++i) {
            if (m_replaceWith->itemText(i) == m_replaceWith->currentText()) {
                alreadyOnList = true;
                break;
            }
        }
        if (!alreadyOnList) {
            m_replaceWith->insertItem(0, m_replaceWith->currentText());
        }
    }

}