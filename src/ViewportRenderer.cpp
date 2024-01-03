#include "ViewportRenderer.h"

#include "Application.h"
#include "backends/imgui_impl_wgpu.h"
#include "glm/fwd.hpp"
#include "webgpu.h"
#include <algorithm>
#include <cstdint>
#include <execution>

namespace Utils {

static uint32_t ConvertToRGBA(const glm::vec4 &color) {
  auto r = (uint8_t)(color.r * 255.0f);
  auto g = (uint8_t)(color.g * 255.0f);
  auto b = (uint8_t)(color.b * 255.0f);
  auto a = (uint8_t)(color.a * 255.0f);

  uint32_t result = (a << 24) | (b << 16) | (g << 8) | r;
  return result;
}

} // namespace Utils

void ViewportRenderer::OnResize(uint32_t width, uint32_t height) {
  if (m_FinalImage) {
    /* std::cout << "Checking resize necessity"; */
    if (m_FinalImage->GetWidth() == width &&
        m_FinalImage->GetHeight() == height)
      return;

    m_FinalImage->Resize(width, height);
  } else {
    m_FinalImage = std::make_shared<Walnut::Image>(width, height,
                                                   Walnut::ImageFormat::RGBA);
  }

  delete[] m_ImageData;
  m_ImageData = new uint32_t[width * height];
  // Multithreading
  m_VerticalIter.resize(height);
  m_HorizotalIter.resize(width);

  for (uint32_t i = 0; i < height; i++)
    m_VerticalIter[i] = i;

  for (uint32_t i = 0; i < width; i++)
    m_HorizotalIter[i] = i;
}

void ViewportRenderer::Render(const Scene &scene, const Camera &camera) {
  m_ActiveScene = &scene;
  m_ActiveCamera = &camera;

  for (uint32_t y = 0; y < m_FinalImage->GetHeight(); y++) {
    for (uint32_t x = 0; x < m_FinalImage->GetWidth(); x++) {
      glm::vec4 color = PerPixel(x, y);
      color = glm::clamp(color, glm::vec4(0.0f), glm::vec4(1.0f));
      m_ImageData[x + y * m_FinalImage->GetWidth()] =
          Utils::ConvertToRGBA(color);
    }
  }
  // TODO: pull data from nextTexture
  m_FinalImage->SetData(m_ImageData);
}

glm::vec4 ViewportRenderer::PerPixel(uint32_t x, uint32_t y) {
  return {1.0, 1.0, 0.5, 1.0};
}
