//
// Created by Snorri Sturluson on 20/08/2018.
//

#ifndef VULKAN_SPRITES_APP_H
#define VULKAN_SPRITES_APP_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class App
{
public:
    App();

    void CreateWindow(int width, int height);
    GLFWwindow* GetWindow();

    void Run();

    static void framebufferResizeCallback(GLFWwindow *window, int width, int height);


protected:
    GLFWwindow *m_window;
    bool m_framebufferResized;
};


#endif //VULKAN_SPRITES_APP_H
