////////////////////////
//
//  Created: Sun Jul 28 2024
//  File: requests.hpp
//
////////////////////////

#pragma once

#include "mce/types.hpp"

namespace mce
{
    template<typename T, typename R, typename ... ARGS>
    struct Request
    {
        Method<T, R, ARGS...> request;
    };

    struct RequestRemoveComponent : public Request<World, void, const Entity &, bool>
    {
        Entity entity;
        bool force;
    };

    struct RequestDestroyEntity : public Request<World, void, const Entity &>
    {
        Entity entity;
    };

    struct RequestUnregisterComponent : public Request<World, void>
    {

    };

    template<typename T>
    using Requests = std::vector<T>;
}
