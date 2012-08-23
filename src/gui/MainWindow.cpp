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

#include "MainWindow.h"
#include <QDebug>
#include <QMenuBar>
#include <QMenu>
#include <QKeySequence>
#include <QFileDialog>
#include <QString>
#include <QVBoxLayout>
#include <QApplication>
#include <QMessageBox>
#include <QDesktopWidget>
#include <QLabel>

#include "Gui.h"
#include <StatusBar.h>
#include <PlainTextEditor.h>
#include <Spellcheck.h>
#include <Book.h>
#include <FindReplaceWindow.h>
#include <HowToUseAspellWindow.h>
#include <SpellCheckingWindow.h>
#include <FindReplaceWindow.h>
#include <OptionsWindow.h>
#include <MetaDataWindow.h>
#include <AboutWindow.h>
#include <PictureViewerWindow.h>
#include <Preferences.h>

namespace {
    QString PROGRAM_TITLE = QObject::tr("BlackMilord eBook editor");
}

MainWindow::MainWindow(QWidget * parent, Qt::WindowFlags flags) :
    QMainWindow(parent, flags),
    m_howToUseAspellWindow(NULL),
    m_spellCheckingWindow(NULL),
    m_findReplaceWindow(NULL),
    m_optionsWindow(NULL),
    m_metadataWindow(NULL),
    m_aboutWindow(NULL),
    m_pictureViewerWindow(NULL)
{
    //Widgets
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *centralLayout = new QVBoxLayout;
    centralWidget->setLayout(centralLayout);
    setCentralWidget(centralWidget);

    Gui::setPlainTextEditor(new PlainTextEditor());
    Gui::plainTextEditor()->setEnabled(false);
    centralLayout->addWidget(Gui::plainTextEditor()->asWidget());
    Gui::setStatusBar(new StatusBar());
    setStatusBar(Gui::statusBar());

    //Menu
    //File
    QMenu *menu = menuBar()->addMenu(tr("&File"));
    QAction *action;
    action = menu->addAction(QIcon(":/resource/icon/menu_new.png"), tr("&New"), this,
            SLOT(createFile()), QKeySequence::New);
    menu->addSeparator();
    QMenu *submenu = menu->addMenu(QIcon(":/resource/icon/menu_import.png"), "&Import");
    action = submenu->addAction(tr("&Open Mobi file"), this,
            SLOT(openMobiFile()), QKeySequence::Open);
    menu->addSeparator();
    action = menu->addAction(QIcon(":/resource/icon/menu_save.png"), tr("&Save"), this,
            SLOT(saveFile()), QKeySequence::Save);
    action->setObjectName("enable_on_open");
    action = menu->addAction(QIcon(":/resource/icon/menu_save_as.png"), tr("&Save As..."), this,
            SLOT(saveFileAs()), QKeySequence::SaveAs);
    action->setObjectName("enable_on_open");
    action = menu->addAction(tr("&Close"), this,
            SLOT(closeFile()), QKeySequence::Close);
    action->setObjectName("enable_on_open");
    menu->addSeparator();
    menu->addAction(QIcon(":/resource/icon/menu_exit.png"), tr("E&xit"), this,
            SLOT(quit()), QKeySequence::Quit);
    //Edit
    menu = menuBar()->addMenu(tr("&Edit"));
    QAction *undoAction = menu->addAction(QIcon(":/resource/icon/menu_undo.png"), tr("&Undo"));
    undoAction->setShortcut(QKeySequence::Undo);
    undoAction->setEnabled(false);
    QAction *redoAction = menu->addAction(QIcon(":/resource/icon/menu_redo.png"), tr("&Redo"));
    redoAction->setShortcut(QKeySequence::Redo);
    redoAction->setEnabled(false);
    menu->addSeparator();
    action = menu->addAction(QIcon(":/resource/icon/menu_find_and_replace.png"), "&Find and Replace",
            this, SLOT(showFindReplaceWindow()), QKeySequence::Find);
    action->setObjectName("enable_on_open");
    if (Spellcheck::instance().isLoaded()) {
        action = menu->addAction(QIcon(":/resource/icon/menu_spell_check.png"), tr("&Spell check "),
                this, SLOT(showSpellCheckingWindow()), QKeySequence(Qt::Key_F7));
        action->setObjectName("enable_on_open");
    }
    else {
        menu->addAction(tr("How to use Aspell..."),
                this, SLOT(showHowToUseAspellWindow()));
    }
    //Book
    menu = menuBar()->addMenu(tr("&Book"));
    action = menu->addAction(QIcon(":/resource/icon/menu_metadata.png"), tr("&Metadata"),
            this, SLOT(showMetadataWindow()));
    action->setObjectName("enable_on_open");
    action = menu->addAction(QIcon(":/resource/icon/menu_images.png"), tr("&Images"),
            this, SLOT(showPictureViewerWindow()));
    action->setObjectName("enable_on_open");
    //Options
    menu = menuBar()->addMenu(tr("&Options"));
    menu->addAction(QIcon(":/resource/icon/menu_settings.png"), "&Settings",
            this, SLOT(showOptionsWindow()));
    //Help
    menu = menuBar()->addMenu(tr("&Help"));
    menu->addAction("&About", this, SLOT(showAboutWindow()));

    //Connections
    connect(&Book::instance(), SIGNAL(fileLoaded()),
            this, SLOT(fileOpened()));
    connect(&Book::instance(), SIGNAL(fileClosed()),
            this, SLOT(fileClosed()));
    connect(&Book::instance(), SIGNAL(fileSaved()),
            this, SLOT(fileSaved()));
    connect(&Book::instance(), SIGNAL(fileCreated()),
            this, SLOT(fileCreated()));
    connect(&Preferences::instance(), SIGNAL(settingsChanged()),
            this, SLOT(applySettings()));
    connect(Gui::plainTextEditor()->asObject(), SIGNAL(modificationChanged(bool)), this, SLOT(setWindowTitle(bool)));
    connect(Gui::plainTextEditor()->asObject(), SIGNAL(redoAvailable(bool)), redoAction, SLOT(setEnabled(bool)));
    connect(Gui::plainTextEditor()->asObject(), SIGNAL(undoAvailable(bool)), undoAction, SLOT(setEnabled(bool)));
    connect(undoAction, SIGNAL(triggered()), Gui::plainTextEditor()->asObject(), SLOT(undo()));
    connect(redoAction, SIGNAL(triggered()), Gui::plainTextEditor()->asObject(), SLOT(redo()));

    //Other things
    QApplication::setWindowIcon(QIcon(":/resource/icon/application_icon.png"));
    setWindowTitle(false);

    updateMenuEnable(false);

    QRect available = QDesktopWidget().availableGeometry(this);
    int defWidth = 300;
    int defHeight = 300;
    int defX = (available.width() - defWidth) / 2;
    int defY = (available.height() - defHeight) / 2;
    setMinimumSize(defWidth, defHeight);
    setGeometry(
        Preferences::instance().getWindowPositionX(defX),
        Preferences::instance().getWindowPositionY(defY),
        Preferences::instance().getWindowWidth(defWidth),
        Preferences::instance().getWindowHeight(defHeight));
    if (QDesktopWidget().screenNumber(this) == -1) {
        //reset position if not visible;
        setGeometry(0, 0, 300, 300);
    }

    applySettings();
}

MainWindow::~MainWindow()
{
}

void MainWindow::applySettings()
{
}

void MainWindow::createFile()
{
    if (Book::instance().isFileOpened()) {
        closeFile();
    }
    Book::instance().newFile();
}

void MainWindow::fileCreated()
{
    Gui::plainTextEditor()->setEnabled(true);
    updateMenuEnable(true);
    setWindowTitle(false);
    Gui::plainTextEditor()->setModified(false);
    Gui::plainTextEditor()->setFocus();
}

void MainWindow::openMobiFile()
{
    if (Book::instance().isFileOpened()) {
        closeFile();
    }
    QString allFiles(tr("All files (*.*)"));
    QString mobiFiles(tr("Mobi files (*.mobi *.prc)"));
    QString fileName = QFileDialog::getOpenFileName(
            this,
            tr("Open file"),
            Preferences::instance().getLastUsedDirectory(),
            allFiles + ";;" + mobiFiles,
            &mobiFiles);
    if (fileName.isNull()) {
        return;
    }
    Preferences::instance().setLastUsedDirectory(fileName);
    if (!Book::instance().openFile(fileName)) {
        QMessageBox::warning(this,
                tr("Cannot open file"),
                Book::instance().getWhy());
    }
}

void MainWindow::fileOpened()
{
    Gui::plainTextEditor()->setEnabled(true);
    updateMenuEnable(true);
    setWindowTitle(false);
    Gui::plainTextEditor()->setFocus();
    Gui::plainTextEditor()->setCursorPosition(0);
    Gui::plainTextEditor()->clearRedoUndoHistory();
    Gui::plainTextEditor()->setModified(false);
}

void MainWindow::saveFile()
{
    QString fileName = Book::instance().getFileName();
    if (fileName.isEmpty()) {
        fileName = QFileDialog::getSaveFileName(
                this,
                tr("Save file"),
                Preferences::instance().getLastUsedDirectory());
        if (fileName.isEmpty()) {
            return;
        }
        Preferences::instance().setLastUsedDirectory(fileName);
        Book::instance().setFileName(fileName);
    }
    if (!Book::instance().saveFile())
    {
        QMessageBox::warning(this,
                tr("File is not saved"),
                Book::instance().getWhy());
    }
}

void MainWindow::saveFileAs()
{
    QString fileName = QFileDialog::getSaveFileName(
            this,
            tr("Save file as..."),
            Preferences::instance().getLastUsedDirectory());
    if (fileName.isEmpty()) {
        return;
    }
    Preferences::instance().setLastUsedDirectory(fileName);
    Book::instance().setFileName(fileName);
    saveFile();
}

void MainWindow::fileSaved()
{
    setWindowTitle(false);
    Gui::plainTextEditor()->setModified(false);
}

void MainWindow::closeFile()
{
    Q_ASSERT(Book::instance().isFileOpened());
    if (Gui::plainTextEditor()->isModified()) {
        QMessageBox::StandardButton answer = QMessageBox::question(this,
                tr("File is not saved"),
                tr("Do you want to save file now?"),
                QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel,
                QMessageBox::Yes);
        if (QMessageBox::Yes == answer) {
            saveFile();
        }
        else if (QMessageBox::Cancel == answer) {
            return;
        }
    }
    Book::instance().closeFile();
}

void MainWindow::fileClosed()
{
    updateMenuEnable(false);
    Gui::plainTextEditor()->setEnabled(false);
    Gui::plainTextEditor()->setModified(false);
    Gui::statusBar()->showMessage(tr("Document is closed"));
    Gui::plainTextEditor()->clearRedoUndoHistory();

    setWindowTitle(false);
}

void MainWindow::showHowToUseAspellWindow()
{
    if (!m_howToUseAspellWindow) {
        Q_ASSERT(!Spellcheck::instance().isLoaded());
        m_howToUseAspellWindow = new HowToUseAspellWindow(this);
        m_howToUseAspellWindow->setModal(true);
    }
    m_howToUseAspellWindow->show();
}

void MainWindow::showSpellCheckingWindow()
{
    if (!m_spellCheckingWindow) {
        Q_ASSERT(Spellcheck::instance().isLoaded());
        m_spellCheckingWindow = new SpellCheckingWindow(this);
    }
    m_spellCheckingWindow->show();
}

void MainWindow::showFindReplaceWindow()
{
    if (!m_findReplaceWindow) {
        m_findReplaceWindow = new FindReplaceWindow(this);
    }
    m_findReplaceWindow->show();
}

void MainWindow::showOptionsWindow()
{
    if (!m_optionsWindow) {
        m_optionsWindow = new OptionsWindow(this);
    }
    m_optionsWindow->show();
}

void MainWindow::showMetadataWindow()
{
    if (!m_metadataWindow) {
        m_metadataWindow = new MetaDataWindow(this);
    }
    m_metadataWindow->show();
}

void MainWindow::showAboutWindow()
{
    if (!m_aboutWindow) {
        m_aboutWindow = new AboutWindow(this);
    }
    m_aboutWindow->show();
}

void MainWindow::showPictureViewerWindow()
{
    if (!m_pictureViewerWindow) {
        m_pictureViewerWindow = new PictureViewerWindow(this);
    }
    m_pictureViewerWindow->show();
}

void MainWindow::setWindowTitle(bool modified)
{
    if (Book::instance().isFileOpened()) {
        QString title = PROGRAM_TITLE + " [" + Book::instance().getFileName() + "]";
        if (modified) {
            title.append(" *");
        }
        QMainWindow::setWindowTitle(title);
    }
    else {
        QMainWindow::setWindowTitle(PROGRAM_TITLE);
    }
}

void MainWindow::quit()
{
    if (Book::instance().isFileOpened()) {
        closeFile();
    }
    if (Book::instance().isFileOpened()) {
        return;
    }
    QApplication::instance()->quit();
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    if (!Preferences::instance().getWindowMaximized()) {
        Preferences::instance().setWindowWidth(event->size().width());
        Preferences::instance().setWindowHeight(event->size().height());
    }
    event->ignore();
}

void MainWindow::moveEvent(QMoveEvent *event)
{
    if (!Preferences::instance().getWindowMaximized()) {
        Preferences::instance().setWindowPositionX(event->pos().x());
        Preferences::instance().setWindowPositionY(event->pos().y());
    }
    event->ignore();
}

void MainWindow::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::WindowStateChange) {
        Preferences::instance().setWindowMaximized(isMaximized());
    }
    event->ignore();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    quit();
    event->ignore();
}

void MainWindow::updateMenuEnable(bool fileOpened)
{
    foreach(QAction *action, menuBar()->findChildren<QAction *>("enable_on_open")) {
        action->setEnabled(fileOpened);
    }
}
