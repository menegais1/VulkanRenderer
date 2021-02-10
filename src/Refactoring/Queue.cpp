//
// Created by menegais on 07/02/2021.
//

#include "Queue.h"

vk::Queue::Queue(VkDevice vkDevice, uint32_t queueFamily, uint32_t queueIndex) : queueFamily(queueFamily), queueIndex(queueIndex) {
    vkGetDeviceQueue(vkDevice, queueFamily, queueIndex, &queue);
}

vk::Queue::Queue() {

}
