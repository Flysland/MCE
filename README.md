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

    // Called when we call 'scene.update()'
    void update()
    { }
};
```
