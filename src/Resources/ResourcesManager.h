//
// Created by Lorenzo on 2/11/12021.
//

#ifndef VULKAN_BASE_DESCRIPTOR_MANAGER_H
#define VULKAN_BASE_DESCRIPTOR_MANAGER_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>

class ResourcesManager
{
public:
    explicit ResourcesManager(VkDevice& device);
    void addTextureSamplerLayout();
    void addUniformBufferLayout();
    std::vector<VkDescriptorSetLayout> commitDescriptorBindings();
    VkDescriptorPool createDescriptorPools(uint32_t frameCount);
    void createDescriptorSets();
private:
    /* Input */
    VkDevice& device;

    /* Descriptor collections */
    std::vector<VkDescriptorSetLayoutBinding> descriptorSetLayoutBindings;
    std::vector<VkDescriptorSetLayout> descriptorSetLayouts;
    VkDescriptorPool descriptorPool{};

    /* Internal control*/
    uint32_t textureSamplerCount;
    uint32_t uniformBufferCount;

    /* Helper function */
    void createDescriptorLayout(VkDescriptorType descriptorType, VkShaderStageFlagBits stageFlags);

};


#endif //VULKAN_BASE_DESCRIPTOR_MANAGER_H
