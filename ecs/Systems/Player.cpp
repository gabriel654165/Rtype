#include "Registry.hpp"

static const float DIAGONAL_FORCE_FACTOR = 0.70710678118; // sin(45)

void player_system([[maybe_unused]] Registry &r, 
                    sparseArray<Velocity> &velArr,
                    sparseArray<Controllable> &ctrlArr,
                    [[maybe_unused]] sparseArray<Animations> &animArr,
                   [[maybe_unused]] sparseArray<Spawner> &spawnArr,
                   [[maybe_unused]] sparseArray<Power> &powerArr
)
{
    auto player = *r.get_tagged_entities("player").begin();
    const auto &keys = ctrlArr[player]->keysPressed;

    //si ils ont pas recu les dernieres directions
    velArr[player]->y = (keys[sf::Keyboard::Down] - keys[sf::Keyboard::Up]) * velArr[player]->speed;
    velArr[player]->x = (keys[sf::Keyboard::Right] - keys[sf::Keyboard::Left]) * velArr[player]->speed;

    if (velArr[player]->x && velArr[player]->y) {
        velArr[player]->x *= DIAGONAL_FORCE_FACTOR;
        velArr[player]->y *= DIAGONAL_FORCE_FACTOR;
    }

    powerArr[player]->buff = keys[sf::Keyboard::C] * powerArr[player]->defaultBuff;
    //spawnArr[player]->active = keys[sf::Keyboard::Space];
}