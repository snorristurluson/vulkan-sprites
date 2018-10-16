//
// Created by snorri on 15.10.2018.
//

#include "catch.hpp"
#include "Paddle.h"

TEST_CASE("Paddle constructor", "[Paddle]") {
    Paddle p;
}

TEST_CASE("Paddle Newly created has zero velocity", "[Paddle]") {
    Paddle p;
    REQUIRE(p.GetVelocity().x == 0.0f);
    REQUIRE(p.GetVelocity().y == 0.0f);
}

TEST_CASE("Paddle::SetPosition", "[Paddle]") {
    Paddle p;
    p.SetPosition({3.14f, 42.0f});

    glm::vec2 expected {3.14f, 42.0f};
    REQUIRE(p.GetPosition() == expected);
}

TEST_CASE("Paddle::SetWidth", "[Paddle]") {
    Paddle p;
    p.SetWidth(10.0f);

    REQUIRE(p.GetWidth() == 10.0f);
}

TEST_CASE("Paddle::Update", "[Paddle]") {
    Paddle p;
    p.SetPosition({0, 0});
    p.SetVelocity({10, 0});
    p.Update(1);

    glm::vec2 expected {10, 0};
    REQUIRE(p.GetPosition().x == expected.x);
    REQUIRE(p.GetPosition().y == expected.y);
}

TEST_CASE("Paddle::SetBounds Left", "[Paddle]") {
    Paddle p;
    p.SetWidth(10.0f);
    p.SetBounds(0, 1000);
    p.SetPosition({0, 0});
    p.SetVelocity({-10, 0});
    p.Update(1);

    glm::vec2 expected {5.0f, 0};
    REQUIRE(p.GetPosition().x == expected.x);
    REQUIRE(p.GetPosition().y == expected.y);
}

TEST_CASE("Paddle::SetBounds Right", "[Paddle]") {
    Paddle p;
    p.SetWidth(10.0f);
    p.SetBounds(0, 1000);
    p.SetPosition({990, 0});
    p.SetVelocity({10, 0});
    p.Update(1);

    glm::vec2 expected {995.0f, 0};
    REQUIRE(p.GetPosition().x == expected.x);
    REQUIRE(p.GetPosition().y == expected.y);
}

