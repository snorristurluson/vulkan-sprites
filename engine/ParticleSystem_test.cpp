//
// Created by snorri on 25.10.2018.
//

#include "catch.hpp"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "Renderer.h"
#include "ParticleSystem.h"
#include "Particle.h"

TEST_CASE("ParticleSystem basics") {
    SECTION("can create") {
        ParticleSystem ps;
    }

    SECTION("newly constructed is empty") {
        ParticleSystem ps;

        REQUIRE(ps.GetNumParticles() == 0);
        REQUIRE(ps.GetNumEmitters() == 0);
    }

    SECTION("Update") {
        ParticleSystem ps;

        SECTION("when empty") {
            ps.Update(1.0f);
            REQUIRE(ps.GetNumParticles() == 0);
        }
    }

    SECTION("Emitter") {
        ParticleSystem ps;
        auto em = ps.AddEmitter();

        SECTION("AddEmitter") {
            REQUIRE(ps.GetNumEmitters() == 1);

            REQUIRE(em->GetEmissionRate() == 0.0f);
            REQUIRE(em->GetNumParticles() == 0);
            REQUIRE(em->GetLifespan() == 1.0f);
            REQUIRE(em->GetDirection() == 0.0f);
            REQUIRE(em->GetDirectionRange() == 0.0f);
            REQUIRE(em->GetSpeed() == 0.0f);
        }

        SECTION("SetEmissionRate") {
            em->SetEmissionRate(1.0f);
            REQUIRE(em->GetEmissionRate() == 1.0f);
        }

        SECTION("SetLifespan") {
            em->SetLifespan(15.0f);
            REQUIRE(em->GetLifespan() == 15.0f);
        }

        SECTION("SetPosition") {
            em->SetPosition({3.14f, 42.0f});
            REQUIRE(em->GetPosition() == glm::vec2 {3.14f, 42.0f});
        }

        SECTION("Update") {
            em->SetEmissionRate(1.0f);
            em->Update(10.0f);
            REQUIRE(em->GetNumParticles() == 10);
        }

        SECTION("SetDirection") {
            em->SetDirection(0.12f);
            REQUIRE(em->GetDirection() == 0.12f);
        }

        SECTION("SetDirectionRange") {
            em->SetDirectionRange(0.12f);
            REQUIRE(em->GetDirectionRange() == 0.12f);
        }

        SECTION("SetSpeed") {
            em->SetSpeed(0.34f);
            REQUIRE(em->GetSpeed() == 0.34f);
        }
    }
}

TEST_CASE("Particle") {
    SECTION("can create") {
        Particle p;
    }
}