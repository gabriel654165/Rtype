//
// Created by spriters on 12/11/2021.
//

#ifndef B_CPP_501_PAR_5_1_RTYPE_MAXIME_DODIN_ANIMATIONS_HPP
#define B_CPP_501_PAR_5_1_RTYPE_MAXIME_DODIN_ANIMATIONS_HPP

#include <string>
#include <map>
#include <memory>
#include <SFML/Graphics.hpp>

struct AnimationData {
    sf::Texture &texture;
    sf::Vector2<float> nextFrameOffset{};
    sf::Vector2<float> firstFramePosition{};
    std::size_t frames{};
    std::size_t currentFrame{};

    double timeStep{};
    double elapsedTime{};
};

struct Animations {
    public:
        Animations() : active(false) {};

        Animations(const Animations &_animations) : animations(_animations.animations), currentAnimation(_animations.currentAnimation), active(false) {};

        Animations(std::map<std::string, AnimationData> &_animations) : animations(_animations), currentAnimation(_animations.begin()), active(false) {};

        std::map<std::string, AnimationData> animations{};
        std::map<std::string, AnimationData>::iterator currentAnimation{};
        
        bool active{};
        bool loop{};
        bool nextFrameStopAnimation{};
};


#endif //B_CPP_501_PAR_5_1_RTYPE_MAXIME_DODIN_ANIMATIONS_HPP
