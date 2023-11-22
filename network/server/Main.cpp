/*
** EPITECH PROJECT, 2021
** Babel
** File description:
** Main.cpp
*/
#include <iostream>
#include "Error.hpp"
#include "CoreServer.hpp"

int main(int ac, char **av)
{
    //refaire leur boucle dans ma boucle de jeu et send quand je veux
    //mettre l'ecs ici
    //ou alors dans mes ressources mettre un fonction run
    try {
/*        boost::asio::io_service io_service;
        NetworkServer::UdpSession UdpSession(std::stoi(av[1]), io_service, 1, 2);
        UdpSession.InitCallBack();
        UdpSession.AddClient(0);
        UdpSession.AddClient(1);
        while (std::cin) {
            //receive udp
        }*/
        NetworkServer::CoreServer Core(std::stoi(av[1]));
        Core.Run();
//        Core.RunOnlyUdpServer();
        std::cout << std::thread::hardware_concurrency() << std::endl;
        return (0);
    } catch (const err::Rtype &e) {
        std::cerr << e.where() << ": " << e.what() << std::endl;
        return (84);
    }
    return 0;
}