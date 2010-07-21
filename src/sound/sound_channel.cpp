/*
**  Windstille - A Sci-Fi Action-Adventure Game
**  Copyright (C) 2010 Ingo Ruhnke <grumbel@gmx.de>
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

#include "sound/sound_channel.hpp"

#include <AL/al.h>
#include <iostream>

#include "math/vector2f.hpp"
#include "util/pathname.hpp"
#include "sound/sound_manager.hpp"
#include "sound/sound_source.hpp"

SoundChannel::SoundChannel(SoundManager& sound_manager) :
  m_sound_manager(sound_manager),
  m_sound_sources(),
  m_gain(1.0f)
{
}

SoundSourcePtr
SoundChannel::play(const Pathname& filename)
{  
  SoundSourcePtr source = prepare(filename);
  source->play();
  return source;
}

SoundSourcePtr
SoundChannel::prepare(std::auto_ptr<SoundFile> sound_file)
{
  // FIXME: implement me
  assert(!"implement me");
  return SoundSourcePtr();
}

SoundSourcePtr
SoundChannel::prepare(const Pathname& filename)
{
  try
  {
    SoundSourcePtr source = m_sound_manager.create_sound_source(filename, *this);
    source->update_gain();
    m_sound_sources.push_back(SoundSourcePtr(source));
    return source;
  }
  catch(const std::exception& e) 
  {
    std::cout << "SoundChannel::prepare(): Couldn't play sound " << filename << ": " << e.what() << "\n";
    return SoundSourcePtr(); // FIXME: not good, should return dummy SoundSource
  }
}

void
SoundChannel::set_gain(float gain)
{
  m_gain = gain;

  for(std::vector<SoundSourcePtr>::iterator i = m_sound_sources.begin(); i != m_sound_sources.end(); ++i)
  {
    (*i)->update_gain();
  }
}

float
SoundChannel::get_gain() const
{
  return m_gain;
}

void
SoundChannel::update(float delta)
{
  for(std::vector<SoundSourcePtr>::iterator i = m_sound_sources.begin(); i != m_sound_sources.end(); ++i)
  {
    (*i)->update(delta);
  }
}

/* EOF */