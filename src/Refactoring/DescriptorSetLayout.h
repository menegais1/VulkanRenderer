//
// Created by menegais on 14/02/2021.
//

#ifndef VULKANBASE_DESCRIPTORSETLAYOUT_H
#define VULKANBASE_DESCRIPTORSETLAYOUT_H

#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>
#include <vector>
#include <unordered_map>

namespace vk {
    class DescriptorSetLayout {
    public:
        VkDescriptorSetLayout layout;
        std::unordered_map<std::string, VkDescriptorSetLayoutBinding> bindings;

        DescriptorSetLayout(VkDevice vkDevice);

        DescriptorSetLayout &build();

        DescriptorSetLayout &addBinding(std::string name, VkDescriptorType descriptorType, uint32_t descriptorCount, VkShaderStageFlags stageFlags);

        VkDescriptorSetLayoutBinding getBinding(std::string name);

    private:
        VkDevice vkDevice;
    };

}
#endif //VULKANBASE_DESCRIPTORSETLAYOUT_H
