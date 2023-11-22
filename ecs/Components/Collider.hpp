//
// Created by spriters on 13/11/2021.
//

#ifndef B_CPP_501_PAR_5_1_RTYPE_MAXIME_DODIN_COLLIDER_HPP
#define B_CPP_501_PAR_5_1_RTYPE_MAXIME_DODIN_COLLIDER_HPP

#include <any>
#include <list>

struct RectangleCollider {
    float xoff{};
    float yoff{};
    unsigned int width;
    unsigned int height;
};

struct Collider {
    explicit Collider(const RectangleCollider &col) : type(RECTANGLE), infos(col){};

    Collider(const Collider &col) : type(col.type), infos(col.infos){};

    enum Type {
        RECTANGLE,
        TYPES_NUMBER
    } type;

    bool trigger{};

    /*
     * @brief infos to know to process a collision
     */
    std::any infos;

    /*
     * @brief every collision hit,
     * @return entity id and informations depending on the type
     */
    std::list<std::size_t> hits;
};

#endif //B_CPP_501_PAR_5_1_RTYPE_MAXIME_DODIN_COLLIDER_HPP
