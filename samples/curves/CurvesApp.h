//
// Created by snorri on 26.10.2018.
//

#ifndef VULKAN_SPRITES_CURVESAPP_H
#define VULKAN_SPRITES_CURVESAPP_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class CurvesApp {
public:
    CurvesApp();

    void CreateWindow(int width, int height);

    void Run();

protected:
    GLFWwindow *m_window;
    int m_width;
    int m_height;
};

#endif //VULKAN_SPRITES_CURVESAPP_H
