#include "Registry.hpp"
#include "Components.hpp"

void registerComponents(Registry &registry) 
{
    registry.register_component<Position>();
    registry.register_component<Velocity>();
    registry.register_component<Sprite>();
    registry.register_component<Controllable>();
    registry.register_component<IU>();
    registry.register_component<Animations>();
    registry.register_component<Collider>();
    registry.register_component<Scale>();
    registry.register_component<Follow>();
    registry.register_component<Spawner>();
    registry.register_component<Uuid>();
    registry.register_component<Container<std::size_t>>();
    registry.register_component<Container<bool>>();
    registry.register_component<AudioSource>();
    registry.register_component<Parent>();
    registry.register_component<BeamBonusInfo>();
    registry.register_component<Power>();
    registry.register_component<Health>();
    registry.register_component<Level>();
}