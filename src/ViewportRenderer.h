#pragma once

#include "Walnut/Image.h"

#include "Camera.h"
#include "Scene.h"
#include "glm/fwd.hpp"

#include <cstdint>
#include <glm/ext/scalar_constants.hpp>
#include <glm/glm.hpp>
#include <memory>

class ViewportRenderer {
public:
  ViewportRenderer() = default;

  void OnResize(uint32_t width, uint32_t height);
  void Render(const Scene &scene, const Camera &camera);

  std::shared_ptr<Walnut::Image> GetFinalImage() const { return m_FinalImage; }

private:
  glm::vec4 PerPixel(uint32_t x, uint32_t y);

private:
  std::shared_ptr<Walnut::Image> m_FinalImage;
  uint32_t *m_ImageData = nullptr;

  const Scene *m_ActiveScene = nullptr;
  const Camera *m_ActiveCamera = nullptr;
  std::vector<uint32_t> m_VerticalIter;
  std::vector<uint32_t> m_HorizotalIter;
};
