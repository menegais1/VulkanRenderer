//
// Created by Lorenzo on 2/12/12021.
//

#ifndef VULKANBASE_MODEL_H
#define VULKANBASE_MODEL_H
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>
#include <string>
#include "../VertexAttributes/PnuVertexInput.h"
#include "../Refactoring/HostDeviceTransfer.h"
#include "../Refactoring/CommandBuffer.h"
#include "../Textures/Texture.h"

struct Model
{
public:
    [[nodiscard]] virtual std::string meshPath() const = 0;
    [[nodiscard]] virtual std::vector<std::string> textures() const = 0;
    [[nodiscard]] virtual glm::vec3 position() const = 0;
    [[nodiscard]] virtual std::string shader() const = 0;


};

#endif //VULKANBASE_MODEL_H
