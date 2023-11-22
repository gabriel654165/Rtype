/*
** EPITECH PROJECT, 2021
** B-CPP-501-PAR-5-1-rtype-maxime.dodin
** File description:
** Graphics.h
*/
#pragma once

#include <iostream>
#include <any>
#include <list>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>

#include "MyClock.hpp"

namespace Resource {
    class Graphics {
    public:

        enum Font {
            FONT1 = 0,
        };

        enum Textures {
            PLAYERS = 0,
            EFFECTS1,
            EFFECTS2,
            EFFECTS3,
            EFFECTS4,
            EFFECTS5,
            PARALAXE,
            BUTTON_EMPTY,
            RTYPE_LOGO,
            LOGO_BACKGROUND,
            BASIC_ENEMIE,
            ADVANCE_ENEMIES,
            ASTRONAUT,
            MDR,
            TEXTURES_COUNT
        };

        enum Musics {
            GAME = 0,
            MENU,
            MUSIC_NUMBER
        };


        enum Sounds {
            EA_SPORT = 0,
            SHOOT,
            MENU_MUSIC,
            GAME_MUSIC,
            BOOM,
            SOUNDS_NUMBER,
       };

        Graphics();

        Graphics(unsigned int x, unsigned int y);

        Graphics(Graphics &copy);

        ~Graphics();

        sf::RenderWindow &getWindow();

        sf::Event &getEvents();

        sf::Font &getFont(Font &&);

        sf::View &getCamera();

        MyClock &getClock();

        void resetClock();
        double &getDeltaTime();

        void setCameraSize(float x, float y);
        void setCameraCenter(float x, float y);

        sf::SoundBuffer &getSound(Sounds &&);
        sf::SoundBuffer &addSound(const std::string &path);

        const std::vector<sf::Texture> &getTextures() const;
        sf::Texture &addTexture(const std::string &path, const sf::IntRect &lol = sf::IntRect());
        sf::Texture &getTexture(Textures &&);
        sf::Sound &getMusicSource();
        void setMusic(const Sounds &sound);

        void clear();

    private:
        sf::Event _eventSystem{};
        sf::RenderWindow _window;
        MyClock _clock;
        sf::View _camera;
        std::vector<sf::Font> _font;
        std::vector<sf::Texture> _textures;


        sf::Sound _musicSource;
        std::vector<sf::SoundBuffer> _sounds;


    };
};
