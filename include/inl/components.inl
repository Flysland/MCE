////////////////////////
//
//  Created: Sun May 12 2024
//  File: components.inl
//
////////////////////////

#include "mce/components.hpp"

namespace mce
{
    template<typename T>
    Components<T>::Components()
        : _components()
        , _map()
    { }

    template<typename T>
    template<typename ... ARGS>
    T *Components<T>::insertEntity(const Entity &entity, ARGS &&... args)
    {
        auto it = _map.find(entity);

        if (it != _map.end())
            return &_components[it->second];

        _map.insert({entity, _components.size()});
        _components.emplace_back(std::forward<ARGS>(args)...);
        return &_components.back();
    }

    template<typename T>
    inline std::size_t Components<T>::size() const
    {
        return _components.size();
    }

    template<typename T>
    inline bool Components<T>::contain(const Entity &entity) const
    {
        return _map.find(entity) != _map.end();
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
    void Components<T>::clear()
    {
        _components.clear();
        _map.clear();
    }

    template<typename T>
    void Components<T>::remove(const Entity &entity)
    {
        auto entity_it = _map.find(entity);

        if (entity_it == _map.end())
            return;

        for (auto it = std::next(entity_it); it != _map.end(); ++it)
            it->second -= 1;

        _components.erase(_components.begin() + entity_it->second);
        _map.erase(entity_it);
    }

    template<typename T>
    T *Components<T>::at(std::size_t index)
    {
        if (size() <= index)
            return nullptr;

        return &_components[index];
    }

    template<typename T>
    const T *Components<T>::at(std::size_t index) const
    {
        if (size() <= index)
            return nullptr;

        return &_components[index];
    }

    template<typename T>
    T *Components<T>::get(const Entity &entity)
    {
        auto it = _map.find(entity);

        if (it == _map.end() || it->second >= _components.size())
            return nullptr;

        return &_components[it->second];
    }

    template<typename T>
    const T *Components<T>::get(const Entity &entity) const
    {
        auto it = _map.find(entity);

        if (it == _map.end() || it->second >= _components.size())
            return nullptr;

        return &_components[it->second];
    }
}
