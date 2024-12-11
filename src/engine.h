#ifndef ENGINE_H
#define ENGINE_H

#include <string>
#include "window.h"
#include "editor.h"
#include "input_system.h"
#include "script_system.h"
#include "camera.h"
#include "scene.h"
#include "framebuffer.h"

class Engine
{
public:
  Engine(const std::string &title, int width, int height);
  void run();

private:
  void update();
  void postUpdate();

  Window window;
  Camera camera;
  Scene scene;
  ScriptSystem scriptSystem;
  Editor editor;
  InputSystem inputSystem;
  Framebuffer framebuffer;
};

#endif
