////////////////////////
//
//  Created: Wed May 01 2024
//  File: scene.cpp
//
////////////////////////

#include "engine/scene.hpp"

namespace engine
{
    Scene::Scene()
        : _current_world_id(0)
        , _available_world_ids()
        , _worlds()
    { }

    Scene::Scene(const Scene &other)
        : _current_world_id(other._current_world_id)
        , _available_world_ids(other._available_world_ids)
        , _worlds(other._worlds)
    { }

    Scene::Scene(Scene &&other)
        : _current_world_id(other._current_world_id)
        , _available_world_ids(other._available_world_ids)
        , _worlds(other._worlds)
    { }

    Scene::~Scene()
    {
        for (auto it = _worlds.begin(); it != _worlds.end(); ++it)
            delete *it;
    }

    Scene &Scene::operator=(const Scene &other)
    {
        _current_world_id = other._current_world_id;
        return *this;
    }

    Scene &Scene::operator=(Scene &&other)
    {
        _current_world_id = other._current_world_id;
        return *this;
    }

    void Scene::destroyWorld(World *world)
    {
        for (auto it = _worlds.begin(); it != _worlds.end(); ++it) {
            if ((*it)->getID() == world->getID()) {
                _available_world_ids.push_back((*it)->getID());
                _worlds.erase(it);
                delete world;
                return;
            }
        }
    }

    World *Scene::createWorld()
    {
        if (_available_world_ids.size()) {
            std::size_t world_id = _available_world_ids.back();
            _available_world_ids.pop_back();
            _worlds.push_back(new World(world_id));
            return _worlds.back();
        }

        _worlds.push_back(new World(_current_world_id++));
        return _worlds.back();
    }
}
