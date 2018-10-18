//
// Created by snorri on 15.10.2018.
//

#include "catch.hpp"
#include <glm/vec2.hpp>
#include "Ball.h"
#include "Paddle.h"
#include "BrickCollection.h"

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
            b.SetPosition({0.0f, 1.5f});
            b.SetVelocity({0.0f, -1.0f});

            Paddle p;
            p.SetPosition({0.0f, 0.0f});
            p.SetWidth(10.0f);

            b.CollideWithPaddle(p);

            REQUIRE(b.GetVelocity().y == 1.0f);
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
            b.SetVelocity({0.0f, 1.0f});

            Paddle p;
            p.SetPosition({0.0f, 0.0f});
            p.SetWidth(10.0f);

            b.CollideWithPaddle(p);

            REQUIRE(b.GetVelocity().y == 1.0f);
        }
    }

    SECTION("CollideWithBrick") {
        Ball b;
        b.SetRadius(1.0f);

        SECTION("no hit") {
            b.SetPosition({0.0f, 50.0f});
            b.SetVelocity({0.0f, -1.0f});

            Brick brick {{0.0f, 0.0f}, 10.0f, 10.0f};

            auto didCollide = b.CollideWithBrick(brick);

            REQUIRE(!didCollide);
            REQUIRE(b.GetVelocity().x == 0.0f);
            REQUIRE(b.GetVelocity().y == -1.0f);
        }
        SECTION("hits from above") {
            b.SetPosition({0.0f, 5.0f});
            b.SetVelocity({0.0f, -1.0f});

            Brick brick {{0.0f, 0.0f}, 10.0f, 10.0f};

            auto didCollide = b.CollideWithBrick(brick);

            REQUIRE(didCollide);
            REQUIRE(b.GetVelocity().x == 0.0f);
            REQUIRE(b.GetVelocity().y == 1.0f);
        }
        SECTION("hits from below") {
            b.SetPosition({0.0f, 0.0f});
            b.SetVelocity({0.0f, 1.0f});

            Brick brick {{0.0f, 5.0f}, 10.0f, 10.0f};

            auto didCollide = b.CollideWithBrick(brick);

            REQUIRE(didCollide);
            REQUIRE(b.GetVelocity().x == 0.0f);
            REQUIRE(b.GetVelocity().y == -1.0f);
        }
        SECTION("hits from the left") {
            b.SetPosition({0.0f, 0.0f});
            b.SetVelocity({1.0f, 0.0f});

            Brick brick {{5.0f, 0.0f}, 10.0f, 10.0f};

            auto didCollide = b.CollideWithBrick(brick);

            REQUIRE(didCollide);
            REQUIRE(b.GetVelocity().x == -1.0f);
            REQUIRE(b.GetVelocity().y == 0.0f);
        }
        SECTION("hits from the right") {
            b.SetPosition({5.0f, 0.0f});
            b.SetVelocity({-1.0f, 0.0f});

            Brick brick {{0.0f, 0.0f}, 10.0f, 10.0f};

            auto didCollide = b.CollideWithBrick(brick);

            REQUIRE(didCollide);
            REQUIRE(b.GetVelocity().x == 1.0f);
            REQUIRE(b.GetVelocity().y == 0.0f);
        }
    }

    SECTION("SetBounds") {
        Ball b;
        b.SetRadius(1.0f);
        b.SetBounds(0.0f, 0.0f, 100.0f, 100.0f);

        SECTION("inside bounds") {
            b.SetPosition({50.0f, 50.0f});
            b.SetVelocity({1.0f, 1.0f});
            b.Update(1.0f);

            glm::vec2 expected {51.0f, 51.0f};
            REQUIRE(b.GetPosition() == expected);
        }

        SECTION("minx") {
            b.SetPosition({0.5f, 50.0f});
            b.SetVelocity({-1.0f, 0.0f});
            b.Update(1.0f);

            REQUIRE(b.GetPosition().x == 0.0f);
            REQUIRE(b.GetPosition().y == 50.0f);
            REQUIRE(b.GetVelocity().x == 1.0f);
            REQUIRE(b.GetVelocity().y == 0.0f);
        }

        SECTION("maxx") {
            b.SetPosition({99.5f, 50.0f});
            b.SetVelocity({1.0f, 0.0f});
            b.Update(1.0f);

            REQUIRE(b.GetPosition().x == 100.0f);
            REQUIRE(b.GetPosition().y == 50.0f);
            REQUIRE(b.GetVelocity().x == -1.0f);
            REQUIRE(b.GetVelocity().y == 0.0f);
        }

        SECTION("miny") {
            b.SetPosition({50.0f, 0.5f});
            b.SetVelocity({0.0f, -1.0f});
            b.Update(1.0f);

            REQUIRE(b.GetPosition().x == 50.0f);
            REQUIRE(b.GetPosition().y == 0.0f);
            REQUIRE(b.GetVelocity().x == 0.0f);
            REQUIRE(b.GetVelocity().y == 1.0f);
        }

        SECTION("maxy") {
            b.SetPosition({50.0f, 99.5f});
            b.SetVelocity({0.0f, 1.0f});
            b.Update(1.0f);

            REQUIRE(b.GetPosition().x == 50.0f);
            REQUIRE(b.GetPosition().y == 100.0f);
            REQUIRE(b.GetVelocity().x == 0.0f);
            REQUIRE(b.GetVelocity().y == -1.0f);
        }
    }

}


