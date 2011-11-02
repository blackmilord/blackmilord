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

#ifndef BLACK_MILORD_MAIN_WINDOW_H
#define BLACK_MILORD_MAIN_WINDOW_H

#include <QMainWindow>
#include <QVector>
#include <AbstractHighlighter.h>

class PlainTextEditor;
class QString;
class MobiFile;
class HowToUseAspellWindow;
class SpellCheckingWindow;
class FindReplaceWindow;
class OptionsWindow;
class MetaDataWindow;
class AboutWindow;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget * parent = 0, Qt::WindowFlags flags = 0);
    virtual ~MainWindow();

private:
    PlainTextEditor *m_editor;
    HowToUseAspellWindow *m_howToUseAspellWindow;
    SpellCheckingWindow *m_spellCheckingWindow;
    FindReplaceWindow *m_findReplaceWindow;
    OptionsWindow *m_optionsWindow;
    MetaDataWindow *m_metadataWindow;
    AboutWindow *m_aboutWindow;

    void resizeEvent(QResizeEvent *event);
    void moveEvent(QMoveEvent *event);
    void changeEvent(QEvent *event);
    void closeEvent(QCloseEvent *event);

    void updateMenuEnable(bool fileOpened);

private slots:
    void applySettings();
    void createFile();
    void fileCreated();
    void openMobiFile();
    void fileOpened();
    void saveFile();
    void saveFileAs();
    void fileSaved();
    void closeFile();
    void fileClosed();
    void showHowToUseAspellWindow();
    void showSpellCheckingWindow();
    void showFindReplaceWindow();
    void showOptionsWindow();
    void showMetadataWindow();
    void showAboutWindow();
    void setWindowTitle(bool modified);
    void quit();
};

#endif /* BLACK_MILORD_MAIN_WINDOW_H */

