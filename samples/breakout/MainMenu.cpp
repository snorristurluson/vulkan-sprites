//
// Created by snorri on 12.10.2018.
//

#include <Logger.h>
#include "MainMenu.h"
#include "Renderer.h"
#include <Font.h>

static auto logger = GetLogger("mainmenu");

MainMenu::MainMenu(App *a) : AppState(a) {}

void MainMenu::Init(Renderer &r) {
    m_titleFont = m_app->GetFont("/usr/share/fonts/truetype/freefont/FreeMonoBoldOblique.ttf", 96);
    m_itemFont = m_app->GetFont("/usr/share/fonts/truetype/freefont/FreeMonoBoldOblique.ttf", 48);
}

void MainMenu::Enter(Renderer &r) {
    logger->debug(__PRETTY_FUNCTION__);
    m_frameCounter = 0;
}

void MainMenu::Render(Renderer &r) {
    m_frameCounter++;

    m_titleFont->Draw(r, 32, 128, "Main menu");
    m_itemFont->Draw(r, 32, 256, "1) Play");
    m_itemFont->Draw(r, 32, 304, "2) Quit");
}

void MainMenu::Exit(Renderer &r) {
    logger->debug(__PRETTY_FUNCTION__);
}

void MainMenu::HandleKey(int key, int scancode, int action, int mods) {
    if(action == GLFW_PRESS) {
        switch(key) {
            case GLFW_KEY_1:
                m_app->ChangeState(GAMEPLAY);
                break;
            case GLFW_KEY_2:
                m_app->ChangeState(SPLASH_SCREEN);
                break;
        }
    }
}
