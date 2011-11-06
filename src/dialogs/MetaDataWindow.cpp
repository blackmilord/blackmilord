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

#include "MetaDataWindow.h"
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QVariant>

#include <Dictionary.h>
#include <MetadataEnum.h>
#include <Book.h>

MetaDataWindow::MetaDataWindow(QWidget *parent) :
    QDialog(parent)
{
    QVBoxLayout *layout = new QVBoxLayout();
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QGridLayout *dataLayout = new QGridLayout();

    QPushButton *okButton = new QPushButton(tr("OK"), this);
    QPushButton *applyButton = new QPushButton(tr("Apply"), this);
    QPushButton *cancelButton = new QPushButton(tr("Cancel"), this);

    buttonLayout->addStretch(1);
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(applyButton);
    buttonLayout->addWidget(cancelButton);

    connect(okButton, SIGNAL(clicked()), this, SLOT(ok()));
    connect(applyButton, SIGNAL(clicked()), this, SLOT(apply()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(close()));

    dataLayout->addWidget(new QLabel(Dictionary::bookMetaDataLabel(METADATA_AUTHOR)), 0, 0);
    dataLayout->addWidget(m_author = new QLineEdit(), 0 , 1);

    dataLayout->addWidget(new QLabel(Dictionary::bookMetaDataLabel(METADATA_PUBLISHER)), 1, 0);
    dataLayout->addWidget(m_publisher = new QLineEdit(), 1 , 1);

    dataLayout->addWidget(new QLabel(Dictionary::bookMetaDataLabel(METADATA_DESCRIPTION)), 2, 0);
    dataLayout->addWidget(m_description = new QLineEdit(), 2 , 1);

    dataLayout->addWidget(new QLabel(Dictionary::bookMetaDataLabel(METADATA_ISBN)), 3, 0);
    dataLayout->addWidget(m_isbn = new QLineEdit(), 3 , 1);

    dataLayout->addWidget(new QLabel(Dictionary::bookMetaDataLabel(METADATA_SUBJECT)), 4, 0);
    dataLayout->addWidget(m_subject = new QLineEdit(), 4 , 1);

    /*
    METADATA_VERSION,
    METADATA_CREATION_DATE,
    METADATA_MODIFICATION_DATE,
    METADATA_LAST_BACKUP_DATE,
    METADATA_MODIFICATION_NUMBER
    */

    dataLayout->setColumnMinimumWidth(0, 75);
    dataLayout->setColumnStretch(0, 0);
    dataLayout->setColumnStretch(1, 1);

    layout->addLayout(dataLayout);
    layout->addStretch(1);
    layout->addLayout(buttonLayout);
    setLayout(layout);
    setMinimumHeight(300);
    setMinimumWidth(300);
    setWindowTitle(tr("Metadata editor"));
    setWindowIcon(QIcon(":/resource/icon/menu_metadata.png"));
    metadataChanged();
}

MetaDataWindow::~MetaDataWindow()
{
}

void MetaDataWindow::apply()
{
    Book::instance().setMetadata(METADATA_AUTHOR, m_author->text());
    Book::instance().setMetadata(METADATA_PUBLISHER, m_publisher->text());
    Book::instance().setMetadata(METADATA_DESCRIPTION, m_description->text());
    Book::instance().setMetadata(METADATA_ISBN, m_isbn->text());
    Book::instance().setMetadata(METADATA_SUBJECT, m_subject->text());
}

void MetaDataWindow::ok()
{
    apply();
    close();
}

void MetaDataWindow::showEvent(QShowEvent *event)
{
    QDialog::showEvent(event);
    metadataChanged();
    m_author->setFocus();
}

void MetaDataWindow::metadataChanged()
{
    m_author->setText(Book::instance().getMetadata(METADATA_AUTHOR).toString());
    m_publisher->setText(Book::instance().getMetadata(METADATA_PUBLISHER).toString());
    m_description->setText(Book::instance().getMetadata(METADATA_DESCRIPTION).toString());
    m_isbn->setText(Book::instance().getMetadata(METADATA_ISBN).toString());
    m_subject->setText(Book::instance().getMetadata(METADATA_SUBJECT).toString());
}
