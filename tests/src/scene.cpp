////////////////////////
//
//  Created: Thu May 02 2024
//  File: scene.cpp
//
////////////////////////

#include "tests.hpp"
#include "engine/scene.hpp"

namespace testing
{
    void test_scene()
    {
        INIT_TEST()

        engine::Scene scene = engine::Scene();
        engine::World *world_1 = scene.createWorld();
        engine::World *world_2 = scene.createWorld();
        std::size_t world_1_id = world_1->getID();

        CHECK(world_1 != world_2)
        CHECK(world_1->getID() != world_2->getID())

        scene.destroyWorld(world_1);
        world_1 = scene.createWorld();

        CHECK(world_1 != world_2)
        CHECK(world_1->getID() != world_2->getID())
        CHECK(world_1_id == world_1->getID())

        TERMINATE_TEST()
    }
}
