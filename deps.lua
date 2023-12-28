add_requires('glfw')

target('glfw3webgpu')
set_kind('static')
add_files('vendor/glfw3webgpu/*.c')
add_includedirs('vendor/glfw3webgpu')
add_includedirs('vendor/webgpu/include/webgpu')
add_packages('glfw')
add_links('wgpu')

target('imgui')
set_kind('static')
add_files('vendor/imgui/*.cpp')
add_files('vendor/imgui/backends/imgui_impl_glfw.cpp')
add_files('vendor/imgui/backends/imgui_impl_wgpu.cpp')
add_files('vendor/imgui/backends/imgui_impl_vulkan.cpp')
add_includedirs('vendor/imgui')
add_packages('glfw')
add_links('wgpu')