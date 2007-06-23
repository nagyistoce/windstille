//  $Id$
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 2002 Ingo Ruhnke <grumbel@gmx.de>
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 2
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

#include "lisp/parser.hpp"
#include "sexpr_file_reader.hpp"
#include "file_reader.hpp"
#include "file_reader_impl.hpp"

FileReader
FileReader::parse(const std::string& filename)
{
  lisp::Lisp* root = lisp::Parser::parse(filename);
  if (!root)
    {
      std::ostringstream msg;
      msg << "'" << filename << "': file not found";
      throw std::runtime_error(msg.str());
    }
  else if (root && root->get_type() == lisp::Lisp::TYPE_LIST && root->get_list_size() >= 1)
    {
      return SExprFileReader(root, root->get_list_elem(0));
    }
  else
    {
      std::ostringstream msg;
      msg << "'" << filename << "': not a valid sexpr file";
      throw std::runtime_error(msg.str());
    }
}

FileReader::FileReader(SharedPtr<FileReaderImpl> impl_)
  : impl(impl_)
{
}

FileReader::FileReader()
  : impl(0)
{
}

std::string
FileReader::get_name() const
{
  if (impl.get())
    return impl->get_name();
  else
    return "";
}

bool
FileReader::read(const char* name, int& value)
{
  if (impl.get())
    return impl->read_int(name, value);
  else
    return false;
}

bool
FileReader::read(const char* name, float& value)
{
  if (impl.get())
    return impl->read_float(name, value);
  else
    return false;
}

bool
FileReader::read(const char* name, bool& value)
{
  if (impl.get())
    return impl->read_bool(name, value);
  else
    return false;
}

bool
FileReader::read(const char* name, std::string& value)
{
  if (impl.get())
    return impl->read_string(name, value);
  else
    return false;
}

bool
FileReader::read(const char* name, std::vector<int>& value)
{
  if (impl.get())
    return impl->get(name, value);
  else
    return false;
}

bool
FileReader::read(const char* name, std::vector<bool>&   value)
{
  if (impl.get())
    return impl->get(name, value);
  else
    return false;  
}

bool
FileReader::read(const char* name, std::vector<std::string>& value)
{
  if (impl.get())
    return impl->get(name, value);
  else
    return false;
}

bool
FileReader::read(const char* name, std::vector<float>& value)
{
  if (impl.get())
    return impl->get(name, value);
  else
    return false;
}

bool
FileReader::read(const char* name, FileReader& reader)
{
  if (impl.get())
    return impl->read_section(name, reader);
  else
    return false;
}

std::vector<std::string>
FileReader::get_section_names() const
{
  if (impl.get())
    return impl->get_section_names();
  else
    return std::vector<std::string>();
}

std::vector<FileReader>
FileReader::get_sections() const
{
  if (impl.get())
    return impl->get_sections();
  else
    return std::vector<FileReader>();
}

void
FileReader::print_unused_warnings(const std::string& title) const
{
  // unimplemented
}

/* EOF */