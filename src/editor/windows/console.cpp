#include "console.h"
#include <sstream>

ConsoleWindow::ConsoleWindow(ConsoleStreamBuffer &console) : console(console) {}

void ConsoleWindow::render(bool &showConsole)
{
  if (!showConsole)
    return;

  ImGui::Begin("Console", &showConsole, ImGuiWindowFlags_AlwaysVerticalScrollbar);
  std::string consoleOutput = console.getOutput();
  std::istringstream stream(consoleOutput);
  std::string line;
  while (std::getline(stream, line))
  {
    ImGui::Text("%s", line.c_str());
  }
  ImGui::End();
}
