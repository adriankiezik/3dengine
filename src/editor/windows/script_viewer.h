#pragma once

#include <string>
#include <imgui.h>
#include "../../lib/imgui/TextEditor.h"

class ScriptViewerWindow
{
public:
  ScriptViewerWindow();
  void render(bool &show);
  void setScript(const std::string &path, const std::string &content);

private:
  bool isOpen = false;
  TextEditor editor;
  std::string currentScriptPath;
};
