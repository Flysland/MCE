////////////////////////
//
//  Created: Wed May 01 2024
//  File: world.hpp
//
////////////////////////

#pragma once

#include "mce/types.hpp"
#include "mce/dependency.hpp"
#include "mce/components.hpp"
#include "mce/requests.hpp"

namespace mce
{
    template<typename ... COMPONENTS>
    struct Require;

    class World
    {
        template<typename ... COMPONENTS>
        friend struct Require;

        public:
            World();

            Entity createEntity();
            void requestDestroyEntity(const Entity &entity);

            void applyRequests();
            bool launchCustomMethod(std::size_t id);

            template<typename ... ARGS>
            bool launchCustomMethod(std::size_t id, ARGS &&... args);

            template<typename T, typename ... ARGS>
            T *addComponent(const Entity &entity, ARGS &&... args);

            template<typename T>
            bool hasComponent(const Entity &entity);

            template<typename T>
            void requestRemoveComponent(const Entity &entity, bool force = false);

            template<typename T>
            inline T *getComponent(const Entity &entity);

            template<typename T>
            Components<T> &getComponents();

            template<typename T>
            void registerComponent();

            template<typename T>
            void unregisterComponent();

            template<typename T, auto M>
            void registerCustomMethod(std::size_t id);

            template<typename T, auto M>
            void unregisterCustomMethod(std::size_t id);

            template<typename T, auto M, typename ... ARGS>
            std::enable_if_t<(sizeof...(ARGS) > 0), void> registerCustomMethod(std::size_t id);

            template<typename T, auto M, typename ... ARGS>
            std::enable_if_t<(sizeof...(ARGS) > 0), void> unregisterCustomMethod(std::size_t id);

        private:
            Entity _current_entity;
            Entities _available_entities;
            ComponentContainer _components;
            ComponentsDependency _components_dependency;
            RequestContainer<RequestRemoveComponent> _remove_component_requests;
            RequestContainer<RequestDestroyEntity> _destroy_entity_requests;
            MethodContainer<World, void, const Entity &, bool> _remove_component_methods;
            std::unordered_map<std::size_t, std::any> _custom_methods_with_args;
            std::unordered_map<std::size_t, MethodContainer<World, void>> _custom_methods_without_args;

            void destroyEntity(const Entity &entity);

            template<typename T, auto M, typename ... ARGS>
            void executeMethod(ARGS &&... args);

            template<typename T, typename REQUIRED>
            void initDependency();

            template<typename T, typename REQUIRED>
            void removeDependency();

            template<typename T>
            void removeComponent(const Entity &entity, bool &&force);
    };
}

#include "inl/world.inl"
