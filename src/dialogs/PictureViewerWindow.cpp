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

#include "PictureViewerWindow.h"
#include <QDebug>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPushButton>
#include <QListWidget>
#include <QString>
#include <QMenu>
#include "Book.h"

PictureViewerWindow::PictureViewerWindow(QWidget *parent) :
    QDialog(parent, Qt::Window),
    m_graphicsScene(new QGraphicsScene()),
    m_graphicsView(new QGraphicsView(m_graphicsScene)),
    m_contentsWidget(new QListWidget())
{
    m_contentsWidget->setFixedWidth(100);
    m_contentsWidget->setContextMenuPolicy(Qt::CustomContextMenu);

    QVBoxLayout *mainLayout = new QVBoxLayout();
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QHBoxLayout *imageLayout = new QHBoxLayout();

    QPushButton *closeButton = new QPushButton(tr("Close"));

    buttonLayout->addStretch(1);
    buttonLayout->addWidget(closeButton);
    connect(closeButton, SIGNAL(released()), this, SLOT(close()));

    imageLayout->addWidget(m_contentsWidget, 0);
    imageLayout->addWidget(m_graphicsView, 1);

    connect(m_contentsWidget, SIGNAL(currentRowChanged(int)),
            this, SLOT(currentRowChanged(int)));
    connect(m_contentsWidget, SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(showContextMenuForWidget(const QPoint &)));

    mainLayout->addLayout(imageLayout, 1);
    mainLayout->addLayout(buttonLayout);
    setLayout(mainLayout);
    setMinimumSize(300, 300);
    setWindowTitle(tr("Images"));
    setWindowIcon(QIcon(":/resource/icon/menu_images.png"));
}

PictureViewerWindow::~PictureViewerWindow()
{
}

void PictureViewerWindow::clearView()
{
    foreach(QGraphicsItem *item, m_graphicsScene->items()) {
        m_graphicsScene->removeItem(item);
    }
}

void PictureViewerWindow::showImage(const QPixmap &image)
{
    clearView();
    m_graphicsScene->setSceneRect(0, 0, image.width(), image.height());
    m_graphicsScene->addPixmap(image);
}

void PictureViewerWindow::showText(const QString &text)
{
    clearView();
    m_graphicsScene->setSceneRect(0, 0, 0, 0);
    m_graphicsScene->addText(text);
}

void PictureViewerWindow::showEvent(QShowEvent *event)
{
    QDialog::showEvent(event);
    reloadImageList();
    if (m_contentsWidget->count() == 0) {
        showText(tr("Book has no pictures."));
    }
}

void PictureViewerWindow::reloadImageList()
{
    m_contentsWidget->clear();
    for (int i = 0; i < Book::instance().getPicturesCount(); ++i) {
        m_contentsWidget->addItem(tr("Image") + " " + QString::number(i));
    }
}

void PictureViewerWindow::currentRowChanged(int currentRow)
{
    if (currentRow >= 0) {
        showImage(*Book::instance().getPicture(currentRow).getCurrentPicture());
    }
}

void PictureViewerWindow::showContextMenuForWidget(const QPoint &pos)
{
    QMenu contextMenu(tr("Context menu"), this);
    QAction *action = NULL;
    action = contextMenu.addAction(tr("&Find in book"), this,
            SLOT(findInDocument()));
    action = contextMenu.addAction(tr("&Remove"), this,
            SLOT(removeImage()));
    contextMenu.exec(mapToGlobal(pos));
}

void PictureViewerWindow::removeImage()
{
    int currentRow = m_contentsWidget->currentRow();
    Book::instance().removePicture(currentRow);
    reloadImageList();
    if (currentRow >= m_contentsWidget->count()) {
        currentRow = m_contentsWidget->count() - 1;
    }
    m_contentsWidget->setCurrentRow(currentRow, QItemSelectionModel::Select);
}

void PictureViewerWindow::findInDocument()
{
    qDebug() << "find";
}