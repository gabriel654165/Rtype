/*
** EPITECH PROJECT, 2021
** B-CPP-501-PAR-5-1-rtype-maxime.dodin
** File description:
** clock.hpp
*/

#pragma once

#include <iostream>
#include <chrono>

class MyClock {
public:
    using chrono = std::chrono::time_point<std::chrono::steady_clock, std::chrono::duration<float>>;
    using Time = std::chrono::steady_clock;
    using float_sec = std::chrono::duration<float>;
    using float_time_point = std::chrono::time_point<Time, float_sec>;

    MyClock();
    MyClock(MyClock &copy);

    ~MyClock() = default;

    chrono &getDeltaTime();
    void resetDeltaTime();
    double &getElapsedTime();
    float_time_point getCurrentTime();

    void setDeltaTime(MyClock::chrono other);
    void setElapsedTime(float other);

    //chrono getDeltaTime();
    //const float getElapsedTime() const;

private:
    chrono _deltaTime = Time::now();
    double _elapsedTime = 0;
};

