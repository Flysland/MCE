# MCE

MCE (Modular Core Engine) is an engine to handle a huge amount of entities.

## Installation
To compile the project
```
cmake -S . -B build
make -C build
```

To compile on debug mode
```
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
make -C build
```

The compilation generate 2 folders:
- bin (that contain all binaries compiled)
- lib (that contain all libraries compiled)

## Usage
MCE use an Entity-Components architecture. An entity can be anything (ex: the player, an enemy, a weapon or an invisible thing like a trigger...). To determine what is the entity, we append to it some components. For example, to create the player entity, we can add this components:
- PlayerMovement (handle inputs to move the player)
- Renderer (display the graphics for the player)
- Collider (interact with other entities)
- ...

> :warning: On MCE, you can add only ONE component type per entity :warning:

Example to create a player entity with MCE
```cpp
// A world store all entities
mce::World world = mce::World();

// We create the player entity
mce::Entity player = world.createEntity();

// We add some components on the player
world.addComponent<PlayerMovement>(player);
world.addComponent<Renderer>(player);
world.addComponent<Collider>(player);
```

Now we have the player entity.
On many Game Engine, there are some specific methods inside the components to update it (ex: start, update, ...). On MCE, you define your own custom methods.
For example we will define 2 custom methods:
- physicUpdate -> called to update all physics components (ex: gravity, colliders, ...)
- renderUpdate -> called to update all render components (ex: 3D model, text, ...)

Only the ``physicUpdate`` methods take a single ``double`` parameter for the delta time between 2 frames. It can take any parameters.

```cpp
// unique identifier for the 'physicUpdate' method.
#define PHYSIC_UPDATE_METHOD_ID 0

// unique identifier for the 'renderUpdate' method.
#define RENDER_UPDATE_METHOD_ID 1

// Concept to check if the type contain the 'physicUpdate' method
template<typename T>
concept HasPhysicUpdate = mce::HasCustomMethod<T, &T::physicUpdate, double>;

// Concept to check if the type contain the 'renderUpdate' method
template<typename T>
concept HasRenderUpdate = mce::HasCustomMethod<T, &T::renderUpdate>;

namespace mce
{
    // Register all custom methods (it called when we register a new component)
    template<typename T>
    void registerCustomMethods(World *world)
    {
        // Check if the type contain the 'physicUpdate' method
        if constexpr(HasPhysicUpdate<T>)
            // Register the 'physicUpdate' method
            world->registerCustomMethod<T, &T::physicUpdate, double>(PHYSIC_UPDATE_METHOD_ID);

        // Check if the type contain the 'renderUpdate' method
        if constexpr(HasRenderUpdate<T>)
            // Register the 'renderUpdate' method
            world->registerCustomMethod<T, &T::renderUpdate>(RENDER_UPDATE_METHOD_ID);
    }

    // Unregister all custom methods (it called when we unregister a component)
    template<typename T>
    void unregisterCustomMethods(World *world)
    {
        // Check if the type contain the 'physicUpdate' method
        if constexpr(HasPhysicUpdate<T>)
            // Unregister the 'physicUpdate' method
            world->unregisterCustomMethod<T, &T::physicUpdate, double>(PHYSIC_UPDATE_METHOD_ID);

        // Check if the type contain the 'renderUpdate' method
        if constexpr(HasRenderUpdate<T>)
            // Register the 'renderUpdate' method
            world->registerCustomMethod<T, &T::renderUpdate>(RENDER_UPDATE_METHOD_ID);
    }
}

```

Now we can call the custom methods

```cpp
// Call the 'physicUpdate' method on all components that contain the method
world.launchCustomMethod(PHYSIC_UPDATE_METHOD_ID, 3.14);

// Call the 'renderUpdate' method on all components that contain the method
world.launchCustomMethod(RENDER_UPDATE_METHOD_ID);
```
