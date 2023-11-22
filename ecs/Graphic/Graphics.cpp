/*
** EPITECH PROJECT, 2021
** B-CPP-501-PAR-5-1-rtype-maxime.dodin
** File description:
** Graphics.cpp
*/

#include "Graphics.hpp"

static const std::string fonts[] = {
        "./assets/fonts/LiberationSans.ttf",
};

static const std::string sounds[] = {
        "./assets/sounds/ea-sports_FKtJ2U8.wav",
        "./assets/sounds/SND.DAT_00000.wav",
        "./assets/musics/InMenuMusic.wav",
        "./assets/musics/inGameMusic.wav",
        "./assets/sounds/SND.DAT_00163.wav",
        "./assets/sounds/SND.DAT_00002.wav",
        "./assets/sounds/SND.DAT_00003.wav",
        "./assets/sounds/SND.DAT_00004.wav",
        "./assets/sounds/SND.DAT_00005.wav",
        "./assets/sounds/SND.DAT_00006.wav",
        "./assets/sounds/SND.DAT_00007.wav",
        "./assets/sounds/SND.DAT_00008.wav",
        "./assets/sounds/SND.DAT_00009.wav",
};

static const std::string musics[] = {
};

static const std::string textures[] = {
    "./assets/textures/players/r-typesheet42.gif",
    "./assets/textures/effects/r-typesheet1.gif",
    "./assets/textures/effects/r-typesheet2.gif",
    "./assets/textures/effects/r-typesheet30a.gif",
    "./assets/textures/effects/r-typesheet43.gif",
    "./assets/textures/effects/r-typesheet44.gif",
    "./assets/textures/paralaxes/ParalaxeBasic.png",
    "./assets/textures/UI/UI_button_animation_simple.png",
    "./assets/textures/UI/Super_R-Type_Logo.png",
    "./assets/textures/UI/backGroundLogo.png",
    "./assets/textures/enemies/r-typesheet12.gif",
    "./assets/textures/enemies/r-typesheet5.gif",
    "./assets/textures/UI/astronautflag.png",
    "./assets/textures/mdr.jpg",
};

Resource::Graphics::Graphics()
{
    _window.create({sf::VideoMode::getDesktopMode().width,
                    sf::VideoMode::getDesktopMode().height},
                   "Rtype");
    _camera.setCenter(static_cast<float>((_window.getSize().x) / 2), static_cast<float>(_window.getSize().y) / 2);
    _camera.setSize(static_cast<float>(sf::VideoMode::getDesktopMode().width), static_cast<float>(sf::VideoMode::getDesktopMode().height));

    for (const auto &font: fonts) {
        auto &it  = _font.emplace_back();
        it.loadFromFile(font);
    }

    for (const auto &texture: textures)
        addTexture(texture);
    for (const auto &sound: sounds)
        addSound(sound);
}

Resource::Graphics::Graphics(unsigned int x, unsigned int y)
{
    _window.create({x, y}, "Rtype");
    _camera.setSize(static_cast<float>(x), static_cast<float>(y));
    _camera.setCenter(static_cast<float>(x) / 2, static_cast<float>(y) / 2);

    for (const auto &font: fonts) {
        auto &it  = _font.emplace_back();
        it.loadFromFile(font);
    }

    for (const auto &texture: textures)
        addTexture(texture);
    for (const auto &sound: sounds)
        addSound(sound);
}

Resource::Graphics::Graphics(Graphics &copy) : _clock(copy.getClock())
{
    _eventSystem = copy.getEvents();
    _window.setSize(copy.getWindow().getSize());
    _camera = copy.getCamera();
}

Resource::Graphics::~Graphics()
{
    _window.close();
}

sf::RenderWindow &Resource::Graphics::getWindow()
{
    return _window;
}

sf::Event &Resource::Graphics::getEvents()
{
    return _eventSystem;
}

MyClock &Resource::Graphics::getClock()
{
    return _clock;
}

sf::View &Resource::Graphics::getCamera()
{
    return _camera;
}

void Resource::Graphics::resetClock()
{
    _clock.resetDeltaTime();
    // _clock.setElapsedTime(0);
}

double &Resource::Graphics::getDeltaTime()
{
    return _clock.getElapsedTime();
}

void Resource::Graphics::setCameraSize(float x, float y)
{
    _camera.setSize(x, y);
}
void Resource::Graphics::setCameraCenter(float x, float y)
{
    _camera.setCenter(x, y);
}

void Resource::Graphics::clear()
{
    if (_window.isOpen()) {
        _window.clear(sf::Color::Yellow);
    }
}

const std::vector<sf::Texture> &Resource::Graphics::getTextures() const
{
    return _textures;
}

sf::Texture &Resource::Graphics::addTexture(const std::string &path, const sf::IntRect &rect)
{
    auto &texture = _textures.emplace_back();
    if (!texture.loadFromFile(path, rect)) {
        std::cout << "error loading\n";
        exit(0);
    }
    return texture;
}

sf::SoundBuffer &Resource::Graphics::addSound(const std::string &path)
{
    auto &soundBuffer = _sounds.emplace_back();
    soundBuffer.loadFromFile(path);
    return soundBuffer;
}

sf::SoundBuffer &Resource::Graphics::getSound(Sounds && s)
{
    return _sounds[s];
}

sf::Texture &Resource::Graphics::getTexture(Resource::Graphics::Textures &&texture)
{
    //std::cout << "check " << texture << std::endl; 
    return _textures[texture];
}

sf::Font &Resource::Graphics::getFont(Resource::Graphics::Font &&font)
{
    //std::cout << "Size font  = " << _font.size() << std::endl;
    return _font[font];
}

sf::Sound &Resource::Graphics::getMusicSource()
{
    return _musicSource;
}

void Resource::Graphics::setMusic(const Resource::Graphics::Sounds &sound)
{
    _musicSource.setBuffer( _sounds[sound]);
    _musicSource.setLoop(true);
    _musicSource.play();
}
