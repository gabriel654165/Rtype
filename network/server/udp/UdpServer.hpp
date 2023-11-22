//
// Created by thomas on 08/11/2021.
//
#pragma once
#include "PortManager.hpp"
#include <unordered_map>
#include "EcsServer.hpp"

namespace NetworkServer {
    class UdpServer {
    public:
        UdpServer();
        ~UdpServer();
        // PUBLIC GETTER
        /**
         * @brief Get the Udp Session Mode object (Lobby or inGame)
         * 
         * @return Network::UdpSessionMode 
         */
        Network::UdpSessionMode GetUdpSessionMode(uint32_t) const;

        /**
         * @brief Set the Udp Session Mode object (Lobby or inGame)
         * 
         */
        void SetUdpSessionMode(uint32_t, Network::UdpSessionMode);

        // PUBLIC UDP SESSION API
        /**
         * @brief Get all client id of udp session
         * 
         * @return std::vector<Network::Digit>& 
         */
        std::vector<Network::Digit> &GetAllClientId(Network::Digit);
        /**
         * @brief insert udp client information to map
         * 
         * @return uint32_t 
         */
        uint32_t CreateUdpSession(Network::Digit);
        /**
         * @brief erase map element of client information
         * 
         */
        void RemoveUdpSession(uint32_t);
        /**
         * @brief Add client to udp session
         * 
         * @param client 
         * @param session 
         * @return Network::UdpSessionRole 
         */
        Network::UdpSessionRole AddClientToUdpSession(uint32_t client, uint32_t session);
        /**
         * @brief Remove client from udp session
         * 
         * @param client 
         * @param session 
         */
        void RemoveClientFromUdpSession(uint32_t client, uint32_t session);
        /**
         * @brief return true if udp session full
         * 
         * @return true 
         * @return false 
         */
        bool IsUdpSessionFull(uint32_t);
        /**
         * @brief Return true if udp session empty
         * 
         * @return true 
         * @return false 
         */
        bool IsUdpSessionEmpty(uint32_t);
        /**
         * @brief Get the Udp Session Port object
         * 
         * @return Network::Digit 
         */
        Network::Digit GetUdpSessionPort(uint32_t);
        /**
         * @brief Set the Udp Session Client Ready State object
         * 
         */
        void SetUdpSessionClientReadyState(uint32_t, uint32_t, bool);
        /**
         * @brief return true if all player ready to play
         * 
         * @return true 
         * @return false 
         */
        bool IsUdpSessionReadyToPlay(uint32_t);
        /**
         * @brief Init udp session
         * 
         */
        void InitUdpSession(uint32_t);
        /**
         * @brief set new client on udp
         * 
         * @return std::size_t 
         */
        std::size_t InitUdpSessionPlayerUuid(uint32_t);
        /**
         * @brief Set the Udp Session Spaceship Uuid object
         * 
         */
        void SetUdpSessionSpaceshipUuid(uint32_t, uint32_t, std::size_t);
        /**
         * @brief Get the Udp Session Spaceship Uuid object
         * 
         * @return std::size_t 
         */
        std::size_t GetUdpSessionSpaceshipUuid(uint32_t, uint32_t) const;
        /**
         * @brief Get the Udp Session Max Player object
         * 
         * @return Network::Digit 
         */
        Network::Digit GetUdpSessionMaxPlayer(uint32_t);
        /**
         * @brief Get the Udp Session Nb Player object
         * 
         * @return Network::Digit 
         */
        Network::Digit GetUdpSessionNbPlayer(uint32_t);
        /**
         * @brief Get the Udp Session Nb Rdy Player object
         * 
         * @return Network::Digit 
         */
        Network::Digit GetUdpSessionNbRdyPlayer(uint32_t);
        /**
         * @brief Get the Every Lobby Info Pack object
         * 
         */
        void GetEveryLobbyInfoPack();

        std::map<uint32_t, std::unique_ptr<EcsServer>> _udpSessionList;

    private:

        std::atomic_int32_t _udpSessionID;
        NetworkServer::PortManager _portManager;
        std::vector<Network::Digit> _idList;

    };
}