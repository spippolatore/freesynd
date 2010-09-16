/************************************************************************
 *                                                                      *
 *  FreeSynd - a remake of the classic Bullfrog game "Syndicate".       *
 *                                                                      *
 *   Copyright (C) 2005  Stuart Binge  <skbinge@gmail.com>              *
 *   Copyright (C) 2005  Joost Peters  <joostp@users.sourceforge.net>   *
 *   Copyright (C) 2006  Trent Waddington <qg@biodome.org>              *
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

#ifndef MENU_H
#define MENU_H

#include <string>
#include <list>
#include <map>
#include "keys.h"
#include "common.h"

class FontManager;
class Menu;
class MenuManager;

//! A text widget.
/*!
 * This class represents a text label.
 * This is the base class for widget : it displays a text
 * at a given location.
 * A widget can be visible or not, and the text is displayed
 * in dark or light green.
 */
class MenuText {
public:
    int x_, y_;
    /*! The text to be displayed.*/
    std::string text_;
    int size_;
    bool dark_;
    bool visible_;

    MenuText() : x_(0), y_(0), text_(""), size_(0), dark_(false),
    visible_(true) {}

    MenuText(int x, int y, const char *text, int size, bool dark,
            bool visible):x_(x), y_(y), text_(text),
        size_(size), dark_(dark), visible_(visible) {
    }

    //! Draw the widget on screen
    virtual void draw();
};
//! A button widget.
/*!
 * This class extends the MenuText class to represent a button on
 * the screen. A button can lead to another screen which name is
 * stored in the field "to".
 */
class Option : public MenuText {
public:
    /*! The name of the next menu.*/
    const char *to_;

    Option() : MenuText() {
        to_ = NULL;
    }

    Option(int x, int y, const char *text, int size,
            const char *to, bool visible):MenuText(x, y, text, size, true, visible) {
        to_ = to;
    }
};

/*!
 * Menu class.
 */
class Menu {
public:
    Menu(MenuManager *menuManager, const char *name, const char *showAnim,
            const char *leaveAnim);
    Menu(MenuManager *menuManager, const char *name, const char *parent);
    virtual ~Menu() {}

    const char *name() { return name_.c_str(); }

    void setClearArea(uint8 *background, int x, int y, int w, int h) {
        background_ = background;
        clear_x_ = x;
        clear_y_ = y;
        clear_w_ = w;
        clear_h_ = h;
    }

    virtual bool isSubMenu() {
        return leaveAnim_.size() == 0;
    }

    bool hasShowAnim() { return showAnim_.size() != 0; }
    const char * getShowAnimName() { return showAnim_.c_str(); }
    bool hasLeaveAnim() { return leaveAnim_.size() != 0; }
    const char * getLeaveAnimName() { return leaveAnim_.c_str(); }
    /*! Returns true if the menu needs to be rendered. */
    bool needRendering() { return need_rendering_; }
    void render();
    void addStatic(int x, int y, const char *text, int size, bool dark);
    void addOption(int x, int y, const char *text, int size, Key key,
            const char *to = NULL, bool visible = true);

    void setParentMenu(const char *m) { parent_menu_ = m; }

    void keyEvent(Key key, KeyMod mod, bool pressed);
    void mouseMotionEvent(int x, int y, int state);
    void mouseDownEvent(int x, int y, int button);
    void mouseUpEvent(int x, int y, int button);

    void hideOption(Key key) {
        if (options_.find(key) == options_.end())
            return;

        options_[key].visible_ = false;
        redrawOptions();
    }

    void showOption(Key key) {
        if (options_.find(key) == options_.end())
            return;

        options_[key].visible_ = true;
        redrawOptions();
    }

    virtual void handleTick(int elapsed) {}
    virtual void handleUnknownKey(Key key, KeyMod mod, bool pressed) {}
    virtual void handleMouseDown(int x, int y, int button) {}
    virtual void handleMouseUp(int x, int y, int button) {}
    virtual void handleMouseMotion(int x, int y, int state) {}

    //! Callback function : Childs can reimplement
    /*! 
     * Called just after the opening animation is played (if one has
     * been defined) and before the menu is rendered for the first time.
     */
    virtual void handleShow() {}

    //! Callback function : Childs can reimplement
    /*! 
     * Called each time a menu is rendered.
     */
    virtual void handleRender() {}

    //! Callback function : Childs can reimplement
    /*! 
     * Called just before the closing animation is played (if one has
     * been defined) and the menu closed.
     */
    virtual void handleLeave() {}
    virtual void handleShowLate() {}
    virtual void handleOption(Key key) {}

protected:
    MenuManager *menu_manager_;
    std::string name_;
    std::string showAnim_, leaveAnim_;
    std::list<MenuText> statics_;
    std::map<Key, Option> options_;
    /*! This flag tells whether the menu needs to be redrawn. */
    bool need_rendering_;
    const char *parent_menu_;
    uint8 *background_;
    int clear_x_, clear_y_, clear_w_, clear_h_;

    void redrawOptions();
};

#endif
