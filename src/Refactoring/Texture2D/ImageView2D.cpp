//
// Created by menegais on 07/02/2021.
//

#include "ImageView2D.h"
#include "../VulkanCreateFunctions.h"
#include "../CreateInfoHelpers.h"

vk::ImageView2D::ImageView2D(VkDevice vkDevice, Image2D image, VkImageAspectFlags imageAspectFlags) : image(image) {
    imageView = vk::createImageView(vkDevice, vk::imageViewCreateInfo(image.image, VK_IMAGE_VIEW_TYPE_2D,
                                                                      image.format, {VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY,
                                                                                     VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY},
                                                                      vk::imageSubresourceRange(imageAspectFlags, 0, 1,
                                                                                                0, 1)));
}
