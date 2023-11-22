/*
** EPITECH PROJECT, 2021
** CPP-RType
** File description:
** Spawner
*/

#include "EntityLoad.hpp"
#include "Registry.hpp"
#ifdef CLIENT
#include "Graphics.hpp"
#else
#include "MyClock.hpp"
#endif

#ifdef CLIENT
Registry::entity_t spawnMob(Registry &game, [[maybe_unused]]Resource::Graphics &rsc, [[maybe_unused]]Spawner &spawner, [[maybe_unused]]std::size_t parent, [[maybe_unused]]EntityLoad &loader)
#else
Registry::entity_t spawnMob(Registry &game, [[maybe_unused]]MyClock &clock, [[maybe_unused]]Spawner &spawner, [[maybe_unused]]std::size_t parent, EntityLoad &loader)
#endif
{
    size_t entityId = 0;
    std::cout << "Spawn random Entity\n";
#ifndef CLIENT
    std::cout << "MORE Spawn random Entity\n";
    entityId = loader.spawnRandomEntity(game);
#endif
    return entityId;
}

#ifdef CLIENT
Registry::entity_t spawnBullet(Registry &game, Resource::Graphics &rsc, Spawner &spawner, std::size_t parent)
#else
Registry::entity_t spawnBullet(Registry &game, [[maybe_unused]] MyClock &clock, Spawner &spawner, std::size_t parent)
#endif
{
    auto bullet = game.spawn_entity();
    auto &infos = std::any_cast<BulletInfo &>(spawner.infos);
    game.add_entity_to_scene(bullet, "game");
    game.tag_entity(bullet, "bullet");

    auto posVector = spawner.offset + spawner.position;
    game.emplace_component<Velocity>(bullet, infos.velocity.x , infos.velocity.y, infos.speed);
    game.emplace_component<Position>(bullet, posVector.x, posVector.y);
    game.emplace_component<Collider>(bullet, RectangleCollider{.width = 17, .height = 12});
    game.emplace_component<Parent>(parent);
#ifdef CLIENT
    game.emplace_component<Animations>(bullet);
    game.emplace_component<Scale>(bullet, 2, 2);
    auto &audioSource = game.emplace_component<AudioSource>(bullet);
    audioSource->sound.setBuffer(rsc.getSound(Resource::Graphics::Sounds::SHOOT));
    audioSource->sound.play();
    auto &bulletTexture  = rsc.getTexture(Resource::Graphics::Textures::EFFECTS1);
    auto &bulletDrawable = game.emplace_component<Sprite>(bullet);
    bulletDrawable->sprite.setTexture(bulletTexture);
    bulletDrawable->sprite.setTextureRect(sf::IntRect(215, 85, 17, 12));

    auto &bulletAnimations = game.get_components<Animations>()[bullet];
    bulletAnimations->animations.emplace("Bullet pfiou",
        AnimationData{
            .texture = bulletTexture,
            .nextFrameOffset = {17, 0},
            .firstFramePosition = {215, 85},
            .frames = 3,
            .timeStep = 0.025,
        });
    bulletAnimations->currentAnimation = bulletAnimations->animations.begin();
    bulletAnimations->active = true;
#endif
    return bullet;
}

#ifdef CLIENT
void spawner_system(
    [[maybe_unused]] Registry &r,
    Resource::Graphics &rsc,
    sparseArray<Spawner> &spawnArr,
    EntityLoad &loader
)
#else
void spawner_system(
    [[maybe_unused]] Registry &r,
    MyClock &clock,
    sparseArray<Spawner> &spawnArr,
    EntityLoad &loader
)
#endif
{
    for (auto id: r.getScene().entities) {
        if (!spawnArr[id].has_value() || !spawnArr[id]->active)
            continue;
#ifdef CLIENT
        spawnArr[id]->timeElapsed += rsc.getDeltaTime();
#else
        spawnArr[id]->timeElapsed += clock.getElapsedTime();
#endif
        if (spawnArr[id]->timeElapsed < spawnArr[id]->timeToWait)
            return;
        else
            spawnArr[id]->timeElapsed = 0;
        if (spawnArr[id]->type == Spawner::BULLET) {
#ifdef CLIENT
            spawnBullet(r, rsc, *spawnArr[id], id);
#else
            spawnBullet(r, clock, *spawnArr[id], id);
#endif
        } else if (spawnArr[id]->type == Spawner::MOB) {
#ifdef CLIENT
            spawnMob(r, rsc, *spawnArr[id], id, loader);
#else
            spawnMob(r, clock, *spawnArr[id], id, loader);
#endif
        }
    }
}
