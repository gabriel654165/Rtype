#include <iostream>

#include "Registry.hpp"
#include "CoreServer.hpp"

void registerComponents(Registry &);

int main([[maybe_unused]] int ac, char **av)
{
    Registry game;
    NetworkServer::CoreServer Core(std::stoi(av[1]));
    Core.Run();
    return 0;
}

//crrer une enité quand un boug join la game