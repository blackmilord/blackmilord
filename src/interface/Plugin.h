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

#ifndef BLACK_MILORD_PLUGIN_H
#define BLACK_MILORD_PLUGIN_H

class QString;
class QLayout;

class Plugin
{
public:
    explicit Plugin(bool enabled = false) :
        m_enabled(enabled)
    {
    }

    virtual ~Plugin()
    {
    }

    /**
     * This funcion provides inteface to configure a highlighter.
     * @return layout with configuration dialog
     */
    virtual QLayout* configurationLayout() = 0;

    /**
     * Reset layout to current settins;
     */
    virtual void resetConfigurationLayout() = 0;

    /**
     * This function saves configuration provided
     * by user in @see configurationLayout() in application preferences.
     * This function should not modify internal settins, but store it in application preferences.
     * Internal settins are updated by @see applySettings() function.
     * It guaranties thread-save behaviour.
     */
    virtual void saveSettings() = 0;

    /**
     * This function is called when highlighter should reread configuration
     * from application preferences. It is guaranted it it's not called during @see highlightBlock() execution.
     */
    virtual void applySettings() = 0;

    virtual bool isEnabled()
    {
        return m_enabled;
    }

    /**
     * Function provides unique highlighter's GUID.
     * @return highlighter's GUID.
     */
    virtual QString guid() const = 0;

    /**
     * Function provides highlighter's name.
     * @return highlighter's name.
     */
    virtual QString name() const = 0;

protected:
    bool m_enabled;
};

Q_DECLARE_INTERFACE(Plugin, "org.blackmilord.Plugin/1.0");

#endif /* BLACK_MILORD_PLUGIN_H */