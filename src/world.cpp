////////////////////////
//
//  Created: Wed May 01 2024
//  File: world.cpp
//
////////////////////////

#include "engine/world.hpp"

namespace engine
{
    World::World(std::size_t id)
        : _id(id)
        , _current_entity(0)
        , _available_entities()
        , _components()
        , _remove_component_requests()
        , _destroy_entity_requests()
        , _remove_component_methods()
        , _custom_methods()
    { }

    World::World(const World &other)
        : _id(other._id)
        , _current_entity(other._current_entity)
        , _available_entities(other._available_entities)
        , _components(other._components)
        , _remove_component_requests(other._remove_component_requests)
        , _destroy_entity_requests(other._destroy_entity_requests)
        , _remove_component_methods(other._remove_component_methods)
        , _custom_methods(other._custom_methods)
    { }

    World::World(World &&other)
        : _id(other._id)
        , _current_entity(other._current_entity)
        , _available_entities(other._available_entities)
        , _components(other._components)
        , _remove_component_requests(other._remove_component_requests)
        , _destroy_entity_requests(other._destroy_entity_requests)
        , _remove_component_methods(other._remove_component_methods)
        , _custom_methods(other._custom_methods)
    { }

    World::~World()
    { }

    World &World::operator=(const World &other)
    {
        _id = other._id;
        _current_entity = other._current_entity;
        _available_entities = other._available_entities;
        _components = other._components;
        _remove_component_requests = other._remove_component_requests;
        _destroy_entity_requests = other._destroy_entity_requests;
        _remove_component_methods = other._remove_component_methods;
        _custom_methods = other._custom_methods;

        return *this;
    }

    World &World::operator=(World &&other)
    {
        _id = other._id;
        _current_entity = other._current_entity;
        _available_entities = other._available_entities;
        _components = other._components;
        _remove_component_requests = other._remove_component_requests;
        _destroy_entity_requests = other._destroy_entity_requests;
        _remove_component_methods = other._remove_component_methods;
        _custom_methods = other._custom_methods;

        return *this;
    }

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

    void World::launchCustomMethod(std::size_t id)
    {
        auto methods = _custom_methods.find(id);

        if (methods == _custom_methods.end())
            return;

        for (auto &method: methods->second)
            (this->*method)();
    }
}
