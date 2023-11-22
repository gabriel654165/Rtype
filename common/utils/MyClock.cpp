/*
** EPITECH PROJECT, 2021
** B-CPP-501-PAR-5-1-rtype-maxime.dodin
** File description:
** MyClock.cpp
*/

#include "MyClock.hpp"

MyClock::MyClock()
{
    _deltaTime = getCurrentTime();
    _elapsedTime = 0;
}

MyClock::MyClock(MyClock &copy)
{
    _deltaTime = copy.getDeltaTime();
    _elapsedTime = copy.getElapsedTime();
}



MyClock::chrono &MyClock::getDeltaTime()
{
    return _deltaTime;
}

void MyClock::resetDeltaTime()
{
    _deltaTime = Time::now();
}

MyClock::float_time_point MyClock::getCurrentTime()
{
    return Time::now();
}

double &MyClock::getElapsedTime()
{
    _elapsedTime = (getCurrentTime() - _deltaTime).count();
    return _elapsedTime;
}

void MyClock::setDeltaTime(MyClock::chrono other)
{
    _deltaTime = other;
}

void MyClock::setElapsedTime(float other)
{
    _elapsedTime = other;
}

/*
Graphics::chrono &Graphics::getDeltaTime()
{
    return _deltaTime;
}
*/
/*
const float &Graphics::getElapsedTime() const
{
    return (getCurrentTime() - _deltaTime).count();
}*/
