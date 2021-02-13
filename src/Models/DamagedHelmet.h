//
// Created by Lorenzo on 2/12/12021.
//

#ifndef VULKAN_BASE_DAMAGED_HELMET_H
#define VULKAN_BASE_DAMAGED_HELMET_H

#include "Model.h"

class DamagedHelmet : public Model
{
    [[nodiscard]] std::string meshPath() const override;
    [[nodiscard]] std::vector<std::string> textures() const override;
    [[nodiscard]] glm::vec3 position() const override;
    [[nodiscard]] std::string shader() const override;
};


#endif //VULKAN_BASE_DAMAGED_HELMET_H
