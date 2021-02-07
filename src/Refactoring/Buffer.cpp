#include "Buffer.h"
#include "../MemoryManagement/VMA.h"
#include "VulkanCreateFunctions.h"
#include "CreateInfoHelpers.h"

vk::Buffer::Buffer(VkDevice vkDevice, VkDeviceSize memorySize, const std::vector<uint32_t>& queues, VkBufferUsageFlags usage,
                   VkSharingMode sharingMode, VkBufferCreateFlags flags, VkMemoryPropertyFlags memoryPropertyFlags) {
    this->buffer = vk::createBuffer(vkDevice, vk::bufferCreateInfo(memorySize, queues,
                                                             usage, sharingMode, flags));
    this->size = memorySize;
    VkMemoryRequirements memoryRequirements = vk::getBufferMemoryRequirements(vkDevice, buffer);
    this->memory = VMA::getInstance().vmalloc(memoryRequirements, memoryPropertyFlags);
    vkBindBufferMemory(vkDevice, buffer, this->memory.vkDeviceMemory, this->memory.vkOffset);
}

vk::Buffer::Buffer() {
    buffer = VK_NULL_HANDLE;
    size = 0;
    memory = AllocationBlock();
}
