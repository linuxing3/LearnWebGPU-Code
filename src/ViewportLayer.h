#include "Application.h"

#include "ViewportRenderer.h"
#include "Walnut/Image.h"
#include "Walnut/Timer.h"

#include "Camera.h"
#include "Renderer.h"
#include "imgui.h"
#include "webgpu.hpp"

#include <float.h>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <memory>

using namespace Walnut;

class ViewportLayer : public Walnut::Layer {
public:
  ViewportLayer() : m_Camera(45.0f, 0.1f, 100.0f) {}

  virtual void OnUpdate(float ts) override { m_Camera.OnUpdate(ts); }

  virtual void OnUIRender() override {
    // Viewport to show Walnut Image in Imgui window
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("Viewport");

    m_ViewportWidth = ImGui::GetContentRegionAvail().x;
    m_ViewportHeight = ImGui::GetContentRegionAvail().y;

    // Call renderer
    auto image = m_Renderer.GetFinalImage();
    if (image) {
      ImGui::Image(image->GetTextureId(),
                   {(float)image->GetWidth(), (float)image->GetHeight()},
                   ImVec2(0, 1), ImVec2(1, 0));
    }

    ImGui::End();
    ImGui::PopStyleVar();

    Render();
  }

  void Render() {
    Timer timer;

    m_Camera.OnResize(m_ViewportWidth, m_ViewportHeight);

    m_Renderer.OnResize(m_ViewportWidth, m_ViewportHeight);
    m_Renderer.Render(m_Scene, m_Camera);

    // render procedure
    m_LastRenderTime = timer.ElapsedMillis();
  }

private:
  ViewportRenderer m_Renderer;
  Camera m_Camera;
  Scene m_Scene;
  uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

  float m_LastRenderTime = 0.0F;
};
