
# R-Type, Game Developer Documentation
## Game Description

***Read the ECS documentation to have a better comprehension***

The game is split into two parts, the Server side and the Client side.<br>
Both parts are relatively similar.

## Server Side
<div style="margin-left: 5%;">

### How to add an entity

To add a new entity, you have to spawn the entity in the function spawnEntities.<br>
In the ```_stringToEntity``` function, you must place the name of your entity (which will be useful later).
```c++
void game::RtypeGameServer::spawnEntities() {
    _stringToEntity[BACKGROUND_NAME] = _reg.spawnEntity();
    _stringToEntity[PLAYER_NAME] = _reg.spawnEntity();
    _stringToEntity[PLAYER_SHOT] = _reg.spawnEntity();
    _stringToEntity[SPAWN_ENEMY_NAME] = _reg.spawnEntity();
    _stringToEntity[ENEMY_SHOT] = _reg.spawnEntity();
    _stringToEntity[SPAWN_BOSS_NAME] = _reg.spawnEntity();
    _stringToEntity[MUSIC_NAME] = _reg.spawnEntity();
}
```

### How to add components
We also want to add some components to our game. To do so, you have to register your new component in the ```registerComponents``` function by adding a new line with the component to add.
```c++
void game::RtypeGameServer::registerComponents() {
    _reg.registerComponent<ecs::component::Position>();
    _reg.registerComponent<ecs::component::Velocity>();
    _reg.registerComponent<ecs::component::Shooting>();
    _reg.registerComponent<ecs::component::Type>();
}
```

### Insert entities to our components
Now we want to add some components to our new entity. To do so, you have to go into the ```addComponentsToEntities``` function.<br><br>
We have to get our component with this line:
```c++
ecs::SparseArray<ecs::component::Type> &types = _reg.getComponents<ecs::component::Type>();
```
And then we have to insert an entity into our component. To do so, we have to add this line:
``` c++
types.insertAt(_stringToEntity[BACKGROUND_NAME], ecs::component::Type{ecs::EntityType::BACKGROUND});
```
The ```types``` is our component, and we insert an entity into it (the entity has to be spawned in ```spawnEntities```, see above) with ```_stringToEntity[BACKGROUND_NAME]```, and we fill our component with data (here it is ```ecs::EntityType::BACKGROUND```), which is the content of our component.<br>
A single entity can have many components, and a component can have many entities.<br><br>
### Add game systems
Now that we have our entities and components well filled, we have to call our system, which will handle the game logic.<br>
To add and call our system, we need to add it to the Registry in the function ```addSystemsToRegistry```. To do so, we have to add the following line in this function:
```c++
_reg.addSystem(ecs::system::FiringSystem());
```
> [!CAUTION]
> **Be careful of the order of your system**<br>

</div>

## Client Side

<div style="margin-left: 5%;">

**The client side is way easier than the server side**<br><br>
You only have to register your components in ```registerComponents``` function and add your systems to the registry in the function ```addSystemsToRegistry``` <br><br>

> [!NOTE]
> Both function works the same way as the server side