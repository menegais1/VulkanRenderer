//
// Created by Lorenzo on 2/12/12021.
//

#include <vector>
#include "Human.h"

#include "../FileManagers/FileLoader.h"

std::string Human::meshPath() const
{
    return FileLoader::getPath("Models/Human/Head/Head.obj");
}

std::vector<std::string> Human::textures() const
{
    return
    {
        FileLoader::getPath("Models/Human/Head/Textures/Colour_8k.jpg"),
        FileLoader::getPath("Models/Human/Head/Textures/NormalMap_SubDivision_1.jpg"),
        FileLoader::getPath("Models/Human/Head/Textures/Spec_8k.jpg"),
        FileLoader::getPath("Models/Human/Head/Textures/Gloss_8k.jpg"),
        FileLoader::getPath("Models/Human/Head/Textures/MicroNormal.jpg"),
    };
}

glm::vec3 Human::position() const
{
    return glm::vec3(0, -0.2f, 0);
}

std::string Human::shader() const
{
    return "HumanSkin";
}
