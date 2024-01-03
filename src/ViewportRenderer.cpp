#include "ViewportRenderer.h"

#include "Application.h"
#include "Walnut/Random.h"
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
  Ray ray;
  ray.Origin = m_ActiveCamera->GetPosition();
  ray.Direction =
      m_ActiveCamera->GetRayDirections()[x + y * m_FinalImage->GetWidth()];

  glm::vec3 color(0.0f);
  float multiplier = 1.0f;

  int bounces = 2;
  for (int i = 0; i < bounces; i++) {
    HitPayload payload = TraceRay(ray);
    if (payload.HitDistance < 0.0f) {
      glm::vec3 skyColor = glm::vec3(0.0f, 0.0f, 0.0f);
      color += skyColor * multiplier;
      break;
    }

    glm::vec3 lightDir = glm::normalize(glm::vec3(-1, -1, -1));
    float lightIntensity = glm::max(glm::dot(payload.WorldNormal, -lightDir),
                                    0.0f); // == cos(angle)

    const Sphere &sphere = m_ActiveScene->Spheres[payload.ObjectIndex];
    glm::vec3 sphereColor = sphere.Albedo;
    sphereColor *= lightIntensity;
    color += sphereColor * multiplier;

    multiplier *= 0.7f;

    ray.Origin = payload.WorldPosition + payload.WorldNormal * 0.0001f;
    ray.Direction = glm::reflect(ray.Direction, payload.WorldNormal);
  }

  return glm::vec4(color, 1.0f);
}

ViewportRenderer::HitPayload ViewportRenderer::TraceRay(const Ray &ray) {
  // (bx^2 + by^2)t^2 + (2(axbx + ayby))t + (ax^2 + ay^2 - r^2) = 0
  // where
  // a = ray origin
  // b = ray direction
  // r = radius
  // t = hit distance

  int closestSphere = -1;
  float hitDistance = std::numeric_limits<float>::max();
  for (size_t i = 0; i < m_ActiveScene->Spheres.size(); i++) {
    const Sphere &sphere = m_ActiveScene->Spheres[i];
    glm::vec3 origin = ray.Origin - sphere.Position;

    float a = glm::dot(ray.Direction, ray.Direction);
    float half_b = glm::dot(origin, ray.Direction);
    float c = glm::dot(origin, origin) - sphere.Radius * sphere.Radius;

    // Quadratic forumula discriminant:
    // b^2 - 4ac

    float discriminant = half_b * half_b - a * c;
    if (discriminant < 0.0f)
      continue;

    // Quadratic formula:
    // (-b +- sqrt(discriminant)) / 2a
    // float t0 = (-b + glm::sqrt(discriminant)) / (2.0f * a); // Second hit
    // distance (currently unused)
    float closestT = -half_b - glm::sqrt(discriminant) / a;
    if (closestT > 0.0f && closestT < hitDistance) {
      hitDistance = closestT;
      closestSphere = (int)i;
    }
  }

  if (closestSphere < 0)
    return Miss(ray);

  return ClosestHit(ray, hitDistance, closestSphere);
}

ViewportRenderer::HitPayload ViewportRenderer::ClosestHit(const Ray &ray,
                                                          float hitDistance,
                                                          int objectIndex) {
  ViewportRenderer::HitPayload payload;
  payload.HitDistance = hitDistance;
  payload.ObjectIndex = objectIndex;

  const Sphere &closestSphere = m_ActiveScene->Spheres[objectIndex];

  // move camera instead to get sphere moved
  glm::vec3 origin = ray.Origin - closestSphere.Position;
  payload.WorldPosition = origin + ray.Direction * hitDistance;
  payload.WorldNormal = glm::normalize(payload.WorldPosition);
  HitPayload::set_sphere_uv(payload.WorldPosition, payload.u, payload.v);
  // move camera back
  payload.WorldPosition += closestSphere.Position;

  return payload;
}

ViewportRenderer::HitPayload ViewportRenderer::Miss(const Ray &ray) {
  ViewportRenderer::HitPayload payload;
  payload.HitDistance = -1.0f;
  return payload;
}
