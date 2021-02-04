//
// Created by Lorenzo on 2/3/12021.
//
#ifndef AUTO_SHADERS
#define AUTO_SHADERS
#include <string>
#include <iostream>
#include <filesystem>
#include <unordered_set>

namespace AutoShaders
{
    static void compile()
    {
        std::unordered_set<std::string> endings = {".vert", ".tesc", ".tese", ".frag"};

        std::string srcDir = "../src/Shaders/";
        std::string dstDir = "../src/Shaders/";
        std::cout << "Compiling shaders..." << std::endl;

        for (const auto &entry : std::filesystem::directory_iterator(srcDir))
        {
            if (!entry.is_regular_file())
            {
                continue;
            }
            if (endings.count(entry.path().extension().string()) > 0)
            {
#ifdef WIN32
                auto cmd = std::string("%VK_SDK_PATH%/Bin/glslc.exe ");
#else
                auto cmd = std::string("$VULKAN_SDK/bin/glslc ");
#endif
                auto srcShader = entry.path().generic_string();
                auto dstShader = dstDir + entry.path().filename().generic_string() + ".spv";
                cmd.append(entry.path().generic_string() + " -o " + dstShader);
                std::cout << cmd << std::endl;
                system(cmd.c_str());
            }
        }
    }

}
#endif //AUTO_SHADERS
