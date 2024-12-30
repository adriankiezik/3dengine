#include "script_viewer.h"
#include <filesystem>
#include "../../utils/path_utils.h"

ScriptViewerWindow::ScriptViewerWindow()
{
  editor.SetLanguageDefinition(TextEditor::LanguageDefinition::Lua());
  editor.SetReadOnly(true);
}

void ScriptViewerWindow::init()
{
  ImGuiIO &io = ImGui::GetIO();
  float fontSize = 16.0f;
  io.Fonts->AddFontFromFileTTF(PathUtils::getResourcePath("resources/fonts/JetBrainsMono-Regular.ttf").c_str(), fontSize);

  auto colors = editor.GetPalette();
  colors[(int)TextEditor::PaletteIndex::Default] = ImGui::ColorConvertFloat4ToU32(ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
  colors[(int)TextEditor::PaletteIndex::Keyword] = ImGui::ColorConvertFloat4ToU32(ImVec4(1.0f, 0.0f, 0.0f, 1.0f)); // Red keywords
  colors[(int)TextEditor::PaletteIndex::Number] = ImGui::ColorConvertFloat4ToU32(ImVec4(0.9f, 0.6f, 0.1f, 1.0f));
  colors[(int)TextEditor::PaletteIndex::String] = ImGui::ColorConvertFloat4ToU32(ImVec4(0.5f, 1.0f, 0.5f, 1.0f)); // Light green strings
  colors[(int)TextEditor::PaletteIndex::Comment] = ImGui::ColorConvertFloat4ToU32(ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
  colors[(int)TextEditor::PaletteIndex::Background] = ImGui::ColorConvertFloat4ToU32(ImVec4(0.0f, 0.0f, 0.0f, 1.0f)); // Black background
  editor.SetPalette(colors);
}

void ScriptViewerWindow::render(bool &show)
{
  if (!show)
    return;

  std::string title = "Script Viewer - " + std::filesystem::path(currentScriptPath).filename().string();
  ImGui::Begin(title.c_str(), &show, ImGuiWindowFlags_HorizontalScrollbar);

  ImVec2 available = ImGui::GetContentRegionAvail();
  editor.Render("##editor", ImVec2(available.x, available.y));

  ImGui::End();
}

void ScriptViewerWindow::setScript(const std::string &path, const std::string &content)
{
  currentScriptPath = path;
  editor.SetText(content);
}
