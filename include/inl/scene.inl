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
        for (World *&world: _worlds)
            world->applyRequests();
    }

    template<typename ... ARGS>
    inline void Scene::launchCustomMethod(std::size_t id, ARGS &&... args)
    {
        for (World *&world: _worlds)
            world->launchCustomMethod<ARGS...>(id, std::forward<ARGS>(args)...);
    }
}
