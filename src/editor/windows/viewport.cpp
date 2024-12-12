#include "viewport.h"

ViewportWindow::ViewportWindow(Framebuffer &framebuffer, Camera &camera)
    : framebuffer(framebuffer), camera(camera), renderer_size(0, 0) {}

void ViewportWindow::render()
{
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
  ImGui::Begin("Renderer");

  adjustViewportSize();

  ImVec2 pos = ImGui::GetCursorScreenPos();
  ImGui::GetWindowDrawList()->AddImage(
      (ImTextureID)(uintptr_t)framebuffer.getTextureId(),
      ImVec2(pos.x, pos.y),
      ImVec2(pos.x + renderer_size.x, pos.y + renderer_size.y),
      ImVec2(0, 1),
      ImVec2(1, 0));

  ImGui::End();
  ImGui::PopStyleVar();
}

void ViewportWindow::adjustViewportSize()
{
  const float window_width = ImGui::GetContentRegionAvail().x;
  const float window_height = ImGui::GetContentRegionAvail().y;

  if (window_width != renderer_size.x || window_height != renderer_size.y)
  {
    renderer_size = ImVec2(window_width, window_height);
    framebuffer.rescale(window_width, window_height);
    camera.setAspectRatio(static_cast<float>(window_width / window_height));
  }

  glViewport(0, 0, window_width, window_height);
}
