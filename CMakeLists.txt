cmake_minimum_required(VERSION 3.1...3.25)
project(
  LearnWebGPU
  VERSION 0.1.0
  LANGUAGES CXX C)

include(utils.cmake)

# We add an option to enable different settings when developping the app than
# when distributing it.
option(DEV_MODE "Set up development helper settings" ON)

add_subdirectory(vendor/glfw)
add_subdirectory(vendor/webgpu)
add_subdirectory(vendor/glfw3webgpu)
add_subdirectory(vendor/imgui)

include_directories(vendor)
include_directories(src)

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -ltbb")
find_package(TBB REQUIRED COMPONENTS tbb)

add_executable(
  App
  src/Application.h
  src/Application.cpp
  src/ResourceManager.h
  src/ResourceManager.cpp
  src/implementations.cpp
  src/Walnut/Image.cpp
  src/Walnut/Image.h
  src/Walnut/Random.h
  src/Walnut/Input/Input.h
  src/Walnut/Random.cpp
  src/Walnut/Input/Input.cpp
  src/Ray.h
  src/RayLayer.h
  src/Layer.h
  src/Renderer.h
  src/Renderer.cpp
  src/Camera.h
  src/Camera.cpp
  src/main.cpp)

if(DEV_MODE)
    # In dev mode, we load resources from the source tree, so that when we
    # dynamically edit resources (like shaders), these are correctly versionned.
    target_compile_definitions(
    App PRIVATE RESOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/resources")
else()
    # In release mode, we just load resources relatively to wherever the
    # executable is launched from, so that the binary is portable
    target_compile_definitions(App PRIVATE RESOURCE_DIR="./resources")
endif()

target_include_directories(App PRIVATE .)
target_link_libraries(App PRIVATE glfw webgpu glfw3webgpu imgui pthread)

if(MSVC)
    target_compile_options(App PRIVATE /wd4201)
endif(MSVC)

set_target_properties(App PROPERTIES CXX_STANDARD 17)
# target_treat_all_warnings_as_errors(App)
target_copy_webgpu_binaries(App)
