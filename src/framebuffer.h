#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "glad.h"

class Framebuffer
{
public:
  Framebuffer() : FBO(0), texture_id(0), RBO(0), MSAA_RBO(0), MSAA_depthRBO(0), samples(4), enableMSAA(false), resolvedFBO(0), resolvedTextureId(0) {}
  ~Framebuffer() { cleanup(); }

  void init(bool enableMSAA = true, int samples = 4, int width = 800, int height = 600);

  void bind();
  void unbind();

  void rescale(int width, int height);

  GLuint getTextureId();

private:
  void cleanup();

  int framebufferWidth;
  int framebufferHeight;

  GLuint FBO;               // Main framebuffer
  GLuint resolvedFBO;       // Resolved framebuffer for MSAA
  GLuint texture_id;        // Texture attachment (non-MSAA)
  GLuint resolvedTextureId; // Resolved texture for MSAA
  GLuint RBO;               // Depth/stencil attachment (non-MSAA)
  GLuint MSAA_RBO;          // Multisampled color renderbuffer
  GLuint MSAA_depthRBO;     // Multisampled depth/stencil renderbuffer

  int samples;
  bool enableMSAA;

  void resolve();
};

#endif
