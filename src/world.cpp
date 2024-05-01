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
        , _remove_component_methods()
    { }

    World::World(const World &other)
        : _id(other._id)
        , _current_entity(other._current_entity)
        , _available_entities(other._available_entities)
        , _components(other._components)
        , _remove_component_requests(other._remove_component_requests)
        , _remove_component_methods(other._remove_component_methods)
    { }

    World::World(World &&other)
        : _id(other._id)
        , _current_entity(other._current_entity)
        , _available_entities(other._available_entities)
        , _components(other._components)
        , _remove_component_requests(other._remove_component_requests)
        , _remove_component_methods(other._remove_component_methods)
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
        _remove_component_methods = other._remove_component_methods;

        return *this;
    }

    World &World::operator=(World &&other)
    {
        _id = other._id;
        _current_entity = other._current_entity;
        _available_entities = other._available_entities;
        _components = other._components;
        _remove_component_requests = other._remove_component_requests;
        _remove_component_methods = other._remove_component_methods;

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
        _available_entities.push_back(entity);

        for (auto it = _remove_component_methods.begin(); it != _remove_component_methods.end(); ++it)
            (*this.**it)(entity);
    }

    void World::applyRequests()
    {
        for (auto it = _remove_component_requests.begin(); it != _remove_component_requests.end(); ++it)
            (*this.*it->second)(it->first);

        _remove_component_requests.clear();
    }
}
