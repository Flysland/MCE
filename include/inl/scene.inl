////////////////////////
//
//  Created: Wed May 01 2024
//  File: scene.inl
//
////////////////////////

#pragma once

#include "engine/scene.hpp"

namespace engine
{
    inline World *Scene::createWorld()
    {
        _worlds.push_back(new World(_current_world_id++));

        return _worlds.back();
    }

    inline void Scene::applyRequests()
    {
        for (auto it = _worlds.begin(); it != _worlds.end(); ++it)
            (*it)->applyRequests();
    }
}
