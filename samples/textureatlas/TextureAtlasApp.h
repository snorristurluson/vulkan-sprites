//
// Created by snorri on 8.10.2018.
//

#ifndef VULKAN_SPRITES_TEXTUREATLASAPP_H
#define VULKAN_SPRITES_TEXTUREATLASAPP_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <string>

class TextureAtlasApp {
public:
    TextureAtlasApp(const std::string& root);

    void CreateWindow(int width, int height);
    GLFWwindow* GetWindow();

    void Run();

protected:
    GLFWwindow *m_window;
    int m_width;
    int m_height;
    std::string m_rootDirectory;
};


#endif //VULKAN_SPRITES_TEXTUREATLASAPP_H
