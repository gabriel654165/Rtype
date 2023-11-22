#pragma once

#include "Registry.hpp"
#ifdef CLIENT
#include "Graphics.hpp"
#endif


/**
 * @brief propertires of the animation
 *
 */
struct AnimationProper {
    sf::Vector2<float> nextFrameOffset{};
    sf::Vector2<float> firstFramePosition{};
    std::size_t frames{};
    double timeStep{};
    bool active;
    bool loop;
};

/**
 * @brief rect of the texture
 *
 */
struct RectProper {
    int rectLeft;
    int rectTop;
    int rectWidth;
    int rectHeight;
};


namespace Shared {
    class AComponents {
        public:


        #ifdef CLIENT
            /**
             * @brief Construct a new AComponents object
             *
             * @param path texture path
             */
            AComponents(Registry &, Resource::Graphics::Textures &&path);
        #else
            /**
             * @brief Construct a new AComponents object
             *
             */
            AComponents(Registry &);
        #endif
        /**
         * @brief spawn an entity
         *
         * @return std::size_t
         */
        virtual std::size_t spawnEntity() = 0;

        #ifdef CLIENT
        /**
         * @brief Get the Text object
         *
         * @return const Resource::Graphics::Textures&
         */
        const Resource::Graphics::Textures &getText() const;
        /**
         * @brief Get the Animation Proper object
         *
         * @return AnimationProper
         */
        AnimationProper getAnimationProper() const;
        /**
         * @brief Get the Rect Proper object
         *
         * @return RectProper
         */
        RectProper getRectProper() const;
        #endif

        protected:

            Registry &_game;

            #ifdef CLIENT
                Resource::Graphics::Textures _path;
                RectProper _rectProper;
                AnimationProper _animationProper;
            #endif
    };
}