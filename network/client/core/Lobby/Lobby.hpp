#pragma once

#include <cstddef>

namespace NetworkClient {
    class Lobby {
    public:
        explicit Lobby(const std::size_t id, const std::size_t maxPlayer, const std::size_t numberPlayer = 0) : _id(id), _maxPlayer(maxPlayer), _numberPlayer(numberPlayer){}
        ~Lobby() {};

    void setMaxPlayer(const std::size_t maxPlayer) {
        _maxPlayer = maxPlayer;
    }
    void setNumberPlayer(const std::size_t numberPlayer) {
        _numberPlayer = numberPlayer;
    }
    void setReadyPlayer(const std::size_t readyPlayer) {
        _readyPlayer = readyPlayer;
    }

    void setStart(const bool state) {
        _startGame = state;
    }

    std::size_t getMaxPlayer() const {
        return _maxPlayer;
    }
    std::size_t getNmberPlayer() const {
        return _numberPlayer;
    }
    std::size_t getReadyPlayer() const {
        return _readyPlayer;
    }
    std::size_t getId() const {
        return _id;
    }

    bool getStartGame() const {
        return _startGame;
    }

    private:
        const std::size_t _id;

        std::size_t _maxPlayer;
        std::size_t _numberPlayer;
        std::size_t _readyPlayer;
        bool _startGame = false;
    };
}
