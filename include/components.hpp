////////////////////////
//
//  Created: Sun May 12 2024
//  File: components.hpp
//
////////////////////////

#pragma once

#include "engine/types.hpp"

namespace engine
{
    template<typename T>
    class Components
    {
        public:
            using container = std::vector<Component<T>>;
            using iterator = container::iterator;
            using const_iterator = container::const_iterator;

            Components();
            Components(const Components<T> &other);
            Components(Components<T> &&other);
            ~Components();

            Components<T> &operator=(const Components<T> &other);
            Components<T> &operator=(Components<T> &&other);

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
            container m_components;
            Entity m_entity_start;
            Entity m_entity_end;

            Component<T> &insert_front(const Entity &entity);
            Component<T> &insert_back(const Entity &entity);
            void optimize_front();
            void optimize_back();
    };
}

#include "engine/inl/components.inl"
