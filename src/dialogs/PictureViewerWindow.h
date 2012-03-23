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

#ifndef BLACK_MILORD_PICTURE_VIEWER_WINDOW_H
#define BLACK_MILORD_PICTURE_VIEWER_WINDOW_H

#include <QDialog>

class QGraphicsScene;
class QGraphicsView;
class QPixmap;
class QListWidget;
class QString;

class PictureViewerWindow : public QDialog
{
    Q_OBJECT
public:
    explicit PictureViewerWindow(QWidget *parent = 0);
    virtual ~PictureViewerWindow();

protected:
    void showEvent(QShowEvent *event);

private:
    QGraphicsScene *m_graphicsScene;
    QGraphicsView *m_graphicsView;
    QListWidget *m_contentsWidget;

    void showImage(const QPixmap &image);
    void showText(const QString &text);
    inline void clearView();

private slots:
    void reloadImageList();
    void currentRowChanged(int currentRow);
    void showContextMenuForWidget(const QPoint &);
    void removeImage();
    void findInDocument();
};

#endif /* BLACK_MILORD_PICTURE_VIEWER_WINDOW_H */