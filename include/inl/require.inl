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

    template<typename ... COMPONENTS>
    bool Require<COMPONENTS...>::isAvailableToRemove(World &world, const Entity &entity)
    {
        bool is_available = true;

        ((is_available = is_available ? world.getComponent<COMPONENTS>(entity) == nullptr : is_available), ...);

        return is_available;
    }
}
