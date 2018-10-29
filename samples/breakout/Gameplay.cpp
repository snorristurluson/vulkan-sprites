//
// Created by snorri on 14.10.2018.
//

#include <Logger.h>
#include <GLFW/glfw3.h>
#include "Font.h"
#include "Gameplay.h"
#include "telemetry.h"
#include "Texture.h"

static auto logger = GetLogger("gameplay");


Gameplay::Gameplay(App *a) : AppState(a) {

}

void Gameplay::Init(Renderer &r) {
    m_titleFont = m_app->GetFont(48);

    m_paddleSpeed = 500.0f;
    m_fieldWidth = 1200.0f;
    m_fieldHeight = 800.0f;

    m_paddle.SetPosition({m_fieldWidth/2.0f, m_fieldHeight - 40});
    m_paddle.SetVelocity({0, 0});
    m_paddle.SetWidth(100);
    m_paddle.SetHeight(10);
    m_paddle.SetBounds(0.0f, m_fieldWidth);
    
    m_ball.SetPosition({m_fieldWidth/2.0f, m_fieldHeight/2.0f});
    m_ball.SetVelocity({500, 500});
    m_ball.SetRadius(10);
    m_ball.SetBounds(0.0f, 0.0f, m_fieldWidth, m_fieldHeight);

    auto tex = r.CreateTexture("resources/ps1.png");
    m_ball.SetTextureForParticles(tex);


    for(int y = 0; y < 5; ++y) {
        for(int x = 0; x < 24; ++x) {
            m_bricks.AddBrick({x*50 + 23, y*30 + 100}, 40, 20);
        }
    }

    m_leftPressed = false;
    m_rightPressed = false;
}

void Gameplay::Enter(Renderer &r) {
    tmFunction(0, 0);

    logger->debug(__PRETTY_FUNCTION__);
    m_frameCounter = 0;

    glfwSetInputMode(m_app->GetWindow(), GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
}

void Gameplay::Render(Renderer &r) {
    tmFunction(0, 0);

    m_frameCounter++;


    float v = 0.0f;
    if(m_leftPressed) {
        v = -m_paddleSpeed;
    } else if(m_rightPressed) {
        v = m_paddleSpeed;
    }
    m_paddle.SetVelocity({v, 0});

    r.SetTexture(nullptr);
    r.SetColor({0.3f, 0.3f, 0.3f, 1.0f});
    r.DrawSprite(0, 0, m_fieldWidth, m_fieldHeight);

    auto td = 1 / 60.0f;
    m_paddle.Update(td);
    m_ball.Update(td);

    m_ball.CollideWithPaddle(m_paddle);
    m_bricks.CollideBallWithBricks(m_ball);

    m_bricks.Render(r);
    m_paddle.Render(r);
    m_ball.Render(r);

}

void Gameplay::Exit(Renderer &r) {
    tmFunction(0, 0);

    logger->debug(__PRETTY_FUNCTION__);
    glfwSetInputMode(m_app->GetWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void Gameplay::HandleKey(int key, int scancode, int action, int mods) {
    tmFunction(0, 0);

    switch(action) {
        case GLFW_PRESS:
            switch(key) {
                case GLFW_KEY_ESCAPE:
                    m_app->ChangeState(GAME_OVER);
                    break;

                case GLFW_KEY_Z:
                    m_leftPressed = true;
                    break;

                case GLFW_KEY_X:
                    m_rightPressed = true;
                    break;
                default:break;
            }
            break;

        case GLFW_RELEASE:
            switch(key) {
                case GLFW_KEY_Z:
                    m_leftPressed = false;
                    break;

                case GLFW_KEY_X:
                    m_rightPressed = false;
                    break;
                default:break;
            }
            break;
        default:break;
    }
}

void Gameplay::HandleCursorPosition(double xpos, double ypos) {
    m_paddle.SetPosition({xpos, m_paddle.GetPosition().y});
}
