//
// Created by snorri on 15.10.2018.
//

#include "catch.hpp"
#include <glm/vec2.hpp>
#include "Ball.h"
#include "Paddle.h"

TEST_CASE("Ball constructor", "[Ball]") {
    Ball b;
}

TEST_CASE("Ball::SetRadius", "[Ball]") {
    Ball b;
    b.SetRadius(4.0f);
    REQUIRE(b.GetRadius() == 4.0f);
}

TEST_CASE("Ball::SetPosition", "[Ball]") {
    Ball b;

    b.SetPosition({3.14f, 42.0f});

    glm::vec2 expected {3.14f, 42.0f};
    REQUIRE(b.GetPosition() == expected);
}

TEST_CASE("Ball::CollideWithPaddle hits", "[Ball]") {
    Ball b;
    b.SetRadius(2.0f);
    b.SetPosition({0.0f, 1.0f});
    b.SetVelocity({0.0f, 1.0f});

    Paddle p;
    p.SetPosition({0.0f, 0.0f});
    p.SetWidth(10.0f);

    b.CollideWithPaddle(p);

    REQUIRE(b.GetVelocity().y == -1.0f);
}

TEST_CASE("Ball::CollideWithPaddle misses when ball is to the right", "[Ball]") {
    Ball b;
    b.SetRadius(2.0f);
    b.SetPosition({50.0f, 1.0f});
    b.SetVelocity({0.0f, 1.0f});

    Paddle p;
    p.SetPosition({0.0f, 0.0f});
    p.SetWidth(10.0f);

    b.CollideWithPaddle(p);

    REQUIRE(b.GetVelocity().y == 1.0f);
}

TEST_CASE("Ball::CollideWithPaddle misses when ball is to the left", "[Ball]") {
    Ball b;
    b.SetRadius(2.0f);
    b.SetPosition({-50.0f, 1.0f});
    b.SetVelocity({0.0f, 1.0f});

    Paddle p;
    p.SetPosition({0.0f, 0.0f});
    p.SetWidth(10.0f);

    b.CollideWithPaddle(p);

    REQUIRE(b.GetVelocity().y == 1.0f);
}

TEST_CASE("Ball::CollideWithPaddle misses when ball is above", "[Ball]") {
    Ball b;
    b.SetRadius(2.0f);
    b.SetPosition({0.0f, -10.0f});
    b.SetVelocity({0.0f, 1.0f});

    Paddle p;
    p.SetPosition({0.0f, 0.0f});
    p.SetWidth(10.0f);

    b.CollideWithPaddle(p);

    REQUIRE(b.GetVelocity().y == 1.0f);
}


TEST_CASE("Ball::CollideWithPaddle misses when ball is moving away", "[Ball]") {
    Ball b;
    b.SetRadius(2.0f);
    b.SetPosition({0.0f, 1.0f});
    b.SetVelocity({0.0f, -1.0f});

    Paddle p;
    p.SetPosition({0.0f, 0.0f});
    p.SetWidth(10.0f);

    b.CollideWithPaddle(p);

    REQUIRE(b.GetVelocity().y == -1.0f);
}
