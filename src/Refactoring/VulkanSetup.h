//
// Created by menegais on 10/11/2020.
//

#ifndef VULKANBASE_VULKANSETUP_H
#define VULKANBASE_VULKANSETUP_H


#include "PhysicalDeviceInfo.h"
#include "PresentationEngineInfo.h"
#include "QueueFamilies.h"

class VulkanSetup {
public:
    VulkanSetup(GLFWwindow *window, std::vector<const char *> instanceExtensions = {},
                std::vector<const char *> instanceLayers = {"VK_LAYER_KHRONOS_validation"},
                std::vector<const char *> deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME});

    VkDevice vkDevice;
    VkInstance vkInstance;
    VkPhysicalDevice vkPhysicalDevice;
    VkSwapchainKHR vkSwapchainKHR;
    VkSurfaceKHR vkSurfaceKHR;
    PhysicalDeviceInfo physicalDeviceInfo;
    PresentationEngineInfo presentationEngineInfo;
private:
    std::vector<const char *> instanceExtensions;

    std::vector<const char *> instanceLayers;

    std::vector<const char *> deviceExtensions;

    std::vector<VkExtensionProperties> queryInstanceExtensions();

    std::vector<VkExtensionProperties> queryDeviceExtensions(VkPhysicalDevice const vkPhysicalDevice);

    std::vector<VkLayerProperties> queryInstanceLayers();

    bool
    validateLayers(const std::vector<const char *> requiredLayers, const std::vector<VkLayerProperties> layers);

    bool validateExtensions(const std::vector<const char *> requiredExtensions,
                            const std::vector<VkExtensionProperties> extensions);

    bool prepareForCreateInstance();

    VkInstance createInstance();

    PhysicalDeviceInfo queryPhysicalDeviceInfo(VkPhysicalDevice vkPhysicalDevice);

    VkPhysicalDevice queryPhysicalDevice(VkInstance vkInstance, PhysicalDeviceInfo &physicalDeviceInfo);

    VkDevice createLogicalDevice(VkPhysicalDevice vkPhysicalDevice, PhysicalDeviceInfo physicalDeviceInfo);

    VkSurfaceKHR createSurface(VkInstance instance, GLFWwindow *window);

    VkSwapchainKHR createSwapchain(VkDevice vkDevice, VkSurfaceKHR vkSurface, PhysicalDeviceInfo physicalDeviceInfo, PresentationEngineInfo presentationEngineInfo);

    QueueFamilies queryQueueFamilies(VkPhysicalDevice vkPhysicalDevice, PhysicalDeviceInfo physicalDeviceInfo);

    PresentationEngineInfo queryPresentationEngineInfo(PhysicalDeviceInfo physicalDeviceInfo);

    int scorePhysicalDevices(const PhysicalDeviceInfo physicalDeviceInfo);

    VkPresentModeKHR vulkanGetSwapchainPresentMode(const PhysicalDeviceInfo physicalDeviceInfo);

    VkSurfaceFormatKHR vulkanGetSwapchainImageFormat(const PhysicalDeviceInfo physicalDeviceInfo);

    VkExtent2D vulkanGetSwapchainImageExtent(const PhysicalDeviceInfo physicalDeviceInfo);
};


#endif //VULKANBASE_VULKANSETUP_H
