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
  ViewportLayer() : m_Camera(45.0f, 0.1f, 100.0f) {

    {
      Sphere sphere;
      sphere.Position = {0.0f, 0.0f, 0.0f};
      sphere.Radius = 0.5f;
      sphere.Albedo = {1.0f, 0.0f, 1.0f};
      m_Scene.Spheres.push_back(sphere);
    }

    {
      Sphere sphere;
      sphere.Position = {1.0f, 0.0f, -5.0f};
      sphere.Radius = 1.5f;
      sphere.Albedo = {0.2f, 0.3f, 1.0f};
      m_Scene.Spheres.push_back(sphere);
    }
  }

  virtual void OnUpdate(float ts) override { m_Camera.OnUpdate(ts); }

  virtual void OnUIRender() override {
    ImGui::Begin("Settings");
    ImGui::Text("Last render: %.3fms", m_LastRenderTime);
    if (ImGui::Button("Render")) {
      Render();
    }
    ImGui::End();

    ImGui::Begin("Scene");
    for (size_t i = 0; i < m_Scene.Spheres.size(); i++) {
      ImGui::PushID(i);

      Sphere &sphere = m_Scene.Spheres[i];
      ImGui::DragFloat3("Position", glm::value_ptr(sphere.Position), 0.1f);
      ImGui::DragFloat("Radius", &sphere.Radius, 0.1f);
      ImGui::ColorEdit3("Albedo", glm::value_ptr(sphere.Albedo));

      ImGui::Separator();

      ImGui::PopID();
    }
    ImGui::End();
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
