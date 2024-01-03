--- @diagnostic disable:undefined-global
add_rules('mode.debug', 'mode.release')

includes('vendor/imgui')
includes('vendor/glfw')
includes('vendor/glfw3webgpu')

-- target
target('wgpu-imgui-app')
set_kind('binary')
set_targetdir('build')
add_defines('RESOURCE_DIR="../resources"')
add_defines('WGPU_TARGET=WGPU_TARGET_LINUX_X11')
add_defines('GLFW_EXPOSE_NATIVE_X11')
add_files('src/**.cpp')
add_includedirs('vendor', 'src')
add_includedirs('vendor/glfw')
add_includedirs('vendor/imgui')
add_includedirs('vendor/glfw3webgpu')
add_includedirs('vendor/webgpu/include/webgpu', 'vendor/webgpu/include')
add_deps('glfw')
add_deps('glfw3webgpu')
add_deps('imgui')
add_linkdirs('vendor/webgpu/bin/linux-x86_64')
add_links('wgpu', 'vulkan')
add_links('tbb', 'pthread')
add_links('X11', 'Xrandr', 'Xrender', 'Xinerama', 'Xfixes', 'Xcursor', 'Xi', 'Xext')
