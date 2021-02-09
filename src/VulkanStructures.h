//
// Created by menegais on 10/11/2020.
//

#ifndef VULKANBASE_VULKANSTRUCTURE_H
#define VULKANBASE_VULKANSTRUCTURE_H

#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>
#include <vector>
#include "MemoryManagement/AllocationBlock.h"



struct Image2D {
    void *data;
    uint32_t width;
    uint32_t height;
    VkImage image;
    VkImageView imageView;
    VkSampler sampler;
    AllocationBlock memory;

};


#endif