/*
**  Windstille - A Sci-Fi Action-Adventure Game
**  Copyright (C) 2005 Ingo Ruhnke <grumbel@gmail.com>
**
**  This program is free software: you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation, either version 3 of the License, or
**  (at your option) any later version.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "screen/title_screen.hpp"

#include "display/display.hpp"
#include "app/menu_manager.hpp"

TitleScreen::TitleScreen()
  : background(Pathname("images/titlescreen.sprite"))
{
  background.set_scale(std::max(float(Display::get_width())  / background.get_width(),
                                float(Display::get_height()) / background.get_height()));
}

TitleScreen::~TitleScreen()
{
}

void
TitleScreen::on_startup()
{
  MenuManager::display_main_menu();
}

void
TitleScreen::draw()
{
  background.draw(Vector2f(static_cast<float>(Display::get_width())  / 2.0f - (background.get_width()  * background.get_scale() / 2.0f),
                           static_cast<float>(Display::get_height()) / 2.0f - (background.get_height() * background.get_scale() / 2.0f)));
}

void
TitleScreen::update(float delta, const Controller& /*controller*/)
{
  background.update(delta);
}

void
TitleScreen::handle_event(const SDL_Event& )
{

}

/* EOF */
