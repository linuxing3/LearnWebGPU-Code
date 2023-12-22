add_rules('mode.debug', 'mode.release')

-- dependencies
includes('deps.lua')

-- requires
add_requires('glfw')
add_requires('webgpu')
add_requires('glfw3webgpu')
add_requires('imgui')

-- target
target('wgpu-imgui-app')
set_kind('binary')
set_targetdir('build')
add_defines("RESOURCE_DIR=\"../resources\"")
add_defines("WGPU_TARGET=WGPU_TARGET_LINUX_X11")
add_defines("GLFW_EXPOSE_NATIVE_X11")
add_files('src/*.cpp')
add_files('src/Walnut/**.cpp')
add_includedirs("vendor/webgpu/include/webgpu")
add_includedirs("vendor/imgui")
add_includedirs("vendor/glfw3webgpu")
add_includedirs("vendor")
add_includedirs("src")
add_linkdirs('build/glfw/src')
add_links('glfw3')
add_linkdirs('build/glfw3webgpu')
add_links('glfw3webgpu')
add_linkdirs('build/imgui')
add_links('imgui')
add_links('tbb','pthread', 'wgpu')
add_links("X11", "Xrandr", "Xrender", "Xinerama", "Xfixes", "Xcursor", "Xi", "Xext")
