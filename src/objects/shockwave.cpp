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

#include "display/opengl_state.hpp"
#include "display/drawing_request.hpp"
#include "display/texture.hpp"
#include "display/shader_program.hpp"
#include "display/shader_object.hpp"
#include "shockwave.hpp"

class ShockwaveDrawingRequest : public DrawingRequest
{
public:
  Texture       noise;
  ShaderProgram shader_program;
  float radius;

  ShockwaveDrawingRequest(const Vector& pos, 
                          const Texture&       noise_,
                          const ShaderProgram& shader_program_,
                          float r,
                          const Matrix& modelview_) 
    : DrawingRequest(pos, 500.0f, modelview_),
      noise(noise_),
      shader_program(shader_program_),
      radius(r)
  {
  }

  ~ShockwaveDrawingRequest()
  {
  }

  bool  needs_prepare()
  {
    return true; 
  }
    
  void prepare(const Texture& screen_texture)
  {
    // FIXME: Clear stuff is only for debugging
    glClearColor(1.0, 0.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPushMatrix();
    glMultMatrixf(modelview.matrix);
    glTranslatef(pos.x, pos.y, 0);

    { // Apply modelview matrix to texture matrix so that we can
      // give texcoords as screencords
      GLdouble modelview[16];
      glMatrixMode(GL_MODELVIEW);
      glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
      glMatrixMode(GL_TEXTURE);
      glLoadIdentity();
      glTranslatef(0, 599, 0);
      glScalef(1, -1, 1);
      glMultMatrixd(modelview);

      //glLoadMatrixd(modelview);
    }

    int count = int(radius);
    OpenGLState state;
    state.bind_texture(screen_texture, 0);
    state.activate();

    float radius = (count)*2.0f + 20.0f; // enlarge radius by 20.0f to handle texture displacement 
    float minradius = 2.0f*count - 164.0f;
    if (minradius < 0)
      minradius = 0;

    int segments = 64;

    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
  
    glBegin(GL_QUADS);
    for (int i = 0; i < segments; ++i)
      {
        float angel = (2*M_PI / segments);

        float x1 =  sin(angel*i)*radius;
        float y1 = -cos(angel*i)*radius;

        float x2 =  sin(angel*(i+1))*radius;
        float y2 = -cos(angel*(i+1))*radius;

        glTexCoord2f(x1+256, (y1+256));
        glVertex3f(x1+256, y1+256, 0);

        glTexCoord2f(x2+256, (y2+256));
        glVertex3f(x2+256, y2+256, 0);


        float x3 =  sin(angel*i)*minradius;
        float y3 = -cos(angel*i)*minradius;

        float x4 =  sin(angel*(i+1))*minradius;
        float y4 = -cos(angel*(i+1))*minradius;

        glTexCoord2f(x4+256, (y4+256));
        glVertex3f(x4+256, y4+256, 0);
        glTexCoord2f(x3+256, (y3+256));
        glVertex3f(x3+256, y3+256, 0);
      }
    glEnd();

    glMatrixMode(GL_TEXTURE);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);

    glPopMatrix();
  }

  void draw(const Texture& tmp_texture)
  {
    glPushMatrix();
    glMultMatrixf(modelview.matrix);
    glTranslatef(pos.x, pos.y, 0);
    if (0)
      {
        Rectf rect(0, 0, 800, 600);
        // Render the screen framebuffer to the actual screen 
        OpenGLState state;
        state.bind_texture(tmp_texture, 0);
        state.activate();

        glBegin(GL_QUADS);
    
        glTexCoord2f(rect.left, rect.bottom);
        glVertex2f(rect.left/2.0f, rect.bottom/2.0f);

        glTexCoord2f(rect.right, rect.bottom);
        glVertex2f(rect.right/2.0f, rect.bottom/2.0f);

        glTexCoord2f(rect.right, rect.top);
        glVertex2f(rect.right/2.0f, rect.top/2.0f);

        glTexCoord2f(rect.left, rect.top);
        glVertex2f(rect.left/2.0f, rect.top/2.0f);
    
        glEnd();
      }
    else
      {
        OpenGLState state;
        state.bind_texture(tmp_texture, 0);
        state.bind_texture(noise, 1);
        state.enable(GL_BLEND);
        state.set_blend_func(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        state.activate();

        glUseProgramObjectARB(shader_program.get_handle());    
        shader_program.set_uniform1f("radius",   radius/512.0f*2.0f);
        shader_program.set_uniform1i("background_tex", 0);
        shader_program.set_uniform1i("noise_tex",   1);
        draw_disc(int(radius));
        glUseProgramObjectARB(0);
      }
    glPopMatrix();
  }

  void draw_disc(int count)
  {
    float radius = (count)*2.0f;
    float minradius = 2.0f*count - 164.0f;
    if (minradius < 0)
      minradius = 0;

    int segments = 64;
  
    glBegin(GL_QUADS);
    for (int i = 0; i < segments; ++i)
      {
        float angel = (2*M_PI / segments);

        float x1 =  sin(angel*i)*radius;
        float y1 = -cos(angel*i)*radius;

        float x2 =  sin(angel*(i+1))*radius;
        float y2 = -cos(angel*(i+1))*radius;

        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
        glTexCoord2f(x1/512.0f+0.5f, y1/512.0f+0.5f);
        glVertex3f(x1+256, y1+256, 0);

        glTexCoord2f(x2/512.0f+0.5f, y2/512.0f+0.5f);
        glVertex3f(x2+256, y2+256, 0);


        float x3 =  sin(angel*i)*minradius;
        float y3 = -cos(angel*i)*minradius;

        float x4 =  sin(angel*(i+1))*minradius;
        float y4 = -cos(angel*(i+1))*minradius;

        glColor4f(1.0f, 1.0f, 1.0f, 0.0f);
        glTexCoord2f(x4/512.0f+0.5f, y4/512.0f+0.5f);
        glVertex3f(x4+256, y4+256, 0);
        glTexCoord2f(x3/512.0f+0.5f, y3/512.0f+0.5f);
        glVertex3f(x3+256, y3+256, 0);
      }
    glEnd();
  }
};

Shockwave::Shockwave(FileReader& props)
  : noise("images/noise3.png")
{
  props.get("pos", pos);
  props.print_unused_warnings("Shockwave");

  radius = 100.0f;

  noise.set_wrap(GL_REPEAT);
  noise.set_filter(GL_LINEAR);

  shader_program.attach(ShaderObject(GL_FRAGMENT_SHADER_ARB, "data/shader/shockwave2.frag"));
  shader_program.link();
}

Shockwave::~Shockwave()
{
}

void
Shockwave::draw (SceneContext& sc)
{
  sc.highlight().draw(new ShockwaveDrawingRequest(pos,
                                                   noise,
                                                   shader_program,
                                                   radius,
                                                   sc.color().get_modelview()));
}

void
Shockwave::update (float delta)
{
  radius += 150.0f * delta;
  if (radius > 300.0f)
    radius = 0;
}

/* EOF */