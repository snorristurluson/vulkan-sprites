//
// Created by Snorri Sturluson on 23/09/2018.
//

#include "catch.hpp"
#include "AreaAllocator.h"

TEST_CASE("AreaAllocator") {
    SECTION("constructor") {
        AreaAllocator aa;
    }

    SECTION("Initialize") {
        AreaAllocator aa;

        aa.Initialize(2048, 2048);

        REQUIRE(aa.GetTotalWidth() == 2048);
        REQUIRE(aa.GetTotalHeight() == 2048);
        REQUIRE(aa.GetFreeAreaCount() == 1);
        REQUIRE(aa.GetFreeAreaSize() == 2048*2048);
        REQUIRE(aa.GetAllocatedAreaCount() == 0);
        REQUIRE(aa.GetAllocatedAreaSize() == 0);
    }

    SECTION("Allocate") {
        AreaAllocator aa;
        aa.Initialize(2048, 2048);

        SECTION("simple case") {
            auto sizeBefore = aa.GetFreeAreaSize();

            Area* a = aa.Allocate(64, 64);
            REQUIRE(a->width == 64);
            REQUIRE(a->height == 64);

            auto sizeAfter = aa.GetFreeAreaSize();
            REQUIRE(sizeAfter == sizeBefore - 64*64);

            REQUIRE(aa.GetFreeAreaSize() + aa.GetAllocatedAreaSize() == aa.GetTotalAreaSize());
        }

        SECTION("too large returns nullptr") {
            auto sizeBefore = aa.GetFreeAreaSize();
            Area* a = aa.Allocate(4096, 4096);

            REQUIRE(!a);
            REQUIRE(aa.GetFreeAreaSize() + aa.GetAllocatedAreaSize() == aa.GetTotalAreaSize());
        }

        SECTION("four quadrants") {
            auto a1 = aa.Allocate(1024, 1024);
            auto a2 = aa.Allocate(1024, 1024);
            auto a3 = aa.Allocate(1024, 1024);
            auto a4 = aa.Allocate(1024, 1024);

            REQUIRE(a1);
            REQUIRE(a2);
            REQUIRE(a3);
            REQUIRE(a4);

            REQUIRE(aa.GetFreeAreaSize() == 0);
            REQUIRE(aa.GetFreeAreaSize() + aa.GetAllocatedAreaSize() == aa.GetTotalAreaSize());
        }

        SECTION("collapse areas") {
            auto a1 = aa.Allocate(1024, 1024);
            aa.Free(a1);

            auto a2 = aa.Allocate(2048, 2048);
            REQUIRE(a2);
            REQUIRE(aa.GetFreeAreaSize() == 0);
            REQUIRE(aa.GetFreeAreaSize() + aa.GetAllocatedAreaSize() == aa.GetTotalAreaSize());
        }
    }

    SECTION("Free") {
        AreaAllocator aa;
        aa.Initialize(2048, 2048);

        Area* a = aa.Allocate(64, 64);
        aa.Free(a);

        REQUIRE(aa.GetFreeAreaSize() + aa.GetAllocatedAreaSize() == aa.GetTotalAreaSize());
    }
}
