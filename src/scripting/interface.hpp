/*  $Id: config.cpp 1132 2005-11-28 14:33:49Z grumbel $
**   __      __ __             ___        __   __ __   __
**  /  \    /  \__| ____    __| _/_______/  |_|__|  | |  |   ____
**  \   \/\/   /  |/    \  / __ |/  ___/\   __\  |  | |  | _/ __ \
**   \        /|  |   |  \/ /_/ |\___ \  |  | |  |  |_|  |_\  ___/
**    \__/\  / |__|___|  /\____ /____  > |__| |__|____/____/\___  >
**         \/          \/      \/    \/                         \/
**  Copyright (C) 2000,2005 Ingo Ruhnke <grumbel@gmx.de>
**
**  This program is free software; you can redistribute it and/or
**  modify it under the terms of the GNU General Public License
**  as published by the Free Software Foundation; either version 2
**  of the License, or (at your option) any later version.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
** 
**  You should have received a copy of the GNU General Public License
**  along with this program; if not, write to the Free Software
**  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
**  02111-1307, USA.
*/

#ifndef __INTERFACE_HPP__
#define __INTERFACE_HPP__

#ifndef SCRIPTING_API
#include <string>
#include <squirrel.h>
#endif

namespace Scripting {

void set_sector(const std::string& filename);

void play_music(const std::string& musicfile);

void stop_music(float fadetime);

void play_sound(const std::string& soundfile);

void caption_add(int alignment, const std::string& text);
void caption_clear();
void caption_end();

void camera_set_active(bool active);
void camera_continue_path();
void camera_begin_path();
void camera_add_point(float x, float y, float zoom);
void camera_end_path();

void camera_set_pos(float x, float y);
void camera_set_zoom(float z);
void set_controller_help_active(bool active);

// dialog alignment constants
static const int VCENTER = 0x00;
static const int LEFT    = 0x01;
static const int RIGHT   = 0x02;
static const int HCENTER = 0x00;
static const int TOP     = 0x10;
static const int BOTTOM  = 0x20;

void dialog_show(int alignment, const std::string& character, const std::string& portrait, const std::string& text);
void wait_for_dialog(HSQUIRRELVM vm) __suspend;
void wait_for_fade(HSQUIRRELVM vm) __suspend;
void wait_for_camera(HSQUIRRELVM vm) __suspend;

void conversation_add(const std::string& text);
void conversation_show();
int  conversation_get_selection();
void wait_for_conversation(HSQUIRRELVM v) __suspend;

void add_objective(const std::string& name, const std::string& text);
void objective_complete(const std::string& name);
bool is_objective_given(const std::string& name);
bool is_objective_complete(const std::string& name);

bool run_before(HSQUIRRELVM vm);

// for testing
void save_state(HSQUIRRELVM v, const std::string& filename);
void load_state(HSQUIRRELVM v, const std::string& filename);

void list_objects();

void set_debug(bool t);
bool get_debug();

float get_game_speed();
void  set_game_speed(float v);

//Waits the specified time in seconds.
void wait(HSQUIRRELVM vm, float time) __suspend;

SQInteger display(HSQUIRRELVM) __custom;
SQInteger println(HSQUIRRELVM) __custom;

void set_console_font(const std::string& font, int size);
void set_gamma(float g);
void set_gamma_rgb(float r, float g, float b);

void show_config();

void cutscene_begin();
void cutscene_end();

void internal_fadeout_rgb(float time, float r, float g, float b);
void internal_fadein(float time);

int  render_mask_get();
void render_mask_set(int mask);

} // namespace Scripting

#endif

/* EOF */