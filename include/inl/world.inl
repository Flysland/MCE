////////////////////////
//
//  Created: Wed May 01 2024
//  File: world.inl
//
////////////////////////

#include <algorithm>
#include <thread>
#include <iostream>
#include "mce/world.hpp"

namespace mce
{
    inline void World::setMaxThreads(std::size_t max_thread)
    {
        std::size_t hardware_threads = std::thread::hardware_concurrency();

        _max_thread = max_thread <= hardware_threads ? max_thread : hardware_threads != 0 ? hardware_threads : 1;
    }

    template<typename ... ARGS>
    bool World::launchCustomMethod(std::size_t id, ARGS &&... args)
    {
        auto methods = _custom_methods_with_args.find(id);

        if (methods == _custom_methods_with_args.end())
            return false;

        for (auto &method: std::any_cast<MethodContainer<World, void, ARGS...> &>(methods->second))
            (this->*method)(std::forward<ARGS>(args)...);

        return true;
    }

    template<typename T, typename ... ARGS>
    T *World::addComponent(const Entity &entity, ARGS &&... args)
    {
        Components<T> &components = getComponents<T>();

        if (components.contain(entity))
            return components.get(entity);

        components.insertEntity(entity, std::forward<ARGS>(args)...);

        if constexpr(HasInit<T>)
            components.get(entity)->init(*this, entity);

        return components.get(entity);
    }

    template<typename T>
    inline void World::requestRemoveComponent(const Entity &entity)
    {
        _remove_component_requests.push_back({entity, &World::removeComponent<T>});
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

        _components.insert({std::type_index(typeid(T)), Components<T>()});
        _remove_component_methods.push_back(&World::requestRemoveComponent<T>);

        registerCustomMethods<T>(*this);
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
                [&](Method<World, void, const Entity &> method) {
                    return method == &World::requestRemoveComponent<T>;
                }),
            _remove_component_methods.end()
        );

        unregisterCustomMethods<T>(*this);
    }

    template<typename T, auto M>
    void World::registerCustomMethod(std::size_t id, bool threaded)
    {
        auto methods = _custom_methods_without_args.find(id);

        if (methods == _custom_methods_without_args.end()) {
            _custom_methods_without_args.insert({id, MethodContainer<World, void>()});
            methods = _custom_methods_without_args.find(id);
        }

        if (threaded)
            methods->second.push_back(&World::executeMethodThreaded<T, M>);
        else
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
    std::enable_if_t<(sizeof...(ARGS) > 0), void> World::registerCustomMethod(std::size_t id, bool threaded)
    {
        auto methods = _custom_methods_with_args.find(id);

        if (methods == _custom_methods_with_args.end()) {
            _custom_methods_with_args.insert({id, MethodContainer<World, void, ARGS...>()});
            methods = _custom_methods_with_args.find(id);
        }

        if (threaded)
            std::any_cast<MethodContainer<World, void, ARGS...> &>(methods->second).push_back(&World::executeMethodThreaded<T, M, ARGS...>);
        else
            std::any_cast<MethodContainer<World, void, ARGS...> &>(methods->second).push_back(&World::executeMethod<T, M, ARGS...>);
    }

    template<typename T, auto M, typename ... ARGS>
    std::enable_if_t<(sizeof...(ARGS) > 0), void> World::unregisterCustomMethod(std::size_t id)
    {
        auto methods = _custom_methods_with_args.find(id);

        if (methods == _custom_methods_with_args.end())
            return;

        MethodContainer<World, void, ARGS...> &container = std::any_cast<MethodContainer<World, void, ARGS...> &>(methods->second);

        container.erase(
            std::remove_if(container.begin(), container.end(),
                [&](Method<World, void, ARGS...> method) {
                    return method == &World::executeMethod<T, M, ARGS...> || method == &World::executeMethodThreaded<T, M, ARGS...>;
                }
            ),
            container.end()
        );

        if (!container.size())
            _custom_methods_with_args.erase(methods);
    }

    inline void World::destroyEntity(const Entity &entity)
    {
        _available_entities.push_back(entity);
    }

    template<typename T, auto M, typename ... ARGS>
    void World::executeMethod(ARGS &&... args)
    {
        Components<T> &components = getComponents<T>();

        for (T &component: components)
            (component.*M)(std::forward<ARGS>(args)...);
    }

    template<typename T, auto M, typename ... ARGS>
    void World::executeMethodThreaded(ARGS &&... args)
    {
        Components<T> &components = getComponents<T>();
        std::vector<std::thread> threads = std::vector<std::thread>();
        auto begin = components.begin();
        auto end = components.end();
        size_t block_size = components.size() / _max_thread;

        threads.reserve(_max_thread);
        auto worker = [&](typename Components<T>::iterator start, typename Components<T>::iterator end) {
            for (auto it = start; it != end; ++it)
                ((*it).*M)(std::forward<ARGS>(args)...);
        };

        auto it = begin;
        for (size_t i = 0; i < _max_thread - 1; ++i) {
            auto block_end = std::next(it, block_size);
            threads.emplace_back(worker, it, block_end);
            it = block_end;
        }
        threads.emplace_back(worker, it, end);

        for (std::thread &t: threads)
            t.join();
    }

    template<typename T>
    void World::removeComponent(const Entity &entity)
    {
        Components<T> &components = getComponents<T>();

        if (!components.contain(entity))
            return;

        components.remove(entity);

        if (!components.size())
            unregisterComponent<T>();
    }
}
