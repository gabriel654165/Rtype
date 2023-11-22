//
// Created by gabi on 07/11/2021.
//

#pragma once

#include <SFML/Graphics.hpp>
#include <any>

struct BulletInfo {
    enum Path {
        STRAIGHT,
        PATH_COUNT
    } path;

    sf::Vector2f velocity;
    std::size_t speed;
    int damage;
};

struct Spawner {
    enum Type {
        MOB = 0,
        BULLET,
        TYPE_COUNT
    } type;

    Spawner(BulletInfo &info, Spawner::Type tpe = Type::BULLET) : type(tpe), infos(info) {};
    Spawner(Spawner::Type tpe, std::any fct = std::any() ) : type(tpe), infos(fct){};

    bool active{};
    std::size_t spawnedCount{};
    double timeElapsed{};
    double timeToWait{};

    sf::Vector2f position;
    sf::Vector2f offset;

    std::any infos;
};
