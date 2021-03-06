/************************************************************************
 *                                                                      *
 *  FreeSynd - a remake of the classic Bullfrog game "Syndicate".       *
 *                                                                      *
 *   Copyright (C) 2012  Benoit Blancard <benblan@users.sourceforge.net>*
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

#include "menus/squadselection.h"
#include "agentmanager.h"
#include "ped.h"
#include "vehicle.h"

/*!
 * Default constructor.
 */
SquadSelection::SquadSelection() {
    pSquad_ = NULL;
    selected_agents_ = 0;
    leader_ = 0;
}

/*!
 * Sets the squad and selects the first selectable agent.
 * Pre existing selection has been cleared.
 * \param pSquad The current squad.
 */
void SquadSelection::setSquad(Squad *pSquad) {
    clear();
    pSquad_ = pSquad;

    for (size_t i = AgentManager::kSlot1; i < AgentManager::kMaxSlot; i++) {
        // try to select agent. if true means agent has been selected
        if (selectAgent(i, false)) {
            break;
        }
    }
}

/*!
 * Clear current selection.
 */
void SquadSelection::clear() {
    selected_agents_ = 0;
}

/*!
 * Returns the number of selected agents
 * \return 0 if no agent is selected.
 */
size_t SquadSelection::size() {
    int agents = 0;

    for (size_t i = AgentManager::kSlot1; i < AgentManager::kMaxSlot; i++) {
        if (isAgentSelected(i)) {
            agents++;
        }
    }

    return agents;
}

/*!
 * Selects or deselects the agent on the given slot.
 * \param agentNo
 * \param addToGroup True if agent is added to the selection.
 * \return True if agent has been selected or deselected.
 */
bool SquadSelection::selectAgent(size_t agentNo, bool addToGroup) {
    if (isAgentSelectable(agentNo)) {
        // saves the current selection to check if it changes
        size_t oldSelection = selected_agents_;
        if (addToGroup) {
            // Add/remove to the group
            selected_agents_ ^= 1 << agentNo;
            // check if leader was deselected
            checkLeader(agentNo);
        } else {
            selected_agents_ = 1 << agentNo;
            leader_ = agentNo;
        }

        return (oldSelection != selected_agents_);
    }

    return false;
}

/*!
 * Deselects an agent. Called when an agent died.
 * \param p_ped The agent to deselect.
 */
void SquadSelection::deselectAgent(PedInstance *p_ped) {
    for (size_t i = AgentManager::kSlot1; i < AgentManager::kMaxSlot; i++) {
        if (pSquad_->member(i) == p_ped) {
            selected_agents_ &= ~(1 << i);
            // check if leader was deselected
            checkLeader(i);
            return;
        }
    }
}

/*!
 * Selects all agents.
 * \param b_selectAll True to select, false deselect
 */
void SquadSelection::selectAllAgents(bool b_selectAll) {
    if (b_selectAll) {
        // Select all agents
        for (size_t i = AgentManager::kSlot1; i < AgentManager::kMaxSlot; i++) {
            if (isAgentSelectable(i)) {
                selected_agents_ |= (1 << i);
            }
        }
    } else {
        // Deselect all except leader
        selected_agents_ = (1 << leader_);
    }
}

/*!
 * Returns true if agent is active and alive.
 */
bool SquadSelection::isAgentSelectable(size_t agentNo) {
    PedInstance *pPed = pSquad_->member(agentNo);
    return  pPed && pPed->isAlive();
}

/*!
 * checks if leader has been deselected then finds
 * a new leader.
 */
void SquadSelection::checkLeader(size_t agentNo) {
    if (agentNo == leader_) {
        // The leader has been deselected
        // find another leader among the remaining selection
        for (size_t i = AgentManager::kSlot1; i < AgentManager::kMaxSlot; i++) {
            if (isAgentSelectable(i) && leader_ != i) {
                leader_ = i;
                break;
            }
        }
    }
}

/*!
 * Deselects all selected agents selected weapon.
 */
void SquadSelection::deselect_all_weapons() {
    for (SquadSelection::Iterator it = begin(); it != end(); ++it) {
            (*it)->deselectWeapon();
    }
}

/*!
 * Selects the given weapon for the leader and updates weapon selection
 * for other selected agents.
 * \param weapon_idx The index in the leader inventory of the weapon to select.
 * \param apply_to_all In case of Medikit, all selected agents must use one.
 */
void SquadSelection::select_weapon_from_leader(int weapon_idx, bool apply_to_all) {
    PedInstance *pLeader = leader();
    PedInstance::WeaponSelectCriteria pw_to_use;
    pw_to_use.desc = WeaponHolder::WeaponSelectCriteria::kCritPlayerSelection;
    pw_to_use.criteria.wi = pLeader->weapon(weapon_idx);
    pw_to_use.apply_to_all = apply_to_all;

    for (SquadSelection::Iterator it = begin(); it != end(); ++it)
    {
        PedInstance *ped = *it;
        if (pLeader == ped) {
            // Forces selection of the weapon for the leader
            pLeader->selectWeapon(weapon_idx);
        } else {
            // For other agents, it depends on their actual selection
           ped->selectRequiredWeapon(&pw_to_use);
        }
    } // end for
}

/*!
 * Choose the first agent who has free space in his inventory and send
 * him pickup weapon.
 * \param pWeapon The weapon to pickup
 * \param addAction True to add the action at the end of the list of action,
 * false to set as the only action.
 */
void SquadSelection::pickupWeapon(ShootableMapObject *pWeapon, bool addAction) {
    for (SquadSelection::Iterator it = begin(); it != end(); ++it)
    {
        PedInstance *pAgent = *it;
        // Agent has space in inventory
        if (pAgent->numWeapons() < Agent::kMaxWeaponPerAgent) {
            PedInstance::actionQueueGroupType as;
            pAgent->createActQPickUp(as, pWeapon);
            as.main_act = as.actions.size() - 1;
            as.group_desc = PedInstance::gd_mStandWalk;
            as.origin_desc = fs_actions::kOrigUser;
            pAgent->addAction(as, addAction);

            break;
        }
    }
}

/*!
 * All selected agents that are not in a vehicle, follows the given pedestrian.
 * \param pPed The ped to follow
 * \param addAction True to add the action at the end of the list of action,
 * false to set as the only action.
 */
void SquadSelection::followPed(ShootableMapObject *pPed, bool addAction) {
    for (SquadSelection::Iterator it = begin(); it != end(); ++it)
    {
        PedInstance *pAgent = *it;
        if (!pAgent->inVehicle()) { // Agent must not be in a vehicle
            PedInstance::actionQueueGroupType as;
            pAgent->createActQFollowing(as, pPed, 0, 192);
            as.main_act = as.actions.size() - 1;
            as.group_desc = PedInstance::gd_mStandWalk;
            as.origin_desc = fs_actions::kOrigUser;
            pAgent->addAction(as, addAction);
        }
    }
}

/*!
 * Selected agents enter or leave the given vehicle. First check where the
 * leader is : 
 * - if he is in a vehicle (can be different from the given one),
 *   every selected agent that is not in a vehicle, gets in the vehicle.
 * - else every selected gets out of the given vehicle.
 * \param pPed The ped to follow
 * \param addAction True to add the action at the end of the list of action,
 * false to set as the only action.
 */
void SquadSelection::enterOrLeaveVehicle(ShootableMapObject *pVehicle, bool addAction) {
    // true means every one get in the vehicle
    bool getIn = leader()->inVehicle() == NULL;

    for (SquadSelection::Iterator it = begin(); it != end(); ++it)
    {
        PedInstance *pAgent = *it;
        PedInstance::actionQueueGroupType as;
        bool action = false;
        
        if (getIn && !pAgent->inVehicle()) {
            // Agent is out and everybody must get in
            pAgent->createActQGetInCar(as, pVehicle);
            action = true;
        } else if (!getIn && pAgent->inVehicle() == (VehicleInstance *)pVehicle) {
            // Agent is in the given car and everybody must get out
            pAgent->createActQLeaveCar(as, pVehicle);
            action = true;
        }
        
        if (action) {
            as.main_act = as.actions.size() - 1;
            as.group_desc = PedInstance::gd_mStandWalk;
            as.origin_desc = fs_actions::kOrigUser;
            pAgent->addAction(as, addAction);
        }
    }
}

/*!
 * Tells every agents in the selection to go to the given point.
 * Agents in a drivable vehicle will use vehicle and other will walk.
 * \param mapPt The destination point
 * \param addAction True to add the action at the end of the list of action,
 * false to set as the only action.
 */
void SquadSelection::moveTo(MapTilePoint &mapPt, bool addAction) {
    int i=0;
    for (SquadSelection::Iterator it = begin(); it != end(); ++it, i++)
    {
        PedInstance *pAgent = *it;
        PedInstance::actionQueueGroupType as;
        if (pAgent->inVehicle()) {
            if (pAgent->inVehicle()->isDrivable()) { 
                // Agent is in drivable vehicle
                VehicleInstance *pVehicle = pAgent->inVehicle();
                if (pVehicle->isDriver(pAgent))
                {
                    int stx = mapPt.tx;
                    int sty = mapPt.ty;
                    //int sox = ox;
                    //int soy = oy;
                    stx = mapPt.tx * 256 + mapPt.ox + 128 * (pVehicle->tileZ() - 1);
                    //sox = stx % 256;
                    stx = stx / 256;
                    sty = mapPt.ty * 256 + mapPt.oy + 128 * (pVehicle->tileZ() - 1);
                    //soy = sty % 256;
                    sty = sty / 256;
                    PathNode tpn = PathNode(stx, sty, 0, 128, 128);
                    pAgent->createActQUsingCar(as, &tpn, pVehicle);
                    as.main_act = as.actions.size() - 1;
                    as.group_desc = PedInstance::gd_mStandWalk;
                    as.origin_desc = fs_actions::kOrigUser;
                    pAgent->addAction(as, addAction);
                }
            }
        } else {
            MapTilePoint tmpPt(mapPt);

            if (size() > 1) {
                //TODO: current group position is like
                // in original this can make non-tile
                // oriented
                //int sox = (i % 2) * (i - 2) * 16;
                //int soy = ((i + 1) % 2) * (i - 1) * 8;

                //this should be romoved if non-tile
                //position needed
                tmpPt.ox = 63 + 128 * (i % 2);
                tmpPt.oy = 63 + 128 * (i >> 1);
            }

            PathNode tpn = PathNode(tmpPt.tx, tmpPt.ty, tmpPt.tz, tmpPt.ox, tmpPt.oy, 0);
            pAgent->createActQWalking(as, &tpn, NULL);
            as.main_act = as.actions.size() - 1;
            as.group_desc = PedInstance::gd_mStandWalk;
            as.origin_desc = fs_actions::kOrigUser;
            pAgent->addAction(as, addAction);
        }
    } // end of for
}