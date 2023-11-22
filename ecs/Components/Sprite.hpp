/*
** EPITECH PROJECT, 2021
** CPP-RType
** File description:
** Sprite
*/

#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>

struct Sprite {
    Sprite() {};

    Sprite(const Sprite &_sprite) : sprite(_sprite.sprite), color(_sprite.color) {};

    Sprite(const sf::Sprite &_sprite, const sf::Color &_color) : sprite(_sprite), color(_color) {};

    sf::Sprite sprite;
    sf::Color color;
};

/* test
struct Sprite {
    public:
        std::string pathAsset;//font or texture
        sf::Color color;
};

struct Sprite: public Sprite {
public:
    sf::Sprite sprite;
    sf::IntRect texture_rect;
};


struct IU: public Sprite {
public:
    sf::Text text;
};*/

//reinterpret_cast<std::optional<Sprite> &>(playerDrawable)->sprite.setTexture(playerDrawable->pathAsset);
//reinterpret_cast<std::optional<Sprite> &>(playerDrawable)->texture_rect = reinterpret_cast<std::optional<Sprite> &>(playerDrawable)->sprite.getTextureRect();


