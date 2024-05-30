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
mce::Scene scene = mce::Scene();
```

With that scene, you can create and delete some worlds:
```c++
mce::World *world = scene.createWorld();
scene.destroyWorld(world);
```

All entities objects is stored inside a world:
```cpp
mce::Entity entity = world->createEntity();
```

And you can access on their components:
```cpp
mce::Component<int> &int_component = world->getComponent(entity);
mce::Components<int> &int_components = world->getComponents();
world->removeComponent<int>(entity);
```

A component can be anything, a built-in types, or a structure/class that contain a special methods:
```cpp
using ExampleComponent = struct ExampleComponent
{
    // Called at the creation of the component
    void init(mce::World *world, mce::Entity entity)
    { }
};
```

You can register your own custom methods. A custom method is a method who can be called by the engine. There are only one rule to register it correctly. It must return ``void``.
For example there is how we can register a method named ``update`` and it take a ``double`` parameter for the delta time. So their prototype is
```cpp
void update(double delta_time);
```
```cpp
// unique identifier for the 'update' method. It must be an integer.
#define UPDATE_METHOD_ID 0

// Concept to check if the type contain the 'update' method
template<typename T>
concept HasUpdate = mce::HasCustomMethod<T, &T::update, double>;

namespace mce
{
    // Register all custom methods (it called when we register a new component)
    template<typename T>
    void registerCustomMethods(World *world)
    {
        // Check if the type contain the 'update' method
        if constexpr(HasUpdate<T>)
            // Register the 'update' method
            world->registerCustomMethod<T, &T::update, double>(UPDATE_METHOD_ID);
    }

    // Unregister all custom methods (it called when we unregister a component)
    template<typename T>
    void unregisterCustomMethods(World *world)
    {
        // Check if the type contain the 'update' method
        if constexpr(HasUpdate<T>)
            // Unregister the 'update' method
            world->unregisterCustomMethod<T, &T::update, double>(UPDATE_METHOD_ID);
    }
}
```
We can now call this method like this:
```cpp
scene.launchCustomMethod(UPDATE_METHOD_ID, 3.14);
```
It execute the ``update`` method on all components in order of component registration.
