////////////////////////
//
//  Created: Wed May 01 2024
//  File: world.inl
//
////////////////////////

#include <algorithm>
#include <thread>
#include "mce/world.hpp"

namespace mce
{
    template<typename ... ARGS>
    bool World::launchCustomMethod(std::size_t id, ARGS &&... args)
    {
        auto methods = _custom_methods_with_args.find(id);

        if (methods == _custom_methods_with_args.end())
            return false;

        for (auto &method: std::any_cast<Methods<World, void, ARGS...> &>(methods->second))
            (this->*method)(std::forward<ARGS>(args)...);

        return true;
    }

    template<typename T, typename ... ARGS>
    T *World::addComponent(const Entity &entity, ARGS &&... args)
    {
        Components<T> &components = getComponents<T>();

        if constexpr(HasApplyRequiredComponents<T>)
            components.get(entity)->applyRequiredComponents(*this, entity);

        if (components.contain(entity))
            return components.get(entity);

        components.insertEntity(entity, std::forward<ARGS>(args)...);

        if constexpr(HasInit<T>)
            components.get(entity)->init(*this, entity);

        return components.get(entity);
    }

    template<typename T>
    bool World::hasComponent(const Entity &entity)
    {
        return getComponent<T>(entity) != nullptr;
    }

    template<typename T>
    void World::requestRemoveComponent(const Entity &entity, bool force)
    {
        RequestRemoveComponent request = RequestRemoveComponent();
        request.request = &World::removeComponent<T>;
        request.entity = entity;
        request.force = force;

        _remove_component_requests.push_back(request);
    }

    template<typename T>
    inline T *World::getComponent(const Entity &entity)
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

        if constexpr(HasInitDependency<T>)
            T::template initDependency<T>(*this);

        _components.insert({std::type_index(typeid(T)), Components<T>()});
        _remove_component_methods.push_back(&World::removeComponent<T>);

        registerCustomMethods<T>(*this);
    }

    template<typename T>
    void World::requestUnregisterComponent()
    {
        RequestUnregisterComponent request = RequestUnregisterComponent();
        request.request = &World::unregisterComponent<T>;

        _unregister_component_requests.push_back(request);
    }

    template<typename T, auto M>
    void World::registerCustomMethod(std::size_t id)
    {
        auto methods = _custom_methods_without_args.find(id);

        if (methods == _custom_methods_without_args.end()) {
            _custom_methods_without_args.insert({id, Methods<World, void>()});
            methods = _custom_methods_without_args.find(id);
        }

        methods->second.push_back(&World::executeMethod<T, M>);
    }

    template<typename T, auto M>
    void World::unregisterCustomMethod(std::size_t id)
    {
        auto methods = _custom_methods_without_args.find(id);

        if (methods == _custom_methods_without_args.end())
            return;

        methods->second.erase(
            std::remove_if(methods->second.begin(), methods->second.end(),
                [&](Method<World, void> method) {
                    return method == &World::executeMethod<T, M>;
                }
            ),
            methods->second.end()
        );

        if (!methods->second.size())
            _custom_methods_without_args.erase(methods);
    }

    template<typename T, auto M, typename ... ARGS>
    std::enable_if_t<(sizeof...(ARGS) > 0), void> World::registerCustomMethod(std::size_t id)
    {
        auto methods = _custom_methods_with_args.find(id);

        if (methods == _custom_methods_with_args.end()) {
            _custom_methods_with_args.insert({id, Methods<World, void, ARGS...>()});
            methods = _custom_methods_with_args.find(id);
        }

        std::any_cast<Methods<World, void, ARGS...> &>(methods->second).push_back(&World::executeMethod<T, M, ARGS...>);
    }

    template<typename T, auto M, typename ... ARGS>
    std::enable_if_t<(sizeof...(ARGS) > 0), void> World::unregisterCustomMethod(std::size_t id)
    {
        auto methods = _custom_methods_with_args.find(id);

        if (methods == _custom_methods_with_args.end())
            return;

        Methods<World, void, ARGS...> &container = std::any_cast<Methods<World, void, ARGS...> &>(methods->second);

        container.erase(
            std::remove_if(container.begin(), container.end(),
                [&](Method<World, void, ARGS...> method) {
                    return method == &World::executeMethod<T, M, ARGS...>;
                }
            ),
            container.end()
        );

        if (!container.size())
            _custom_methods_with_args.erase(methods);
    }

    template<typename T, auto M, typename ... ARGS>
    void World::executeMethod(ARGS &&... args)
    {
        Components<T> &components = getComponents<T>();

        for (T &component: components)
            (component.*M)(std::forward<ARGS>(args)...);
    }

    template<typename T, typename REQUIRED>
    void World::initDependency()
    {
        auto components_dependency = _components_dependency.find(std::type_index(typeid(REQUIRED)));

        if (components_dependency == _components_dependency.end()) {
            _components_dependency.insert({std::type_index(typeid(REQUIRED)), ComponentDependency()});
            components_dependency = _components_dependency.find(std::type_index(typeid(REQUIRED)));
        }

        components_dependency->second.contain_main_components.push_back(&World::hasComponent<T>);
        components_dependency->second.remove_main_components.push_back(&World::removeComponent<T>);
    }

    template<typename T, typename REQUIRED>
    void World::removeDependency()
    {
        auto components_dependency = _components_dependency.find(std::type_index(typeid(REQUIRED)));

        if (components_dependency == _components_dependency.end())
            return;

        ComponentDependency &dependency = components_dependency->second;

        dependency.contain_main_components.erase(
            std::remove_if(dependency.contain_main_components.begin(), dependency.contain_main_components.end(),
                [&](Method<World, bool, const Entity &> method) {
                    return method == &World::hasComponent<T>;
                }
            ),
            dependency.contain_main_components.end()
        );

        dependency.remove_main_components.erase(
            std::remove_if(dependency.remove_main_components.begin(), dependency.remove_main_components.end(),
                [&](Method<World, void, const Entity &, bool &&> method) {
                    return method == &World::removeComponent<T>;
                }
            ),
            dependency.remove_main_components.end()
        );

        if (!dependency.contain_main_components.size() && !dependency.remove_main_components.size())
            _components_dependency.erase(components_dependency);
    }

    template<typename T>
    void World::unregisterComponent()
    {
        auto components = _components.find(std::type_index(typeid(T)));

        if (components == _components.end())
            return;

        if constexpr(HasRemoveDependency<T>)
            T::template removeDependency<T>(*this);

        _components.erase(std::type_index(typeid(T)));
        _remove_component_methods.erase(
            std::remove_if(_remove_component_methods.begin(), _remove_component_methods.end(),
                [&](Method<World, void, const Entity &, bool &&> method) {
                    return method == &World::removeComponent<T>;
                }),
            _remove_component_methods.end()
        );

        unregisterCustomMethods<T>(*this);
    }

    template<typename T>
    void World::removeComponent(const Entity &entity, bool &&force)
    {
        Components<T> &components = getComponents<T>();

        if (!components.contain(entity)) {
            if (!components.size())
                requestUnregisterComponent<T>();

            return;
        }

        auto components_dependency = _components_dependency.find(std::type_index(typeid(T)));

        if (components_dependency != _components_dependency.end()) {
            if (force)
                for (auto &remove_main_component: components_dependency->second.remove_main_components)
                    (this->*remove_main_component)(entity, true);
            else
                for (auto &contain_main_component: components_dependency->second.contain_main_components)
                    if ((this->*contain_main_component)(entity))
                        return;
        }

        components.remove(entity);

        if (!components.size())
            requestUnregisterComponent<T>();
    }
}
