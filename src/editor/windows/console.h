#ifndef CONSOLE_WINDOW_H
#define CONSOLE_WINDOW_H

#include <string>
#include <imgui.h>
#include "../../console_stream_buffer.h"

class ConsoleWindow
{
public:
  explicit ConsoleWindow(ConsoleStreamBuffer &console);
  void render(bool &showConsole);

private:
  ConsoleStreamBuffer &console;
};

#endif
