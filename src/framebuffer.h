#include <glad.h>

#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

class Framebuffer
{
public:
  void init();

  void bind();
  void unbind();
  void rescale(float width, float height);

  GLuint getTextureId();

private:
  GLuint FBO;
  GLuint RBO;
  GLuint texture_id;
};

#endif
