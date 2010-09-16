/************************************************************************
 *                                                                      *
 *  FreeSynd - a remake of the classic Bullfrog game "Syndicate".       *
 *                                                                      *
 *   Copyright (C) 2005  Stuart Binge  <skbinge@gmail.com>              *
 *   Copyright (C) 2005  Joost Peters  <joostp@users.sourceforge.net>   *
 *   Copyright (C) 2006  Trent Waddington <qg@biodome.org>              *
 *   Copyright (C) 2006  Tarjei Knapstad <tarjei.knapstad@gmail.com>    *
 *   Copyright (C) 2010  Benoit Blancard <benblan@users.sourceforge.net>*
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

#ifndef MAPMENU_H
#define MAPMENU_H

#include <assert.h>

//! Displays the mission selection map.
/*!
 * This menu displays a world map on which the player
 * selects the next mission to play.
 */
class MapMenu : public Menu {
public:
    MapMenu(MenuManager *m);
    ~MapMenu();

    void handleTick(int elapsed);
    void handleRender();
    void handleLeave();
    void handleMouseDown(int x, int y, int button);
    void handleOption(Key key);
    void handleUnknownKey(Key key, KeyMod mod, bool pressed);

    void setBlkColour(int n, uint8 c) {
        assert(n < 50);
        blk_colours_[n] = c;
    }

protected:
    int mapblk_size_;
    uint8 *mapblk_data_;
    uint8 *orig_pixels_;
    uint8 blk_colours_[50];
    int tick_count_;

    void drawSelector();
};

#endif
