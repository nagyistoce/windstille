/*
**  Windstille - A Sci-Fi Action-Adventure Game
**  Copyright (C) 2005 Ingo Ruhnke <grumbel@gmx.de>
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

#include "objects/background_gradient.hpp"

#include "display/vertex_array_drawing_request.hpp"
#include "display/display.hpp"

BackgroundGradient::BackgroundGradient(FileReader& props)
  : colors(),
    z_pos(0.0f)
{
  props.get("z-pos",  z_pos);
  props.get("colors", colors);
  if (colors.size() % (3 + 4 + 4 + 2) != 0)
    {
      std::cout << "BackgroundGradient: specified color gradient is invalid" << std::endl;
      /** Color gradients are in the format:
          
      (colors start midpoint end R1 G1 B1 A1 R2 G2 B2 A2 I I
              start midpoint end R1 G1 B1 A1 R2 G2 B2 A2 I I
              ...)

              I is ignored

          all specified in float, this is similar to Gimps gradients
          so you can easily copy&paste
       */
      colors.clear();
    }
}

BackgroundGradient::~BackgroundGradient()
{  
}
  
void
BackgroundGradient::update(float /*delta*/)
{
}

void
BackgroundGradient::draw(SceneContext& sc)
{
  if (colors.empty())
    return ;

  // Reset modelview so we can draw in screen space
  sc.color().push_modelview();
  sc.color().set_modelview(Matrix::identity());
  
  Color topcolor(0.0f, 0.0f, 0.5f);
  Color bottomcolor(0.5f, 0.5f, 1.0f);

  Rectf rect(0.0f, 0.0f, 
             static_cast<float>(Display::get_width()), static_cast<float>(Display::get_height()));
  VertexArrayDrawingRequest* array = new VertexArrayDrawingRequest(Vector2f(0, 0), z_pos, 
                                                                   sc.color().get_modelview());

  array->set_mode(GL_QUAD_STRIP);
  array->set_blend_func(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  for(int i = 0; i < int(colors.size()); i += (3 + 4 + 4 + 2))
    {
      const float& start    = colors[i + 0];
      const float& midpoint = colors[i + 1];
      const float& end      = colors[i + 2];
      const Color color1(colors[i + 3], colors[i + 4], colors[i + 5], colors[i + 6]);
      const Color color2(colors[i + 7], colors[i + 8], colors[i + 9], colors[i + 10]);
      const Color midcolor((color1.r + color2.r)/2,
                     (color1.g + color2.g)/2,
                     (color1.b + color2.b)/2,
                     (color1.a + color2.a)/2);

      array->color(color1);
      array->vertex(rect.left, rect.top + start * rect.get_height());

      array->color(color1);
      array->vertex(rect.right, rect.top + start * rect.get_height());

      array->color(midcolor);
      array->vertex(rect.left, rect.top + midpoint * rect.get_height());

      array->color(midcolor);
      array->vertex(rect.right, rect.top + midpoint * rect.get_height());

      array->color(color2);
      array->vertex(rect.left, rect.top + end * rect.get_height());

      array->color(color2);
      array->vertex(rect.right, rect.top + end * rect.get_height());  
    }

  sc.color().draw(array);  

  sc.color().pop_modelview();
}

/* EOF */
