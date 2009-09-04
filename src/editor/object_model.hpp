/*
**  Windstille - A Sci-Fi Action-Adventure Game
**  Copyright (C) 2009 Ingo Ruhnke <grumbel@gmx.de>
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

#ifndef HEADER_WINDSTILLE_EDITOR_OBJECT_MODEL_HPP
#define HEADER_WINDSTILLE_EDITOR_OBJECT_MODEL_HPP

#include <boost/weak_ptr.hpp>

#include "editor/select_mask.hpp"
#include "util/file_writer.hpp"
#include "editor/control_point.hpp"
#include "editor/snap_data.hpp"

class ObjectModel;
class FileReader;
class SceneContext;
class SectorModel;
typedef boost::shared_ptr<ObjectModel> ObjectModelHandle;
typedef boost::weak_ptr<ObjectModel>   ObjectModelPtr;

class ObjectModel
{
protected:
  std::string name;
  Vector2f    rel_pos;
  SelectMask  select_mask;

  ObjectModelPtr parent_ptr;

public:
  ObjectModel(const FileReader& reader);
  ObjectModel(const std::string& name, const Vector2f& pos);
  virtual ~ObjectModel();
  
  void set_parent(const ObjectModelHandle& parent_, bool recalc_pos = true);
  ObjectModelHandle get_parent() const;

  std::string get_name() const { return name; }
  std::string get_id() const;

  virtual Vector2f get_world_pos() const;
  virtual void set_world_pos(const Vector2f& p);

  virtual Vector2f get_rel_pos() const { return rel_pos; }
  virtual void     set_rel_pos(const Vector2f& rel_pos_);
  
  virtual SelectMask get_select_mask() const { return select_mask; }
  virtual void   set_select_mask(const SelectMask& select_mask_) { select_mask = select_mask_; }

  SnapData snap_object(const Rectf& rect) const;

  virtual bool get_hflip() const { return false; }
  virtual bool get_vflip() const { return false; }

  virtual void set_hflip(bool) { }
  virtual void set_vflip(bool) { }

  /** Reset scale and rotation to default values */
  virtual void reset();

  virtual bool is_at(const Vector2f& pos) const;

  virtual bool is_snappable() const { return true; }

  virtual void draw_select(SceneContext& sc, bool highlight);

  virtual void draw(SceneContext& sc);
  virtual void update(float /*delta*/) {}
  virtual Rectf get_bounding_box() const =0;
  virtual ObjectModelHandle clone() const =0;

  virtual void write(FileWriter& writer) const =0;
  virtual FileWriter& write_member(FileWriter& writer) const;

  virtual void add_control_points(std::vector<ControlPointHandle>& control_points) {}
  
  /** This lets the object add things to the SceneGraph or do other
      things needed to make it properly visible in the SectorModel */
  virtual void add_to_sector(SectorModel& sector)    =0;

  virtual void remove_from_sector(SectorModel& sector) =0;
};

#endif

/* EOF */
