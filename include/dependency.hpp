////////////////////////
//
//  Created: Fri Aug 02 2024
//  File: dependency.hpp
//
////////////////////////

#pragma once

#include "mce/types.hpp"

namespace mce
{
    struct ComponentDependency
    {
        MethodContainer<World, bool, const Entity &> contain_main_components;
        MethodContainer<World, void, const Entity &, bool &&> remove_main_components;
    };

    using ComponentsDependency = std::unordered_map<std::type_index, ComponentDependency>;
}
