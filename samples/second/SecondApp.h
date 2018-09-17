//
// Created by snorri on 17.9.2018.
//

#ifndef VULKAN_SPRITES_APP_H
#define VULKAN_SPRITES_APP_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class SecondApp
{
public:
    SecondApp();

    void CreateWindow(int width, int height);
    GLFWwindow* GetWindow();

    void Run();

    static void framebufferResizeCallback(GLFWwindow *window, int width, int height);


    void HandleKey(GLFWwindow *window, int key, int scancode, int action, int mods);

protected:
    GLFWwindow *m_window;
    bool m_framebufferResized;

    int m_width;
    int m_height;

    float m_x;
    float m_y;

    float m_dx;
    float m_vx;
};

#endif //VULKAN_SPRITES_APP_H
