//
// Created by Snorri Sturluson on 24/09/2018.
//

#include <gtest/gtest.h>
#include "Area.h"

TEST(Area, IsAdjacent_TrueVertical) {
    Area area1 {0, 0, 32, 32};
    Area area2 {0, 32, 32, 32};

    EXPECT_EQ(area1.IsAdjacent(area2), true);
    EXPECT_EQ(area2.IsAdjacent(area1), true);
}

TEST(Area, IsAdjacent_TrueHorizontal) {
    Area area1 {0, 0, 32, 32};
    Area area2 {32, 0, 32, 32};

    EXPECT_EQ(area1.IsAdjacent(area2), true);
    EXPECT_EQ(area2.IsAdjacent(area1), true);
}

TEST(Area, IsAdjacent_False) {
    Area area1 {0, 0, 32, 32};
    Area area2 {33, 0, 32, 32};

    EXPECT_EQ(area1.IsAdjacent(area2), false);
    EXPECT_EQ(area2.IsAdjacent(area1), false);
}

TEST(Area, CombineWith_ToTheRight) {
    Area area1 {0, 0, 32, 32};
    Area area2 {32, 0, 32, 32};

    bool result = area1.CombineWith(area2);

    EXPECT_EQ(result, true);
    EXPECT_EQ(area1.x, 0);
    EXPECT_EQ(area1.width, 64);
}

TEST(Area, CombineWith_ToTheLeft) {
    Area area1 {32, 0, 32, 32};
    Area area2 {0, 0, 32, 32};

    bool result = area1.CombineWith(area2);

    EXPECT_EQ(result, true);
    EXPECT_EQ(area1.x, 0);
    EXPECT_EQ(area1.width, 64);
}

TEST(Area, CombineWith_Below) {
    Area area1 {0, 0, 32, 32};
    Area area2 {0, 32, 32, 32};

    bool result = area1.CombineWith(area2);

    EXPECT_EQ(result, true);
    EXPECT_EQ(area1.y, 0);
    EXPECT_EQ(area1.height, 64);
}

TEST(Area, CombineWith_Above) {
    Area area1 {0, 32, 32, 32};
    Area area2 {0, 0, 32, 32};

    bool result = area1.CombineWith(area2);

    EXPECT_EQ(result, true);
    EXPECT_EQ(area1.y, 0);
    EXPECT_EQ(area1.height, 64);
}

TEST(Area, CombineWith_False) {
    Area area1 {0, 0, 32, 32};
    Area area2 {33, 0, 32, 32};

    bool result = area1.CombineWith(area2);

    EXPECT_EQ(result, false);
    EXPECT_EQ(area1.x, 0);
    EXPECT_EQ(area1.width, 32);
    EXPECT_EQ(area1.y, 0);
    EXPECT_EQ(area1.height, 32);
}
