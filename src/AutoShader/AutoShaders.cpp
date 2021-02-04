//
// Created by Lorenzo on 2/3/12021.
//
#ifndef AUTO_SHADER
#define AUTO_SHADER
#include <string>
#include <iostream>
#include <filesystem>
#include <unordered_set>
class AutoShaders
{
public:
    static void compile()
    {
        std::unordered_set<std::string> endings = {".vert", ".tesc", ".tese", ".frag"};

        std::string srcDir = "../Shaders/";
        std::string dstDir = "../Shaders/";
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
                auto cmd = glslcWindows();
#else
                auto cmd = glslcLinux();
#endif
                auto srcShader = entry.path().generic_string();
                auto dstShader = dstDir + entry.path().filename().generic_string() + ".spv";
                cmd.append(entry.path().generic_string() + " -o " + dstShader);
                std::cout << cmd << std::endl;
                system(cmd.c_str());
            }
        }
    }

    static std::string glslcWindows()
    {
        return "%VK_SDK_PATH%/Bin/glslc.exe ";
    }

    static std::string glslcLinux()
    {
        return "$VULKAN_SDK/bin/glslc ";
    }

};
#endif //AUTO_SHADER
