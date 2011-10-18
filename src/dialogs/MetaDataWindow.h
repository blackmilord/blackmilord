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

#ifndef BLACK_MILORD_METADATA_WINDOW_H
#define BLACK_MILORD_METADATA_WINDOW_H

#include <QDialog>

class QLineEdit;

class MetaDataWindow : public QDialog
{
    Q_OBJECT
public:
    MetaDataWindow(QWidget *parent = 0);
    ~MetaDataWindow();

private slots:
    void apply();
    void ok();
    void metadataChanged();

private:
    QLineEdit *m_author;
    QLineEdit *m_publisher;
    QLineEdit *m_description;
    QLineEdit *m_isbn;
    QLineEdit *m_subject;

    void reloadMetadata();
};

#endif /* BLACK_MILORD_METADATA_WINDOW_H */
