//
// Created by snorri on 18.10.2018.
//

#include "catch.hpp"
#include "BrickCollection.h"
#include "Ball.h"

TEST_CASE("BrickCollection") {
    SECTION("constructor") {
        BrickCollection bc;

        SECTION("newly constructed is empty") {
            REQUIRE(bc.GetBrickCount() == 0);
        }
    }

    SECTION("AddBrick") {
        BrickCollection bc;
        bc.AddBrick({0, 0}, 64, 32);
        REQUIRE(bc.GetBrickCount() == 1);
    }

    SECTION("CollideBallWithBricks") {
        BrickCollection bc;

        Ball b;
        b.SetRadius(1.0f);

        SECTION("empty") {
            b.SetPosition({0.0f, 1.0f});
            b.SetVelocity({0.0f, 1.0f});

            bc.CollideBallWithBricks(b);

            REQUIRE(b.GetVelocity().x == 0.0f);
            REQUIRE(b.GetVelocity().y == 1.0f);
        }
        
        SECTION("simple") {
            b.SetPosition({0.0f, 0.0f});
            b.SetVelocity({0.0f, 1.0f});

            bc.AddBrick({0.0f, 5.0f}, 10.0f, 10.0f);
            bc.CollideBallWithBricks(b);

            REQUIRE(bc.GetBrickCount() == 0);
            REQUIRE(b.GetVelocity().x == 0.0f);
            REQUIRE(b.GetVelocity().y == -1.0f);
        }
    }
}