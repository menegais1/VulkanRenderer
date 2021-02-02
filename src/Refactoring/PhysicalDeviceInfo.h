//
// Created by menegais on 02/02/2021.
//

#ifndef VULKANBASE_PHYSICALDEVICEINFO_H
#define VULKANBASE_PHYSICALDEVICEINFO_H

#include "QueueFamilies.h"
#include <vector>
struct PhysicalDeviceInfo {
    VkPhysicalDeviceFeatures physicalDeviceFeatures;
    VkPhysicalDeviceProperties physicalDeviceProperties;
    VkSurfaceCapabilitiesKHR surfaceCapabilities;
    VkPhysicalDeviceMemoryProperties physicalDeviceMemoryProperties;
    std::vector <VkPresentModeKHR> surfacePresentModes;
    std::vector <VkSurfaceFormatKHR> surfaceFormats;
    std::vector <VkQueueFamilyProperties> queueFamilyProperties;
    QueueFamilies queueFamilies;
};
#endif //VULKANBASE_PHYSICALDEVICEINFO_H
