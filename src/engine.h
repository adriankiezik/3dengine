#ifndef ENGINE_H
#define ENGINE_H

#include <string>
#include "window.h"
#include "editor/editor.h"
#include "input_system.h"
#include "script_system.h"
#include "camera.h"
#include "scene.h"
#include "framebuffer.h"
#include "console_stream_buffer.h"

class Engine
{
public:
  Engine(const std::string &title, int width, int height);
  void run();

private:
  void update();
  void postUpdate();

  ConsoleStreamBuffer consoleBuffer;

  Window window;
  Camera camera;
  Scene scene;
  ScriptSystem scriptSystem;
  Editor editor;
  InputSystem inputSystem;
};

#endif
