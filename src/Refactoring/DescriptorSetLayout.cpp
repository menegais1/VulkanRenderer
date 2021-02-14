//
// Created by menegais on 14/02/2021.
//

#include "DescriptorSetLayout.h"
#include "CreateInfoHelpers.h"
#include "VulkanCreateFunctions.h"

vk::DescriptorSetLayout &vk::DescriptorSetLayout::build() {
    std::vector<VkDescriptorSetLayoutBinding> bindingsVector;
    for (auto binding : bindings) {
        bindingsVector.push_back(binding.second);
    }
    layout = vk::createDescriptorSetLayout(vkDevice, vk::descriptorSetLayoutCreateInfo(bindingsVector));
    return *this;
}

vk::DescriptorSetLayout &vk::DescriptorSetLayout::addBinding(std::string name, VkDescriptorType descriptorType, uint32_t descriptorCount, VkShaderStageFlags stageFlags) {
    VkDescriptorSetLayoutBinding binding = vk::descriptorSetLayoutBinding(bindings.size(), descriptorType, descriptorCount, stageFlags);
    bindings.emplace(name, binding);
    return *this;
}

int min(int x, int y) {
    return x < y ? x : y;
}

int compare(std::string s1, std::string s2) {
    int i, j, l1, l2, t, track;
    int dist[50][50];
    l1 = s1.size();
    l2 = s2.size();
    for (i = 0; i <= l1; i++) {
        dist[0][i] = i;
    }
    for (j = 0; j <= l2; j++) {
        dist[j][0] = j;
    }
    for (j = 1; j <= l1; j++) {
        for (i = 1; i <= l2; i++) {
            if (s1[i - 1] == s2[j - 1]) {
                track = 0;
            } else {
                track = 1;
            }
            t = min((dist[i - 1][j] + 1), (dist[i][j - 1] + 1));
            dist[i][j] = min(t, (dist[i - 1][j - 1] + track));
        }
    }
    return dist[l2][l1];
}

VkDescriptorSetLayoutBinding vk::DescriptorSetLayout::getBinding(std::string name) {
    if (bindings.count(name) == 0) {
        int minDist = 1000;
        std::string key = "";
        for (auto binding : bindings) {
            int dist = compare(binding.first, name);
            if (dist < minDist) {
                key = binding.first;
                minDist = dist;
            }
        }
        std::cerr << "KEY (" << name << ") NOT FOUND IN LAYOUT, DID YOU MEAN (" << key << ")" << std::endl;
        exit(-1);
    }
    return bindings[name];
}

vk::DescriptorSetLayout::DescriptorSetLayout(VkDevice vkDevice) : vkDevice(vkDevice){

}
