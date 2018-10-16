//
// Created by snorri on 15.10.2018.
//

#include <gtest/gtest.h>
#include "Paddle.h"

TEST(Paddle, CanCreate) {
    Paddle p;
}

TEST(Paddle, NewlyCreatedHasZeroVelocity) {
    Paddle p;
    EXPECT_EQ(p.GetVelocity().x, 0.0f);
    EXPECT_EQ(p.GetVelocity().y, 0.0f);
}

TEST(Paddle, SetPosition) {
    Paddle p;
    p.SetPosition({3.14f, 42.0f});

    glm::vec2 expected {3.14f, 42.0f};
    EXPECT_EQ(p.GetPosition(), expected);
}

TEST(Paddle, SetWidth) {
    Paddle p;
    p.SetWidth(10.0f);

    EXPECT_EQ(p.GetWidth(), 10.0f);
}

TEST(Paddle, Update) {
    Paddle p;
    p.SetPosition({0, 0});
    p.SetVelocity({10, 0});
    p.Update(1);

    glm::vec2 expected {10, 0};
    EXPECT_EQ(p.GetPosition().x, expected.x);
    EXPECT_EQ(p.GetPosition().y, expected.y);
}

TEST(Paddle, Bounds_Left) {
    Paddle p;
    p.SetWidth(10.0f);
    p.SetBounds(0, 1000);
    p.SetPosition({0, 0});
    p.SetVelocity({-10, 0});
    p.Update(1);

    glm::vec2 expected {5.0f, 0};
    EXPECT_EQ(p.GetPosition().x, expected.x);
    EXPECT_EQ(p.GetPosition().y, expected.y);
}

TEST(Paddle, Bounds_Right) {
    Paddle p;
    p.SetWidth(10.0f);
    p.SetBounds(0, 1000);
    p.SetPosition({990, 0});
    p.SetVelocity({10, 0});
    p.Update(1);

    glm::vec2 expected {995.0f, 0};
    EXPECT_EQ(p.GetPosition().x, expected.x);
    EXPECT_EQ(p.GetPosition().y, expected.y);
}

