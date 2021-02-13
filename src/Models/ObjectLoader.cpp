//
// Created by Lorenzo on 2/11/12021.
//

#include <tiny_obj_loader.h>
#include <iostream>
#include "ObjectLoader.h"

void ObjectLoader::loadPnuModel(const std::string &inputFile, std::vector<PnuVertexInput> &vertexInputs,
                                std::vector<uint32_t> &indexVector) {
    tinyobj::ObjReader reader;
    if (!reader.ParseFromFile(inputFile)) {
        if (!reader.Error().empty()) {
            std::cerr << "TinyObjReader: " << reader.Error();
        }
        exit(1);
    }

    if (!reader.Warning().empty()) {
        std::cout << "TinyObjReader: " << reader.Warning();
    }

    auto &attrib = reader.GetAttrib();
    auto &shapes = reader.GetShapes();
    auto &primaryMesh = shapes[0]; /* Fetch the first shape */
    std::cout << "Loading model: " << primaryMesh.name << std::endl;
    vertexInputs.resize((int) (attrib.vertices.size()));
    for (auto &index : primaryMesh.mesh.indices) {
        glm::vec3 pos = {attrib.vertices[3 * index.vertex_index + 0],
                         attrib.vertices[3 * index.vertex_index + 1],
                         attrib.vertices[3 * index.vertex_index + 2]};

        glm::vec2 texCoord = {attrib.texcoords[2 * index.texcoord_index + 0],
                              1.0 - attrib.texcoords[2 * index.texcoord_index + 1]};

        glm::vec3 normal = {attrib.normals[3 * index.normal_index + 0],
                            attrib.normals[3 * index.normal_index + 1],
                            attrib.normals[3 * index.normal_index + 2]};
        vertexInputs[index.vertex_index] = PnuVertexInput(pos, normal, texCoord);
        indexVector.push_back(index.vertex_index);
    }
    std::cout << primaryMesh.name << " loaded!" << std::endl;
}
