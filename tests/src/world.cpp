////////////////////////
//
//  Created: Thu May 02 2024
//  File: world.cpp
//
////////////////////////

#include "tests.hpp"
#include "mce/require.hpp"

namespace testing
{
    using TestComponentInit = struct TestComponentInit : mce::Require<float, double>
    {
        bool init_called;
        mce::World *world;
        mce::Entity entity;

        TestComponentInit()
            : init_called(false)
            , world(nullptr)
            , entity()
        { }

        void init(mce::World &world, mce::Entity entity)
        {
            init_called = true;
            this->world = &world;
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
        mce::World world = mce::World();
        mce::Entity entity_1 = world.createEntity();
        mce::Entity entity_2 = world.createEntity();

        CHECK(entity_1 != entity_2)

        world.requestDestroyEntity(entity_1);
        world.applyRequests();

        entity_1 = world.createEntity();

        CHECK(entity_1 != entity_2)

        TERMINATE_TEST()
    }

    void test_components()
    {
        INIT_TEST()
        mce::World world = mce::World();
        mce::Entity entity = world.createEntity();
        mce::Entity entity2 = world.createEntity();

        CHECK(world.getComponent<int>(entity) == nullptr)

        world.addComponent<int>(entity, 42);

        CHECK(world.getComponent<int>(entity) != nullptr)
        CHECK(*world.getComponent<int>(entity) == 42)
        CHECK(world.getComponents<int>().size() == 1)

        world.addComponent<int>(entity2, 84);

        CHECK(world.getComponent<int>(entity2) != nullptr)
        CHECK(*world.getComponent<int>(entity2) == 84)
        CHECK(world.getComponents<int>().size() == 2)

        world.requestRemoveComponent<int>(entity);
        world.applyRequests();

        CHECK(world.getComponent<int>(entity) == nullptr)
        CHECK(world.getComponent<int>(entity2) != nullptr)
        CHECK(*world.getComponent<int>(entity2) == 84)
        CHECK(world.getComponents<int>().size() == 1)

        world.addComponent<TestComponentInit>(entity);

        world.requestDestroyEntity(entity);
        world.applyRequests();
        entity = world.createEntity();

        world.addComponent<TestComponentInit>(entity);

        world.requestRemoveComponent<float>(entity, true);
        world.applyRequests();

        CHECK(world.getComponent<float>(entity) == nullptr);
        CHECK(world.getComponent<double>(entity) != nullptr);
        CHECK(world.getComponent<TestComponentInit>(entity) != nullptr);

        world.addComponent<TestComponentInit>(entity);

        CHECK(world.getComponent<TestComponentInit>(entity)->init_called)
        CHECK(world.getComponent<TestComponentInit>(entity)->world == &world)
        CHECK(world.getComponent<TestComponentInit>(entity)->entity == entity)
        CHECK(world.getComponents<float>().size() == 1)
        CHECK(world.getComponents<double>().size() == 1)

        world.requestRemoveComponent<float>(entity);
        world.requestRemoveComponent<double>(entity);
        world.applyRequests();

        CHECK(world.getComponents<float>().size() == 1)
        CHECK(world.getComponents<double>().size() == 1)

        world.requestRemoveComponent<TestComponentInit>(entity);
        world.requestRemoveComponent<float>(entity);
        world.requestRemoveComponent<double>(entity);
        world.applyRequests();

        CHECK(world.getComponents<float>().size() == 0)
        CHECK(world.getComponents<double>().size() == 0)

        world.addComponent<TestComponentUpdate>(entity);

        CHECK(!world.getComponent<TestComponentUpdate>(entity)->update_called);
        CHECK(world.getComponent<TestComponentUpdate>(entity)->a == 0);
        CHECK(world.getComponent<TestComponentUpdate>(entity)->ptr == nullptr);

        world.launchCustomMethod(UPDATE_METHOD_ID);

        CHECK(world.getComponent<TestComponentUpdate>(entity)->update_called);

        world.launchCustomMethod(UPDATE_WITH_PARAM_METHOD_ID, 42);

        CHECK(world.getComponent<TestComponentUpdate>(entity)->a == 42);

        int value = 42;
        world.launchCustomMethod(REFERENCE_CUSTOM_METHOD_ID, value);

        CHECK(world.getComponent<TestComponentUpdate>(entity)->ptr == &value);
        CHECK(value == 12);
        TERMINATE_TEST()
    }
}
