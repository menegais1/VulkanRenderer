cmake_minimum_required(VERSION 3.17)
project(VulkanBase)

set(CMAKE_CXX_STANDARD 17)

find_package(Vulkan REQUIRED)

#Finds all .h and .cpp
file(GLOB Dear "Dependencies/imgui/*.h" "Dependencies/imgui/*.cpp")
file(GLOB_RECURSE Source "src/*.h" "src/*.cpp")
ADD_SUBDIRECTORY(Dependencies/glfw-3.3.2)
ADD_SUBDIRECTORY(Dependencies/glm)
include_directories(Dependencies/imgui)
include_directories(Dependencies/tinyobjloader)
add_executable(VulkanBase
        Dependencies/stb/stb_image.h
        ${Source}
        ${Dear})

target_link_libraries(VulkanBase glfw glm ${GLFW_LIBRARIES} Vulkan::Vulkan)
