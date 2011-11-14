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

#include <PlainTextEditor.h>
#include <StatusBar.h>

#include "Gui.h"

PlainTextEditor* Gui::m_editor = NULL;
StatusBar* Gui::m_statusBar = NULL;

PlainTextEditor* Gui::plainTextEditor()
{
    Q_ASSERT(NULL != m_editor);
    return m_editor;
}

void Gui::setPlainTextEditor(PlainTextEditor *editor)
{
    Q_ASSERT(NULL == m_editor || editor == NULL);
    m_editor = editor;
}

StatusBar* Gui::statusBar()
{
    Q_ASSERT(NULL != m_statusBar);
    return m_statusBar;
}

void Gui::setStatusBar(StatusBar *statusBar)
{
    Q_ASSERT(NULL == m_statusBar || statusBar == NULL);
    m_statusBar = statusBar;
}
