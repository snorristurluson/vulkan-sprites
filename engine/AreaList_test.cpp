//
// Created by Snorri Sturluson on 24/09/2018.
//

#include <gtest/gtest.h>
#include "AreaList.h"

TEST(AreaList, CanCreate) {
    AreaList al;
}

TEST(AreaList, Basics) {
    AreaList al;

    auto a1 = new Area {0, 0, 32, 32};
    al.emplace_back(a1);
    auto a2 = new Area {32, 0, 32, 32};
    al.emplace_back(a2);
}

TEST(AreaList, FindAdjacent) {
    AreaList al;

    auto area = new Area {0, 0, 32, 32};

    auto a1 = new Area {32, 0, 32, 32};
    al.emplace_back(a1);
    auto a2 = new Area {0, 32, 32, 32};
    al.emplace_back(a2);
    auto a3 = new Area {32, 32, 32, 32};
    al.emplace_back(a3);

    auto it = al.FindAdjacent(*area);

    EXPECT_EQ(*it, a1);
}

TEST(AreaList, FindArea_Empty) {
    AreaList al;

    auto area = al.FindArea(32, 32);

    EXPECT_EQ(area, al.end());
}

TEST(AreaList, FindArea_SingleFitting) {
    AreaList al;

    auto a1 = new Area {0, 0, 32, 32};
    al.emplace_back(a1);

    auto area = al.FindArea(32, 32);
    EXPECT_EQ(area, al.begin());
}

TEST(AreaList, FindArea_SingleTooSmall) {
    AreaList al;

    auto a1 = new Area {0, 0, 32, 32};
    al.emplace_back(a1);

    auto area = al.FindArea(64, 64);
    EXPECT_EQ(area, al.end());
}

TEST(AreaList, FindArea_SingleLargerThanNeeded) {
    AreaList al;

    auto a1 = new Area {0, 0, 32, 32};
    al.emplace_back(a1);

    auto area = al.FindArea(24, 24);
    EXPECT_EQ(area, al.begin());
}

