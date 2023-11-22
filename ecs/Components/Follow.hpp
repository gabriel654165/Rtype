/*
** EPITECH PROJECT, 2021
** CPP-RType
** File description:
** Follow
*/

#pragma once

#include <cstddef>
#include <SFML/System/Vector2.hpp>

struct Follow {
    enum Type {
        PRECISE,
        SMOOTH
    } type{};

    bool active{};
    std::size_t following{};
    sf::Vector2f offset{};
};
