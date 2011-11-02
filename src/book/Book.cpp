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

#include "Book.h"
#include <QFile>
#include <QStringList>
#include <QDebug>
#include <QVariant>
#include <QTextCursor>

#include <PlainTextEditor.h>
#include <Preferences.h>
#include "AbstractBook.h"

Book* Book::m_instance = NULL;

Book::Book(PlainTextEditor *editor) :
    m_editor(editor)
{
    reset();
}

Book::~Book()
{
}

void Book::createInstance(PlainTextEditor *editor)
{
    Q_ASSERT(m_instance == NULL);
    m_instance = new Book(editor);
}

Book& Book::instance()
{
    Q_ASSERT(m_instance != NULL);
    return *m_instance;
}

bool Book::openFile(const QString &fileName)
{
    Q_ASSERT(!isFileOpened());
    bool result = AbstractBookFactory::getObject()->openFile(fileName);
    if (result) {
        m_fileName = fileName;
        m_fileOpened = true;
        emit fileLoaded();
    }
    return result;
}

bool Book::saveFile()
{
    if (m_fileName.isEmpty()) {
        return false;
    }
    if (QFile::exists(m_fileName) && Preferences::instance().getValue(Preferences::PROP_MAKE_BACKUP_BEFORE_OVERWRITE, true).toBool()) {
        if (!m_backupManager.create(m_fileName)) {
            m_why = tr("Cannot create backup file.");
            return false;
        }
    }
    bool result = AbstractBookFactory::getObject()->saveFile(m_fileName);
    if (result) {
        emit fileSaved();
    }
    return result;
}

bool Book::closeFile()
{
    setText("");
    m_editor->clearRedoUndoHistory();
    reset();
    emit fileClosed();
    return true;
}

bool Book::newFile()
{
    reset();
    bool result = AbstractBookFactory::getObject()->newFile();
    if (result) {
        m_fileOpened = true;
        emit fileCreated();
    }
    return result;
}

void Book::reset()
{
    m_fileOpened = false;

    m_author.clear();
    m_publisher.clear();
    m_description.clear();
    m_isbn.clear();
    m_subject.clear();
    m_version = 0;
    m_creationDate = QDateTime();
    m_modificationDate = QDateTime();
    m_lastBackupDate = QDateTime();
    m_modificationNumber = 0;

    m_fileName.clear();
    m_why.clear();
}

bool Book::isFileOpened()
{
    return m_fileOpened;
}

//book properties

QVariant Book::getMetadata(MetaData metadata) const
{
    switch (metadata)
    {
    case METADATA_AUTHOR:
        return m_author;
    case METADATA_PUBLISHER:
        return m_publisher;
    case METADATA_DESCRIPTION:
        return m_description;
    case METADATA_ISBN:
        return m_isbn;
    case METADATA_SUBJECT:
        return m_subject;
    case METADATA_VERSION:
        return m_version;
    case METADATA_CREATION_DATE:
        return m_creationDate;
    case METADATA_MODIFICATION_DATE:
        return m_modificationDate;
    case METADATA_LAST_BACKUP_DATE:
        return m_lastBackupDate;
    case METADATA_MODIFICATION_NUMBER:
        return m_modificationNumber;
    default:
        break;
    }
    Q_ASSERT(false);
    return QVariant();
}

void Book::setMetadata(MetaData metadata, const QVariant &data)
{
    switch (metadata)
    {
    case METADATA_AUTHOR:
        m_author = data.toString();
        break;
    case METADATA_PUBLISHER:
        m_publisher = data.toString();
        break;
    case METADATA_DESCRIPTION:
        m_description = data.toString();
        break;
    case METADATA_ISBN:
        m_isbn = data.toString();
        break;
    case METADATA_SUBJECT:
        m_subject = data.toString();
        break;
    case METADATA_VERSION:
        m_version = data.toUInt();
        break;
    case METADATA_CREATION_DATE:
        m_creationDate = data.toDateTime();
        break;
    case METADATA_MODIFICATION_DATE:
        m_modificationDate = data.toDateTime();
        break;
    case METADATA_LAST_BACKUP_DATE:
        m_lastBackupDate = data.toDateTime();
        break;
    case METADATA_MODIFICATION_NUMBER:
        m_modificationNumber = data.toUInt();
        break;
    default:
        Q_ASSERT(false);
    }
    emit metadataChanged();
}

QString Book::getText() const
{
    return m_editor->toPlainText();
}

void Book::replace(int position, int length, const QString &after)
{
    Q_ASSERT(position >= 0);
    Q_ASSERT(position + length <= m_editor->toPlainText().length());

    QTextCursor cursor = m_editor->textCursor();
    cursor.setPosition(position);
    cursor.setPosition(position + length, QTextCursor::KeepAnchor);
    m_editor->setTextCursor(cursor);
    cursor.insertText(after);
}

void Book::setText(const QString& text)
{
    const QString &old = getText();
    if (old.length() != text.length() || old != text) {
        QTextCursor cursor = m_editor->textCursor();
        cursor.select(QTextCursor::Document);
        m_editor->setTextCursor(cursor);
        cursor.insertText(text);
    }
}

int Book::getCursorPosition() const
{
    return m_editor->textCursor().position();
}

void Book::setCursorPosition(int position)
{
    if (getCursorPosition() != position) {
        QTextCursor cursor = m_editor->textCursor();
        cursor.setPosition(position);
        m_editor->setTextCursor(cursor);
    }
}

void Book::setCursorPositionToStart()
{
    QTextCursor cursor = m_editor->textCursor();
    cursor.movePosition(QTextCursor::Start);
    m_editor->setTextCursor(cursor);
}

void Book::setCursorPositionToEnd()
{
    QTextCursor cursor = m_editor->textCursor();
    cursor.movePosition(QTextCursor::End);
    m_editor->setTextCursor(cursor);
}

int Book::getSelectionStart() const
{
    Q_ASSERT(hasSelection());
    return m_editor->textCursor().selectionStart();
}

int Book::getSelectionEnd() const
{
    Q_ASSERT(hasSelection());
    return m_editor->textCursor().selectionEnd();
}

bool Book::hasSelection() const
{
    return m_editor->textCursor().hasSelection();
}

void Book::setSelection(int selectionStart, int selectionEnd)
{
    QTextCursor cursor = m_editor->textCursor();
    int oldSelectionStart = cursor.selectionStart();
    int oldSelectionEnd = cursor.selectionEnd();
    Q_ASSERT(selectionStart >= 0 && selectionEnd >= 0);
    if (oldSelectionStart != selectionStart || oldSelectionEnd != selectionEnd) {
        cursor.setPosition(selectionStart);
        cursor.setPosition(selectionEnd, QTextCursor::KeepAnchor);
        m_editor->setTextCursor(cursor);
    }
}

void Book::clearSelection()
{
    m_editor->textCursor().clearSelection();
}

QString Book::getFileName() const
{
    return m_fileName;
}

void Book::setFileName(const QString fileName)
{
    m_fileName = fileName;
}

QString Book::getWhy() const
{
    return m_why;
}

void Book::setWhy(const QString &why)
{
    m_why = why;
}
