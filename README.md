# game-engine

## Installation
Compile the lib
```sh
make
```

Compile on debug mode
```sh
make debug
```

Compile and launch tests
```sh
make tests_run
```

## Usage
Firstly you need to create a ``scene`` object:
```cpp
engine::Scene scene = engine::Scene();
```

With that scene, you can create and delete some worlds:
```c++
engine::World *world = scene.createWorld();
scene.destroyWorld(world);
```

All entities objects is stored inside a world:
```cpp
engine::Entity entity = world->createEntity();
```

And you can access on their components:
```cpp
engine::Component<int> &int_component = world->getComponent(entity);
engine::Components<int> &int_components = world->getComponents();
world->removeComponent<int>(entity);
```

A component can be anything, a built-in types, or a structure/class that contain a special methods:
```cpp
using ExampleComponent = struct ExampleComponent
{
    // Called at the creation of the component
    void init(engine::World *world, engine::Entity entity)
    { }
};
```

You can register your own custom methods. For example, there is the way to create a ``update`` custom method. A custom method must have this prototype:
```cpp
void MyComponent::method();
```
```cpp
// unique identifier for the 'update' method
#define UPDATE_METHOD_ID 0

// Concept to check if the type contain the 'update' method
template<typename T>
concept HasUpdate = engine::HasCustomMethod<T, &T::update>;

namespace engine
{
    // Register all custom methods (it called when we register a new component)
    template<typename T>
    void registerCustomMethods(World *world)
    {
        // Check if the type contain the 'update' method
        if constexpr(HasUpdate<T>)
            // Register the 'update' method
            world->registerCustomMethod<T, &T::update>(UPDATE_METHOD_ID);
    }

    // Unregister all custom methods (it called when we unregister a component)
    template<typename T>
    void unregisterCustomMethods(World *world)
    {
        // Check if the type contain the 'update' method
        if constexpr(HasUpdate<T>)
            // Unregister the 'update' method
            world->unregisterCustomMethod<T, &T::update>(UPDATE_METHOD_ID);
    }
}
```
We can now call this method like this:
```cpp
scene.launchCustomMethod(UPDATE_METHOD_ID);
```
