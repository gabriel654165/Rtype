//
// Created by spriters on 07/11/2021.
//

#ifndef B_CPP_501_PAR_5_1_RTYPE_MAXIME_DODIN_POSITION_HPP
#define B_CPP_501_PAR_5_1_RTYPE_MAXIME_DODIN_POSITION_HPP

struct Position {
    public:
        Position(float xx, float yy) : x(xx), y(yy) {};

        Position(const Position &pos) : x(pos.x), y(pos.y) {};
        float x;
        float y;
};

#endif //B_CPP_501_PAR_5_1_RTYPE_MAXIME_DODIN_POSITION_HPP
