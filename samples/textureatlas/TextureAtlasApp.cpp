//
// Created by snorri on 8.10.2018.
//

#include "TextureAtlasApp.h"
#include "../../engine/Renderer.h"
#include "../../engine/TextureAtlas.h"
#include "../../engine/Logger.h"

#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;

static auto logger = GetLogger("main");

TextureAtlasApp::TextureAtlasApp(const std::string& root) : m_rootDirectory(root) {
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
}

void TextureAtlasApp::CreateWindow(int width, int height) {
    m_window = glfwCreateWindow(width, height, "TextureAtlas", nullptr, nullptr);

    glfwGetFramebufferSize(m_window, &m_width, &m_height);

    m_width *= 2;
    m_height *= 2;

    glfwSetWindowUserPointer(m_window, this);
}

bool endsWith(const std::string& s, const std::string& suffix)
{
    return s.rfind(suffix) == (s.size()-suffix.size());
}

void TextureAtlasApp::Run() {
    Renderer r;
    r.Initialize(m_window, Renderer::ENABLE_VALIDATION);

    auto ta = r.CreateTextureAtlas(2048, 2048);

    std::vector<std::shared_ptr<AtlasTexture>> textures;
    for(auto& p: fs::directory_iterator(m_rootDirectory)) {
        std::string s = p.path();

        if(endsWith(s, ".png")) {
            auto t = ta->Add(s);
            if(t) {
                logger->debug("Added {} to atlas", s);
                textures.emplace_back(t);
            } else {
                logger->debug("{} didn't fit", s);
            }
        } else {
            logger->debug("{} is not a texture", s);
        }
    }

    r.SetClearColor({0.0f, 0.0f, 1.0f, 1.0f});
    while (!glfwWindowShouldClose(m_window)) {
        if (r.StartFrame()) {
            r.SetTexture(ta);
            r.DrawSprite(0, 0, 1024, 1024);
            r.EndFrame();
        }

        glfwPollEvents();
    }
    r.WaitUntilDeviceIdle();
}
