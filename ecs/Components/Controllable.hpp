//
// Created by gabi on 07/11/2021.
//

#pragma once

#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <array>

struct Controllable {
public:
    std::array<bool, sf::Keyboard::KeyCount> keysPressed;
    bool keyboardPressed;

    sf::Mouse::Button mouseInputs = sf::Mouse::ButtonCount;
    sf::Vector2f mousePosition = {0, 0};
    bool isMousePressed = false;
    bool isClicked = false;
};
