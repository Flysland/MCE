////////////////////////
//
//  Created: Wed May 01 2024
//  File: scene.hpp
//
////////////////////////

#pragma once

#include "engine/types.hpp"
#include "engine/world.hpp"

namespace engine
{
    class Scene
    {
        public:
            Scene();
            Scene(const Scene &other);
            Scene(Scene &&other);
            ~Scene();

            Scene &operator=(const Scene &other);
            Scene &operator=(Scene &&other);

            void destroyWorld(World *world);

            inline World *createWorld();
            inline void applyRequests();

        private:
            std::size_t _current_world_id;
            std::vector<World *> _worlds;
    };
}

#include "inl/scene.inl"
