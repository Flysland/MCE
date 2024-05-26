////////////////////////
//
//  Created: Wed May 01 2024
//  File: world.hpp
//
////////////////////////

#pragma once

#include "engine/types.hpp"
#include "engine/components.hpp"

namespace engine
{
    class World
    {
        friend class Scene;

        public:
            World(const World &other);
            World(World &&other);
            ~World();

            World &operator=(const World &other);
            World &operator=(World &&other);

            Entity createEntity();
            void requestDestroyEntity(const Entity &entity);

            inline const std::size_t &getID() const;

            template<typename T, typename ... ARGS>
            Component<T> &addComponent(const Entity &entity, ARGS &&... args);

            template<typename T>
            inline void requestRemoveComponent(const Entity &entity);

            template<typename T>
            inline Component<T> &getComponent(const Entity &entity);

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

        private:
            std::size_t _id;
            Entity _current_entity;
            Entities _available_entities;
            ComponentContainer _components;
            RequestContainer<Entity, World, void, const Entity &> _remove_component_requests;
            RequestContainer<Entity, World, void, const Entity &> _destroy_entity_requests;
            MethodContainer<World, void, const Entity &> _remove_component_methods;
            std::unordered_map<std::size_t, MethodContainer<World, void>> _custom_methods;

            World(std::size_t id);

            void applyRequests();
            void launchCustomMethod(std::size_t id);

            inline void destroyEntity(const Entity &entity);

            template<typename T, auto M, typename ... ARGS>
            void executeMethod(ARGS &&... args);

            template<typename T>
            void removeComponent(const Entity &entity);
    };
}

#include "inl/world.inl"
