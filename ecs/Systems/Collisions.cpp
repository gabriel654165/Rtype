#include "Registry.hpp"
#include "sparseArray.hpp"
#ifdef CLIENT
#include "Graphics.hpp"
#else
#include "MyClock.hpp"
#endif
#include <algorithm>
#include <valarray>

static const unsigned int TILE_HEIGHT = 128;
static const unsigned int TILE_WIDTH = 128;
static const unsigned int GRID_HEIGHT = (1080 + 128) / TILE_HEIGHT;
static const unsigned int GRID_WIDTH = (1920 + 128) / TILE_WIDTH;

sf::FloatRect getNextEntityRect(Registry::entity_t entity, double deltaTime, sparseArray<Position> &posArr,
                                sparseArray<Collider> &colArr, sparseArray<Scale> &scaleArr,
                                sparseArray<Velocity> &velArr) {
    auto fEntityCol = std::any_cast<RectangleCollider>(colArr[entity]->infos);
    sf::Vector2<float> size(fEntityCol.width, fEntityCol.height);
    Position pos = *posArr[entity];

    if (velArr[entity].has_value()) {
        Velocity &vel = *velArr[entity];
        pos.x += vel.x * deltaTime;
        pos.y += vel.y * deltaTime;
    }
    if (scaleArr[entity].has_value()) {
        size.x *= scaleArr[entity]->x;
        size.y *= scaleArr[entity]->y;
    }

    return {pos.x + fEntityCol.xoff,
            pos.y + fEntityCol.yoff,
            size.x, size.y};

}

static void
fillCollisionGrid(Registry &r, std::set<Registry::entity_t> (&_grid)[GRID_HEIGHT][GRID_WIDTH], double deltaTime,
                  sparseArray<Position> &posArr, sparseArray<Collider> &colArr, sparseArray<Velocity> &velArr,
                  sparseArray<Scale> &scaleArr) {
    for (auto &entity: r.getScene().entities) {

        if (!colArr[entity].has_value() || !posArr[entity].has_value())
            continue;
        if (colArr[entity]->type != Collider::RECTANGLE)
            continue;
        colArr[entity]->hits.clear();

        sf::FloatRect entityRect = getNextEntityRect(entity, deltaTime, posArr, colArr, scaleArr, velArr);

        // calculate every cell in which the entity is
        sf::Vector2i topLeftCell(entityRect.left / TILE_WIDTH,
                                 entityRect.top / TILE_HEIGHT);
        sf::Vector2i bottomRightCell((entityRect.left + entityRect.width) / TILE_WIDTH,
                                     (entityRect.top + entityRect.height) / TILE_HEIGHT);

        if (topLeftCell.x < 0 || topLeftCell.x > GRID_WIDTH - 1 ||
            topLeftCell.y < 0 || topLeftCell.y > GRID_HEIGHT - 1 ||
            bottomRightCell.x < 0 || bottomRightCell.x > GRID_WIDTH - 1 ||
            bottomRightCell.y < 0 || bottomRightCell.y > GRID_HEIGHT - 1)
            continue;
        for (int y = topLeftCell.y; y <= bottomRightCell.y; y++) {
            for (int x = topLeftCell.x; x <= bottomRightCell.x; x++) {
                _grid[y][x].emplace(entity);
            }
        }
/*
        if (topLeftCell.x < 0 || topLeftCell.x > GRID_WIDTH - 1 ||
            topLeftCell.y < 0 || topLeftCell.y > GRID_HEIGHT - 1) {
            //std::cout << "ENTITY id = " << entity << " IL CONTINUE LE FDP" << std::endl;
            continue;
        }

        // fill grid
        for (int y = topLeftCell.y; y < GRID_HEIGHT && y <= bottomRightCell.y; y++)
            for (int x = topLeftCell.x; x < GRID_WIDTH && x <= bottomRightCell.x; x++) {
              //  std::cout << "GRID x = " << x << " GRID y = " << y << " ENTITY IDX = " << entity << std::endl;
                _grid[y][x].emplace(entity);
            }
        */
    }

}

void checkTileCollision(std::set<Registry::entity_t> &tile, double deltaTime, sparseArray<Position> &posArr,
                        sparseArray<Collider> &colArr, sparseArray<Scale> &scaleArr, sparseArray<Velocity> &velArr) {
    while (!tile.empty()) {
        Registry::entity_t entity = *tile.begin();

        tile.erase(entity);

        for (auto otherEntity: tile) {
            auto entityRect = getNextEntityRect(entity, deltaTime, posArr, colArr, scaleArr, velArr);
            auto otherEntityRect = getNextEntityRect(otherEntity, deltaTime, posArr, colArr, scaleArr, velArr);
            if (entityRect.intersects(otherEntityRect)) {
              //  std::cout << "-----------------COLLIDE FDP NIK TA GRAND MERE" << std::endl;
                colArr[entity]->hits.emplace_back(otherEntity);
                colArr[otherEntity]->hits.emplace_back(entity);}
        }
    }
}

#ifdef CLIENT
void collision_system([[maybe_unused]] Registry &r, Resource::Graphics &rsc, sparseArray<Position> &posArr,
                      sparseArray<Collider> &colArr, sparseArray<Scale> &scaleArr, sparseArray<Velocity> &velArr)
{
    auto deltaTime = rsc.getDeltaTime();
#else
void collision_system([[maybe_unused]] Registry &r, MyClock &clock, sparseArray<Position> &posArr, sparseArray<Collider> &colArr, sparseArray<Scale> &scaleArr, sparseArray<Velocity> &velArr)
{
    auto deltaTime = clock.getElapsedTime();
#endif
    std::set<Registry::entity_t> _collisionGrid[GRID_HEIGHT][GRID_WIDTH]{};

   // std::cout << "DeltaTime collision =" << deltaTime << std::endl;

    fillCollisionGrid(r, _collisionGrid, deltaTime, posArr, colArr, velArr, scaleArr);
    for (auto &line: _collisionGrid)
        for (auto &tile: line)
            checkTileCollision(tile, deltaTime, posArr, colArr, scaleArr, velArr);
}