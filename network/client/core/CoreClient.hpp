#pragma once

#include "UdpClient.hpp"
#include "TcpClient.hpp"
#include "Lobby/Lobby.hpp"
#include "Manager.hpp"
#include <map>

namespace NetworkClient {
    class CoreClient {
    public:
        /**
         * @brief Construct a new Core Client object
         *
         * @param host hostname
         * @param port connection port
         */
        CoreClient(const std::string &host, unsigned short port);
        /**
         * @brief Construct a new Core Client object
         *
         * @param copy of CoreCLient
         */
        CoreClient(NetworkClient::CoreClient &copy);
        /**
         * @brief Destroy the Core Client object
         *
         */
        ~CoreClient() = default;
        /**
         * @brief main loop from Coreclient needed to handle callback aan send
         *
         */
        void Run();
        /**
         * @brief Create a New Udp Client object
         *
         */
        void CreateNewUdpClient();
        /**
         * @brief Set the Udp Port object
         *
         * @param short set a new udp port to prepare connections
         */
        void SetUdpPort(unsigned short);
        /**
         * @brief Get the Tcp Safe Queue object
         *
         * @return Network::LockedQueue<unsigned char *>&
         */
        Network::LockedQueue<unsigned char *> &GetTcpSafeQueue();
        /**
         * @brief Get the Udp Safe Queue object
         *
         * @return Network::LockedQueue<unsigned char *>&
         */
        Network::LockedQueue<unsigned char *> &GetUdpSafeQueue();
        /**
         * @brief know if client is already start
         *
         * @return true
         * @return false
         */
        bool IsUdpClientStart() const;
        /**
         * @brief Set the Udp State object
         *
         */
        void SetUdpState(bool);
        /**
         * @brief Get the Udp State object
         *
         * @return true
         * @return false
         */
        bool GetUdpState() const;
        /**
         * @brief Set the Remote End Point object
         *
         */
        void SetRemoteEndPoint(std::string &);
        /**
         * @brief Get the Remote End Point object
         *
         * @return std::string&
         */
        std::string &GetRemoteEndPoint();
        /**
         * @brief request to disconnect client from server
         *
         */
        void _reqDisconnection();
        /**
         * @brief handle deconnection from signal handler
         * @param size_t signal handling
         */
        void ReqDisconnectionBySignal(const std::size_t);
        /**
         * @brief request to connect to a lobby
         * @param size_t lobby id
         */
        void _reqConnectLobby(const std::size_t);
        /**
         * @brief requets disconnection from lobby
         *
         */
        void _reqDisconnectLobby();
        /**
         * @brief request create a lobby
         *
         */
        void _reqCreateLobby(const std::size_t);
        /**
         * @brief requets to move a player in udp
         *
         * @param entityId id of the player to move
         * @param pos new pos of the player
         */
        void _reqMovePlayer(const std::size_t entityId, std::pair<std::size_t, const std::size_t> pos);
        /**
         * @brief request spawn a new entity
         *
         * @param entityId id of the entity to spawn
         * @param pos position of the new entity
         */
        void _reqSpawnEntity(const std::size_t entityId, std::pair<std::size_t, const std::size_t> pos);
        /**
         * @brief request from game event (key pressed, ...)
         *
         * @param entityId if off the player 
         * @param event key pressed info
         */
        void _reqGameEvent(const std::size_t entityId, const std::size_t event);
        /**
         * @brief requets from player who are ready to play a game
         *
         */
        void _reqReadyToPlay();
        /**
         * @brief requets from player who are no longer ready to create a bullet
         *
         */
        void _reqNotReadyToPlay();
        /**
         * @brief Get the Udp Client object
         *
         * @return const NetworkClient::Udp::UdpClient&
         */
        const NetworkClient::Udp::UdpClient &getUdpClient() const;
        /**
         * @brief Get the Tcp Client object
         *
         * @return const NetworkClient::Tcp::TcpClient&
         */
        const NetworkClient::Tcp::TcpClient &getTcpClient() const;
        std::unique_ptr<NetworkClient::Udp::UdpClient> _udpClient;//private
        Network::LockedQueue<unsigned char *> _udpSafeQueue;//private
        /**
         * @brief Get the Tcp Client object
         *
         * @return std::unique_ptr<NetworkClient::Tcp::TcpClient>&
         */
        std::unique_ptr<NetworkClient::Tcp::TcpClient> &GetTcpClient();
        /**
         * @brief Get the Lobbys object
         *
         * @return std::unordered_map<Network::Digit, std::unique_ptr<NetworkClient::Lobby>>&
         */
        std::unordered_map<Network::Digit, std::unique_ptr<NetworkClient::Lobby>> &GetLobbys();
        /**
         * @brief Set the Start Game object
         *
         */
        void SetStartGame(bool);
        /**
         * @brief Get the Start Game object
         *
         * @return true
         * @return false
         */
        bool getStartGame() const;

    private:

        /**
         * @brief start a new game
         *
         */
        void startGame();

        std::unique_ptr<NetworkClient::Tcp::TcpClient> _tcpClient;
        Network::LockedQueue<unsigned char *> _tcpSafeQueue;
        std::unordered_map<Network::Digit, std::unique_ptr<NetworkClient::Lobby>> _lobbys;

        const std::string _host;
        std::thread _threadContext;
        bool _readyToPlay;
        std::string _remoteEndpoint;
        bool _startGame;
        unsigned short _udpPort;
        bool _isUdpOpen = false;
    };
}