#include "AComponents.hpp"

#ifdef CLIENT
    Shared::AComponents::AComponents(Registry &game, Resource::Graphics::Textures &&path) : _game(game), _path(path) {};
#else
    Shared::AComponents::AComponents(Registry &game) : _game(game) {};
#endif


#ifdef CLIENT
const Resource::Graphics::Textures &Shared::AComponents::getText() const {
    return _path;
}

AnimationProper Shared::AComponents::getAnimationProper() const {
    return _animationProper;
}

RectProper Shared::AComponents::getRectProper() const {
    return _rectProper;
}
#endif