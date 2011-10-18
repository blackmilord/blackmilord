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

#ifndef BLACK_MILORD_BOOK_H
#define BLACK_MILORD_BOOK_H

#include <QDateTime>
#include <MobiFileObject.h>
#include "BackupManager.h"
#include "MetadataEnum.h"

class QString;
class QVariant;
class PlainTextEditor;

class Book : public QObject
{
    Q_OBJECT
    Book(PlainTextEditor *editor);
    ~Book();

public:
    static void createInstance(PlainTextEditor *editor);
    static Book& instance();

    bool openFile(const QString &fileName);
    bool saveFile();
    bool closeFile();
    bool newFile();

    bool isFileOpened();

    QString getText() const;

    int getCursorPosition() const;
    int getSelectionStart() const;
    int getSelectionEnd() const;
    bool hasSelection() const;
    void clearSelection();

    QVariant getMetadata(MetaData metadata) const;

    QString getFileName() const;
    void setFileName(const QString fileName);

    QString getWhy() const;

public slots:
    void setText(const QString& text);
    void setCursorPosition(int position);
    void setSelection(int selectionStart, int selectionEnd);
    void setMetadata(MetaData metadata, const QVariant &data);
    void setWhy(const QString &why);

signals:
    void fileLoaded();
    void fileSaved();
    void fileClosed();
    void fileCreated();
    void textChanged();
    void cursonPositionChanged();
    void selectionChanged();
    void metadataChanged();

private:
    void reset();

    static Book *m_instance;
    PlainTextEditor *m_editor;

    //book metadata
    QString m_author;
    QString m_publisher;
    QString m_description;
    QString m_isbn;
    QString m_subject;
    quint16 m_version;
    QDateTime m_creationDate;
    QDateTime m_modificationDate;
    QDateTime m_lastBackupDate;
    quint32 m_modificationNumber;

    //internal metadata
    bool m_fileOpened;
    QString m_fileName;
    QString m_why;

    BackupManager m_backupManager;
};

#endif /* BLACK_MILORD_BOOK_H */
