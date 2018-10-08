//
// Created by snorri on 8.10.2018.
//

#ifndef VULKAN_SPRITES_TEXTUREATLASAPP_H
#define VULKAN_SPRITES_TEXTUREATLASAPP_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class TextureAtlasApp {
public:
    TextureAtlasApp();

    void CreateWindow(int width, int height);
    GLFWwindow* GetWindow();

    void Run();

    static void framebufferResizeCallback(GLFWwindow *window, int width, int height);

    GLFWwindow *m_window;
    int m_width;
    int m_height;
};


#endif //VULKAN_SPRITES_TEXTUREATLASAPP_H
