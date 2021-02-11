//
// Created by Lorenzo on 2/11/12021.
//

#ifndef VULKAN_BASE_DESCRIPTOR_MANAGER_H
#define VULKAN_BASE_DESCRIPTOR_MANAGER_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>

/* This class idea is pretty straightforward
 *
 * It is initialized with only a basic device
 *
 * The user defines all the resources it wants to use (textures and uniform supported for the moment) and the specific
 * attribute that makes that resource different from others:
 *      In the case of a texture it is easy: the texture path
 *      In the case of an uniform buffer, I guess the specific attribute would be the type of the uniform itself? Must check it after
 *
 * Then, after defining all the resources one wants to use it might commit the results creating the Descriptor Set
 * which might be then forwarded for the pipeline, for example.
 *
 * Todo: The steps described ahead are not implemented yet
 * Later, just before start the rendering loop, one might call create descriptors giving interfaces that know how to
 * create a texture and how to create a frame buffer.
 * */
class ResourcesManager
{
public:
    explicit ResourcesManager(VkDevice& device);
    void addTextureSamplerLayout(uint32_t sets);
    void addUniformBufferLayout(uint32_t sets);
    std::vector<VkDescriptorSetLayout> commitDescriptorBindings();
    VkDescriptorPool createDescriptorPools();
    void createDescriptorSets();
private:
    /* Input */
    VkDevice& device;

    /* Descriptor collections */
    std::vector<VkDescriptorSetLayoutBinding> descriptorSetLayoutBindings;
    std::vector<VkDescriptorSetLayout> descriptorSetLayouts;
    VkDescriptorPool descriptorPool{};

    /* Internal control*/
    uint32_t textureSamplerCount;
    uint32_t uniformBufferCount;

    /* Helper function */
    void createDescriptorLayout(VkDescriptorType descriptorType, VkShaderStageFlags stageFlags);

};


#endif //VULKAN_BASE_DESCRIPTOR_MANAGER_H
