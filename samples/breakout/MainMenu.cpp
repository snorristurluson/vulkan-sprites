//
// Created by snorri on 12.10.2018.
//

#include <Logger.h>
#include "MainMenu.h"
#include "Renderer.h"
#include <Font.h>
#include "telemetry.h"

static auto logger = GetLogger("mainmenu");

MainMenu::MainMenu(App *a) : AppState(a) {}

void MainMenu::Init(Renderer &r) {
    m_titleFont = m_app->GetFont(48);
    m_itemFont = m_app->GetFont(32);
}

void MainMenu::Enter(Renderer &r) {
    tmFunction(0, 0);

    logger->debug(__PRETTY_FUNCTION__);
    m_frameCounter = 0;
}

void MainMenu::Render(Renderer &r) {
    tmFunction(0, 0);

    m_frameCounter++;

    m_titleFont->Draw(r, 32, 128, "Main menu");
    m_itemFont->Draw(r, 32, 256, "1) Play");
    m_itemFont->Draw(r, 32, 304, "2) Quit");
}

void MainMenu::Exit(Renderer &r) {
    tmFunction(0, 0);

    logger->debug(__PRETTY_FUNCTION__);
}

void MainMenu::HandleKey(int key, int scancode, int action, int mods) {
    tmFunction(0, 0);

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

void MainMenu::HandleCursorPosition(double xpos, double ypos) {

}
