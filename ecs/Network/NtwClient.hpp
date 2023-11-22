/*
** EPITECH PROJECT, 2021
** B-CPP-501-PAR-5-1-rtype-maxime.dodin
** File description:
** NtwClient.hpp
*/
#pragma once

#include "CoreClient.hpp"
#include "Manager.hpp"

namespace Resource {
    class NtwClient {
    public:
        NtwClient(const std::string &host, unsigned short port);
        NtwClient(NtwClient &copy);
        ~NtwClient();


        NetworkClient::CoreClient &getClient();
        udp::endpoint &getEnpoint();
        std::list<std::pair<Network::Protocol::UnpackDataUdp, bool>> &getInputHistory();
        void send(Network::Protocol::Packet &packet, udp::endpoint endPoint);
        void setNbPlayers(std::size_t &nbPlayers);
        void setYourUuid(std::size_t &yourUuid);
        std::size_t &getYourUuid();
        std::size_t &getNbPlayers();
        std::mutex _mutexKillEntity;

    private:
        NetworkClient::CoreClient _client;
            
        //garde ses input en memoire
        //quand tu envois un input il dois etre repondu par le serveur 
        // (avec ton enity_t pas celui des autres)
        //ensuite bouger en fonction de ca

        //le client est tjr X input dans le passé

        std::list<std::pair<Network::Protocol::UnpackDataUdp, bool>> _inputHistory;
        std::size_t _nbPlayers;
        std::size_t _yourUuid;
    };
};