//
// Created by snorri on 15.10.2018.
//

#include <gtest/gtest.h>
#include <glm/vec2.hpp>
#include "Ball.h"
#include "Paddle.h"

TEST(Ball, CanCreate) {
    Ball b;
}

TEST(Ball, SetRadius) {
    Ball b;
    b.SetRadius(4.0f);
    EXPECT_EQ(b.GetRadius(), 4.0f);
}

TEST(Ball, SetPosition) {
    Ball b;

    b.SetPosition({3.14f, 42.0f});

    glm::vec2 expected {3.14f, 42.0f};
    EXPECT_EQ(b.GetPosition(), expected);
}

TEST(Ball, CollideWithPaddle_Hit) {
    Ball b;
    b.SetRadius(2.0f);
    b.SetPosition({0.0f, 1.0f});
    b.SetVelocity({0.0f, 1.0f});

    Paddle p;
    p.SetPosition({0.0f, 0.0f});
    p.SetWidth(10.0f);

    b.CollideWithPaddle(p);

    EXPECT_EQ(b.GetVelocity().y, -1.0f);
}

TEST(Ball, CollideWithPaddle_Miss_Right) {
    Ball b;
    b.SetRadius(2.0f);
    b.SetPosition({50.0f, 1.0f});
    b.SetVelocity({0.0f, 1.0f});

    Paddle p;
    p.SetPosition({0.0f, 0.0f});
    p.SetWidth(10.0f);

    b.CollideWithPaddle(p);

    EXPECT_EQ(b.GetVelocity().y, 1.0f);
}

TEST(Ball, CollideWithPaddle_Miss_Left) {
    Ball b;
    b.SetRadius(2.0f);
    b.SetPosition({-50.0f, 1.0f});
    b.SetVelocity({0.0f, 1.0f});

    Paddle p;
    p.SetPosition({0.0f, 0.0f});
    p.SetWidth(10.0f);

    b.CollideWithPaddle(p);

    EXPECT_EQ(b.GetVelocity().y, 1.0f);
}

TEST(Ball, CollideWithPaddle_Miss_Above) {
    Ball b;
    b.SetRadius(2.0f);
    b.SetPosition({0.0f, -10.0f});
    b.SetVelocity({0.0f, 1.0f});

    Paddle p;
    p.SetPosition({0.0f, 0.0f});
    p.SetWidth(10.0f);

    b.CollideWithPaddle(p);

    EXPECT_EQ(b.GetVelocity().y, 1.0f);
}

TEST(Ball, CollideWithPaddle_Miss_MovingAway) {
    Ball b;
    b.SetRadius(2.0f);
    b.SetPosition({0.0f, 1.0f});
    b.SetVelocity({0.0f, -1.0f});

    Paddle p;
    p.SetPosition({0.0f, 0.0f});
    p.SetWidth(10.0f);

    b.CollideWithPaddle(p);

    EXPECT_EQ(b.GetVelocity().y, -1.0f);
}
