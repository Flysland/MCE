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
    inline void Scene::applyRequests()
    {
        for (auto it = _worlds.begin(); it != _worlds.end(); ++it)
            (*it)->applyRequests();
    }

    inline void Scene::launchCustomMethod(std::size_t id)
    {
        for (auto it = _worlds.begin(); it != _worlds.end(); ++it)
            (*it)->launchCustomMethod(id);
    }
}
