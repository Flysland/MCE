////////////////////////
//
//  Created: Sun May 12 2024
//  File: components.hpp
//
////////////////////////

#pragma once

#include "mce/types.hpp"

namespace mce
{
    template<typename T>
    class Components
    {
        public:
            using container = std::vector<T>;
            using iterator = container::iterator;
            using const_iterator = container::const_iterator;
            using map = std::map<Entity, std::size_t>;

            Components();

            template<typename ... ARGS>
            T *insertEntity(const Entity &entity, ARGS &&... args);

            inline std::size_t size() const;
            inline bool contain(const Entity &entity) const;
            inline iterator begin();
            inline const_iterator begin() const;
            inline iterator end();
            inline const_iterator end() const;

            void clear();
            void remove(const Entity &entity);
            T *at(std::size_t index);
            const T *at(std::size_t index) const;
            T *get(const Entity &entity);
            const T *get(const Entity &entity) const;

        private:
            container _components;
            map _map;
            Entity _entity_start;
            Entity _entity_end;
    };
}

#include "mce/inl/components.inl"
