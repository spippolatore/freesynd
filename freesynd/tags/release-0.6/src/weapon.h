/************************************************************************
 *                                                                      *
 *  FreeSynd - a remake of the classic Bullfrog game "Syndicate".       *
 *                                                                      *
 *   Copyright (C) 2005  Stuart Binge  <skbinge@gmail.com>              *
 *   Copyright (C) 2005  Joost Peters  <joostp@users.sourceforge.net>   *
 *   Copyright (C) 2006  Trent Waddington <qg@biodome.org>              *
 *   Copyright (C) 2006  Tarjei Knapstad <tarjei.knapstad@gmail.com>    *
 *   Copyright (C) 2010  Bohdan Stelmakh <chamel@users.sourceforge.net> *
 *   Copyright (C) 2011  Mark <mentor66@users.sourceforge.net>          *
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

#ifndef WEAPON_H
#define WEAPON_H

#include <string>
#include <vector>
#include "mapobject.h"
#include "sound/sound.h"

class WeaponInstance;

/*!
 * Weapon class.
 */
class Weapon {
public:

    WeaponInstance *createInstance();
    typedef enum {
        Persuadatron = 0,
        Pistol = 1,
        GaussGun = 2,
        Shotgun = 3,
        Uzi = 4,
        Minigun = 5,
        Laser = 6,
        Flamer = 7,
        LongRange = 8,
        Scanner = 9,
        MediKit = 10,
        TimeBomb = 11,
        AccessCard = 12,
        EnergyShield = 13,
        Unknown = 14
    } WeaponType;

    typedef enum {
        Unarmed_Anim,
        Pistol_Anim,
        Minigun_Anim,
        Flamer_Anim,
        LongRange_Anim,
        EnergyShield_Anim,
        Uzi_Anim,
        Laser_Anim,
        Gauss_Anim,
        Shotgun_Anim,
    } WeaponAnimIndex;

    Weapon(const std::string& w_name, int smallIcon, int bigIcon, int w_cost,
            int w_ammo, int w_range, int w_shot, int w_rank, int w_anim,
            WeaponAnimIndex w_idx, snd::InGameSample w_sample,
            WeaponType w_type, MapObject::DamageType w_dmg_type,
            int w_ammo_per_shot, int w_time_for_shot, int w_time_reload,
            unsigned int w_shot_property, int w_hit_anim, int w_obj_hit_anim,
            int w_rd_anim, int w_trace_anim, int w_range_dmg,
            double w_shot_angle, double w_shot_accuracy, int w_shot_speed = 0);

    const char *getName() { return name_.c_str(); }

    int cost() { return cost_; }
    int ammo() { return ammo_; }
    int range() { return range_; }
    int damagePerShot() { return damage_per_shot_; }
    int rank() { return rank_; }
    int anim() { return anim_; }
    snd::InGameSample getSound() { return sample_; }
	int getSmallIconId() { return small_icon_; }
	int getBigIconId() { return big_icon_; }

    int selector() {
        return small_icon_ == 28 ? 1618 : small_icon_ - 14 + 1602;
    }

    WeaponAnimIndex index() { return idx_; }
    WeaponType getWeaponType() { return type_; }
    MapObject::DamageType dmgType() { return dmg_type_; }

    bool operator==(Weapon weapon) { return this->type_ == weapon.getWeaponType(); }

    int ammoPerShot() { return ammo_per_shot_; }
    int timeForShot() { return time_for_shot_; }
    int timeReload() { return time_reload_; }

    bool wasSubmittedToSearch() { return submittedToSearch_; }
    void submitToSearch() { submittedToSearch_ = true; }

    typedef enum {
        spe_None = 0x0,
        // can shoot only at owner
        spe_Owner = 0x0001,

        spe_PointToPoint = 0x0002,
        spe_PointToManyPoints = 0x0004,

        spe_TargetReachInstant = 0x0008,

        spe_TargetReachNeedTime = 0x0010,
        spe_CreatesProjectile = 0x0010,

        spe_RangeDamageOnReach = 0x0020,
        // ignore accuracy
        spe_ShootsWhileNoTarget = 0x0040,
        spe_UsesAmmo = 0x0080,
        spe_ChangeAttribute = 0x0100,
        spe_SelfDestruction = 0x0200,
        spe_TargetPedOnly = 0x0400,
        spe_DamageAll = 0x0800
    }ShotPropertyEnum;

    typedef enum {
        wspt_None = spe_None,
        wspt_Persuadatron = (spe_PointToPoint | spe_TargetReachInstant
            | spe_ShootsWhileNoTarget | spe_TargetPedOnly),
        wspt_Pistol =
            (spe_PointToPoint | spe_TargetReachInstant | spe_UsesAmmo
            | spe_DamageAll),
        wspt_GaussGun =
            (spe_PointToPoint | spe_TargetReachNeedTime | spe_UsesAmmo
            | spe_RangeDamageOnReach | spe_DamageAll),
        wspt_Shotgun =
            (spe_PointToManyPoints | spe_TargetReachInstant | spe_UsesAmmo
            | spe_DamageAll),
        wspt_Uzi = (spe_PointToManyPoints | spe_TargetReachInstant
            | spe_UsesAmmo | spe_DamageAll),
        wspt_Minigun =
            (spe_PointToManyPoints | spe_TargetReachInstant | spe_UsesAmmo
            | spe_DamageAll),
        wspt_Laser =
            (spe_PointToPoint | spe_TargetReachInstant
            | spe_RangeDamageOnReach | spe_UsesAmmo | spe_DamageAll),
        wspt_Flamer =
            (spe_PointToPoint | spe_TargetReachNeedTime | spe_UsesAmmo
            | spe_DamageAll),
        wspt_LongRange =
            (spe_PointToPoint | spe_TargetReachInstant | spe_UsesAmmo
            | spe_DamageAll),
        wspt_Scanner = (spe_Owner | spe_ChangeAttribute),
        wspt_MediKit = (spe_Owner | spe_UsesAmmo),
        wspt_TimeBomb = (spe_ShootsWhileNoTarget | spe_TargetReachInstant
            | spe_RangeDamageOnReach | spe_SelfDestruction),
        wspt_AccessCard = (spe_Owner | spe_ChangeAttribute),
        wspt_EnergyShield =
            (spe_Owner | spe_ChangeAttribute | spe_UsesAmmo),
    }WeaponShotPropertyType;

    typedef enum {
        stm_AllObjects = MapObject::mjt_Ped | MapObject::mjt_Vehicle
        | MapObject::mjt_Static | MapObject::mjt_Weapon,
    }SearchTargetMask;

    typedef struct {
        PathNode tpn;
        toDefineXYZ tp;
        ShootableMapObject::DamageInflictType d;
        ShootableMapObject *smo;
    }ShotDesc;

    typedef struct {
        // when weapon hits something other then object, ground
        int hit_anim;
        int obj_hit_anim;
        int trace_anim;
        // if weapon can do range damage this is used for range definition
        // with animation
        int rd_anim;
    }ad_HitAnims;

    unsigned int shotProperty() { return shot_property_; }

    ad_HitAnims * anims() { return &anims_; }
    int rangeDmg() { return range_dmg_; }
    double shotAngle() { return shot_angle_; }
    double shotAcurracy() { return shot_accuracy_; }
    int ammoCost() { return ammo_cost_; }
    int shotSpeed() { return shot_speed_; }

protected:
    std::string name_;
    int small_icon_, big_icon_;
    int cost_, ammo_cost_, ammo_, range_, damage_per_shot_;
    int anim_;
    int rank_;  //!> weapon rank
    WeaponType type_;
    MapObject::DamageType dmg_type_;
    WeaponAnimIndex idx_;
    snd::InGameSample sample_;
    int ammo_per_shot_;
    // time weapon uses to do a single shot
    int time_for_shot_;
    // time required to make weapon ready to shoot
    int time_reload_;
    /*! True when weapon was found and submit to search manager.*/
    bool submittedToSearch_;
    unsigned int shot_property_;
    ad_HitAnims anims_;
    int range_dmg_;
    // some weapons have wider shot
    double shot_angle_;
    // agent accuracy will be applied to this, later to shot_angle_
    double shot_accuracy_;
    // only projectiles have this set (gauss, flamer)
    int shot_speed_;
};

class ShotClass {
public:
    void setOwner(ShootableMapObject *owner) {
        last_owner_ = owner_;
        owner_ = owner;
    }
    ShootableMapObject *getOwner() { return owner_; }
    bool hasOwner() { return owner_ != NULL; }

    void shotTargetRandomizer(toDefineXYZ * cp, toDefineXYZ * tp, double angle,
        double dist_new = -1, bool exclude_z = false);
    void rangeDamageAnim(toDefineXYZ &cp, double dmg_rng,
        int max_anims, int rngdamg_anim);

protected:
    void makeShot(bool rangeChecked, toDefineXYZ &cp, int anim_hit,
        std::vector <Weapon::ShotDesc> &all_shots, int anim_obj_hit,
        WeaponInstance *w = NULL);
protected:
    ShootableMapObject *owner_;
    ShootableMapObject *last_owner_;
};

/*!
 * Weapon instance class.
 */
class WeaponInstance : public ShootableMapObject, public ShotClass{
public:
    WeaponInstance(Weapon *w);

    int ammoRemaining() { return ammo_remaining_; }
    void setAmmoRemaining(int n) {  ammo_remaining_ = n; }

    bool animate(int elapsed);
    void draw(int x, int y);
    bool inflictDamage(ShootableMapObject * tobj, PathNode * tp,
        int *elapsed = NULL, bool ignoreBlocker = false,
        int *make_shots = NULL);

    Weapon *getWeaponClass() { return pWeaponClass_; }

    int range() { return pWeaponClass_->range(); }
    int ammo() { return pWeaponClass_->ammo(); }
    int rank() { return pWeaponClass_->rank(); }
    unsigned int shotProperty() { return pWeaponClass_->shotProperty(); }
    const char * name() { return pWeaponClass_->getName(); }

    Weapon::WeaponAnimIndex index() { return pWeaponClass_->index(); }
    Weapon::WeaponType getWeaponType() { return pWeaponClass_->getWeaponType(); }

    bool operator==(WeaponInstance wi) {
        return pWeaponClass_->getWeaponType() == wi.getWeaponType();
    }

    //! Plays the weapon's sound.
    void playSound();

    void resetWeaponUsedTime() { weapon_used_time_ = 0; }

    uint8 inRange(toDefineXYZ & cp, ShootableMapObject ** t,
        PathNode * pn = NULL, bool setBlocker = false,
        bool checkTileOnly = false, int maxr = -1);
    uint8 inRangeNoCP(ShootableMapObject ** t, PathNode * pn = NULL,
        bool setBlocker = false, bool checkTileOnly = false,
        int maxr = -1);


    int getShots(int *elapsed = NULL, int make_shots = 0);
    void getInRangeOne(toDefineXYZ & cp, ShootableMapObject * & target,
        uint8 mask, bool checkTileOnly = true, int maxr = -1);
    void getInRangeAll(toDefineXYZ & cp, std::vector<ShootableMapObject *> & targets,
        uint8 mask, bool checkTileOnly = true, int maxr = -1);
    bool isReloading();
    void activate();
    void deactivate();

    void getHostileInRange(toDefineXYZ * cp, ShootableMapObject * & target,
        uint8 mask, bool checkTileOnly = true, int maxr = -1);
    void getNonFriendInRange(toDefineXYZ * cp,
        ShootableMapObject * & target, bool checkTileOnly = true,
        int maxr = -1);
    bool handleDamage(ShootableMapObject::DamageInflictType * d);

protected:
    Weapon *pWeaponClass_;
    int ammo_remaining_;
    // if this value is smaller time_for_shot_ shot cannot be done 
    // if is greater then time_for_shot_ reload is in execution
    // if is greater then time_for_shot_ + time_reload_ then full shot is done
    int weapon_used_time_;
    bool activated_;
};

class ProjectileShot: public ShotClass {
public:
    ProjectileShot(toDefineXYZ &cp, Weapon::ShotDesc & sd, int d_range,
        Weapon::ad_HitAnims *panims, ShootableMapObject * ignrd_obj = NULL,
        int range_max = 1, int shot_speed = 0);
    ~ProjectileShot() {}
    bool animate(int elapsed, Mission *m);
    bool prjsLifeOver() { return life_over_; }

protected:
    toDefineXYZ cur_pos_;
    toDefineXYZ base_pos_;
    Weapon::ShotDesc sd_prj_;
    int dmg_range_;
    double dist_max_;
    double dist_passed_;
    double last_anim_dist_;
    double cur_dist_;
    double speed_;
    bool life_over_;
    // owner should be ignored
    ShootableMapObject * ignored_obj_;
    double inc_x_;
    double inc_y_;
    double inc_z_;
    Weapon::ad_HitAnims anims_;
};
#endif
