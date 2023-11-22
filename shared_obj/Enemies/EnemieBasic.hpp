#pragma once

#include <iostream>

#include "Position.hpp"
#include "Collider.hpp"
#include "Velocity.hpp"

#include "AComponents.hpp"

class EnemieBasic : public Shared::AComponents {

    public:
        /**
         * @brief Construct a new Enemie Basic object
         *
         */
         EnemieBasic(Registry &);
         /**
         * @brief Destroy the Enemie Basic object
         *
         */
        ~EnemieBasic();
        /**
         * @brief spawn an entity
         *
         * @return std::size_t
         */
        std::size_t spawnEntity() noexcept override;
};