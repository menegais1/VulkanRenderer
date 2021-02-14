//
// Created by Lorenzo on 2/11/12021.
//
#include "PnuVertexInput.h"
#include "../Refactoring/CreateInfoHelpers.h"

std::vector<VkVertexInputBindingDescription> PnuVertexInput::getInputBindingDescription() {
    return std::vector<VkVertexInputBindingDescription>
            {
                    vk::vertexInputBindingDescription(0, sizeof(PnuVertexInput), VK_VERTEX_INPUT_RATE_VERTEX)
            };
}

std::vector<VkVertexInputAttributeDescription> PnuVertexInput::getInputAttributeDescription() {
    return std::vector<VkVertexInputAttributeDescription>
            {
                    vk::vertexInputAttributeDescription(0, VK_FORMAT_R32G32B32_SFLOAT, 0, offsetof(PnuVertexInput, position)),
                    vk::vertexInputAttributeDescription(0, VK_FORMAT_R32G32B32_SFLOAT, 1, offsetof(PnuVertexInput, normal)),
                    vk::vertexInputAttributeDescription(0, VK_FORMAT_R32G32_SFLOAT, 2, offsetof(PnuVertexInput, uv)),
                    vk::vertexInputAttributeDescription(0, VK_FORMAT_R32G32B32_SFLOAT, 3, offsetof(PnuVertexInput, tangent))
            };
}

bool PnuVertexInput::operator==(const PnuVertexInput &other) const
{
    return position == other.position && uv == other.uv && normal == other.normal;
}

PnuVertexInput::PnuVertexInput() : position(glm::vec3(0, 0, 0)), normal(glm::vec3(0, 0, 0)), uv(glm::vec3(0, 0, 0)), tangent(glm::vec3(0, 0, 0)) {}

PnuVertexInput::PnuVertexInput(const glm::vec3 &position, const glm::vec3 &normal, const glm::vec2 &uv) : position(position), normal(normal), uv(uv), tangent(glm::vec3(0, 0, 0)) {}
