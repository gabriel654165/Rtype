#pragma once

#include <iostream>

#include "Position.hpp"
#include "Collider.hpp"
#include "Velocity.hpp"

#include "AComponents.hpp"

class EnemieAdvance : public Shared::AComponents {

    public:
        /**
         * @brief Construct a new Enemie Advance object
         *
         */
         EnemieAdvance(Registry &);
         /**
          * @brief Destroy the Enemie Advance object
          *
          */
        ~EnemieAdvance();
        /**
         * @brief
         *
         * @return std::size_t
         */
        std::size_t spawnEntity() noexcept override;
};