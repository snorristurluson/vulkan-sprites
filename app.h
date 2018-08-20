//
// Created by Snorri Sturluson on 20/08/2018.
//

#ifndef VULKAN_SPRITES_APP_H
#define VULKAN_SPRITES_APP_H

#include <GLFW/glfw3.h>

class App
{
public:
    App();

    void CreateWindow(int width, int height);

    void Run();

    static void framebufferResizeCallback(GLFWwindow *window, int width, int height);


protected:
    GLFWwindow *m_window;
    bool m_framebufferResized;
};


#endif //VULKAN_SPRITES_APP_H
