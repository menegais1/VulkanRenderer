cmake_minimum_required(VERSION 3.17)
project(VulkanBase)

set(CMAKE_CXX_STANDARD 17)

find_package(Vulkan REQUIRED)

ADD_SUBDIRECTORY(Dependencies/glfw-3.3.2)
ADD_SUBDIRECTORY(Dependencies/glm)
include_directories(Dependencies/imgui)
include_directories(Dependencies/tinyobjloader)
add_executable(VulkanBase
        Dependencies/imgui/imgui_draw.cpp
        Dependencies/imgui/imstb_rectpack.h
        Dependencies/imgui/imgui.h
        Dependencies/imgui/imgui_internal.h
        Dependencies/imgui/imgui_impl_glfw.cpp
        Dependencies/imgui/imgui.cpp
        Dependencies/imgui/imgui_impl_vulkan.h
        Dependencies/imgui/imgui_tables.cpp
        Dependencies/imgui/imgui_impl_vulkan.cpp
        Dependencies/imgui/imgui_widgets.cpp
        Dependencies/imgui/imstb_textedit.h
        Dependencies/imgui/imgui_impl_glfw.h
        Dependencies/imgui/imstb_truetype.h
        Dependencies/imgui/imconfig.h

        src/VulkanDebug.h
        src/Models/DamagedHelmet.cpp
        src/Models/Model.h
        src/Models/Human.h
        src/Models/DamagedHelmet.h
        src/Models/Human.cpp
        src/Models/ObjectLoader.cpp
        src/Models/ObjectLoader.h
        src/main.cpp
        src/VertexAttributes/PnuVertexInput.cpp
        src/VertexAttributes/PnuVertexInput.h
        src/MemoryManagement/PassThroughAllocator.cpp
        src/MemoryManagement/IMemoryAllocator.h
        src/MemoryManagement/VMA.h
        src/MemoryManagement/MemoryAllocationHelpers.h
        src/MemoryManagement/VideoMemoryAllocator.cpp
        src/MemoryManagement/VideoMemoryAllocator.h
        src/MemoryManagement/AllocationBlock.h
        src/MemoryManagement/VMA.cpp
        src/MemoryManagement/PassThroughAllocator.h
        src/VulkanStructures.h
        src/VulkanHelpers.h
        src/FileManagers/FileLoader.h
        src/FileManagers/Bitmap/Bitmap.h
        src/FileManagers/Bitmap/Bitmap.cpp
        src/FileManagers/FileLoader.cpp
        src/VulkanGui.h
        src/AutoShaders/AutoShaders.h
        src/Refactoring/Buffer.cpp
        src/Refactoring/CommandBuffer.cpp
        src/Refactoring/VulkanSetup.h
        src/Refactoring/Queue.h
        src/Refactoring/QueueFamilies.h
        src/Refactoring/PhysicalDeviceInfo.h
        src/Refactoring/VulkanCreateFunctions.h
        src/Refactoring/HostDeviceTransfer.h
        src/Refactoring/Buffer.h
        src/Refactoring/CommandBuffer.h
        src/Refactoring/Queue.cpp
        src/Refactoring/CommandBufferUtils.cpp
        src/Refactoring/CommandBufferUtils.h
        src/Refactoring/RenderFrame.h
        src/Refactoring/HostDeviceTransfer.cpp
        src/Refactoring/CreateInfoHelpers.h
        src/Refactoring/Texture2D/Image2D.h
        src/Refactoring/Texture2D/ImageView2D.h
        src/Refactoring/Texture2D/ImageView2D.cpp
        src/Refactoring/Texture2D/Image2D.cpp
        src/Refactoring/VulkanSetup.cpp
        src/Refactoring/PresentationEngineInfo.h
        src/Textures/Texture.cpp
        src/Textures/Texture.h
        src/Resources/ResourcesManager.h
        src/Resources/ResourcesManager.cpp)
target_link_libraries(VulkanBase glfw glm ${GLFW_LIBRARIES} Vulkan::Vulkan)
