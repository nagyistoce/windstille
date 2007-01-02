/*  $Id$
**   __      __ __             ___        __   __ __   __
**  /  \    /  \__| ____    __| _/_______/  |_|__|  | |  |   ____
**  \   \/\/   /  |/    \  / __ |/  ___/\   __\  |  | |  | _/ __ \
**   \        /|  |   |  \/ /_/ |\___ \  |  | |  |  |_|  |_\  ___/
**    \__/\  / |__|___|  /\____ /____  > |__| |__|____/____/\___  >
**         \/          \/      \/    \/                         \/
**  Copyright (C) 2005 Ingo Ruhnke <grumbel@gmx.de>
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

#ifndef HEADER_TEXT_AREA_HPP
#define HEADER_TEXT_AREA_HPP

#include "math/rect.hpp"
#include <string>

class TTFFont;
class TextAreaImpl;

/** A class for managing multiple lines of text in combination with
    special formating.
*/
class TextArea
{
public:
  /** The area which the TextArea should cover */
  TextArea(const Rectf& rect, bool letter_by_letter);
  ~TextArea();

  /** Set the rectangle into which the TextArea should fill its text */
  void set_rect(const Rectf& rect);

  /** Sets the text to be displayed in the text box */
  void set_text(const std::string& str);

  /** Sets the default font to be used in the text box, can be changed
      with special tags */
  void set_font(TTFFont* font);

  /** Skips letter by letter display */  
  void set_progress_complete();
  
  bool is_progress_complete();

  void update(float delta);
  void draw();

  /** Return the rectangle that TextArea fills its text into */
  Rectf get_rect() const;

  /** Return the position of the last character in the TextArea,
      usefull to display a blinking cursor or things like that */
  Vector get_cursor_pos() const;
private:
  TextAreaImpl* impl;
};

#endif

/* EOF */