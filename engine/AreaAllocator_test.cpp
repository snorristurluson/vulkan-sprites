//
// Created by Snorri Sturluson on 23/09/2018.
//

#include <gtest/gtest.h>
#include "AreaAllocator.h"

TEST(AreaAllocator, CanCreate) {
    AreaAllocator aa;
}

TEST(AreaAllocator, Initialize) {
    AreaAllocator aa;

    aa.Initialize(2048, 2048);
    EXPECT_EQ(aa.GetTotalWidth(), 2048);
    EXPECT_EQ(aa.GetTotalHeight(), 2048);
    EXPECT_EQ(aa.GetFreeAreaCount(), 1);
    EXPECT_EQ(aa.GetFreeAreaSize(), 2048*2048);
    EXPECT_EQ(aa.GetAllocatedAreaCount(), 0);
    EXPECT_EQ(aa.GetAllocatedAreaSize(), 0);
}

TEST(AreaAllocator, Allocate) {
    AreaAllocator aa;
    aa.Initialize(2048, 2048);
    auto sizeBefore = aa.GetFreeAreaSize();

    Area* a = aa.Allocate(64, 64);
    EXPECT_EQ(a->width, 64);
    EXPECT_EQ(a->height, 64);

    auto sizeAfter = aa.GetFreeAreaSize();
    EXPECT_EQ(sizeAfter, sizeBefore - 64*64);

    EXPECT_EQ(aa.GetFreeAreaSize() + aa.GetAllocatedAreaSize(), aa.GetTotalAreaSize());
}

TEST(AreaAllocator, Free) {
    AreaAllocator aa;
    aa.Initialize(2048, 2048);

    Area* a = aa.Allocate(64, 64);
    aa.Free(a);

    EXPECT_EQ(aa.GetFreeAreaSize() + aa.GetAllocatedAreaSize(), aa.GetTotalAreaSize());
}

TEST(AreaAllocator, AllocateTooLargeReturnsNullPtr) {
    AreaAllocator aa;
    aa.Initialize(32, 32);

    Area* a = aa.Allocate(64, 64);

    EXPECT_EQ(a, nullptr);
    EXPECT_EQ(aa.GetFreeAreaSize() + aa.GetAllocatedAreaSize(), aa.GetTotalAreaSize());
}

TEST(AreaAllocator, AllocateFourQuadrants) {
    AreaAllocator aa;
    aa.Initialize(64, 64);

    auto a1 = aa.Allocate(32, 32);
    auto a2 = aa.Allocate(32, 32);
    auto a3 = aa.Allocate(32, 32);
    auto a4 = aa.Allocate(32, 32);

    EXPECT_NE(a1, nullptr);
    EXPECT_NE(a2, nullptr);
    EXPECT_NE(a3, nullptr);
    EXPECT_NE(a4, nullptr);

    EXPECT_EQ(aa.GetFreeAreaSize(), 0);
    EXPECT_EQ(aa.GetFreeAreaSize() + aa.GetAllocatedAreaSize(), aa.GetTotalAreaSize());
}

TEST(AreaAllocator, CollapseAreas) {
    AreaAllocator aa;
    aa.Initialize(64, 64);

    auto a1 = aa.Allocate(32, 32);
    aa.Free(a1);

    auto a2 = aa.Allocate(64, 64);
    EXPECT_NE(a2, nullptr);
    EXPECT_EQ(aa.GetFreeAreaSize(), 0);
    EXPECT_EQ(aa.GetFreeAreaSize() + aa.GetAllocatedAreaSize(), aa.GetTotalAreaSize());
}