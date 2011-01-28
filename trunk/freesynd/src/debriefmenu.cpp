/************************************************************************
 *                                                                      *
 *  FreeSynd - a remake of the classic Bullfrog game "Syndicate".       *
 *                                                                      *
 *   Copyright (C) 2005  Stuart Binge  <skbinge@gmail.com>              *
 *   Copyright (C) 2005  Joost Peters  <joostp@users.sourceforge.net>   *
 *   Copyright (C) 2006  Trent Waddington <qg@biodome.org>              *
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

#include <stdio.h>
#include <assert.h>
#include "app.h"
#include "debriefmenu.h"

DebriefMenu::DebriefMenu(MenuManager *m) : Menu(m, "debrief", "mdebrief.dat",
     "mdeout.dat") {
    int y = 35;
    int right_x = 310;
    int left_x = 20;
    std::string str;

    addStatic(100, y, "#DEBRIEF_TITLE", FontManager::SIZE_4, true);
    y = 100;
    menu_manager_->getMessage("DEBRIEF_SUBTITLE", str);
    addStatic(left_x, y, str.c_str(), FontManager::SIZE_2, false);
    separatorSize_ = g_App.fonts().textWidth(str.c_str(), FontManager::SIZE_2);

    y = 120;
    addStatic(left_x, y, "#DEBRIEF_STATUS", FontManager::SIZE_2, false);
    txtStatusId_ = addStatic(right_x, y, "", FontManager::SIZE_2, false);
    y += 15;
    addStatic(left_x, y, "#DEBRIEF_AGENTS", FontManager::SIZE_2, false);
    txtUsedId_ = addStatic(right_x, y, "", FontManager::SIZE_2, false);
    y += 15;
    addStatic(left_x, y, "#DEBRIEF_NEW_AGENT", FontManager::SIZE_2, false);
    txtAgentCapturedId_ = addStatic(right_x, y, "", FontManager::SIZE_2, false);
    y += 15;
    addStatic(left_x, y, "#DEBRIEF_TIME", FontManager::SIZE_2, false);
    txtTimeId_ = addStatic(right_x, y, "", FontManager::SIZE_2, false);
    y += 15;
    addStatic(left_x, y, "#DEBRIEF_AGT_KILLED", FontManager::SIZE_2, false);
    txtAgentKilledId_ = addStatic(right_x, y, "", FontManager::SIZE_2, false);
    y += 15;
    addStatic(left_x, y, "#DEBRIEF_CRIM_KILLED", FontManager::SIZE_2, false);
    txtCrimKilledId_ = addStatic(right_x, y, "", FontManager::SIZE_2, false);
    y += 15;
    addStatic(left_x, y, "#DEBRIEF_CIV_KILLED", FontManager::SIZE_2, false);
    txtCivilKilledId_ = addStatic(right_x, y, "", FontManager::SIZE_2, false);
    y += 15;
    addStatic(left_x, y, "#DEBRIEF_POL_KILLED", FontManager::SIZE_2, false);
    txtPoliceKilledId_ = addStatic(right_x, y, "", FontManager::SIZE_2, false);
    y += 15;
    addStatic(left_x, y, "#DEBRIEF_GUARD_KILLED", FontManager::SIZE_2, false);
    txtGardKilledId_ = addStatic(right_x, y, "", FontManager::SIZE_2, false);
    y += 15;
    addStatic(left_x, y, "#DEBRIEF_PERSUADED", FontManager::SIZE_2, false);
    txtConvincedId_ = addStatic(right_x, y, "", FontManager::SIZE_2, false);
    y += 15;
    addStatic(left_x, y, "#DEBRIEF_HIT", FontManager::SIZE_2, false);
    txtPrecisionId_ = addStatic(right_x, y, "", FontManager::SIZE_2, false);

    y = 352;
    menu_manager_->getMessage("MENU_ACC_BUT", str);
    int butX = 80 - g_App.fonts().textWidth(str.c_str(), FontManager::SIZE_2) / 2;
    addOption(butX, y, str.c_str(), FontManager::SIZE_2, KEY_F5, "map");
    addOption(535, y, "#MENU_MAIN_BUT", FontManager::SIZE_2, KEY_F6, "main");
}

/*!
 * This method sets the menu cursor and shows it and
 * updates infos based on mission statistics.
 */
void DebriefMenu::handleShow() {
    g_System.useMenuCursor();
    g_System.showCursor();

    Mission *pMission = g_Session.getMission();
    MissionStats *pStats = pMission->getStatistics();

    if (pMission->getStatus() == Mission::FAILED) {
        getStatic(txtStatusId_)->setText("#DEBRIEF_MIS_FAILED");
    } else if (pMission->getStatus() == Mission::COMPLETED) {
        getStatic(txtStatusId_)->setText("#DEBRIEF_MIS_COMP");
    } else if (pMission->getStatus() == Mission::ABORTED) {
        getStatic(txtStatusId_)->setText("#DEBRIEF_MIS_ABORT");
    }

    getStatic(txtUsedId_)->setTextFormated("%i", pStats->agents);
    getStatic(txtAgentCapturedId_)->setTextFormated("%i", pStats->agentCaptured);

    int days = 0;
    int hours = 0;
    g_Session.getDayHourFromPeriod(pStats->mission_duration, days, hours);
    getStatic(txtTimeId_)->setTextFormated("#DEBRIEF_TIME_FORMAT", hours, days);
    getStatic(txtAgentKilledId_)->setTextFormated("%i", pStats->enemyKilled);
    getStatic(txtCrimKilledId_)->setTextFormated("%i", pStats->criminalKilled);
    getStatic(txtCivilKilledId_)->setTextFormated("%i", pStats->civilKilled);
    getStatic(txtPoliceKilledId_)->setTextFormated("%i", pStats->policeKilled);
    getStatic(txtGardKilledId_)->setTextFormated("%i", pStats->guardKilled);
    getStatic(txtConvincedId_)->setTextFormated("%i", pStats->convinced);

    if (pStats->nbOfShots == 0) {
        getStatic(txtPrecisionId_)->setText("#DEBRIEF_NO_BULLET");
    } else {
        int precision = (pStats->nbOfHits * 100) / pStats->nbOfShots ;
        getStatic(txtPrecisionId_)->setTextFormated("%i%%", precision);
    }
}

void DebriefMenu::handleRender() {
    // Display team logo
    g_Screen.drawLogo(18, 14, g_App.getGameSession().getLogo(), g_App.getGameSession().getLogoColour());

    // Draws separator line between title and statistics
    g_Screen.drawLine(20, 117, 20 + separatorSize_, 117, 252);
    g_Screen.drawLine(20, 116, 20 + separatorSize_, 116, 252);
}

void DebriefMenu::handleLeave() {
    // We're leaving to the map menu
    // So at this point the current mission is no more needed
    // as a new one will be selected in the map menu
    g_Session.setMission(NULL);

    g_System.hideCursor();
}
