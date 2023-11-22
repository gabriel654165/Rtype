#include "Registry.hpp"
#include "Graphic/Graphics.hpp"
#include "sparseArray.hpp"
#include <cmath>

void movement_system([[maybe_unused]] Registry &r, Resource::Graphics &rsc, sparseArray<Position> &ctrlPos, sparseArray<Velocity> &ctrlVel, sparseArray<Sprite> &spriteArr, sparseArray<Scale> &scaleArr)
{
    //il faut etre 1 action dans le passé
    double deltaTime = rsc.getDeltaTime();

    for (const auto &entity : r.getScene().entities) {
        if (!ctrlPos[entity].has_value() || !spriteArr[entity].has_value())
            continue;
        if (scaleArr[entity].has_value())
            spriteArr[entity]->sprite.setScale(scaleArr[entity]->x, scaleArr[entity]->y);
        if (ctrlVel[entity].has_value()) {
            ctrlPos[entity]->x += ctrlVel[entity]->x * deltaTime;
            ctrlPos[entity]->y += ctrlVel[entity]->y * deltaTime;
        }
        spriteArr[entity]->sprite.setPosition(ctrlPos[entity]->x, ctrlPos[entity]->y);
    }
}