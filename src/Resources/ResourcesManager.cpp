//
// Created by Lorenzo on 2/11/12021.
//

#include "ResourcesManager.h"
#include "../Refactoring/CreateInfoHelpers.h"
#include "../Refactoring/VulkanCreateFunctions.h"

ResourcesManager::ResourcesManager(VkDevice &device) : device(device), uniformBufferCount(0), textureSamplerCount(0){ }

void ResourcesManager::addUniformBufferLayout(uint32_t sets)
{
    createDescriptorLayout(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT);
    uniformBufferCount += sets;
}

void ResourcesManager::addTextureSamplerLayout(uint32_t sets)
{
    createDescriptorLayout(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT);
    textureSamplerCount += sets;
}

/* Create and return descriptor layouts */
std::vector<VkDescriptorSetLayout> ResourcesManager::commitDescriptorBindings()
{
    auto descriptorSetLayout = vk::createDescriptorSetLayout(device, vk::descriptorSetLayoutCreateInfo(descriptorSetLayoutBindings));
    descriptorSetLayouts.push_back(descriptorSetLayout);
    return descriptorSetLayouts;
}

void ResourcesManager::createDescriptorLayout(VkDescriptorType descriptorType, VkShaderStageFlagBits stageFlags)
{
    uint32_t binding = descriptorSetLayoutBindings.size();
    auto descriptorSetLayout = vk::descriptorSetLayoutBinding(binding, descriptorType, 1, stageFlags);
    descriptorSetLayoutBindings.push_back(descriptorSetLayout);
}

VkDescriptorPool ResourcesManager::createDescriptorPools()
{
    std::vector<VkDescriptorPoolSize> poolSizes =
    {
        vk::descriptorPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, uniformBufferCount),
        vk::descriptorPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, textureSamplerCount)
    };
    VkDescriptorPoolCreateInfo descriptorPoolInfo =	vk::descriptorPoolCreateInfo(poolSizes, poolSizes.size());

    descriptorPool = vk::createDescriptorPool(device, descriptorPoolInfo);

    return descriptorPool;
}

// Todo: Implement! How? By saving the base properties (texture saves path) and uniform saves ?
void ResourcesManager::createDescriptorSets()
{
    //std::vector<VkWriteDescriptorSet> descriptorWriteSet{};

    //vkUpdateDescriptorSets(device, descriptorWriteSet.size(), descriptorWriteSet.data(), 0, nullptr);
}