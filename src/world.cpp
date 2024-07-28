////////////////////////
//
//  Created: Wed May 01 2024
//  File: world.cpp
//
////////////////////////

#include "mce/world.hpp"

namespace mce
{
    World::World()
        : _current_entity(0)
        , _available_entities()
        , _components()
        , _components_dependency()
        , _max_thread(1)
        , _remove_component_requests()
        , _destroy_entity_requests()
        , _remove_component_methods()
        , _custom_methods_with_args()
        , _custom_methods_without_args()
    { }

    Entity World::createEntity()
    {
        if (_available_entities.size()) {
            Entity entity = _available_entities.back();

            _available_entities.pop_back();
            return entity;
        }

        return _current_entity++;
    }

    void World::requestDestroyEntity(const Entity &entity)
    {
        for (auto &method: _remove_component_methods)
            (this->*method)(entity);

        // TODO: implement force remove component for destroy entity

        _destroy_entity_requests.push_back({entity, &World::destroyEntity});
    }

    void World::applyRequests()
    {
        for (auto &request: _remove_component_requests)
            (this->*request.second)(request.first);

        for (auto &request: _destroy_entity_requests)
            (this->*request.second)(request.first);

        _remove_component_requests.clear();
        _destroy_entity_requests.clear();
    }

    bool World::launchCustomMethod(std::size_t id)
    {
        auto methods = _custom_methods_without_args.find(id);

        if (methods == _custom_methods_without_args.end())
            return false;

        for (auto &method: methods->second)
            (this->*method)();

        return true;
    }
}
