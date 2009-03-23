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

#include <iostream>
#include <boost/function.hpp>
#include <gtkmm/treeselection.h>
#include <gtkmm/treemodelcolumn.h>

#include "object_selector.hpp"

class ObjectIconColumns : public Gtk::TreeModel::ColumnRecord
{
public:
  Gtk::TreeModelColumn<std::string> pathname;
  Gtk::TreeModelColumn<Glib::RefPtr<Gdk::Pixbuf> >  icon;
  
  static ObjectIconColumns& instance() {
    if (instance_)
      return *instance_;
    else
      return *(instance_ = new ObjectIconColumns());
  }

private:
  static ObjectIconColumns* instance_;

  ObjectIconColumns() {
    add(pathname); 
    add(icon);
  }
};

ObjectIconColumns* ObjectIconColumns::instance_ = 0;

ObjectSelector::ObjectSelector()
  : label("Object Selector", Gtk::ALIGN_LEFT)
{
  list_store = Gtk::ListStore::create(ObjectIconColumns::instance());

  // Change background color
  // iconview.modify_base(Gtk::STATE_NORMAL, Gdk::Color("grey"));
  
  iconview.set_model(list_store);

  iconview.set_pixbuf_column(ObjectIconColumns::instance().icon);
  //iconview.set_text_column(ObjectIconColumns::instance().pathname);

  iconview.set_model(list_store);

  std::vector<Gtk::TargetEntry> targets;
  targets.push_back(Gtk::TargetEntry("WindstilleObject"));
  iconview.drag_source_set(targets, Gdk::BUTTON1_MASK, Gdk::ACTION_COPY);

  iconview.signal_drag_begin().connect(sigc::mem_fun(*this, &ObjectSelector::on_drag_begin));
  iconview.signal_drag_data_get().connect(sigc::mem_fun(*this, &ObjectSelector::on_drag_data_get));

  scrolled.set_policy(Gtk::POLICY_NEVER, Gtk::POLICY_ALWAYS);

  scrolled.add(iconview);
  pack_start(label, Gtk::PACK_SHRINK);
  add(scrolled);
  
  //show_all();
}

ObjectSelector::~ObjectSelector()
{

}

void
ObjectSelector::add_object(const std::string& pathname,
                           const Glib::RefPtr<Gdk::Pixbuf>& icon)
{
  Gtk::ListStore::iterator it  = list_store->append();
  (*it)[ObjectIconColumns::instance().pathname] = pathname;
  (*it)[ObjectIconColumns::instance().icon]     = icon;
}
                    
void
ObjectSelector::on_drag_begin(const Glib::RefPtr<Gdk::DragContext>& context)
{
  std::string iconpath = "data/editor/icon.png";

Gtk::IconView::ArrayHandle_TreePaths selection = iconview.get_selected_items();
  for(Gtk::IconView::ArrayHandle_TreePaths::iterator i = selection.begin();
      i != selection.end();
      ++i)
    {
      Gtk::ListStore::iterator it = list_store->get_iter(*i);
      iconpath = (*it)[ObjectIconColumns::instance().pathname];
    }

  Glib::RefPtr<Gdk::Pixbuf> pixbuf = Gdk::Pixbuf::create_from_file(iconpath);
  context->set_icon(pixbuf, pixbuf->get_width()/2, pixbuf->get_height()/2);
}     

void
ObjectSelector::on_drag_data_get(const Glib::RefPtr<Gdk::DragContext>& context, 
                                 Gtk::SelectionData& selection_data, 
                                 guint info, guint time)
{
  std::cout << "ObjectSelector: on_drag_data_get" << std::endl;

  Gtk::IconView::ArrayHandle_TreePaths selection = iconview.get_selected_items();

  for(Gtk::IconView::ArrayHandle_TreePaths::iterator i = selection.begin();
      i != selection.end();
      ++i)
    {
      Gtk::ListStore::iterator it = list_store->get_iter(*i);

      //if (it)
      //  std::cout << "on_drag_begin: " << (*it)[ObjectIconColumns::instance().pathname] << std::endl;

      selection_data.set("data", (*it)[ObjectIconColumns::instance().pathname]);
    }
}

/* EOF */