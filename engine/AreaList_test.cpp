//
// Created by Snorri Sturluson on 24/09/2018.
//

#include "catch.hpp"
#include "AreaList.h"

TEST_CASE("AreaList") {
    SECTION("constructor") {
        AreaList al;
    }

    SECTION("basics") {
        AreaList al;

        auto a1 = new Area {0, 0, 32, 32};
        al.emplace_back(a1);
        auto a2 = new Area {32, 0, 32, 32};
        al.emplace_back(a2);
    }

    SECTION("FindAdjacent") {
        AreaList al;

        auto area = new Area {0, 0, 32, 32};

        auto a1 = new Area {32, 0, 32, 32};
        al.emplace_back(a1);
        auto a2 = new Area {0, 32, 32, 32};
        al.emplace_back(a2);
        auto a3 = new Area {32, 32, 32, 32};
        al.emplace_back(a3);

        auto it = al.FindAdjacent(*area);

        REQUIRE(*it == a1);
    }

    SECTION("FindArea") {
        SECTION("empty") {
            AreaList al;

            auto area = al.FindArea(32, 32);

            REQUIRE(area == al.end());
        }

        SECTION("single fitting") {
            AreaList al;

            auto a1 = new Area {0, 0, 32, 32};
            al.emplace_back(a1);

            auto area = al.FindArea(32, 32);
            REQUIRE(area == al.begin());
        }

        SECTION("single too small") {
            AreaList al;

            auto a1 = new Area {0, 0, 32, 32};
            al.emplace_back(a1);

            auto area = al.FindArea(64, 64);
            REQUIRE(area == al.end());
        }

        SECTION("single larger than needed") {
            AreaList al;

            auto a1 = new Area {0, 0, 32, 32};
            al.emplace_back(a1);

            auto area = al.FindArea(24, 24);
            REQUIRE(area == al.begin());
        }
    }
}
