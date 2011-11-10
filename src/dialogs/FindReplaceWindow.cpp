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

#include <Gui.h>
#include <PlainTextEditor.h>

FindReplaceWindow::FindReplaceWindow(QWidget *parent) :
    QDialog(parent),
    m_notFoundLastTime(false)
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
    m_findNextButton = new QPushButton(tr("Fin&d next"));
    m_replaceButton = new QPushButton(tr("&Replace"));
    m_replaceAllButton = new QPushButton(tr("Repl&ace All"));
    QPushButton *closeButton = new QPushButton(tr("Close"));
    buttonsLayout->addWidget(m_findNextButton);
    buttonsLayout->addWidget(m_replaceButton);
    buttonsLayout->addWidget(m_replaceAllButton);
    buttonsLayout->addStretch(1);
    buttonsLayout->addWidget(closeButton);

    layout->addLayout(labelsLayout, 0);
    layout->addLayout(controlsLayout, 1);
    layout->addLayout(buttonsLayout, 0);

    connect(m_findNextButton, SIGNAL(released()), this, SLOT(find()));
    connect(m_replaceButton, SIGNAL(released()), this, SLOT(replace()));
    connect(m_replaceAllButton, SIGNAL(released()), this, SLOT(replaceAll()));
    connect(closeButton, SIGNAL(released()), this, SLOT(close()));

    connect(m_findWhat, SIGNAL(editTextChanged(const QString &)), this, SLOT(updateButtons()));
    connect(m_replaceWith, SIGNAL(editTextChanged(const QString &)), this, SLOT(updateButtons()));

    connect(m_regExp, SIGNAL(toggled(bool)), this, SLOT(checkboxChanged()));
    connect(m_backward, SIGNAL(toggled(bool)), this, SLOT(checkboxChanged()));
    connect(m_wordsOnly, SIGNAL(toggled(bool)), this, SLOT(checkboxChanged()));
    connect(m_caseSensitive, SIGNAL(toggled(bool)), this, SLOT(checkboxChanged()));

    setLayout(layout);
    setWindowTitle(tr("Find and Replace"));
    setWindowIcon(QIcon(":/resource/icon/menu_find_and_replace.png"));
}

FindReplaceWindow::~FindReplaceWindow()
{
}

void FindReplaceWindow::showEvent(QShowEvent *event)
{
    m_findWhat->clearEditText();
    m_replaceWith->clearEditText();
    m_notFoundLastTime = false;
    updateButtons();
    QDialog::showEvent(event);
}

void FindReplaceWindow::find(bool showDialogs)
{
    saveValues();

    const QString &text = Gui::plainTextEditor()->toPlainText();

    //prepare query
    QString pattern = m_findWhat->currentText();
    if (!m_regExp->isChecked()) {
        pattern = QRegExp::escape(pattern);
    }
    if (m_wordsOnly->isChecked()) {
        pattern = "\\b" + pattern + "\\b";
    }
    QRegExp rx(pattern);
    rx.setMinimal(true);
    rx.setCaseSensitivity(m_caseSensitive->isChecked() ? Qt::CaseSensitive : Qt::CaseInsensitive);

    //find text
    int from = Gui::plainTextEditor()->getCursorPosition();
    int position = -1;
    int length = 0;
    if (m_backward->isChecked()) {
        if (Gui::plainTextEditor()->hasSelection()) {
            from = Gui::plainTextEditor()->getSelectionStart();
        }
        position = text.lastIndexOf(rx, from - 1);
    }
    else {
        if (Gui::plainTextEditor()->hasSelection()) {
            from = Gui::plainTextEditor()->getSelectionEnd();
        }
        position = text.indexOf(rx, from);
    }
    length = rx.matchedLength();

    //handle result
    if (position != -1) {
        //select found text
        m_notFoundLastTime = false;
        Gui::plainTextEditor()->setSelection(position, position + length);
    }
    else {
        //give dialog box about not found text
        if (m_notFoundLastTime) {
            //already tried with reseted position
            m_notFoundLastTime = false;
            if (showDialogs) {
                QMessageBox::question(this, tr("No more matches"), tr("There is no more matches."));
            }
            if (m_backward->isChecked()) {
                Gui::plainTextEditor()->setCursorPositionToStart();
            }
            else {
                Gui::plainTextEditor()->setCursorPositionToEnd();
            }
            return;
        }
        //reset position of the cursor and continue
        QMessageBox::StandardButton answer = QMessageBox::No;
        if (showDialogs) {
            answer = QMessageBox::question(this, tr("No more matches"),
                tr("There is no more matches. Do you want to start from ") + (m_backward->isChecked() ? tr("the end") : tr("the beginning")) + "?",
                QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        }
        if (QMessageBox::Yes == answer) {
            m_notFoundLastTime = true;
            if (m_backward->isChecked()) {
                Gui::plainTextEditor()->setCursorPositionToEnd();
            }
            else {
                Gui::plainTextEditor()->setCursorPositionToStart();
            }
            find();
        }
        else {
            m_notFoundLastTime = false;
            Gui::plainTextEditor()->clearSelection();
        }
    }
}

void FindReplaceWindow::replace()
{
    saveValues();

    if (Gui::plainTextEditor()->hasSelection()) {
        if (m_backward->isChecked()) {
            Gui::plainTextEditor()->setCursorPosition(Gui::plainTextEditor()->getSelectionEnd());
        }
        else {
            Gui::plainTextEditor()->setCursorPosition(Gui::plainTextEditor()->getSelectionStart());
        }
    }

    find();

    if (!Gui::plainTextEditor()->hasSelection()) {
        Gui::plainTextEditor()->setCursorPosition(0);
        find();
        if (!Gui::plainTextEditor()->hasSelection()) {
            QMessageBox::information(this, QObject::tr("Text not found"), QObject::tr("There is no more text to replace."));
            return;
        }
    }

    Gui::plainTextEditor()->replace(
        Gui::plainTextEditor()->getSelectionStart(),
        Gui::plainTextEditor()->getSelectionEnd() - Gui::plainTextEditor()->getSelectionStart(),
        m_replaceWith->currentText());

    find();
}

void FindReplaceWindow::replaceAll()
{
    saveValues();

    Gui::plainTextEditor()->setCursorPositionToStart();

    find(false);
    while (Gui::plainTextEditor()->hasSelection()) {
        Gui::plainTextEditor()->replace(
            Gui::plainTextEditor()->getSelectionStart(),
            Gui::plainTextEditor()->getSelectionEnd() - Gui::plainTextEditor()->getSelectionStart(),
            m_replaceWith->currentText());
        find(false);
    }

    Gui::plainTextEditor()->setCursorPositionToEnd();
}

void FindReplaceWindow::checkboxChanged()
{
    if (m_regExp->isChecked()) {
        m_wordsOnly->setChecked(false);
    }
    m_wordsOnly->setEnabled(!m_regExp->isChecked());
}

void FindReplaceWindow::updateButtons()
{
    m_findNextButton->setEnabled(!m_findWhat->currentText().isEmpty());
    m_replaceButton->setEnabled(!m_findWhat->currentText().isEmpty());
    m_replaceAllButton->setEnabled(!m_findWhat->currentText().isEmpty() &&
        m_findWhat->currentText() != m_replaceWith->currentText());
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