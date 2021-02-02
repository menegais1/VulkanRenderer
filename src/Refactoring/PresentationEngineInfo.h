//
// Created by menegais on 02/02/2021.
//

#ifndef VULKANBASE_PRESENTATIONENGINEINFO_H
#define VULKANBASE_PRESENTATIONENGINEINFO_H

struct PresentationEngineInfo {
    unsigned int imageCount;
    VkSurfaceFormatKHR format;
    VkExtent2D extents;
    VkPresentModeKHR presentMode;
};

#endif //VULKANBASE_PRESENTATIONENGINEINFO_H
