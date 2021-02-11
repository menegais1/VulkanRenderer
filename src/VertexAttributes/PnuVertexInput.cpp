//
// Created by Lorenzo on 2/11/12021.
//
#include "PnuVertexInput.h"
#include "../Refactoring/CreateInfoHelpers.h"

std::vector<VkVertexInputBindingDescription> PnuVertexInput::getInputBindingDescription()
{
    return std::vector<VkVertexInputBindingDescription>
    {
        vk::vertexInputBindingDescription(0, sizeof(PnuVertexInput), VK_VERTEX_INPUT_RATE_VERTEX)
    };
}

std::vector<VkVertexInputAttributeDescription> PnuVertexInput::getInputAttributeDescription()
{
    return std::vector<VkVertexInputAttributeDescription>
    {
        vk::vertexInputAttributeDescription(0, VK_FORMAT_R32G32B32_SFLOAT, 0, offsetof(PnuVertexInput, position)),
        vk::vertexInputAttributeDescription(0, VK_FORMAT_R32G32B32_SFLOAT, 1, offsetof(PnuVertexInput, normal)),
        vk::vertexInputAttributeDescription(0, VK_FORMAT_R32G32_SFLOAT, 2, offsetof(PnuVertexInput, uv))
    };
}