//
// Created by Lorenzo on 2/12/12021.
//

#ifndef VULKANBASE_HUMAN_H
#define VULKANBASE_HUMAN_H


#include "Model.h"

class Human : public Model
{
    [[nodiscard]] std::string meshPath() const override;
    [[nodiscard]] std::vector<std::string> textures() const override;
    [[nodiscard]] glm::vec3 position() const override;
    std::string shader() const override;
};


#endif //VULKANBASE_HUMAN_H
