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
    void Require<COMPONENTS...>::applyRequiredComponents(World &world, const Entity &entity)
    {
        ((world.addComponent<COMPONENTS>(entity)), ...);
    }
}
