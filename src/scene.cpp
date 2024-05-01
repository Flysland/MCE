////////////////////////
//
//  Created: Wed May 01 2024
//  File: scene.cpp
//
////////////////////////

#include "scene.hpp"

namespace engine
{
    Scene::Scene()
        : _current_world_id(0)
    { }

    Scene::Scene(const Scene &other)
        : _current_world_id(other._current_world_id)
    { }

    Scene::Scene(Scene &&other)
        : _current_world_id(other._current_world_id)
    { }

    Scene::~Scene()
    { }

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
                _worlds.erase(it);
                delete world;
                return;
            }
        }
    }
}
