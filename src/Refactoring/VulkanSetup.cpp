//
// Created by menegais on 10/11/2020.
//

#include <vector>

#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>
#include <cstring>
#include <stdexcept>
#include <set>
#include <iostream>
#include "VulkanSetup.h"
#include "../VulkanDebug.h"
#include "CreateInfoHelpers.h"
#include "VulkanCreateFunctions.h"


VkPresentModeKHR
VulkanSetup::vulkanGetSwapchainPresentMode(const PhysicalDeviceInfo physicalDeviceInfo) {
    for (auto presentMode: physicalDeviceInfo.surfacePresentModes) {
        if (presentMode == VK_PRESENT_MODE_FIFO_KHR) return presentMode;
    }
    return physicalDeviceInfo.surfacePresentModes[0];
}

VkSurfaceFormatKHR
VulkanSetup::vulkanGetSwapchainImageFormat(const PhysicalDeviceInfo physicalDeviceInfo) {
    for (auto imageFormat: physicalDeviceInfo.surfaceFormats) {
        if (imageFormat.format == VK_FORMAT_B8G8R8A8_SRGB &&
            imageFormat.colorSpace == VK_COLORSPACE_SRGB_NONLINEAR_KHR)
            return imageFormat;
    }
    return physicalDeviceInfo.surfaceFormats[0];
}

VkExtent2D
VulkanSetup::vulkanGetSwapchainImageExtent(const PhysicalDeviceInfo physicalDeviceInfo) {
    return physicalDeviceInfo.surfaceCapabilities.minImageExtent;
}


std::vector<VkExtensionProperties> VulkanSetup::queryInstanceExtensions() {
    unsigned int extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
    std::vector<VkExtensionProperties> extensions(extensionCount);

    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

    return extensions;
}

std::vector<VkExtensionProperties> VulkanSetup::queryDeviceExtensions(const VkPhysicalDevice vkPhysicalDevice) {
    unsigned int extensionCount = 0;
    vkEnumerateDeviceExtensionProperties(vkPhysicalDevice, nullptr, &extensionCount, nullptr);
    std::vector<VkExtensionProperties> extensions(extensionCount);

    vkEnumerateDeviceExtensionProperties(vkPhysicalDevice, nullptr, &extensionCount, extensions.data());

    return extensions;
}

std::vector<VkLayerProperties> VulkanSetup::queryInstanceLayers() {
    unsigned int layerCount = 0;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
    std::vector<VkLayerProperties> layers(layerCount);

    vkEnumerateInstanceLayerProperties(&layerCount, layers.data());

    return layers;
}

bool VulkanSetup::validateLayers(const std::vector<const char *> requiredLayers, const std::vector<VkLayerProperties> layers) {
    bool present = false;
    for (int i = 0; i < requiredLayers.size(); ++i) {
        for (int j = 0; j < layers.size(); ++j) {
            if (strcmp(requiredLayers[i], layers[j].layerName) == 0) {
                present = true;
                break;
            }
            present = false;
        }
        if (!present) return false;
    }
    return true;
}

bool VulkanSetup::validateExtensions(const std::vector<const char *> requiredExtensions, const std::vector<VkExtensionProperties> extensions) {
    bool present = false;
    for (int i = 0; i < requiredExtensions.size(); ++i) {
        for (int j = 0; j < extensions.size(); ++j) {
            if (strcmp(requiredExtensions[i], extensions[j].extensionName) == 0) {
                present = true;
                break;
            }
            present = false;
        }
        if (!present) return false;
    }
    return true;
}

bool VulkanSetup::prepareForCreateInstance() {
    unsigned int count = 0;
    const char **glfwExtensions = glfwGetRequiredInstanceExtensions(&count);
    for (int i = 0; i < count; ++i) {
        instanceExtensions.push_back(glfwExtensions[i]);
    }
    auto extensions = queryInstanceExtensions();
    auto layers = queryInstanceLayers();
    bool layersPresent = validateLayers(instanceLayers, layers);
    bool extensionsPresent = validateExtensions(instanceExtensions, extensions);
    return layersPresent && extensionsPresent;
}

VkInstance VulkanSetup::createInstance() {
    if (!prepareForCreateInstance()) {
        throw std::runtime_error("Missing layers or extension in instance");
    }
    VkApplicationInfo vkApplicationInfo = vk::applicationInfo(VK_API_VERSION_1_2, VK_MAKE_VERSION(1, 0, 0),
                                                              VK_MAKE_VERSION(1, 0, 0), "APP", "ENGINE");
    VkInstanceCreateInfo vkInstanceCreateInfo = vk::instanceCreateInfo(instanceExtensions, instanceLayers, &vkApplicationInfo);
    VkInstance instance;
    vk::VK_ASSERT(vkCreateInstance(&vkInstanceCreateInfo, nullptr, &instance));
    return instance;
}

PhysicalDeviceInfo VulkanSetup::queryPhysicalDeviceInfo(VkPhysicalDevice vkPhysicalDevice) {
    PhysicalDeviceInfo physicalDeviceInfo;
    unsigned int surfaceFormatCount = 0;
    unsigned int presentModeCount = 0;
    unsigned int queueFamilyPropertiesCount = 0;
    vkGetPhysicalDeviceFeatures(vkPhysicalDevice, &physicalDeviceInfo.physicalDeviceFeatures);
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(vkPhysicalDevice, vkSurfaceKHR, &physicalDeviceInfo.surfaceCapabilities);
    vkGetPhysicalDeviceProperties(vkPhysicalDevice, &physicalDeviceInfo.physicalDeviceProperties);
    vkGetPhysicalDeviceMemoryProperties(vkPhysicalDevice, &physicalDeviceInfo.physicalDeviceMemoryProperties);

    vkGetPhysicalDeviceSurfaceFormatsKHR(vkPhysicalDevice, vkSurfaceKHR, &surfaceFormatCount, nullptr);
    vkGetPhysicalDeviceSurfacePresentModesKHR(vkPhysicalDevice, vkSurfaceKHR, &presentModeCount, nullptr);
    vkGetPhysicalDeviceQueueFamilyProperties(vkPhysicalDevice, &queueFamilyPropertiesCount, nullptr);

    physicalDeviceInfo.surfaceFormats.resize(surfaceFormatCount);
    physicalDeviceInfo.surfacePresentModes.resize(presentModeCount);
    physicalDeviceInfo.queueFamilyProperties.resize(queueFamilyPropertiesCount);

    vkGetPhysicalDeviceSurfaceFormatsKHR(vkPhysicalDevice, vkSurfaceKHR, &surfaceFormatCount, physicalDeviceInfo.surfaceFormats.data());
    vkGetPhysicalDeviceSurfacePresentModesKHR(vkPhysicalDevice, vkSurfaceKHR, &presentModeCount, physicalDeviceInfo.surfacePresentModes.data());
    vkGetPhysicalDeviceQueueFamilyProperties(vkPhysicalDevice, &queueFamilyPropertiesCount, physicalDeviceInfo.queueFamilyProperties.data());

    physicalDeviceInfo.queueFamilies = queryQueueFamilies(vkPhysicalDevice, physicalDeviceInfo);

    return physicalDeviceInfo;
}

VkPhysicalDevice VulkanSetup::queryPhysicalDevice(VkInstance vkInstance, PhysicalDeviceInfo &physicalDeviceInfo) {
    unsigned int physicalDevicesCount = 0;
    vkEnumeratePhysicalDevices(vkInstance, &physicalDevicesCount, nullptr);
    std::vector<VkPhysicalDevice> physicalDevices(physicalDevicesCount);
    vkEnumeratePhysicalDevices(vkInstance, &physicalDevicesCount, physicalDevices.data());

    int lastScore = 0;
    int lastScoreIndex = -1;
    std::vector<PhysicalDeviceInfo> physicalDeviceInfoList(physicalDevicesCount);
    for (int i = 0; i < physicalDevicesCount; ++i) {
        auto extensions = queryDeviceExtensions(physicalDevices[i]);
        if (!validateExtensions(deviceExtensions, extensions)) {
            break;
        }
        physicalDeviceInfoList[i] = queryPhysicalDeviceInfo(physicalDevices[i]);

        int score = scorePhysicalDevices(physicalDeviceInfoList[i]);
        if (score > lastScore) {
            lastScore = score;
            lastScoreIndex = i;
        }
    }
    physicalDeviceInfo = physicalDeviceInfoList[lastScoreIndex];
    return physicalDevices[lastScoreIndex];
}


int VulkanSetup::scorePhysicalDevices(const PhysicalDeviceInfo physicalDeviceInfo) {
    int score = 0;

    if (physicalDeviceInfo.queueFamilies.presentationFamily == -1) {
        return -1;
    }

    if (physicalDeviceInfo.physicalDeviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
        score += 10;
    } else {
        score += 5;
    }

    return score;
}

VkDevice VulkanSetup::createLogicalDevice(VkPhysicalDevice vkPhysicalDevice, PhysicalDeviceInfo physicalDeviceInfo) {
    float priority = 1.0;
    std::vector<float> graphicsComputePriorities = {priority, priority};
    VkDeviceQueueCreateInfo graphicsPresentationComputeQueues = vk::queueCreateInfo(physicalDeviceInfo.queueFamilies.graphicsFamily, 2, graphicsComputePriorities.data());
    VkDeviceQueueCreateInfo transferQueue = vk::queueCreateInfo(physicalDeviceInfo.queueFamilies.transferFamily, 1, &priority);

    std::vector<VkPhysicalDeviceFeatures> physicalDeviceFeatures = {physicalDeviceInfo.physicalDeviceFeatures};
    std::vector<VkDeviceQueueCreateInfo> queuesCreateInfo = {graphicsPresentationComputeQueues, transferQueue};
    VkDeviceCreateInfo vkDeviceCreateInfo = vk::deviceCreateInfo(deviceExtensions, std::vector<const char *>(), physicalDeviceFeatures, queuesCreateInfo);

    VkDevice vkDevice;
    vk::VK_ASSERT(vkCreateDevice(vkPhysicalDevice, &vkDeviceCreateInfo, nullptr, &vkDevice));
    return vkDevice;
}

VkSurfaceKHR VulkanSetup::createSurface(VkInstance vkInstance, GLFWwindow *window) {
    VkSurfaceKHR vkSurfaceKhr;
    vk::VK_ASSERT(glfwCreateWindowSurface(vkInstance, window, nullptr, &vkSurfaceKhr));
    return vkSurfaceKhr;
}

VkSwapchainKHR VulkanSetup::createSwapchain(VkDevice vkDevice, VkSurfaceKHR vkSurface, PhysicalDeviceInfo physicalDeviceInfo, PresentationEngineInfo presentationEngineInfo) {
    std::vector<uint32_t> queueFamilyIndices = {physicalDeviceInfo.queueFamilies.presentationFamily};
    VkSwapchainCreateInfoKHR vkSwapchainCreateInfoKhr = vk::swapchainCreateInfoKhr(vkSurface, presentationEngineInfo.presentMode, presentationEngineInfo.format.format,
                                                                                   presentationEngineInfo.format.colorSpace, presentationEngineInfo.imageCount,
                                                                                   presentationEngineInfo.extents, queueFamilyIndices);
    VkSwapchainKHR vkSwapchainKhr;
    vk::VK_ASSERT(vkCreateSwapchainKHR(vkDevice, &vkSwapchainCreateInfoKhr, nullptr, &vkSwapchainKhr));
    return vkSwapchainKhr;
}

QueueFamilies VulkanSetup::queryQueueFamilies(VkPhysicalDevice vkPhysicalDevice, PhysicalDeviceInfo physicalDeviceInfo) {
    QueueFamilies queueFamilyInfo;
    for (int i = 0; i < physicalDeviceInfo.queueFamilyProperties.size(); ++i) {
        VkBool32 hasPresentationCapability = VK_FALSE;
        if (physicalDeviceInfo.queueFamilyProperties[i].queueFlags & (VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT) && queueFamilyInfo.graphicsFamily == -1) {
            queueFamilyInfo.graphicsFamily = i;
            queueFamilyInfo.computeFamily = i;
            vkGetPhysicalDeviceSurfaceSupportKHR(vkPhysicalDevice, i, vkSurfaceKHR, &hasPresentationCapability);
            if (hasPresentationCapability) queueFamilyInfo.presentationFamily = i;
        }
        if (!(physicalDeviceInfo.queueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) &&
            physicalDeviceInfo.queueFamilyProperties[i].queueFlags & VK_QUEUE_TRANSFER_BIT && queueFamilyInfo.transferFamily == -1) {
            queueFamilyInfo.transferFamily = i;
        }
        printQueueFamilies(physicalDeviceInfo.queueFamilyProperties[i], hasPresentationCapability);
    }
    if (queueFamilyInfo.transferFamily == -1) {
        queueFamilyInfo.transferFamily = queueFamilyInfo.graphicsFamily;
    }
    return queueFamilyInfo;
}

PresentationEngineInfo VulkanSetup::queryPresentationEngineInfo(PhysicalDeviceInfo physicalDeviceInfo) {
    uint32_t minImageCount = physicalDeviceInfo.surfaceCapabilities.minImageCount + 1;
    PresentationEngineInfo presentationEngineInfo{};
    presentationEngineInfo.format = vulkanGetSwapchainImageFormat(physicalDeviceInfo);
    presentationEngineInfo.presentMode = vulkanGetSwapchainPresentMode(physicalDeviceInfo);
    presentationEngineInfo.extents = vulkanGetSwapchainImageExtent(physicalDeviceInfo);
    presentationEngineInfo.imageCount = physicalDeviceInfo.surfaceCapabilities.maxImageCount > minImageCount ? minImageCount : physicalDeviceInfo.surfaceCapabilities.maxImageCount;
    return presentationEngineInfo;
}

VulkanSetup::VulkanSetup(GLFWwindow *window, std::vector<const char *> instanceExtensions,
                         std::vector<const char *> instanceLayers, std::vector<const char *> deviceExtensions) : instanceLayers(instanceLayers),
                                                                                                                 instanceExtensions(instanceExtensions),
                                                                                                                 deviceExtensions(deviceExtensions) {
    vkInstance = createInstance();
    vkSurfaceKHR = createSurface(vkInstance, window);
    vkPhysicalDevice = queryPhysicalDevice(vkInstance, physicalDeviceInfo);
    vkDevice = createLogicalDevice(vkPhysicalDevice, physicalDeviceInfo);
    presentationEngineInfo = queryPresentationEngineInfo(physicalDeviceInfo);
    vkSwapchainKHR = createSwapchain(vkDevice, vkSurfaceKHR, physicalDeviceInfo, presentationEngineInfo);
}

