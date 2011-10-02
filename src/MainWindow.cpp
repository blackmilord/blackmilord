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
#include <QPlainTextEdit>
#include <QVBoxLayout>
#include <QApplication>
#include <QSyntaxHighlighter>
#include <QMessageBox>
#include <QDesktopWidget>

#include "AspellWrapper.h"
#include <HighlighterManager.h>
#include <Book.h>
#include <FindReplaceWindow.h>
#include <HowToUseAspellWindow.h>
#include <SpellCheckingWindow.h>
#include <FindReplaceWindow.h>
#include <OptionsWindow.h>
#include <MetaDataWindow.h>
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
    m_metadataWindow(NULL)
{
    //Widgets
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *centralLayout = new QVBoxLayout;
    centralWidget->setLayout(centralLayout);

    m_editor = new QPlainTextEdit(this);
    m_editor->setEnabled(false);
    centralLayout->addWidget(m_editor);
    setCentralWidget(centralWidget);

    HighlighterManagerFactory::createInstance(m_editor);
    Book::createInstance(m_editor);

    //Menu
    QMenu *menu = menuBar()->addMenu(tr("&File"));
    QAction *action;
    action = menu->addAction(QIcon(":/icon/menu_new.png"), tr("&New"), this,
            SLOT(createFile()), QKeySequence(Qt::CTRL + Qt::Key_N));

    menu->addSeparator();
    QMenu *submenu = menu->addMenu(QIcon(":/icon/menu_import.png"), "&Import");

    action = submenu->addAction(tr("&Open Mobi file"), this,
            SLOT(openMobiFile()), QKeySequence(Qt::CTRL + Qt::Key_O));

    menu->addSeparator();

    action = menu->addAction(QIcon(":/icon/menu_save.png"), tr("&Save"), this,
            SLOT(saveFile()), QKeySequence(Qt::CTRL + Qt::Key_S));
    action->setObjectName("enable_on_open");

    action = menu->addAction(QIcon(":/icon/menu_save_as.png"), tr("&Save As..."), this,
            SLOT(saveFileAs()), QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_S));
    action->setObjectName("enable_on_open");

    action = menu->addAction(tr("&Close"), this,
            SLOT(closeFile()), QKeySequence(Qt::CTRL + Qt::Key_F4));
    action->setObjectName("enable_on_open");

    menu->addSeparator();

    menu->addAction(QIcon(":/icon/menu_exit.png"), tr("E&xit"), QApplication::instance(),
            SLOT(quit()), QKeySequence(Qt::ALT + Qt::Key_F4));

    menu = menuBar()->addMenu(tr("&Edit"));

    action = menu->addAction(tr("Metadata"), this, SLOT(showMetadataWindow()));
    action->setObjectName("enable_on_open");

    menu->addSeparator();

    action = menu->addAction(QIcon(":/icon/menu_find_and_replace.png"), "&Find and Replace", this,
            SLOT(showFindReplaceWindow()), QKeySequence(Qt::CTRL + Qt::Key_F));
    action->setObjectName("enable_on_open");

    if (ASpellWrapper::instance().isLoaded()) {
        action = menu->addAction(QIcon(":/icon/menu_check_spelling.png"), tr("Check &spelling"), this,
                SLOT(showSpellCheckingWindow()), QKeySequence(Qt::Key_F7));
        action->setObjectName("enable_on_open");
    }
    else {
        menu->addAction(tr("How to use Aspell..."),
                this, SLOT(showHowToUseAspellWindow()));
    }

    menu = menuBar()->addMenu(tr("&Options"));
    menu->addAction(QIcon(":/icon/menu_settings.png"), "&Settings", this, SLOT(showOptionsWindow()));

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
    connect(m_editor, SIGNAL(modificationChanged(bool)),
            this, SLOT(setWindowTitle(bool)));
    connect(m_editor->document(), SIGNAL(contentsChange(int, int, int)),
            this, SLOT(contentsChange(int, int, int)));

    //Other things
    QApplication::setWindowIcon(QIcon(":/icon/application_icon.png"));
    setWindowTitle(false);

    updateMenuEnable(false);

    QRect available = QDesktopWidget().availableGeometry(this);
    int defWidth = 300;
    int defHeight = 300;
    int defX = (available.width() - defWidth) / 2;
    int defY = (available.height() - defHeight) / 2;
    setMinimumSize(defWidth, defHeight);
    setGeometry(
        Preferences::instance().getValue(Preferences::PROP_WINDOW_POSITION_X, defX).toInt(),
        Preferences::instance().getValue(Preferences::PROP_WINDOW_POSITION_Y, defY).toInt(),
        Preferences::instance().getValue(Preferences::PROP_WINDOW_WIDTH, defWidth).toInt(),
        Preferences::instance().getValue(Preferences::PROP_WINDOW_HEIGHT, defHeight).toInt());
    if (QDesktopWidget().screenNumber(this) == -1) {
        //reset position if not visible;
        setGeometry(0, 0, 300, 300);
    }
    if (Preferences::instance().getValue(Preferences::PROP_WINDOW_MAXIMIZED, isMaximized()).toBool()) {
        showMaximized();
    }

    applySettings();

    show();
}

MainWindow::~MainWindow()
{
}

void MainWindow::applySettings()
{
    m_editor->setFont(Preferences::instance().getDefaultFontEditor());
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
    //m_editor->setPlainText(Book::instance().getText());
    m_editor->setEnabled(true);
    updateMenuEnable(true);
    setWindowTitle(false);
    m_editor->document()->setModified(false);
    m_editor->setFocus();
}

void MainWindow::openMobiFile()
{
    if (Book::instance().isFileOpened()) {
        closeFile();
    }
    QString allFiles(tr("All files (*.*)"));
    QString mobiFiles(tr("Mobi files (*.mobi *.prc)"));
    QString fileName = QFileDialog::getOpenFileName(this,
            tr("Open file"),
            Preferences::instance().getValue(
                    Preferences::PROP_LAST_USED_DIRECTORY, "").toString(),
            allFiles + ";;" + mobiFiles,
            &mobiFiles);
    if (fileName.isNull()) {
        return;
    }
    Preferences::instance().saveLastDir(fileName);
    if (!Book::instance().openFile(fileName)) {
        QMessageBox::warning(this,
                tr("Cannot open file"),
                Book::instance().getWhy());
    }
    Book::instance().setFileName(fileName);
}

void MainWindow::fileOpened()
{
    //m_editor->setPlainText(Book::instance().getText());
    m_editor->setEnabled(true);
    updateMenuEnable(true);
    setWindowTitle(false);
    m_editor->document()->setModified(false);
    m_editor->setFocus();
}

void MainWindow::saveFile()
{
    Book::instance().setText(m_editor->toPlainText());
    QString fileName = Book::instance().getFileName();
    if (fileName.isEmpty()) {
        fileName = QFileDialog::getSaveFileName(this,
                tr("Save file"),
                Preferences::instance().getValue(
                        Preferences::PROP_LAST_USED_DIRECTORY, "").toString());
        if (fileName.isEmpty()) {
            return;
        }
        Preferences::instance().saveLastDir(fileName);
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
    QString fileName = QFileDialog::getSaveFileName(this,
            tr("Save file as..."),
            Preferences::instance().getValue(
                    Preferences::PROP_LAST_USED_DIRECTORY, "").toString());
    if (fileName.isEmpty()) {
        return;
    }
    Preferences::instance().saveLastDir(fileName);
    Book::instance().setFileName(fileName);
    saveFile();
}

void MainWindow::fileSaved()
{
    setWindowTitle(false);
    m_editor->document()->setModified(false);
}

void MainWindow::closeFile()
{
    Q_ASSERT(Book::instance().isFileOpened());
    if (m_editor->document()->isModified()) {
        if (QMessageBox::Yes == QMessageBox::question(this,
                tr("File is not saved"),
                tr("Do you want to save file now?"),
                QMessageBox::Yes | QMessageBox::No,
                QMessageBox::Yes))
        {
            saveFile();
        }
                
    }
    Book::instance().closeFile();
}

void MainWindow::fileClosed()
{
    updateMenuEnable(false);
    m_editor->clear();
    m_editor->setEnabled(false);
    m_editor->document()->setModified(false);
    setWindowTitle(false);
}

void MainWindow::showHowToUseAspellWindow()
{
    if (!m_howToUseAspellWindow) {
        Q_ASSERT(!ASpellWrapper::instance().isLoaded());
        m_howToUseAspellWindow = new HowToUseAspellWindow(this);
        m_howToUseAspellWindow->setModal(true);
    }
    m_howToUseAspellWindow->show();
}

void MainWindow::showSpellCheckingWindow()
{
    if (!m_spellCheckingWindow) {
        Q_ASSERT(ASpellWrapper::instance().isLoaded());
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

void MainWindow::resizeEvent(QResizeEvent *event)
{
    if (!Preferences::instance().getValue(Preferences::PROP_WINDOW_MAXIMIZED, isMaximized()).toBool()) {
        Preferences::instance().setValue(Preferences::PROP_WINDOW_WIDTH, event->size().width());
        Preferences::instance().setValue(Preferences::PROP_WINDOW_HEIGHT, event->size().height());
    }
    event->ignore();
}

void MainWindow::moveEvent(QMoveEvent *event)
{
    if (!Preferences::instance().getValue(Preferences::PROP_WINDOW_MAXIMIZED, isMaximized()).toBool()) {
        Preferences::instance().setValue(Preferences::PROP_WINDOW_POSITION_X, event->pos().x());
        Preferences::instance().setValue(Preferences::PROP_WINDOW_POSITION_Y, event->pos().y());
    }
    event->ignore();
}

void MainWindow::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::WindowStateChange) {
        Preferences::instance().setValue(Preferences::PROP_WINDOW_MAXIMIZED, isMaximized());
    }
    event->ignore();
}

void MainWindow::updateMenuEnable(bool fileOpened)
{
    foreach(QAction *action, menuBar()->findChildren<QAction *>("enable_on_open")) {
        action->setEnabled(fileOpened);
    }
}

void MainWindow::contentsChange(int position, int charsRemoved, int charsAdded)
{
    Q_UNUSED(charsRemoved);
    const QTextBlock &firstBlock = m_editor->document()->findBlock(position);
    const QTextBlock &lastBlock = m_editor->document()->findBlock(position + charsAdded);
    for (int i = firstBlock.blockNumber(); i <= lastBlock.blockNumber(); ++i) {
        HighlighterManagerFactory::instance().highlightBlock(i);
    }
}
