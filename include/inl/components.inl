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
        : _components()
        , _entity_start(0)
        , _entity_end(0)
    { }

    template<typename T>
    Components<T>::Components(const Components<T> &other)
        : _components(other._components)
        , _entity_start(other._entity_start)
        , _entity_end(other._entity_end)
    { }

    template<typename T>
    Components<T>::Components(Components<T> &&other)
        : _components(other._components)
        , _entity_start(other._entity_start)
        , _entity_end(other._entity_end)
    { }

    template<typename T>
    Components<T>::~Components()
    { }

    template<typename T>
    Components<T> &Components<T>::operator=(const Components<T> &other)
    {
        _components = other._components;
        _entity_start = other._entity_start;
        _entity_end = other._entity_end;

        return *this;
    }

    template<typename T>
    Components<T> &Components<T>::operator=(Components<T> &&other)
    {
        _components = other._components;
        _entity_start = other._entity_start;
        _entity_end = other._entity_end;

        return *this;
    }

    template<typename T>
    inline std::size_t Components<T>::size() const
    {
        return _components.size();
    }

    template<typename T>
    inline bool Components<T>::contain(const Entity &entity) const
    {
        return _entity_start <= entity && _entity_end >= entity;
    }

    template<typename T>
    inline Components<T>::iterator Components<T>::begin()
    {
        return _components.begin();
    }

    template<typename T>
    inline Components<T>::const_iterator Components<T>::begin() const
    {
        return _components.begin();
    }

    template<typename T>
    inline Components<T>::iterator Components<T>::end()
    {
        return _components.end();
    }

    template<typename T>
    inline Components<T>::const_iterator Components<T>::end() const
    {
        return _components.end();
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
        _components.clear();
        _entity_start = 0;
        _entity_end = 0;
    }

    template<typename T>
    Component<T> &Components<T>::insert_entity(const Entity &entity)
    {
        if (entity < _entity_start)
            return insert_front(entity);

        if (entity > _entity_end)
            return insert_back(entity);

        if (entity == 0 && _entity_start == 0)
            return insert_front(entity);

        return get(entity);
    }

    template<typename T>
    Component<T> &Components<T>::at(std::size_t index)
    {
        static Component<T> null_component = std::nullopt;

        if (size() <= index)
            return null_component;

        return _components.at(index);
    }

    template<typename T>
    const Component<T> &Components<T>::at(std::size_t index) const
    {
        static Component<T> null_component = std::nullopt;

        if (size() <= index)
            return null_component;

        return _components.at(index);
    }

    template<typename T>
    Component<T> &Components<T>::get(const Entity &entity)
    {
        static Component<T> null_component = std::nullopt;

        if (entity < _entity_start || entity - _entity_start >= _components.size())
            return null_component;

        return _components.at(entity - _entity_start);
    }

    template<typename T>
    const Component<T> &Components<T>::get(const Entity &entity) const
    {
        static Component<T> null_component = std::nullopt;

        if (entity < _entity_start || entity - _entity_start >= _components.size())
            return null_component;

        return _components.at(entity - _entity_start);
    }

    template<typename T>
    Component<T> &Components<T>::insert_front(const Entity &entity)
    {
        if (!_components.size())
            _entity_end = entity;

        _components.insert(_components.begin(), _entity_end - entity + 1 - _components.size(), std::nullopt);
        _entity_start = entity;

        return _components.at(entity - _entity_start);
    }

    template<typename T>
    Component<T> &Components<T>::insert_back(const Entity &entity)
    {
        if (!_components.size())
            _entity_start = entity;

        _components.resize(entity - _entity_start + 1);
        _entity_end = entity;

        return _components.at(entity - _entity_start);
    }

    template<typename T>
    void Components<T>::optimize_front()
    {
        std::size_t index = 0;

        for (auto it = _components.begin(); it != _components.end(); ++it, ++index) {
            if (it->has_value()) {
                if (!index)
                    return;

                _components.erase(_components.begin(), it);
                _entity_start += index;
                return;
            }
        }
        clear();
    }

    template<typename T>
    void Components<T>::optimize_back()
    {
        std::size_t index = _components.size() - 1;

        for (auto it = _components.end(); it != _components.begin(); --it, --index) {
            if (it->has_value()) {
                if (index == _components.size() - 1)
                    return;

                _components.erase(it + 1, _components.end());
                _entity_end = index;
                return;
            }
        }
        clear();
    }
}
