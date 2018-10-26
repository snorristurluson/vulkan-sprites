//
// Created by snorri on 26.10.2018.
//

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include "catch.hpp"
#include "Curve.h"

TEST_CASE("Curves") {
    SECTION("Curve") {
        SECTION("float") {
            Curve <float>c;

            SECTION("empty curve") {
                REQUIRE(c.GetNumKeys() == 0);
                REQUIRE(c.GetValue(0) == 0);
                REQUIRE(c.GetExtrapolationType() == CurveExtrapolation::CLAMP);
            }

            SECTION("single key") {
                c.AddKey(0, 3.14f);
                REQUIRE(c.GetNumKeys() == 1);
                REQUIRE(c.GetValue(0) == 3.14f);
            }

            SECTION("two keys, linear interpolation") {
                c.AddKey(0, 1.0f);
                c.AddKey(1, 2.0f);
                REQUIRE(c.GetNumKeys() == 2);
                REQUIRE(c.GetValue(0.5f) == Approx(1.5f));
            }

            SECTION("three keys") {
                c.AddKey(0, 1.0f);
                c.AddKey(1, 2.0f);
                c.AddKey(2, -1.0f);

                REQUIRE(c.GetNumKeys() == 3);
                REQUIRE(c.GetValue(1.5f) == Approx(0.5f));
            }

            SECTION("three keys added out of order") {
                c.AddKey(1, 2.0f);
                c.AddKey(0, 1.0f);
                c.AddKey(2, -1.0f);

                REQUIRE(c.GetNumKeys() == 3);
                REQUIRE(c.GetValue(1.5f) == Approx(0.5f));
            }

            SECTION("extrapolation") {
                c.AddKey(0, 1.0f);
                c.AddKey(1, 2.0f);
                c.AddKey(2, -1.0f);

                SECTION("clamp") {
                    c.SetExtrapolationType(CurveExtrapolation::CLAMP);
                    REQUIRE(c.GetValue(-1.0f) == 1.0f );
                    REQUIRE(c.GetValue(3.0f) == -1.0f );
                }

                SECTION("cycle") {
                    c.SetExtrapolationType(CurveExtrapolation::CYCLE);
                    REQUIRE(c.GetValue(-1.0f) == 2.0f );
                    REQUIRE(c.GetValue(3.0f) == 2.0f );
                    REQUIRE(c.GetValue(2.5f) == 1.5f );
                }

                SECTION("mirror") {
                    c.SetExtrapolationType(CurveExtrapolation::MIRROR);
                    REQUIRE(c.GetValue(-1.0f) == 2.0f );
                    REQUIRE(c.GetValue(3.0f) == 2.0f );
                    REQUIRE(c.GetValue(2.5f) == 0.5f );
                }

                SECTION("linear") {
                    c.SetExtrapolationType(CurveExtrapolation::LINEAR);
                    REQUIRE(c.GetValue(-1.0f) == 0.0f );
                    REQUIRE(c.GetValue(3.0f) == -4.0f );
                }

            }
        }

        SECTION("vec2") {
            Curve<glm::vec2> c;

            SECTION("empty curve") {
                REQUIRE(c.GetNumKeys() == 0);
                REQUIRE(c.GetExtrapolationType() == CurveExtrapolation::CLAMP);

                auto value = c.GetValue(0);
                REQUIRE(value.x == 0.0f);
                REQUIRE(value.y == 0.0f);
            }

            SECTION("single key") {
                c.AddKey(0, glm::vec2 {3.14f, 42.0f});

                REQUIRE(c.GetNumKeys() == 1);

                auto value = c.GetValue(0);
                REQUIRE(value.x == 3.14f);
                REQUIRE(value.y == 42.0f);
            }

        }

        SECTION("vec3") {
            Curve<glm::vec3> c;

            SECTION("empty curve") {
                REQUIRE(c.GetNumKeys() == 0);
                REQUIRE(c.GetExtrapolationType() == CurveExtrapolation::CLAMP);

                auto value = c.GetValue(0);
                REQUIRE(value.x == 0.0f);
                REQUIRE(value.y == 0.0f);
                REQUIRE(value.z == 0.0f);
            }

            SECTION("single key") {
                c.AddKey(0, glm::vec3 {3.14f, 42.0f, -1.0f});

                REQUIRE(c.GetNumKeys() == 1);

                auto value = c.GetValue(0);
                REQUIRE(value.x == 3.14f);
                REQUIRE(value.y == 42.0f);
                REQUIRE(value.z == -1.0f);
            }

        }
    }
}