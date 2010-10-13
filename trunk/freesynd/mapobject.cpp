/************************************************************************
 *                                                                      *
 *  FreeSynd - a remake of the classic Bullfrog game "Syndicate".       *
 *                                                                      *
 *   Copyright (C) 2005  Stuart Binge  <skbinge@gmail.com>              *
 *   Copyright (C) 2005  Joost Peters  <joostp@users.sourceforge.net>   *
 *   Copyright (C) 2006  Trent Waddington <qg@biodome.org>              *
 *   Copyright (C) 2006  Tarjei Knapstad <tarjei.knapstad@gmail.com>    *
 *   Copyright (C) 2010  Bohdan Stelmakh <chamel@users.sourceforge.net> *
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

#include "common.h"
#include "app.h"
#ifdef SYSTEM_SDL
#include "system_sdl.h"
#endif
MapObject::MapObject(int m):map_(m), frame_(0), elapsed_carry_(0),
frames_per_sec_(15), sub_type_(0), main_type_(0)
{
}

int MapObject::screenX()
{
    return g_App.maps().map(map())->tileToScreenX(tile_x_, tile_y_,
                                                  tile_z_, off_x_, off_y_);
}

int MapObject::screenY()
{
    return g_App.maps().map(map())->tileToScreenY(tile_x_, tile_y_,
                                                  tile_z_, off_x_, off_y_);
}

void MapObject::setOffX(int n)
{
    off_x_ = n;
    while (off_x_ < 0) {
        off_x_ += 256;
        tile_x_--;
    }
    while (off_x_ > 255) {
        off_x_ -= 256;
        tile_x_++;
    }
}

void MapObject::setOffY(int n)
{
    off_y_ = n;
    while (off_y_ < 0) {
        off_y_ += 256;
        tile_y_--;
    }
    while (off_y_ > 255) {
        off_y_ -= 256;
        tile_y_++;
    }
}

void MapObject::setOffZ(int n)
{
    off_z_ = n;
    while (off_z_ < 0) {
        off_z_ += 128;
        tile_z_--;
    }
    while (off_z_ > 127) {
        off_z_ -= 128;
        tile_z_++;
    }
}

void MapObject::addOffs(int &x, int &y)
{
    x += ((off_x_ - off_y_) * (TILE_WIDTH / 2)) / 256;
    y += ((off_x_ + off_y_) * (TILE_HEIGHT / 3)) / 256;
    y -= (off_z_ * (TILE_HEIGHT / 3)) / 128;
    y += (tile_z_ - vis_z_) * TILE_HEIGHT / 3;
}

bool MapObject::animate(int elapsed)
{
    int frame_tics_ = 1000 / frames_per_sec_;
    int total_elapsed = elapsed + elapsed_carry_;
    elapsed_carry_ = total_elapsed % frame_tics_;
    int framewas = frame_;
    bool changed = true;
    frame_ += (total_elapsed / frame_tics_);
    if (framewas == frame_)
        changed = false;
    frame_ %= frames_per_sec_ << 2;
    return changed;
}

ShootableMapObject::ShootableMapObject(int m):MapObject(m)
{
}

ShootableMovableMapObject::
ShootableMovableMapObject(int m):ShootableMapObject(m), dir_(0),
speed_(0)
{
}

bool ShootableMovableMapObject::updatePlacement(int nOffX, int nOffY)
{

    off_x_ = nOffX;
    off_y_ = nOffY;
    bool changed = false;

    while (off_x_ < 0) {
        off_x_ += 256;
        tile_x_--;
        changed = true;
    }
    while (off_x_ > 255) {
        off_x_ -= 256;
        tile_x_++;
        changed = true;
    }
    while (off_y_ < 0) {
        off_y_ += 256;
        tile_y_--;
        changed = true;
    }
    while (off_y_ > 255) {
        off_y_ -= 256;
        tile_y_++;
        changed = true;
    }

    return changed;
}

Static *Static::loadInstance(uint8 * data, int m)
{
    Mission::LEVELDATA_STATICS * gamdata =
        (Mission::LEVELDATA_STATICS *) data;
    Static *s = 0;

    // TODO: find where object current state is,
    // some doors are open, windows are broken etc
    // Also verify whether object description is correct
    uint16 curanim = READ_LE_UINT16(gamdata->index_current_anim);
    switch(gamdata->sub_type) {
        case 0x01:
            // phone booth
            s = new EtcObj(m, curanim, curanim, curanim);
            break;
        case 0x05:
            // crossroad things
            s = new EtcObj(m, curanim, curanim, curanim);
            break;
        case 0x06:
            // crossroad things
            s = new EtcObj(m, curanim, curanim, curanim);
            break;
        case 0x07:
            // crossroad things
            s = new EtcObj(m, curanim, curanim, curanim);
            break;
        case 0x08:
            // crossroad things 0x0270
            s = new EtcObj(m, curanim, curanim, curanim);
            break;
        case 0x0B:
            // 0x0270 animation, is this object present in original game?
            //s = new EtcObj(m, curanim, curanim, curanim);
            //printf("0x0B anim %X\n", curanim);
            break;
        case 0x0A:
            s = new NeonSign(m, curanim);
            break;
        case 0x0C:
            s = new Door(m, curanim, curanim + 2, curanim + 4, curanim + 6);
            break;
        case 0x0D:
            // TODO: open doors? verify
            s = new Door(m, curanim, curanim + 2, curanim + 4, curanim + 6);
            break;
        case 0x0E:
            // TODO: open doors? verify
            s = new Door(m, curanim, curanim + 2, curanim + 4, curanim + 6);
            break;
        case 0x0F:
            // TODO: open doors? verify
            s = new Door(m, curanim, curanim + 2, curanim + 4, curanim + 6);
            break;
        case 0x11:
            // ???? what is this
            //s = new EtcObj(m, curanim, curanim, curanim);
            //printf("0x11 anim %X\n", curanim);
            break;
        case 0x12:
            s = new WindowObj(m, curanim, curanim + 2, curanim + 4);
            break;
        case 0x13:
            s = new WindowObj(m, curanim, curanim + 4, curanim + 6);
            break;
        case 0x15:
            // damaged window
            s = new WindowObj(m, curanim, curanim, curanim);
            break;
        case 0x16:
            s = new Tree(m, curanim, curanim + 1, curanim + 2);
            break;
        case 0x19:
            // trash can / mail box
            s = new EtcObj(m, curanim, curanim, curanim);
            break;
        case 0x1A:
            // advertisement on wall
            s = new EtcObj(m, curanim, curanim, curanim);
            break;
        case 0x1C:
            // ???? what is this?
            //s = new EtcObj(m, curanim, curanim, curanim);
            //printf("0x1C anim %X\n", curanim);
            break;
        case 0x1F:
            // advertisement on wall
            s = new EtcObj(m, curanim, curanim, curanim);
            break;
        case 0x20:
            // window without light animated
            s = new EtcObj(m, curanim, curanim, curanim);
            break;
        case 0x21:
            // window without light animated
            s = new EtcObj(m, curanim, curanim, curanim);
            break;
        case 0x23:
            // window with person's shadow
            s = new EtcObj(m, curanim, curanim, curanim);
            break;
        case 0x24:
            // window with person's shadow
            s = new EtcObj(m, curanim, curanim, curanim);
            break;
        case 0x25:
            // window without light
            s = new EtcObj(m, curanim, curanim, curanim);
            break;
        case 0x26:
            // 0x00,0x80 south - north = 0
            // 0x40,0xC0 weast - east = 2
            s = new LargeDoor(m, curanim, curanim + 1, curanim + 2);
            if (gamdata->orientation == 0x00 || gamdata->orientation == 0x80)
                s->setSubType(0);
            else
                s->setSubType(2);
            break;
        default:
            printf("uknown type %02X , %02X, %X\n", gamdata->sub_type,
                gamdata->orientation,
                READ_LE_UINT16(gamdata->index_current_frame));
            printf("x is %i, xoff is %i ==", gamdata->mapposx[1],
                gamdata->mapposx[0]);
            printf("y is %i, yoff is %i ==", gamdata->mapposy[1],
                gamdata->mapposy[0]);
            printf("z is %i, zoff is %i\n", gamdata->mapposz[1],
                gamdata->mapposz[0]);
            break;
    }

    if (s) {
        int z = READ_LE_UINT16(gamdata->mapposz) >> 7;
        z--;
        int oz = gamdata->mapposz[0] & 0x7F;
        s->setVisZ(z);
        if (oz > 0)
            z++;
        s->setPosition(gamdata->mapposx[1], gamdata->mapposy[1],
                       z, gamdata->mapposx[0],
                       gamdata->mapposy[0], oz);
        s->setMainType(gamdata->sub_type);
    }

    return s;
}

Door::Door(int m, int anim, int closingAnim, int openAnim, int openingAnim):Static(m),
anim_(anim), closing_anim_(closingAnim),open_anim_(openAnim),
opening_anim_(openingAnim)
{
}

void Door::draw(int x, int y)
{
    addOffs(x, y);
    g_App.gameSprites().drawFrame(anim_, frame_, x, y);
}

bool Door::animate(int elapsed, Mission *obj)
{
    return MapObject::animate(elapsed);
}

LargeDoor::LargeDoor(int m, int anim, int closingAnim, int openingAnim):Static(m), anim_(anim),
closing_anim_(closingAnim), opening_anim_(openingAnim)
{
    state_ = 2;// closed
}

void LargeDoor::draw(int x, int y)
{
    addOffs(x, y);
    switch(state_) {
        case 0:
            break;
        case 1:
            g_App.gameSprites().drawFrame(closing_anim_, frame_, x, y);
            break;
        case 2:
            g_App.gameSprites().drawFrame(anim_, frame_, x, y);
            break;
        case 3:
            g_App.gameSprites().drawFrame(opening_anim_, frame_, x, y);
            break;
    }
}

bool LargeDoor::animate(int elapsed, Mission *obj)
{
    // TODO: there must be somewhere locked door
    VehicleInstance *v = NULL;
    PedInstance *p = NULL;
    int x = tileX();
    int y = tileY();
    int z = tileZ();
    int mt;
    int si;
    char inc_rel = 0, rel_inc = 0;
    char *i = 0, *j = 0;
    bool found = false;

    bool changed = MapObject::animate(elapsed);
    switch(state_) {
        case 0: //open
            if (sub_type_ == 0) {
                i = &rel_inc;
                j = &inc_rel;
            } else if (sub_type_ == 2) {
                i = &inc_rel;
                j = &rel_inc;
            }
            assert(i != 0 && j != 0);
            *j = -1;
            for(*i = -2; *i < 3; *i += 1) {
                mt = 4; si = 0;
                v = (VehicleInstance *)(obj->findAt(x + inc_rel,
                    y + rel_inc,z, &mt, &si, true));
                if (!v && state_ == 0 && !found) {
                    state_ = state_++;
                    frame_ = 0;
                } else if (v){
                    state_ = 0;
                    found = true;
                    v->hold_on_.wayFree = 0;
                }
            }
            *j = 1;
            for(*i = -2; *i < 3; *i += 1) {
                mt = 4; si = 0;
                v = (VehicleInstance *)(obj->findAt(x + inc_rel,
                    y + rel_inc,z,&mt,&si,true));
                if (!v && state_ == 0 && !found) {
                    state_ = state_++;
                    frame_ = 0;
                } else if (v){
                    state_ = 0;
                    found = true;
                    v->hold_on_.wayFree = 0;
                }
            }
            for (int a = (y - 1); a <= (y + 1); a++ ) {
                for (int b = (x - 1); b <= (x + 1); b++) {
                    mt = 1; si = 0;
                    do {
                        p = (PedInstance *)(obj->findAt(b, a, z,
                            &mt, &si, true));
                        if (p) {
                            p->hold_on_.wayFree = 0;
                        }
                    } while (p);
                }
            }
            break;
        case 2: //closed
            char sign;
            if (sub_type_ == 0) {
                i = &rel_inc;
                j = &inc_rel;
                sign = 1;
            } else if (sub_type_ == 2) {
                i = &inc_rel;
                j = &rel_inc;
                sign = -1;
            }
            assert(i != 0 && j != 0);
            *j = -1 * sign;
            *i = -2;
            mt = 4; si = 0;
            v = (VehicleInstance *)(obj->findAt(x + inc_rel,
                y + rel_inc,z,&mt,&si,true));
            if (v) {
                if (!found) {
                    state_ = state_++;
                    found = true;
                    frame_ = 0;
                }
                v->hold_on_.wayFree = 1;
            }
            *j = 1 * sign;
            *i = 2;
            mt = 4; si = 0;
            v = (VehicleInstance *)(obj->findAt(x + inc_rel,
                y + rel_inc,z,&mt,&si,true));
            if (v) {
                if (!found) {
                    state_ = state_++;
                    found = true;
                    frame_ = 0;
                }
                v->hold_on_.wayFree = 1;
            }
            *j = -1;
            for ( *i = -1; *i <= 1; *i += 1 ) {
                mt = 1; si = 0;
                do {
                    p = (PedInstance *)(obj->findAt(x + rel_inc,
                        y + inc_rel, z, &mt, &si, true));
                    if (p) {
                        p->hold_on_.wayFree = 2;
                        p->hold_on_.tilex = x;
                        p->hold_on_.tiley = y;
                        if (sub_type_ == 0) {
                            p->hold_on_.xadj = 1;
                            p->hold_on_.yadj = 0;
                        } else if (sub_type_ == 2) {
                            p->hold_on_.xadj = 0;
                            p->hold_on_.yadj = 1;
                        }
                        p->hold_on_.tilez = z;
                    }
                } while (p);
            }
            *j = 1;
            for ( *i = -1; *i <= 1; *i += 1 ) {
                mt = 1; si = 0;
                do {
                    p = (PedInstance *)(obj->findAt(x + rel_inc,
                        y + inc_rel, z, &mt, &si, true));
                    if (p) {
                        p->hold_on_.wayFree = 2;
                        p->hold_on_.tilex = x;
                        p->hold_on_.tiley = y;
                        if (sub_type_ == 0) {
                            p->hold_on_.xadj = 1;
                            p->hold_on_.yadj = 0;
                        } else if (sub_type_ == 2) {
                            p->hold_on_.xadj = 0;
                            p->hold_on_.yadj = 1;
                        }
                        p->hold_on_.tilez = z;
                    }
                } while (p);
            }
            break;
        case 1: //closing
            if (frame_ >= g_App.gameSprites().lastFrame(closing_anim_)) {
                state_ = 2;
                frame_ = 0;
            }
            break;
        case 3: // opening
            if (frame_ >= g_App.gameSprites().lastFrame(opening_anim_)) {
                state_ = 0;
                frame_ = 0;
            }
            break;
    }
    return changed;
}

Tree::Tree(int m, int anim, int burningAnim, int damagedAnim):Static(m),
anim_(anim), burning_anim_(burningAnim), damaged_anim_(damagedAnim)
{
}

void Tree::draw(int x, int y)
{
    addOffs(x, y);
    g_App.gameSprites().drawFrame(anim_, frame_, x, y);
}

WindowObj::WindowObj(int m, int anim, int breakingAnim, int damagedAnim):Static(m),
anim_(anim), breaking_anim_(breakingAnim), damaged_anim_(damagedAnim)
{
}

void WindowObj::draw(int x, int y)
{
    addOffs(x, y);
    g_App.gameSprites().drawFrame(anim_, frame_, x, y);
}

EtcObj::EtcObj(int m, int anim, int burningAnim , int damagedAnim):Static(m),
anim_(anim), burning_anim_(burningAnim), damaged_anim_(damagedAnim)
{
}

void EtcObj::draw(int x, int y)
{
    addOffs(x, y);
    g_App.gameSprites().drawFrame(anim_, frame_, x, y);
}

NeonSign::NeonSign(int m, int anim):Static(m),
anim_(anim)
{
}

void NeonSign::draw(int x, int y)
{
    addOffs(x, y);
    g_App.gameSprites().drawFrame(anim_, frame_, x, y);
}
