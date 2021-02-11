//
// Created by Lorenzo on 2/11/12021.
//

#ifndef VULKANBASE_OBJECTLOADER_H
#define VULKANBASE_OBJECTLOADER_H


#include "../VertexAttributes/PnuVertexInput.h"

class ObjectLoader
{
public:
    static void loadPnuModel(const std::string &inputFile, std::vector<PnuVertexInput> &vertexInputs, std::vector<uint32_t> &indexVector);
};


#endif //VULKANBASE_OBJECTLOADER_H
