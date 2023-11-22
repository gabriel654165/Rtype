#include "Registry.hpp"
#include "Graphics.hpp"
#include "sparseArray.hpp"

void paralaxe_system([[maybe_unused]] Registry &r, [[maybe_unused]] Resource::Graphics &resources, sparseArray<Position> &arrPos)
{
    auto paralaxe = *r.get_tagged_entities("paralaxe").begin();

    if (arrPos[paralaxe]->x <= ((-1080) * 2) )
         arrPos[paralaxe]->x = 0;
}