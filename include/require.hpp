////////////////////////
//
//  Created: Sun Jul 28 2024
//  File: require.hpp
//
////////////////////////

#pragma once

#include "mce/world.hpp"

namespace mce
{
    template<typename ... COMPONENTS>
    struct Require
    {
        void applyRequiredComponents(World &world, const Entity &entity);
        bool isAvailableToRemove(World &world, const Entity &entity);
    };
}

#include "mce/inl/require.inl"
