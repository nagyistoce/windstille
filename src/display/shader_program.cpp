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

#include "display/shader_program.hpp"

#include <boost/scoped_array.hpp>
#include <iostream>
#include <stdexcept>

#include "display/assert_gl.hpp"
#include "display/assert_gl.hpp"
#include "display/shader_object.hpp"

ShaderProgramPtr
ShaderProgram::load_from_file(const std::string& frag_filename,
                              const std::string& vert_filename)
{
  ShaderProgramPtr prog(new ShaderProgram());

  if (!frag_filename.empty())
  {
    ShaderObjectPtr frag = ShaderObject::create_from_file(GL_FRAGMENT_SHADER, frag_filename);
    prog->attach(frag);
  }

  if (!vert_filename.empty())
  {
    ShaderObjectPtr vert = ShaderObject::create_from_file(GL_VERTEX_SHADER, vert_filename);
    prog->attach(vert);
  }

  return prog;
}

ShaderProgramPtr
ShaderProgram::create_from_file(const std::string& frag_filename,
                                const std::string& vert_filename)
{
  ShaderProgramPtr prog = load_from_file(frag_filename, vert_filename);
  prog->link();
  return prog;
}

ShaderProgramPtr
ShaderProgram::create()
{
  return ShaderProgramPtr(new ShaderProgram());
}

ShaderProgram::ShaderProgram() :
 m_handle(0)
{
  m_handle = glCreateProgram();
}

ShaderProgram::~ShaderProgram()
{
  glDeleteProgram(m_handle);
}

void
ShaderProgram::attach(ShaderObjectPtr obj)
{
  glAttachShader(m_handle, obj->get_handle());
  assert_gl("ShaderProgram::attach()");
}

void
ShaderProgram::link()
{
  glLinkProgram(m_handle);
  if (!get_link_status())
  {
    throw std::runtime_error(get_info_log());
  }
}

GLint
ShaderProgram::get_uniform_location(const char* name)
{
  GLint loc;

  loc = glGetUniformLocation(m_handle, name);

  if (loc == -1)
    std::cout << "No such uniform named \"%s\"\n" << name << std::endl;

  assert_gl("ShaderProgram::get_uniform_location()");

  return loc;
}

GLuint
ShaderProgram::get_handle() const
{
  return m_handle;
}

void
ShaderProgram::set_uniform1f(const char* name, GLfloat v0)
{
  GLint location = glGetUniformLocation(m_handle, name);
  if (location == -1)
    std::cout << "No such uniform named \"" << name << "\"" << std::endl;
  else
    glUniform1f(location, v0);
}

void
ShaderProgram::set_uniform2f(const char* name, GLfloat v0, GLfloat v1)
{
  GLint location = glGetUniformLocation(m_handle, name);
  if (location == -1)
    std::cout << "No such uniform named \"" << name << "\"" << std::endl;
  else
    glUniform2f(location, v0, v1);
}

void
ShaderProgram::set_uniform3f(const char* name, GLfloat v0, GLfloat v1, GLfloat v2)
{
  GLint location = glGetUniformLocation(m_handle, name);
  if (location == -1)
    std::cout << "No such uniform named \"" << name << "\"" << std::endl;
  else
    glUniform3f(location, v0, v1, v2);
}

void
ShaderProgram::set_uniform4f(const char* name, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)
{
  GLint location = glGetUniformLocation(m_handle, name);
  if (location == -1)
    std::cout << "No such uniform named \"" << name << "\"" << std::endl;
  else
    glUniform4f(location, v0, v1, v2, v3);
}

void
ShaderProgram::set_uniform1i(const char* name, GLint v0)
{
  GLint location = glGetUniformLocation(m_handle, name);
  if (location == -1)
    std::cout << "No such uniform named \"" << name << "\"" << std::endl;
  else
    glUniform1i(location, v0);
}

void
ShaderProgram::set_uniform2i(const char* name, GLint v0, GLint v1)
{
  GLint location = glGetUniformLocation(m_handle, name);
  if (location == -1)
    std::cout << "No such uniform named \"" << name << "\"" << std::endl;
  else
    glUniform2i(location, v0, v1);
}

void
ShaderProgram::set_uniform3i(const char* name, GLint v0, GLint v1, GLint v2)
{
  GLint location = glGetUniformLocation(m_handle, name);
  if (location == -1)
    std::cout << "No such uniform named \"" << name << "\"" << std::endl;
  else
    glUniform3i(location, v0, v1, v2);
}

void
ShaderProgram::set_uniform4i(const char* name, GLint v0, GLint v1, GLint v2, GLint v3)
{
  GLint location = glGetUniformLocation(m_handle, name);
  if (location == -1)
    std::cout << "No such uniform named \"" << name << "\"" << std::endl;
  else
    glUniform4i(location, v0, v1, v2, v3);
}

void
ShaderProgram::bind_frag_data_location(GLuint color_number, const char* name)
{
  assert(glBindFragDataLocation);
  glBindFragDataLocation(m_handle, color_number, name);
  assert_gl("ShaderProgram::bind_frag_data_location");
}

bool
ShaderProgram::get_link_status()
{
  int status;
  glGetProgramiv(m_handle, GL_LINK_STATUS, &status);
  return (status == GL_TRUE);
}

std::string
ShaderProgram::get_info_log()
{
  int info_log_len = 0;
  int charsWritten  = 0;

  glGetProgramiv(m_handle, GL_INFO_LOG_LENGTH, &info_log_len);

  if (info_log_len > 0)
  {
    boost::scoped_array<GLchar> info_log(new GLchar[info_log_len]);
    glGetProgramInfoLog(m_handle, info_log_len, &charsWritten, info_log.get());
    return info_log.get();
  }
  else
  {
    return std::string("<empty>");
  }
}

/* EOF */
