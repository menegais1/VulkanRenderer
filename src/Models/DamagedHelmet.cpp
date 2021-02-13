//
// Created by Lorenzo on 2/12/12021.
//

#include "DamagedHelmet.h"
#include "../FileManagers/FileLoader.h"


std::string DamagedHelmet::meshPath() const
{
    return FileLoader::getPath("Models/DamagedHelmet/DamagedHelmet.obj");
}

std::vector<std::string> DamagedHelmet::textures() const
{
    return
    {
        FileLoader::getPath("Models/DamagedHelmet/Textures/Material_MR_baseColor.jpeg"),
        FileLoader::getPath("Models/DamagedHelmet/Textures/Material_MR_normal.jpeg"),
        FileLoader::getPath("Models/DamagedHelmet/Textures/Material_MR_metallicRoughness.png"),
        FileLoader::getPath("Models/DamagedHelmet/Textures/Material_MR_emissive.jpeg")
    };
}

glm::vec3 DamagedHelmet::position() const
{
    return glm::vec3(0, 0, 0);
}

std::string DamagedHelmet::shader() const
{
    return "PBR";
}
