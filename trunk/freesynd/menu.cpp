/************************************************************************
 *                                                                      *
 *  FreeSynd - a remake of the classic Bullfrog game "Syndicate".       *
 *                                                                      *
 *   Copyright (C) 2005  Stuart Binge  <skbinge@gmail.com>              *
 *   Copyright (C) 2005  Joost Peters  <joostp@users.sourceforge.net>   *
 *   Copyright (C) 2006  Trent Waddington <qg@biodome.org>              *
 *   Copyright (C) 2006  Tarjei Knapstad <tarjei.knapstad@gmail.com>    *
 *                                                                      *
 *    This program is free software;  you can redistribute it and / or  *
 *  modify it  under the  terms of the  GNU General  Public License as  *
 *  published by the Free Software Foundation; either version 2 of the  *
 *  License, or (at your option) any later version.                     *
 *                                                                      *
 *    This program is  distributed in the hope that it will be useful,  *
 *  but WITHOUT  ANY WARRANTY;  without even  the implied  warranty of  *
 *  MERCHANTABILITY  or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU  *
 *  General Public License for more details.                            *
 *                                                                      *
 *    You can view the GNU  General Public License, online, at the GNU  *
 *  project's  web  site;  see <http://www.gnu.org/licenses/gpl.html>.  *
 *  The full text of the license is also included in the file COPYING.  *
 *                                                                      *
 ************************************************************************/

#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "app.h"
#include "file.h"
#include "fliplayer.h"
#include "screen.h"
#ifdef SYSTEM_SDL
#include "system_sdl.h"
#endif


/*!
 * Draw the widget at the current position and only if it's
 * visible.
 * Actually, only a text is drawn (see Font). The borders are
 * already drawn on the background image.
 */
void MenuText::draw() {
    if (visible_)
        g_App.fonts().drawText(x_, y_, text_.c_str(), size_, dark_);
}

Menu::Menu(MenuManager * menuManager, const char *menu_name,
           const char *showAnim,
           const char *leaveAnim):menu_manager_(menuManager),
name_(menu_name), showAnim_(showAnim), leaveAnim_(leaveAnim),
parent_menu_(NULL), background_(NULL)
{
    menuManager->addMenu(this);
    need_rendering_ = true;
}

Menu::Menu(MenuManager * menuManager, const char *menu_name,
           const char *parent):menu_manager_(menuManager),
name_(menu_name), showAnim_(""), leaveAnim_(""),
parent_menu_(parent), background_(NULL)
{
    menuManager->addMenu(this);
    need_rendering_ = true;
}

void Menu::redrawOptions()
{
    for (std::map < Key, Option >::iterator it = options_.begin();
         it != options_.end(); it++) {
        Option & m = it->second;
        m.draw();
    }
    handleShowLate();
}

void Menu::render()
{

    if (background_) {
        g_Screen.scale2x(clear_x_, clear_y_, clear_w_ / 2, clear_h_ / 2,
                         background_ + (clear_x_ / 2) +
                         (clear_y_ / 2) * 320, 320);
    }

    for (std::list < MenuText >::iterator it = statics_.begin();
         it != statics_.end(); it++) {
        MenuText & m = *it;
        g_App.fonts().drawText(m.x_, m.y_, m.text_.c_str(), m.size_,
                               m.dark_);
    }

    handleRender();

    redrawOptions();
    
    need_rendering_ = false;
}

void Menu::addStatic(int x, int y, const char *text, int size, bool dark)
{
    MenuText m(x, y, text, size, dark, true);
    statics_.push_back(m);
}

void Menu::addOption(int x, int y, const char *text, int size, Key key,
                     const char *to, bool visible)
{
    Option m(x, y, text, size, to, visible);
    options_[key] = m;
}

void Menu::keyEvent(Key key, KeyMod mod, bool pressed)
{

    if (key == KEY_ESCAPE) {
        menu_manager_->changeCurrentMenu(parent_menu_);
        return;
    }

    if (pressed) {
        std::vector < MenuText * >options_vec;
        for (std::map < Key, Option >::iterator it = options_.begin();
             it != options_.end(); it++)
            options_vec.push_back(&it->second);

        if (options_vec.size()) {
            unsigned int curOpt = 0;
            for (unsigned int i = 0; i < options_vec.size(); i++)
                if (!options_vec[i]->dark_) {
                    curOpt = i;
                    break;
                }
            if (key == VK_UP || key == KEY_UP) {
                options_vec[curOpt]->dark_ = true;
                if (curOpt == 0)
                    curOpt = options_vec.size() - 1;
                else
                    curOpt--;
                options_vec[curOpt]->dark_ = false;
                render();
            }
            if (key == VK_DOWN || key == KEY_DOWN) {
                if (!options_vec[curOpt]->dark_) {
                    options_vec[curOpt]->dark_ = true;
                    curOpt++;
                    if (curOpt >= options_vec.size())
                        curOpt = 0;
                }
                options_vec[curOpt]->dark_ = false;
                render();
            }
            if (key == VK_FB || key == KEY_RETURN || key == KEY_KP_ENTER) {
                for (std::map < Key, Option >::iterator it =
                     options_.begin(); it != options_.end(); it++)
                    if (&it->second == options_vec[curOpt])
                        key = it->first;
            }
        }
    }

    if (options_.find(key) == options_.end()) {
        handleUnknownKey(key, mod, pressed);
        return;
    }
    handleOption(key);
    if (options_[key].to_)
        menu_manager_->changeCurrentMenu(options_[key].to_);
}

void Menu::mouseMotionEvent(int x, int y, int state)
{
    handleMouseMotion(x, y, state);

    int id = 0;
    for (std::map < Key, Option >::iterator it = options_.begin();
         it != options_.end(); it++) {
        Option & m = it->second;
        int width = 300;
        if (m.text_.size() == 1)
            width = 30;
        else
            width = g_App.fonts().textWidth(m.text_.c_str(), m.size_);
        if (m.visible_ && x > m.x_ - 2 && x < m.x_ + width && y >= m.y_ - 2
            && y < m.y_ + g_App.fonts().textHeight(m.size_) - 2) {
            if ( !m.dark_ ) {
                // the button was already highlighted : don't do anything
                break;
            } else {
                // The button is now highlighted
                m.dark_ = false;
                need_rendering_ = true;
            }
        } else {
            if (!m.dark_) {
                // The button was highlighted but not anymore : refresh
                need_rendering_ = true;
            }
            m.dark_ = true;
        }
    }
}

void Menu::mouseDownEvent(int x, int y, int button)
{
    for (std::map < Key, Option >::iterator it = options_.begin();
         it != options_.end(); it++) {
        Option & m = it->second;
        int width = 300;
        if (m.text_.size() == 1)
            width = 30;
        else
            width = g_App.fonts().textWidth(m.text_.c_str(), m.size_);
        if (m.visible_ && x > m.x_ - 2 && x < m.x_ + width && y >= m.y_ - 2
            && y < m.y_ + g_App.fonts().textHeight(m.size_) - 2) {
            keyEvent(it->first, KMD_NONE, true);
            return;
        }
    }
    handleMouseDown(x, y, button);
}

void Menu::mouseUpEvent(int x, int y, int button)
{
    handleMouseUp(x, y, button);
}
