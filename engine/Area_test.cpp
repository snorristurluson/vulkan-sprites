//
// Created by Snorri Sturluson on 24/09/2018.
//

#include "catch.hpp"
#include "Area.h"

TEST_CASE("Area") {
    SECTION("IsAdjacent") {
        SECTION("true when vertical") {
            Area area1 {0, 0, 32, 32};
            Area area2 {0, 32, 32, 32};

            REQUIRE(area1.IsAdjacent(area2));
            REQUIRE(area2.IsAdjacent(area1));
        }

        SECTION("true when horizontal") {
            Area area1 {0, 0, 32, 32};
            Area area2 {32, 0, 32, 32};

            REQUIRE(area1.IsAdjacent(area2));
            REQUIRE(area2.IsAdjacent(area1));
        }

        SECTION("false") {
            Area area1 {0, 0, 32, 32};
            Area area2 {33, 0, 32, 32};

            REQUIRE(!area1.IsAdjacent(area2));
            REQUIRE(!area2.IsAdjacent(area1));
        }
    }

    SECTION("CombineWith") {
        SECTION("to the right") {
            Area area1 {0, 0, 32, 32};
            Area area2 {32, 0, 32, 32};

            bool result = area1.CombineWith(area2);

            REQUIRE(result);
            REQUIRE(area1.x == 0);
            REQUIRE(area1.width == 64);
        }

        SECTION("to the left") {
            Area area1 {32, 0, 32, 32};
            Area area2 {0, 0, 32, 32};

            bool result = area1.CombineWith(area2);

            REQUIRE(result);
            REQUIRE(area1.x == 0);
            REQUIRE(area1.width == 64);
        }

        SECTION("below") {
            Area area1 {0, 0, 32, 32};
            Area area2 {0, 32, 32, 32};

            bool result = area1.CombineWith(area2);

            REQUIRE(result);
            REQUIRE(area1.y == 0);
            REQUIRE(area1.height == 64);
        }

        SECTION("above") {
            Area area1 {0, 32, 32, 32};
            Area area2 {0, 0, 32, 32};

            bool result = area1.CombineWith(area2);

            REQUIRE(result);
            REQUIRE(area1.y == 0);
            REQUIRE(area1.height == 64);
        }

        SECTION("false") {
            Area area1 {0, 0, 32, 32};
            Area area2 {33, 0, 32, 32};

            bool result = area1.CombineWith(area2);

            REQUIRE(!result);
            REQUIRE(area1.x == 0);
            REQUIRE(area1.width == 32);
            REQUIRE(area1.y == 0);
            REQUIRE(area1.height == 32);
        }
    }
}
