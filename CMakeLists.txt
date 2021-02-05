cmake_minimum_required(VERSION 3.17)
project(VulkanBase)

set(CMAKE_CXX_STANDARD 17)

find_package(Vulkan REQUIRED)

ADD_SUBDIRECTORY(Dependencies/glfw-3.3.2)
ADD_SUBDIRECTORY(Dependencies/glm)
add_executable(VulkanBase

        Dependencies/stb/stb_image.h

        src/main.cpp
        src/Refactoring/VulkanSetup.cpp
        src/Refactoring/VulkanSetup.h
#        src/VulkanStructures.h
        src/VulkanDebug.h
        src/FileManagers/FileLoader.h
        src/FileManagers/FileLoader.cpp
        src/FileManagers/Bitmap/Bitmap.h
        src/FileManagers/Bitmap/Bitmap.cpp
#        src/VulkanHelpers.h
        src/Refactoring/CommandBufferUtils.cpp
        src/Refactoring/CommandBufferUtils.h
        src/MemoryManagement/AllocationBlock.h
        src/MemoryManagement/VideoMemoryAllocator.cpp
        src/MemoryManagement/VideoMemoryAllocator.h
        src/MemoryManagement/IMemoryAllocator.h
        src/MemoryManagement/PassThroughAllocator.cpp
        src/MemoryManagement/PassThroughAllocator.h
        src/MemoryManagement/MemoryAllocationHelpers.h
        src/Refactoring/CreateInfoHelpers.h
        src/Refactoring/PresentationEngineInfo.h
        src/Refactoring/QueueFamilies.h
        src/Refactoring/PhysicalDeviceInfo.h
        src/Refactoring/RenderFrame.h
        src/Refactoring/VulkanCreateFunctions.h
        src/Refactoring/Buffer.h
        src/AutoShaders/AutoShaders.h)

target_include_directories(VulkanBase PRIVATE ${Stb_INCLUDE_DIRS})


target_link_libraries(VulkanBase glfw glm ${GLFW_LIBRARIES} Vulkan::Vulkan)
