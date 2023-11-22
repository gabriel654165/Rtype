/*
** EPITECH PROJECT, 2021
** CPP-RType
** File description:
** Follow
*/

#include "Registry.hpp"

// ! Not tested

void follow_system([[maybe_unused]] Registry &r, sparseArray<Follow> &followArr, sparseArray<Position> &posArr, sparseArray<Velocity> &velArr)
{
    const float velBrake = 0.5;
    long entity = -1;

    for (auto &followComp: followArr) {
        entity++;
        if (!followComp.has_value() || !posArr[entity].has_value())
            continue;

        if (!posArr[followComp->following].has_value() || !followComp->active)
            continue;
        auto followingPos = posArr[followComp->following];

        followingPos->x += followComp->offset.x;
        followingPos->y += followComp->offset.y;

        auto &followerPos = posArr[entity];

        if (followComp->type == Follow::Type::PRECISE) {
            followerPos = followingPos;
            return;
        }

        const sf::Vector2f distance{
            followingPos->x - followerPos->x, 
            followingPos->y - followerPos->y 
        };

        velArr[entity]->x = distance.x / velBrake;
        velArr[entity]->y = distance.y / velBrake;

    }

}
