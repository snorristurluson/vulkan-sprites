//
// Created by Snorri Sturluson on 20/08/2018.
//

#ifndef VULKAN_SPRITES_APP_H
#define VULKAN_SPRITES_APP_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <Renderer.h>

class BlendModesApp
{
public:
    BlendModesApp();

    void CreateWindow(int width, int height);
    GLFWwindow* GetWindow();

    void Run();

protected:
    GLFWwindow *m_window;

    void DrawOverlappingSprites(Renderer &r, glm::vec2 offset) const;
};


#endif //VULKAN_SPRITES_APP_H
