//
// Created by snorri on 15.10.2018.
//

#include "catch.hpp"
#include <glm/vec2.hpp>
#include "Ball.h"
#include "Paddle.h"

TEST_CASE("Ball") {
    SECTION("constructor") {
        Ball b;
    }
    SECTION("SetRadius") {
        Ball b;
        b.SetRadius(4.0f);
        REQUIRE(b.GetRadius() == 4.0f);
    }

    SECTION("SetPosition") {
        Ball b;

        b.SetPosition({3.14f, 42.0f});

        glm::vec2 expected {3.14f, 42.0f};
        REQUIRE(b.GetPosition() == expected);
    }

    SECTION("CollideWithPaddle") {
        SECTION("hits") {
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
        SECTION("misses when ball is to the right") {
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

        SECTION("misses when ball is to the left") {
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

        SECTION("misses when ball is above") {
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


        SECTION("misses when ball is moving away") {
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
    }

}


