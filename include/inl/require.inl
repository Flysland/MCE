////////////////////////
//
//  Created: Sun Jul 28 2024
//  File: require.inl
//
////////////////////////

#include "mce/require.hpp"

namespace mce
{
    template<typename ... COMPONENTS>
    inline void Require<COMPONENTS...>::applyRequiredComponents(World &world, const Entity &entity)
    {
        ((world.addComponent<COMPONENTS>(entity)), ...);
    }

    template<typename ... COMPONENTS>
    template<typename T>
    inline void Require<COMPONENTS...>::initDependency(World &world)
    {
        ((world.initDependency<T, COMPONENTS>()), ...);
    }

    template<typename ... COMPONENTS>
    template<typename T>
    inline void Require<COMPONENTS...>::removeDependency(World &world)
    {
        ((world.removeDependency<T, COMPONENTS>()), ...);
    }
}
