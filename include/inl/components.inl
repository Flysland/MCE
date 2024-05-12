////////////////////////
//
//  Created: Sun May 12 2024
//  File: components.inl
//
////////////////////////

#include "engine/components.hpp"

namespace engine
{
    template<typename T>
    Components<T>::Components()
        : m_components()
        , m_entity_start(0)
        , m_entity_end(0)
    { }

    template<typename T>
    Components<T>::Components(const Components<T> &other)
        : m_components(other.m_components)
        , m_entity_start(other.m_entity_start)
        , m_entity_end(other.m_entity_end)
    { }

    template<typename T>
    Components<T>::Components(Components<T> &&other)
        : m_components(other.m_components)
        , m_entity_start(other.m_entity_start)
        , m_entity_end(other.m_entity_end)
    { }

    template<typename T>
    Components<T>::~Components()
    { }

    template<typename T>
    Components<T> &Components<T>::operator=(const Components<T> &other)
    {
        m_components = other.m_components;
        m_entity_start = other.m_entity_start;
        m_entity_end = other.m_entity_end;
        return *this;
    }

    template<typename T>
    Components<T> &Components<T>::operator=(Components<T> &&other)
    {
        m_components = other.m_components;
        m_entity_start = other.m_entity_start;
        m_entity_end = other.m_entity_end;
        return *this;
    }

    template<typename T>
    inline std::size_t Components<T>::size() const
    {
        return m_components.size();
    }

    template<typename T>
    inline bool Components<T>::contain(const Entity &entity) const
    {
        return m_entity_start <= entity && m_entity_end >= entity;
    }

    template<typename T>
    inline Components<T>::iterator Components<T>::begin()
    {
        return m_components.begin();
    }

    template<typename T>
    inline Components<T>::const_iterator Components<T>::begin() const
    {
        return m_components.begin();
    }

    template<typename T>
    inline Components<T>::iterator Components<T>::end()
    {
        return m_components.end();
    }

    template<typename T>
    inline Components<T>::const_iterator Components<T>::end() const
    {
        return m_components.end();
    }

    template<typename T>
    inline void Components<T>::optimize()
    {
        optimize_front();
        optimize_back();
    }

    template<typename T>
    void Components<T>::clear()
    {
        m_components.clear();
        m_entity_start = 0;
        m_entity_end = 0;
    }

    template<typename T>
    Component<T> &Components<T>::insert_entity(const Entity &entity)
    {
        if (entity < m_entity_start)
            return insert_front(entity);

        if (entity > m_entity_end)
            return insert_back(entity);

        if (entity == 0 && m_entity_start == 0)
            return insert_front(entity);

        return get(entity);
    }

    template<typename T>
    Component<T> &Components<T>::at(std::size_t index)
    {
        static Component<T> null_component = std::nullopt;

        if (size() <= index)
            return null_component;

        return m_components.at(index);
    }

    template<typename T>
    const Component<T> &Components<T>::at(std::size_t index) const
    {
        static Component<T> null_component = std::nullopt;

        if (size() <= index)
            return null_component;

        return m_components.at(index);
    }

    template<typename T>
    Component<T> &Components<T>::get(const Entity &entity)
    {
        static Component<T> null_component = std::nullopt;

        if (entity < m_entity_start || entity - m_entity_start >= m_components.size())
            return null_component;

        return m_components.at(entity - m_entity_start);
    }

    template<typename T>
    const Component<T> &Components<T>::get(const Entity &entity) const
    {
        static Component<T> null_component = std::nullopt;

        if (entity < m_entity_start || entity - m_entity_start >= m_components.size())
            return null_component;

        return m_components.at(entity - m_entity_start);
    }

    template<typename T>
    Component<T> &Components<T>::insert_front(const Entity &entity)
    {
        if (!m_components.size())
            m_entity_end = entity;

        m_components.insert(m_components.begin(), m_entity_end - entity + 1 - m_components.size(), std::nullopt);
        m_entity_start = entity;

        return m_components.at(entity - m_entity_start);
    }

    template<typename T>
    Component<T> &Components<T>::insert_back(const Entity &entity)
    {
        if (!m_components.size())
            m_entity_start = entity;

        m_components.resize(entity - m_entity_start + 1);
        m_entity_end = entity;

        return m_components.at(entity - m_entity_start);
    }

    template<typename T>
    void Components<T>::optimize_front()
    {
        std::size_t index = 0;

        for (auto it = m_components.begin(); it != m_components.end(); ++it, ++index) {
            if (it->has_value()) {
                if (!index)
                    return;

                m_components.erase(m_components.begin(), it);
                m_entity_start += index;
                return;
            }
        }
        clear();
    }

    template<typename T>
    void Components<T>::optimize_back()
    {
        std::size_t index = m_components.size() - 1;

        for (auto it = m_components.end(); it != m_components.begin(); --it, --index) {
            if (it->has_value()) {
                if (index == m_components.size() - 1)
                    return;

                m_components.erase(it + 1, m_components.end());
                m_entity_end = index;
                return;
            }
        }
        clear();
    }
}
