#include "script_viewer.h"
#include <filesystem>

ScriptViewerWindow::ScriptViewerWindow() {
    editor.SetLanguageDefinition(TextEditor::LanguageDefinition::Lua());
    editor.SetReadOnly(true);
    
    // Set a dark color scheme
    auto colors = editor.GetPalette();
    colors[(int)TextEditor::PaletteIndex::Default] = ImGui::ColorConvertFloat4ToU32(ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
    colors[(int)TextEditor::PaletteIndex::Keyword] = ImGui::ColorConvertFloat4ToU32(ImVec4(0.4f, 0.4f, 1.0f, 1.0f));
    colors[(int)TextEditor::PaletteIndex::Number] = ImGui::ColorConvertFloat4ToU32(ImVec4(0.9f, 0.6f, 0.1f, 1.0f));
    colors[(int)TextEditor::PaletteIndex::String] = ImGui::ColorConvertFloat4ToU32(ImVec4(0.5f, 0.9f, 0.5f, 1.0f));
    colors[(int)TextEditor::PaletteIndex::Comment] = ImGui::ColorConvertFloat4ToU32(ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
    colors[(int)TextEditor::PaletteIndex::Background] = ImGui::ColorConvertFloat4ToU32(ImVec4(0.2f, 0.2f, 0.2f, 1.0f));
    editor.SetPalette(colors);
}

void ScriptViewerWindow::render(bool& show) {
    if (!show) return;

    std::string title = "Script Viewer - " + std::filesystem::path(currentScriptPath).filename().string();
    ImGui::Begin(title.c_str(), &show, ImGuiWindowFlags_HorizontalScrollbar);

    // Make editor take up all available space in window
    ImVec2 available = ImGui::GetContentRegionAvail();
    editor.Render("##editor", ImVec2(available.x, available.y));

    ImGui::End();
}

void ScriptViewerWindow::setScript(const std::string& path, const std::string& content) {
    currentScriptPath = path;
    editor.SetText(content);
}
