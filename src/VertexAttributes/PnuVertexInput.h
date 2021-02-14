#ifndef VULKAN_BASE_PNU_VERTEX_INPUT_H
#define VULKAN_BASE_PNU_VERTEX_INPUT_H

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <vector>

#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>
#include <glm/gtx/hash.hpp>

/* Position + Normal + UV Vertex */
struct PnuVertexInput {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 uv;
    glm::vec3 tangent{};

    PnuVertexInput(const glm::vec3 &position, const glm::vec3 &normal, const glm::vec2 &uv);

    PnuVertexInput();

    bool operator==(const PnuVertexInput &other) const;

    static std::vector<VkVertexInputBindingDescription> getInputBindingDescription();

    static std::vector<VkVertexInputAttributeDescription> getInputAttributeDescription();
};

namespace std
{
    template<> struct hash<PnuVertexInput>
    {
        size_t operator()(PnuVertexInput const& vertex) const {
            return ((hash<glm::vec3>()(vertex.position) ^
                    (hash<glm::vec2>()(vertex.uv) << 1)) >> 1) ^
                    (hash<glm::vec3>()(vertex.normal) << 1);
        }
    };
}

#endif //VULKAN_BASE_PNU_VERTEX_INPUT_H
