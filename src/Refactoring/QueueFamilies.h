//
// Created by menegais on 02/02/2021.
//

#ifndef VULKANBASE_QUEUEFAMILIES_H
#define VULKANBASE_QUEUEFAMILIES_H
struct QueueFamilies {
    uint32_t graphicsFamily = -1;
    uint32_t computeFamily = -1;
    uint32_t transferFamily = -1;
    uint32_t presentationFamily = -1;
};

#endif //VULKANBASE_QUEUEFAMILIES_H
