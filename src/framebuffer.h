#include <glad.h>

#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

class Framebuffer
{
public:
  Framebuffer(int width, int height);

  void bind();
  void unbind();
  void rescale(int width, int height);
  GLuint getTextureId();

private:
  GLuint FBO;
  GLuint RBO;
  GLuint texture_id;
};

#endif
