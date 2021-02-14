//
// Created by Lorenzo on 2/11/12021.
//

#include <iostream>
#include <filesystem>
#include "Texture.h"
#include "../../Dependencies/stb/stb_image.h"
#include "../Refactoring/VulkanCreateFunctions.h"
#include "../Refactoring/CreateInfoHelpers.h"
#include "../MemoryManagement/VMA.h"

Texture::Texture(const VkDevice& vkDevice, const std::string& path, vk::HostDeviceTransfer& hostDeviceTransfer,
                 const std::vector<uint32_t>& graphicsAndTransferQueues, const vk::CommandBuffer& commandBuffer)
{
    std::filesystem::path texturePath(path);
    std::cout << "Loading " << texturePath.filename().string() << std::endl;
    int width, height, textureChannels, desiredChannels = 4;
    unsigned char *result = stbi_load(path.c_str(), &width, &height, &textureChannels, desiredChannels);
    if (result == nullptr)
    {
        if (stbi_failure_reason())
        {
            std::cerr << stbi_failure_reason();
        }
        else
        {
            std::cerr << "Failed for unknown reasons";
        }
        exit(-1);
    }
    std::cout << texturePath.filename().string() << " loaded from disk. (" << width << "x" << height << "x" << textureChannels << ")" << std::endl;


    image = vk::createImage(vkDevice, vk::imageCreateInfo(0, VK_IMAGE_TYPE_2D, VK_FORMAT_R8G8B8A8_UNORM,
                                                          vk::extent3D(width, height, 1), 1, 1, VK_SAMPLE_COUNT_1_BIT,
                                                          VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
                                                          VK_SHARING_MODE_CONCURRENT, graphicsAndTransferQueues, VK_IMAGE_LAYOUT_UNDEFINED));


    VkMemoryRequirements requirements = vk::getImageMemoryRequirements(vkDevice, image);
    AllocationBlock memory = VMA::getInstance().vmalloc(requirements, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    vkBindImageMemory(vkDevice, image, memory.vkDeviceMemory, memory.vkOffset);
    imageView = vk::createImageView(vkDevice,
                                    vk::imageViewCreateInfo(image, VK_IMAGE_VIEW_TYPE_2D,
                                                            VK_FORMAT_R8G8B8A8_UNORM,
                                                                {VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY,
                                                                 VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY},
                                                                vk::imageSubresourceRange(VK_IMAGE_ASPECT_COLOR_BIT,
                                                                                          0, 1, 0, 1)));

    VkSamplerCreateInfo samplerCreateInfo = vk::samplerCreateInfo();
    samplerCreateInfo.unnormalizedCoordinates = VK_FALSE;
    samplerCreateInfo.compareEnable = VK_FALSE;
    samplerCreateInfo.anisotropyEnable = VK_FALSE;
    samplerCreateInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerCreateInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerCreateInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    sampler = vk::createSampler(vkDevice, samplerCreateInfo);

    hostDeviceTransfer.submitOneTimeTransferBuffer([this](VkCommandBuffer transferBuffer)
    {
        VkImageMemoryBarrier transferSrcMemoryBarrier = vk::imageMemoryBarrier(0, VK_ACCESS_TRANSFER_WRITE_BIT, VK_IMAGE_LAYOUT_UNDEFINED,
                                                                               VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_QUEUE_FAMILY_IGNORED, VK_QUEUE_FAMILY_IGNORED,
                                                                               image, vk::imageSubresourceRange(VK_IMAGE_ASPECT_COLOR_BIT, 0,
                                                                                                                1, 0, 1));
        vkCmdPipelineBarrier(transferBuffer, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0,
                         nullptr, 0, nullptr, 1, &transferSrcMemoryBarrier);

    });

    hostDeviceTransfer.transferImageFromBuffer(width, height, desiredChannels, sizeof(uint8_t), result, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

    VkPipelineStageFlags waitDstFlags = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    commandBuffer.submitOneTime({}, {}, &waitDstFlags, [this](VkCommandBuffer commandBuffer) {
        VkImageMemoryBarrier imageReadMemoryBarrier = vk::imageMemoryBarrier(VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_SHADER_READ_BIT, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                                                                             VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_QUEUE_FAMILY_IGNORED, VK_QUEUE_FAMILY_IGNORED,
                                                                             image, vk::imageSubresourceRange(VK_IMAGE_ASPECT_COLOR_BIT, 0,
                                                                                                              1, 0, 1));
        vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
                             0, nullptr,
                             0, nullptr, 1, &imageReadMemoryBarrier);
    });

    std::cout << texturePath.filename().string() << " loaded" << std::endl;
    delete result;

}

VkWriteDescriptorSet Texture::getWriteDescriptorSet(uint32_t dstBinding, const VkDescriptorSet& descriptorSet)
{
    imageInfo = vk::descriptorImageInfo(sampler, imageView, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
    return vk::writeDescriptorSet(&imageInfo, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, descriptorSet, dstBinding, 0);
}
