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

#include "Tests.h"
#include "options/Preferences.h"
#include <QtTest/QtTest>
#include <QDebug>

#include <MainWindow.h>
#include <Gui.h>
#include <PlainTextEditor.h>

void BlackMilordTests::initTestCase()
{
}

void BlackMilordTests::cleanupTestCase()
{
}

void BlackMilordTests::init()
{
    m_mainWindow = new MainWindow();
}

void BlackMilordTests::cleanup()
{
    delete m_mainWindow;
    Gui::setPlainTextEditor(NULL);
    Gui::setStatusBar(NULL);
}

void BlackMilordTests::check_PlainTextEditor_setTextgetText()
{
    PlainTextEditor *editor = Gui::plainTextEditor();
    QString testString = "test";
    editor->setPlainText(testString);
    QVERIFY(editor->toPlainText() == testString);
    testString = "test\ntest";
    editor->setPlainText(testString);
    QVERIFY(editor->toPlainText() == testString);
}

void BlackMilordTests::check_PlainTextEditor_typing()
{
    PlainTextEditor *editor = Gui::plainTextEditor();
    QTest::keyEvent(QTest::Click, editor->asWidget(), Qt::Key_A);
    QVERIFY(editor->toPlainText() == "a");
    QTest::keyEvent(QTest::Click, editor->asWidget(), Qt::Key_B);
    QVERIFY(editor->toPlainText() == "ab");
}

void BlackMilordTests::check_PlainTextEditor_visibleBlocks()
{
    PlainTextEditor *editor = Gui::plainTextEditor();
    QString testString = "test\ntest";
    editor->setPlainText(testString);
    QVERIFY(editor->firstVisibleBlock() == 0);
    QVERIFY(editor->lastVisibleBlock() == 1);
}

void BlackMilordTests::check_PlainTextEditor_getCursorPosition()
{
    PlainTextEditor *editor = Gui::plainTextEditor();
    QVERIFY(editor->getCursorPosition() == 0);
    QTest::keyEvent(QTest::Click, editor->asWidget(), Qt::Key_A);
    QVERIFY(editor->getCursorPosition() == 1);
    QTest::keyEvent(QTest::Click, editor->asWidget(), Qt::Key_B);
    QVERIFY(editor->getCursorPosition() == 2);
    QTest::keyEvent(QTest::Click, editor->asWidget(), Qt::Key_Enter);
    QVERIFY(editor->getCursorPosition() == 3);
    QTest::keyEvent(QTest::Click, editor->asWidget(), Qt::Key_A);
    QVERIFY(editor->getCursorPosition() == 4);
}

void BlackMilordTests::check_PlainTextEditor_setCursorPosition()
{
    PlainTextEditor *editor = Gui::plainTextEditor();
    QString testString = "testtest";
    editor->setPlainText(testString);
    QVERIFY(editor->getCursorPosition() == 8);
    editor->setCursorPosition(4);
    QVERIFY(editor->getCursorPosition() == 4);
    editor->setCursorPosition(0);
    QVERIFY(editor->getCursorPosition() == 0);
    editor->setCursorPositionToEnd();
    QVERIFY(editor->getCursorPosition() == 8);
    editor->setCursorPositionToStart();
    QVERIFY(editor->getCursorPosition() == 0);
}

void BlackMilordTests::check_PlainTextEditor_selection()
{
    PlainTextEditor *editor = Gui::plainTextEditor();
    QString testString = "testtest";
    editor->setPlainText(testString);
    QVERIFY(editor->hasSelection() == false);
    QVERIFY(editor->getSelectionStart() == editor->getSelectionEnd());
    QVERIFY(editor->getSelectionStart() == editor->getCursorPosition());
    editor->setSelection(0, 4);
    QVERIFY(editor->hasSelection() == true);
    QVERIFY(editor->getSelectionStart() == 0);
    QVERIFY(editor->getSelectionEnd() == 4);
    QVERIFY(editor->getSelectedText() == "test");
    editor->clearSelection();
    QVERIFY(editor->hasSelection() == false);
    QVERIFY(editor->getSelectionStart() == editor->getSelectionEnd());
    QVERIFY(editor->getSelectionStart() == editor->getCursorPosition());
}

void BlackMilordTests::check_PlainTextEditor_blockCount()
{
    PlainTextEditor *editor = Gui::plainTextEditor();
    QString testString = "test\ntest";
    editor->setPlainText(testString);
    QVERIFY(editor->blockCount() == 2);
}

void BlackMilordTests::check_PlainTextEditor_redoUndoAvailability()
{
    PlainTextEditor *editor = Gui::plainTextEditor();
    QVERIFY(!editor->canRedo());
    QVERIFY(!editor->canUndo());
    QTest::keyEvent(QTest::Click, editor->asWidget(), Qt::Key_A);
    QVERIFY(!editor->canRedo());
    QVERIFY(!editor->canUndo());
    QTest::keyEvent(QTest::Click, editor->asWidget(), Qt::Key_Enter);
    QVERIFY(!editor->canRedo());
    QVERIFY(editor->canUndo());
    editor->undo();
    QVERIFY(editor->canRedo());
    QVERIFY(!editor->canUndo());
}