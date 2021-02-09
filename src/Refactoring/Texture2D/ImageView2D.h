//
// Created by menegais on 07/02/2021.
//

#ifndef VULKANBASE_IMAGEVIEW2D_H
#define VULKANBASE_IMAGEVIEW2D_H


#include "Image2D.h"

namespace vk {
    class ImageView2D {
    public:
        Image2D image;
        VkImageView imageView;
        ImageView2D(VkDevice vkDevice, Image2D image, VkImageAspectFlags imageAspectFlags);
    };
}

#endif //VULKANBASE_IMAGEVIEW2D_H
