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
            using container = std::vector<Component<T>>;
            using iterator = container::iterator;
            using const_iterator = container::const_iterator;

            Components();

            inline std::size_t size() const;
            inline bool contain(const Entity &entity) const;
            inline iterator begin();
            inline const_iterator begin() const;
            inline iterator end();
            inline const_iterator end() const;
            inline void optimize();

            void clear();
            Component<T> &insert_entity(const Entity &entity);
            Component<T> &at(std::size_t index);
            const Component<T> &at(std::size_t index) const;
            Component<T> &get(const Entity &entity);
            const Component<T> &get(const Entity &entity) const;

        private:
            container _components;
            Entity _entity_start;
            Entity _entity_end;

            Component<T> &insertFront(const Entity &entity);
            Component<T> &insertBack(const Entity &entity);
            void optimizeFront();
            void optimizeBack();
    };
}

#include "mce/inl/components.inl"
