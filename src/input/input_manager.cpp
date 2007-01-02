/*  $Id$
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

#include <iostream>
#include <assert.h>
#include <stdexcept>
#include <sstream>
#include <memory>

#include "lisp/parser.hpp"
#include "lisp/lisp.hpp"
#include "lisp/properties.hpp"
#include "input_manager_sdl.hpp"
#include "input_manager_impl.hpp"
#include "input_manager.hpp"

InputManagerImpl* InputManager::impl = 0;

void
InputManager::init(const std::string& filename)
{
  std::auto_ptr<lisp::Lisp> root (lisp::Parser::parse(filename));
  lisp::Properties rootp(root.get());

  std::cout << "InputManager: " << filename << std::endl;

  const lisp::Lisp* controller = 0;
  if(rootp.get("windstille-controller", controller) == false) {
    std::ostringstream msg;
    msg << "'" << filename << "' is not a windstille-controller file";
    throw std::runtime_error(msg.str());
  }
  
  impl = new InputManagerSDL(controller);
}

void 
InputManager::deinit()
{
  delete impl;
}

void
InputManager::update(float delta)
{
  assert(impl);
  impl->update(delta);
}

const Controller&
InputManager::get_controller()
{
  assert(impl);
  return impl->get_controller();
}

void
InputManager::clear()
{
  impl->clear();
}

/* EOF */