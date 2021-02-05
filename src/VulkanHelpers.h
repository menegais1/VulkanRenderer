//
// Created by menegais on 22/11/2020.
//

#ifndef VULKANBASE_VULKANHELPERS_H
#define VULKANBASE_VULKANHELPERS_H

#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <cstring>
#include "VulkanStructures.h"
#include "Refactoring/CommandBufferUtils.h"
#include "MemoryManagement/VideoMemoryAllocator.h"


void copyBufferHostDevice(VulkanHandles vulkanHandles, PhysicalDeviceInfo physicalDeviceInfo,
                          CommandBufferStructure transferStructure, Buffer source, Buffer destination,
                          VkAccessFlags dstAccess, VkPipelineStageFlags dstStageMask, uint32_t dstQueueFamilyIndex) {
    CommandBufferUtils::vulkanBeginCommandBuffer(vulkanHandles, transferStructure.commandBuffer,
                                                 VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
                                                 {transferStructure.bufferAvaibleFence});
    {
        VkBufferCopy vkBufferCopy{};
        vkBufferCopy.size = destination.size;
        vkBufferCopy.srcOffset = 0;
        vkBufferCopy.dstOffset = 0;
        vkCmdCopyBuffer(transferStructure.commandBuffer, source.buffer, destination.buffer, 1, &vkBufferCopy);
        VkBufferMemoryBarrier vkBufferMemoryBarrier{};
        vkBufferMemoryBarrier.sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;
        vkBufferMemoryBarrier.buffer = destination.buffer;
        vkBufferMemoryBarrier.size = VK_WHOLE_SIZE;
        vkBufferMemoryBarrier.offset = 0;
        vkBufferMemoryBarrier.srcAccessMask = VK_ACCESS_MEMORY_WRITE_BIT;
        vkBufferMemoryBarrier.dstAccessMask = dstAccess;
        vkBufferMemoryBarrier.srcQueueFamilyIndex = transferStructure.queueFamilyIndex;
        vkBufferMemoryBarrier.dstQueueFamilyIndex = dstQueueFamilyIndex;
        vkCmdPipelineBarrier(transferStructure.commandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT,
                             dstStageMask, 0,
                             0, nullptr, 1, &vkBufferMemoryBarrier, 0, nullptr);
    }
    CommandBufferUtils::vulkanSubmitCommandBuffer(transferStructure.queue, transferStructure.commandBuffer,
                                                  std::vector<VkSemaphore>(), std::vector<VkSemaphore>(), nullptr,
                                                  transferStructure.bufferAvaibleFence);
}



VkImage vulkanCreateImage2D(VulkanHandles vulkanHandles, VkExtent2D extent, VkFormat format, VkImageUsageFlags usage) {
    VkImageCreateInfo imageCreateInfo{};
    imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
    imageCreateInfo.usage = usage;
    imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    imageCreateInfo.format = format;
    imageCreateInfo.extent = {extent.width, extent.height, 1};
    imageCreateInfo.arrayLayers = 1;
    imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    imageCreateInfo.mipLevels = 1;
    VkImage image;
    VK_ASSERT(vkCreateImage(vulkanHandles.device, &imageCreateInfo, nullptr, &image));
    return image;
}

VkImageView
vulkanCreateImageView2D(VulkanHandles vulkanHandles, VkImage image, VkFormat format, VkImageAspectFlags aspectMask) {

    VkImageViewCreateInfo vkImageViewCreateInfo{} = vk::imageViewCreateInfo;
    vkImageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    vkImageViewCreateInfo.image = image;
    vkImageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    vkImageViewCreateInfo.format = format;
    vkImageViewCreateInfo.subresourceRange = {aspectMask, 0, 1, 0, 1};
    vkImageViewCreateInfo.components = {VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY,
                                        VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY};



    VkImageView imageView;
    VK_ASSERT(vkCreateImageView(vulkanHandles.device, &vkImageViewCreateInfo, nullptr, &imageView));

    return imageView;

}

VkSampler vulkanCreateSampler2D(VulkanHandles vulkanHandles, VkSamplerAddressMode addressMode, VkBool32 unnormalizedCoordinates) {
    VkSamplerCreateInfo textureSamplerInfo{};
    textureSamplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    textureSamplerInfo.addressModeU = addressMode;
    textureSamplerInfo.addressModeV = addressMode;
    textureSamplerInfo.addressModeW = addressMode;
    textureSamplerInfo.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_BLACK;
    textureSamplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_NEAREST;
    textureSamplerInfo.anisotropyEnable = VK_FALSE;
    textureSamplerInfo.compareEnable = VK_FALSE;
    textureSamplerInfo.minFilter = VK_FILTER_LINEAR;
    textureSamplerInfo.magFilter = VK_FILTER_LINEAR;
    textureSamplerInfo.unnormalizedCoordinates = unnormalizedCoordinates;

    VkSampler sampler;
    VK_ASSERT(vkCreateSampler(vulkanHandles.device, &textureSamplerInfo, nullptr, &sampler));

    return sampler;
}

Texture2D
createTexture2D(VulkanHandles vulkanHandles, PhysicalDeviceInfo physicalDeviceInfo, void *data, VkExtent2D extents,
                VkFormat format, VkImageUsageFlags usage,
                VkImageAspectFlags aspectMask, VkSamplerAddressMode addressMode,
                VkMemoryPropertyFlagBits memoryPropertyFlags, VkBool32 unnormalizedCoordinates) {
    Texture2D texture2D{};
    texture2D.data = data;
    texture2D.width = extents.width;
    texture2D.height = extents.height;
    texture2D.image = vulkanCreateImage2D(vulkanHandles, extents, format, usage);
    VkMemoryRequirements memoryRequirements = vulkanGetImageMemoryRequirements(vulkanHandles, texture2D.image);
    texture2D.memory = vma.vmalloc(memoryRequirements, memoryPropertyFlags);

    VK_ASSERT(vkBindImageMemory(vulkanHandles.device, texture2D.image, texture2D.memory.vkDeviceMemory, texture2D.memory.vkOffset));

    texture2D.imageView = vulkanCreateImageView2D(vulkanHandles, texture2D.image, format, aspectMask);
    texture2D.sampler = vulkanCreateSampler2D(vulkanHandles, addressMode, unnormalizedCoordinates);
    return texture2D;
}

void copyBufferTextureHostDevice(VulkanHandles vulkanHandles, CommandBufferStructure transferStructure, Buffer sourceBuffer, Texture2D texture, VkPipelineStageFlags srcStage,
                                 VkPipelineStageFlags dstStage, uint32_t dstQueueFamilyIndex) {
    VkImageMemoryBarrier vkImageMemoryBarrier{};
    vkImageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    CommandBufferUtils::vulkanBeginCommandBuffer(vulkanHandles, transferStructure.commandBuffer,
                                                 VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
                                                 {transferStructure.bufferAvaibleFence});
    {
        vkImageMemoryBarrier.image = texture.image;
        vkImageMemoryBarrier.subresourceRange = {VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1};
        vkImageMemoryBarrier.srcAccessMask = 0;
        vkImageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        vkImageMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        vkImageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        vkImageMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        vkImageMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

        vkCmdPipelineBarrier(transferStructure.commandBuffer, srcStage,
                             dstStage, 0, 0,
                             nullptr, 0, nullptr,
                             1, &vkImageMemoryBarrier);
        VkImageSubresourceLayers vkImageSubresourceLayers = {VK_IMAGE_ASPECT_COLOR_BIT, 0, 0, 1};
        VkBufferImageCopy vkBufferImageCopy{};
        vkBufferImageCopy.imageExtent = {texture.width, texture.height, 1};
        vkBufferImageCopy.bufferImageHeight = 0;
        vkBufferImageCopy.bufferRowLength = 0;
        vkBufferImageCopy.bufferOffset = 0;
        vkBufferImageCopy.imageOffset = {0, 0, 0};
        vkBufferImageCopy.imageSubresource = vkImageSubresourceLayers;

        vkCmdCopyBufferToImage(transferStructure.commandBuffer, sourceBuffer.buffer, texture.image,
                               VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                               1, &vkBufferImageCopy);
    }
    CommandBufferUtils::vulkanSubmitCommandBuffer(transferStructure.queue, transferStructure.commandBuffer,
                                                  std::vector<VkSemaphore>(), std::vector<VkSemaphore>(), nullptr,
                                                  transferStructure.bufferAvaibleFence);

}

void transitionImageInPipeline(VulkanHandles vulkanHandles, CommandBufferStructure graphicsStructure, Texture2D texture, VkAccessFlags srcAccessMask, VkAccessFlags dstAccessMask, VkImageLayout oldLayout,
                               VkImageLayout newLayout, VkPipelineStageFlags srcStage,
                               VkPipelineStageFlags dstStage) {
    VkImageMemoryBarrier vkImageMemoryBarrier{};
    vkImageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    CommandBufferUtils::vulkanBeginCommandBuffer(vulkanHandles, graphicsStructure.commandBuffer,
                                                 VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
                                                 {graphicsStructure.bufferAvaibleFence});
    {
        vkImageMemoryBarrier.image = texture.image;
        vkImageMemoryBarrier.subresourceRange = {VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1};
        vkImageMemoryBarrier.srcAccessMask = srcAccessMask;
        vkImageMemoryBarrier.dstAccessMask = dstAccessMask;
        vkImageMemoryBarrier.oldLayout = oldLayout;
        vkImageMemoryBarrier.newLayout = newLayout;
        vkImageMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        vkImageMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

        vkCmdPipelineBarrier(graphicsStructure.commandBuffer, srcStage,
                             dstStage, 0, 0,
                             nullptr, 0, nullptr,
                             1, &vkImageMemoryBarrier);
    }

    CommandBufferUtils::vulkanSubmitCommandBuffer(graphicsStructure.queue, graphicsStructure.commandBuffer,
                                                  std::vector<VkSemaphore>(), std::vector<VkSemaphore>(), nullptr,
                                                  graphicsStructure.bufferAvaibleFence);
}

#endif //VULKANBASE_VULKANHELPERS_H
