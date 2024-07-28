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
        inline void applyRequiredComponents(World &world, const Entity &entity);

        template<typename T>
        inline static void initDependency(World &world);

        template<typename T>
        inline static void removeDependency(World &world);
    };
}

#include "mce/inl/require.inl"
