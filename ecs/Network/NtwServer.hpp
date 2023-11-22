/*
** EPITECH PROJECT, 2021
** B-CPP-501-PAR-5-1-rtype-maxime.dodin
** File description:
** NtServer.hpp
*/
#pragma once

#include "CoreServer.hpp"
#include "Manager.hpp"
#include "MyClock.hpp"

namespace Resource {
    class NtwServer {
    public:
        NtwServer(unsigned short port);
        ~NtwServer();

        void resetClock();
        MyClock &getClock();
        double &getDeltaTime();
        NetworkServer::CoreServer &getServer();
        NetworkServer::UdpSession* getSession(std::size_t idUdpSession);
        Network::PairQueue<Network::Digit, unsigned char *> &getSafeQueue(std::size_t idUdpSession);
        //future -> safequeue ?
        //map avec id client + dataUdp (pour renvoyer que au boug si utile des fois)
        Network::Protocol::UnpackDataUdp &getDataClient();


    private:
        NetworkServer::CoreServer _server;
        Network::Protocol::UnpackDataUdp _dataUdp;
        MyClock _clock;
    };
};