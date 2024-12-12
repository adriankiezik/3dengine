#include "framebuffer.h"
#include <iostream>

void Framebuffer::init(bool enableMSAA, int samples, int width, int height)
{
  this->enableMSAA = enableMSAA;
  this->samples = samples;

  // Create main framebuffer
  glGenFramebuffers(1, &FBO);
  glBindFramebuffer(GL_FRAMEBUFFER, FBO);

  if (enableMSAA)
  {
    // Create multisampled color renderbuffer
    glGenRenderbuffers(1, &MSAA_RBO);
    glBindRenderbuffer(GL_RENDERBUFFER, MSAA_RBO);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_RGB8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, MSAA_RBO);

    // Create multisampled depth/stencil renderbuffer
    glGenRenderbuffers(1, &MSAA_depthRBO);
    glBindRenderbuffer(GL_RENDERBUFFER, MSAA_depthRBO);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, MSAA_depthRBO);

    // Create a resolved framebuffer (non-MSAA)
    glGenFramebuffers(1, &resolvedFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, resolvedFBO);

    // Attach a texture to the resolved framebuffer
    glGenTextures(1, &resolvedTextureId);
    glBindTexture(GL_TEXTURE_2D, resolvedTextureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, resolvedTextureId, 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
      std::cout << "ERROR::FRAMEBUFFER:: Resolved framebuffer is not complete!\n";
  }
  else
  {
    // Create a texture for color attachment
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_id, 0);

    // Create a renderbuffer for depth/stencil attachment
    glGenRenderbuffers(1, &RBO);
    glBindRenderbuffer(GL_RENDERBUFFER, RBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);
  }

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!\n";

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::bind()
{
  glBindFramebuffer(GL_FRAMEBUFFER, FBO);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glClearColor(0.12f, 0.12f, 0.12f, 1.0f);
}

void Framebuffer::unbind()
{
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  if (enableMSAA)
  {
    resolve();
  }
}

void Framebuffer::resolve()
{
  glBindFramebuffer(GL_READ_FRAMEBUFFER, FBO);         // Source: MSAA framebuffer
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, resolvedFBO); // Target: resolved framebuffer

  // Blit (resolve) the color buffer
  glBlitFramebuffer(
      0, 0, framebufferWidth, framebufferHeight, // Source dimensions
      0, 0, framebufferWidth, framebufferHeight, // Destination dimensions
      GL_COLOR_BUFFER_BIT,
      GL_NEAREST);

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::rescale(int width, int height)
{
  framebufferWidth = width;
  framebufferHeight = height;

  if (enableMSAA)
  {
    glBindRenderbuffer(GL_RENDERBUFFER, MSAA_RBO);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_RGB8, width, height);

    glBindRenderbuffer(GL_RENDERBUFFER, MSAA_depthRBO);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_DEPTH24_STENCIL8, width, height);

    glBindTexture(GL_TEXTURE_2D, resolvedTextureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
  }
  else
  {
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

    glBindRenderbuffer(GL_RENDERBUFFER, RBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
  }
}

GLuint Framebuffer::getTextureId()
{
  return enableMSAA ? resolvedTextureId : texture_id;
}

void Framebuffer::cleanup()
{
  if (FBO)
    glDeleteFramebuffers(1, &FBO);
  if (resolvedFBO)
    glDeleteFramebuffers(1, &resolvedFBO);
  if (texture_id)
    glDeleteTextures(1, &texture_id);
  if (resolvedTextureId)
    glDeleteTextures(1, &resolvedTextureId);
  if (RBO)
    glDeleteRenderbuffers(1, &RBO);
  if (MSAA_RBO)
    glDeleteRenderbuffers(1, &MSAA_RBO);
  if (MSAA_depthRBO)
    glDeleteRenderbuffers(1, &MSAA_depthRBO);
}
