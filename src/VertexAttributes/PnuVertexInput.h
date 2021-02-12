#ifndef VULKAN_BASE_PNU_VERTEX_INPUT_H
#define VULKAN_BASE_PNU_VERTEX_INPUT_H
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <vector>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
/* Position + Normal + UV Vertex */
struct PnuVertexInput
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 uv;
    glm::vec3 tangent;

    PnuVertexInput(const glm::vec3 &position, const glm::vec3 &normal, const glm::vec2 &uv) : position(position), normal(normal), uv(uv) {}

    PnuVertexInput();

    static std::vector<VkVertexInputBindingDescription> getInputBindingDescription();
    static std::vector<VkVertexInputAttributeDescription> getInputAttributeDescription();
};

#endif //VULKAN_BASE_PNU_VERTEX_INPUT_H
