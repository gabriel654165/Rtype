//
// Created by spriters on 21/11/2021.
//

#include "Registry.hpp"

void power_system(
    [[maybe_unused]] Registry &r,
    sparseArray<Power> &powerArr)
{
    for (auto &power: powerArr) {
        if (!power.has_value())
            continue;
        power->power += power->buff;
        power->power = std::min(power->power, power->maxPower);
        power->power = std::max(power->power, 0);
    }
}