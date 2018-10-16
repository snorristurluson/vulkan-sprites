//
// Created by snorri on 15.10.2018.
//

#include "catch.hpp"
#include "Paddle.h"


TEST_CASE("Paddle") {
    SECTION("constructor") {
        Paddle p;
    }

    SECTION("newly created has zero velocity") {
        Paddle p;
        REQUIRE(p.GetVelocity().x == 0.0f);
        REQUIRE(p.GetVelocity().y == 0.0f);
    }

    SECTION("SetPosition") {
        Paddle p;
        p.SetPosition({3.14f, 42.0f});

        glm::vec2 expected {3.14f, 42.0f};
        REQUIRE(p.GetPosition() == expected);
    }

    SECTION("SetWidth") {
        Paddle p;
        p.SetWidth(10.0f);

        REQUIRE(p.GetWidth() == 10.0f);
    }

    SECTION("Update") {
        Paddle p;
        p.SetPosition({0, 0});
        p.SetVelocity({10, 0});
        p.Update(1);

        glm::vec2 expected {10, 0};
        REQUIRE(p.GetPosition().x == expected.x);
        REQUIRE(p.GetPosition().y == expected.y);
    }

    SECTION("SetBounds") {
        SECTION("left") {
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

        SECTION("right") {
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
    }
}
