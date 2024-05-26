////////////////////////
//
//  Created: Wed May 01 2024
//  File: world.inl
//
////////////////////////

#include <algorithm>
#include "engine/world.hpp"

namespace engine
{
    inline const std::size_t &World::getID() const
    {
        return _id;
    }

    template<typename T, typename ... ARGS>
    Component<T> &World::addComponent(const Entity &entity, ARGS &&... args)
    {
        Components<T> &components = getComponents<T>();

        components.insert_entity(entity);

        if (components.get(entity).has_value())
            return components.get(entity);

        components.get(entity).emplace(std::forward<ARGS>(args)...);

        if constexpr(HasInit<T>)
            components.get(entity)->init(this, entity);

        return components.get(entity);
    }

    template<typename T>
    inline void World::requestRemoveComponent(const Entity &entity)
    {
        _remove_component_requests.push_back({entity, &World::removeComponent<T>});
    }

    template<typename T>
    inline Component<T> &World::getComponent(const Entity &entity)
    {
        return getComponents<T>().get(entity);
    }

    template<typename T>
    Components<T> &World::getComponents()
    {
        auto components = _components.find(std::type_index(typeid(T)));

        if (components == _components.end()) {
            registerComponent<T>();
            return getComponents<T>();
        }

        return std::any_cast<Components<T> &>(components->second);
    }

    template<typename T>
    void World::registerComponent()
    {
        auto components = _components.find(std::type_index(typeid(T)));

        if (components != _components.end())
            return;

        _components.insert({std::type_index(typeid(T)), Components<T>()});
        _remove_component_methods.push_back(&World::requestRemoveComponent<T>);

        registerCustomMethods<T>(this);
    }

    template<typename T>
    void World::unregisterComponent()
    {
        auto components = _components.find(std::type_index(typeid(T)));

        if (components == _components.end())
            return;

        _components.erase(std::type_index(typeid(T)));
        _remove_component_methods.erase(
            std::remove_if(_remove_component_methods.begin(), _remove_component_methods.end(),
                [&](void (World::*method)(const Entity &entity)) {
                    return method == &World::requestRemoveComponent<T>;
                }),
            _remove_component_methods.end()
        );

        unregisterCustomMethods<T>(this);
    }

    template<typename T, auto M, typename ... ARGS>
    void World::registerCustomMethod(std::size_t id)
    {
        auto methods = _custom_methods.find(id);

        if (methods == _custom_methods.end()) {
            _custom_methods.insert({id, MethodContainer<World, void, ARGS...>()});
            methods = _custom_methods.find(id);
        }

        auto &container = std::any_cast<MethodContainer<World, void, ARGS...> &>(methods->second);
        container.push_back(static_cast<Method<World, void, ARGS...>>(&World::template executeMethod<T, M, ARGS...>));
    }

    template<typename T, auto M, typename ... ARGS>
    void World::unregisterCustomMethod(std::size_t id)
    {
        auto methods = _custom_methods.find(id);

        if (methods == _custom_methods.end())
            return;

        MethodContainer<World, void, ARGS...> &container = std::any_cast<MethodContainer<World, void, ARGS...> &>(methods->second);

        container.erase(
            std::remove_if(container.begin(), container.end(),
                [&](Method<World, void, ARGS...> method) {
                    return method == &World::executeMethod<T, M, ARGS...>;
                }),
            container.end()
        );

        if (!container.size())
            _custom_methods.erase(methods);
    }

    template<typename ... ARGS>
    void World::launchCustomMethod(std::size_t id, ARGS &&... args)
    {
        auto methods = _custom_methods.find(id);

        if (methods == _custom_methods.end())
            return;

        for (auto &method: std::any_cast<MethodContainer<World, void, ARGS...> &>(methods->second))
            (this->*method)(std::forward<ARGS>(args)...);
    }

    inline void World::destroyEntity(const Entity &entity)
    {
        _available_entities.push_back(entity);
    }

    template<typename T, auto M, typename ... ARGS>
    void World::executeMethod(ARGS &&... args)
    {
        Components<T> &components = getComponents<T>();

        for (Component<T> &component: components) {
            if (!component.has_value())
                continue;

            (component.value().*M)(std::forward<ARGS>(args)...);
        }
    }

    template<typename T>
    void World::removeComponent(const Entity &entity)
    {
        Components<T> &components = getComponents<T>();

        if (!components.contain(entity) || !components.get(entity).has_value())
            return;

        components.get(entity).reset();
        components.optimize();

        if (!components.size())
            unregisterComponent<T>();
    }
}
