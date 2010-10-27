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

#ifndef SELECTMENU_H
#define SELECTMENU_H

/*!
 * Select Menu class.
 */
class SelectMenu : public Menu {
public:
    SelectMenu(MenuManager *m);
    ~SelectMenu();

    void handleTick(int elapsed);
    void handleShow();
    void handleRender();
    void handleShowLate();
    void handleLeave();
    void handleMouseDown(int x, int y, int button, const int modKeys);
    void handleOption(Key key);

protected:
    void updateClock();
    //! Draws a focus around the selected agent picture
    void drawAgentSelector(int x, int y);
    void drawAgent();
    void showTeamList();
    void hideTeamList();
    void showModsList();
    void hideModsList();
    void showEquipList();
    void hideEquipList();

    void addModOptions();
    void addWeaponOptions();
    void addRecruitOptions();

    void toggleAgent(int n);

protected:
    uint8 *orig_pixels_;
    int tab_;
    int cur_agent_;
    /*! Counter to update the rnd_ field.*/
    int tick_count_;
    /*! A counter used to draw the agent selector.*/
    int rnd_;
    int sel_weapon_, sel_mod_, sel_weapon_inst_;
    bool sel_all_;
};

#endif
