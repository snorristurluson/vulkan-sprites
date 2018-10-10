//
// Created by snorri on 8.10.2018.
//

#ifndef VULKAN_SPRITES_TEXTUREATLASAPP_H
#define VULKAN_SPRITES_TEXTUREATLASAPP_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <string>
#include <memory>
#include <vector>
#include "../../engine/Renderer.h"

class TextureAtlas;
class ITexture;

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

    void LoadTextures(Renderer &renderer, std::shared_ptr<TextureAtlas> &ta,
                          std::vector<std::shared_ptr<ITexture>> &textures) const;
};


#endif //VULKAN_SPRITES_TEXTUREATLASAPP_H
