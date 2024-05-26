////////////////////////
//
//  Created: Thu May 02 2024
//  File: world.cpp
//
////////////////////////

#include "tests.hpp"

namespace testing
{
    using TestComponentInit = struct TestComponentInit
    {
        bool init_called;
        engine::World *world;
        engine::Entity entity;

        TestComponentInit()
            : init_called(false)
            , world(nullptr)
            , entity()
        { }

        void init(engine::World *world, engine::Entity entity)
        {
            init_called = true;
            this->world = world;
            this->entity = entity;
        }
    };

    using TestComponentUpdate = struct TestComponentUpdate
    {
        bool update_called;
        int a;
        int *ptr;

        TestComponentUpdate()
            : update_called(false)
            , a(0)
            , ptr(nullptr)
        { }

        void update()
        {
            update_called = true;
        }

        void updateWithParam(int a)
        {
            this->a = a;
        }

        void referenceMethod(int &a)
        {
            this->ptr = &a;
            a = 12;
        }
    };

    void test_entities()
    {
        INIT_TEST()
        engine::Scene scene = engine::Scene();
        engine::World *world = scene.createWorld();
        engine::Entity entity_1 = world->createEntity();
        engine::Entity entity_2 = world->createEntity();

        CHECK(entity_1 != entity_2)

        world->requestDestroyEntity(entity_1);
        scene.applyRequests();

        entity_1 = world->createEntity();

        CHECK(entity_1 != entity_2)

        TERMINATE_TEST()
    }

    void test_components()
    {
        INIT_TEST()
        engine::Scene scene = engine::Scene();
        engine::World *world = scene.createWorld();
        engine::Entity entity = world->createEntity();
        engine::Entity entity2 = world->createEntity();

        CHECK(!world->getComponent<int>(entity).has_value())

        world->addComponent<int>(entity, 42);

        CHECK(world->getComponent<int>(entity).has_value())
        CHECK(world->getComponent<int>(entity).value() == 42)
        CHECK(world->getComponents<int>().size() == 1)

        world->addComponent<int>(entity2, 84);

        CHECK(world->getComponent<int>(entity2).has_value())
        CHECK(world->getComponent<int>(entity2).value() == 84)
        CHECK(world->getComponents<int>().size() == 2)

        world->requestRemoveComponent<int>(entity);
        scene.applyRequests();

        CHECK(!world->getComponent<int>(entity).has_value())
        CHECK(world->getComponent<int>(entity2).has_value())
        CHECK(world->getComponent<int>(entity2).value() == 84)
        CHECK(world->getComponents<int>().size() == 1)

        world->addComponent<TestComponentInit>(entity);

        CHECK(world->getComponent<TestComponentInit>(entity)->init_called)
        CHECK(world->getComponent<TestComponentInit>(entity)->world == world)
        CHECK(world->getComponent<TestComponentInit>(entity)->entity == entity)

        world->addComponent<TestComponentUpdate>(entity);

        CHECK(!world->getComponent<TestComponentUpdate>(entity)->update_called);
        CHECK(world->getComponent<TestComponentUpdate>(entity)->a == 0);
        CHECK(world->getComponent<TestComponentUpdate>(entity)->ptr == nullptr);

        scene.launchCustomMethod(UPDATE_METHOD_ID);

        CHECK(world->getComponent<TestComponentUpdate>(entity)->update_called);

        scene.launchCustomMethod(UPDATE_WITH_PARAM_METHOD_ID, 42);

        CHECK(world->getComponent<TestComponentUpdate>(entity)->a == 42);

        int value = 42;
        scene.launchCustomMethod(REFERENCE_CUSTOM_METHOD_ID, value);

        CHECK(world->getComponent<TestComponentUpdate>(entity)->ptr == &value);
        CHECK(value == 12);
        TERMINATE_TEST()
    }
}
