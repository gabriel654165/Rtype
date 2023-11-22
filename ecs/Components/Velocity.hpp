//
// Created by spriters on 07/11/2021.
//

#ifndef B_CPP_501_PAR_5_1_RTYPE_MAXIME_DODIN_VELOCITY_HPP
#define B_CPP_501_PAR_5_1_RTYPE_MAXIME_DODIN_VELOCITY_HPP

struct Velocity {
    public:
        Velocity(const Velocity &velocity) : x(velocity.x), y(velocity.y), speed(velocity.speed){};
        Velocity(float xx, float yy, float spd) : x(xx), y(yy), speed(spd){};
        float x;
        float y;
        float speed;
};

#endif //B_CPP_501_PAR_5_1_RTYPE_MAXIME_DODIN_VELOCITY_HPP
