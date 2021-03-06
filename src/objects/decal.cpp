/*
**  Windstille - A Sci-Fi Action-Adventure Game
**  Copyright (C) 2009 Ingo Ruhnke <grumbel@gmail.com>
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

#include "objects/decal.hpp"

#include "engine/sector.hpp"
#include "scenegraph/scene_graph.hpp"
#include "scenegraph/surface_drawable.hpp"
#include "display/surface_drawing_parameters.hpp"
#include "display/scene_context.hpp"

Decal::Decal(const FileReader& reader) :
  drawable(),
  pos()
{
  std::string path;
  Vector2f scale(1.0f, 1.0f);
  float    angle = 0.0f;

  bool hflip = false;
  bool vflip = false;

  int map_type = 0;
  reader.get("type", map_type);

  reader.get("pos",   pos);
  reader.get("path",  path);
  reader.get("scale", scale);
  reader.get("angle", angle);
  reader.get("vflip", vflip);
  reader.get("hflip", hflip);

  SurfacePtr surface = Surface::create(Pathname(path));

  SurfaceDrawingParameters params;

  switch(map_type)
  {
    // FIXME: Evil hardcoded constans, see edtior/decal_object_model.hpp
    case 0: break;
    case 1: params.set_blend_func(GL_SRC_ALPHA, GL_ONE); break;
    case 2: params.set_blend_func(GL_SRC_ALPHA, GL_ONE); break;
  }

  Vector2f center_offset(-surface->get_width() /2,
                         -surface->get_height()/2);

  center_offset.x *= scale.x;
  center_offset.y *= scale.y;

  drawable.reset(new SurfaceDrawable(surface,
                                     params
                                     .set_pos(pos + center_offset)
                                     .set_angle(angle)
                                     .set_hflip(hflip)
                                     .set_vflip(vflip)
                                     .set_scale(scale),
                                     0,
                                     Matrix(1.0f)));

  switch(map_type)
  {
    // FIXME: Evil hardcoded constans
    case 0: // color
      drawable->set_render_mask(SceneContext::COLORMAP);
      break;

    case 1: // lightmap
      drawable->set_render_mask(SceneContext::LIGHTMAP);
      break;

    case 2: // highlight
      drawable->set_render_mask(SceneContext::HIGHLIGHTMAP);
      break;
  }
  Sector::current()->get_scene_graph().add_drawable(drawable);
}

Decal::~Decal()
{
}

void
Decal::draw(SceneContext& /*context*/)
{
}

void
Decal::update(float /*delta*/)
{
}

void
Decal::set_parent(GameObject* parent)
{
  Decal* decal = dynamic_cast<Decal*>(parent);
  if (decal)
  { // FIXME: Not going to work with double parenting
    pos += decal->pos;

    Vector2f center_offset(-drawable->get_surface()->get_width() /2,
                           -drawable->get_surface()->get_height()/2);

    center_offset.x *= drawable->get_params().scale.x;
    center_offset.y *= drawable->get_params().scale.y;

    drawable->get_params().set_pos(pos + center_offset);
  }
}

/* EOF */
