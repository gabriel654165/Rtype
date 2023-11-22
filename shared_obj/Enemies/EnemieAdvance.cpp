#include "EnemieAdvance.hpp"

#ifdef CLIENT
    EnemieAdvance::EnemieAdvance(Registry &game) : Shared::AComponents(game, Resource::Graphics::Textures::ADVANCE_ENEMIES) {};
#else
    EnemieAdvance::EnemieAdvance(Registry &game) : Shared::AComponents(game) {};
#endif

EnemieAdvance::~EnemieAdvance() {};

std::size_t EnemieAdvance::spawnEntity() noexcept
{
    auto enemies = _game.spawn_entity();
    _game.tag_entity(enemies, "enemies");
#ifdef CLIENT
    _game.add_entity_to_scene(enemies, "game");
#else
    _game.add_entity_to_scene(enemies, "default");
#endif
    _game.emplace_component<Velocity>(enemies, -50, 0, 0);
    _game.emplace_component<Position>(enemies, 1800, 300);
    _game.emplace_component<Scale>(enemies, 2, 2);
    _game.emplace_component<Collider>(enemies, RectangleCollider{.width = 34, .height = 38});
#ifdef CLIENT
        _animationProper.nextFrameOffset = {34, 0};
        _animationProper.firstFramePosition = sf::Vector2f(0, 0);
        _animationProper.frames = 8;
        _animationProper.timeStep = 0.12;
        _animationProper.active = true;
        _animationProper.loop = true;
        _rectProper.rectLeft = 0;
        _rectProper.rectTop = 0;
        _rectProper.rectHeight = 36;
        _rectProper.rectWidth = 33;
#endif
    return enemies;
}

extern "C" std::shared_ptr<Shared::AComponents> getComponent(Registry &game)
{
    return std::shared_ptr<Shared::AComponents>(new EnemieAdvance(game));
}