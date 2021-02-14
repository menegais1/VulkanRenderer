//
// Created by menegais on 14/02/2021.
//

#ifndef VULKANBASE_DESCRIPTORSET_H
#define VULKANBASE_DESCRIPTORSET_H

#include "DescriptorSetLayout.h"

namespace vk {
    class DescriptorSet {
    public:
        std::vector<std::pair<VkDescriptorType, float>> descriptorPoolSizes = {
                {VK_DESCRIPTOR_TYPE_SAMPLER, 1},
                {VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1},
                {VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 2},
                {VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 3},
                {VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 4},
                {VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 5},
                {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 6},
                {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 7},
                {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 8},
                {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 9},
                {VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 10}
        };

        DescriptorSet(VkDevice vkDevice, vk::DescriptorSetLayout layout);
    private:
        VkDevice vkDevice;
    };
}

#endif //VULKANBASE_DESCRIPTORSET_H
