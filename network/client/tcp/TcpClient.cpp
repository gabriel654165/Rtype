#include "TcpClient.hpp"
#include "Log.hpp"
#include <iostream>

// PACK
/*
Protocol::Manager protocol;
std::string logMsg = Protocol::CodeArray[Protocol::ReqLogin].msg;

_userInfo.usr = username;
logMsg.replace(logMsg.find('\''), 2, username);
logMsg.replace(logMsg.find('\''), 2, password);
LOG(SERVER, HIGH, COUT, "===Sending a TCP packet to the server===");
LOG(SERVER, HIGH, COUT, logMsg);
LOG(SERVER, HIGH, COUT, "Status Code: ", CODE(Protocol::Code::ReqLogin));
protocol.packTcp("ddlsls", Protocol::Code::ReqLogin, _userInfo.id, username.size(), username.c_str(), password.size(), password.c_str());
_tcpClient->send(protocol.p.buffer, protocol.p.sizeOfPacket);
*/

// UNPACK
/*
    ClientInfo cli;
    Protocol::TcpUnpackData t;
    Protocol::Manager protocol;

    NtwClient::Protocol::Protocol::Manager::unpackTcp(packet, "dddlslsls", &t.statusCode, &t.id[REQ_CLI], &t.port, &t.lenIp[REQ_CLI], t.ip[REQ_CLI],
        &t.lenUsername[REQ_CLI], t.username[REQ_CLI], &t.lenUsername[RES_CLI], t.username[RES_CLI]);
    t.display("Request Start a Call");
    auto it = std::find_if(_clientConnection.begin(), _clientConnection.end(),
        [&t](std::pair<ClientInfo, std::shared_ptr<TcpConnection>> &e) {
            if (e.first.id == t.id[REQ_CLI]) {
                e.first.ip = t.ip[REQ_CLI];
                e.first.port = t.port;
            }
            return (e.first.usr == t.username[RES_CLI]);
        });
*/

namespace NetworkClient::Tcp
{
    NetworkClient::Tcp::TcpClient::TcpClient(std::string host, unsigned short server_port, Network::LockedQueue<unsigned char *> &tcpSafeQueue)
            : _socket(_io_context, tcp::endpoint(tcp::v4(), 0)), _service_thread(&NetworkClient::Tcp::TcpClient::RunService, this), _deadline(_io_context), _heartbeat_timer(_io_context), _stopped(false), _server_port(server_port), _tcpSafeQueue(tcpSafeQueue)
    {
        tcp::resolver resolver(_io_context);
        tcp::resolver::query query(tcp::v4(), host, std::to_string(server_port));
        _endpoints = resolver.resolve(query);
    }

    void TcpClient::Start() {
        StartConnect(_endpoints.begin());
        _deadline.async_wait(boost::bind(&Tcp::TcpClient::CheckDeadline, this));
    }

    void TcpClient::StartConnect(tcp::resolver::results_type::iterator endpoint_iter)
    {
        if (endpoint_iter != _endpoints.end())
        {
            std::cout << "Trying " << endpoint_iter->endpoint() << "...\n";

            // Set a deadline for the connect operation.
            _deadline.expires_after(boost::asio::chrono::seconds(6000));

            // Start the asynchronous connect operation.
            _socket.async_connect(endpoint_iter->endpoint(), boost::bind(&Tcp::TcpClient::HandleConnect, this, _1, endpoint_iter));
            std::cout << "after async connect" << '\n';
        }
        else
        {
            std::cout << "Strt connect Stop" << '\n';
            // There are no more endpoints to try. Shut down the client.
            Stop();
        }
    }

    void TcpClient::HandleConnect(const boost::system::error_code &ec,
                                  tcp::resolver::results_type::iterator endpoint_iter)
    {
        std::cout << "handle connect begun" << '\n';
        if (_stopped)
        {
            std::cout << "handle connect Stop" << '\n';
            return;
        }

        // The async_connect() function automatically opens the socket at the start
        // of the asynchronous operation. If the socket is closed at this time then
        // the timeout handler must have run first.
        if (!_socket.is_open())
        {
            std::cout << "Connect timed out\n";
            // Try the next available endpoint.
            StartConnect(++endpoint_iter);
        }
            // Check if the connect operation failed before the deadline expired.
        else if (ec)
        {
            std::cout << "Connect error: " << ec.message() << "\n";
            // We need to close the socket used in the previous connection attempt
            // before starting a new one.
            _socket.close();
            // Try the next available endpoint.
            StartConnect(++endpoint_iter);
        }
            // Otherwise we have successfully established a connection.
        else
        {
            std::cout << "Connected to " << endpoint_iter->endpoint() << "\n";
            // Start the input actor.
            StartReceive();
            // Start the heartbeat actor.
        }
    }

    TcpClient::~TcpClient()
    {
        std::cout << "tcpCLient destructor" << std::endl;
        if (!_stopped) {
            _stopped = true;
            _io_context.stop();
            _service_thread.join();
            _deadline.cancel();
            _heartbeat_timer.cancel();
            _socket.close();
        }
    }

	void TcpClient::Stop()
	{
		std::cout << "Stop connection" << "\n";
		_stopped = true;
		_io_context.stop();
		boost::system::error_code ignored_ec;
		_deadline.cancel();
		_heartbeat_timer.cancel();
		_service_thread.join();
		_socket.close(ignored_ec);
	}

    void TcpClient::StartReceive()
    {
        const boost::system::error_code err;
        size_t byteTransferred;
//        std::cout << "Strt receive" << '\n';
        //std::cout << "io_service : " << (io_context._stopped() ? "doesn't work" : "work") << std::endl;
        //deadline.expires_after(boost::asio::chrono::seconds(60000));
        boost::asio::async_read(_socket, boost::asio::buffer(_buff),
                              std::bind(&TcpClient::HandleReceive, this, err, byteTransferred));
//		boost::asio::async_read_until(socket, boost::asio::dynamic_buffer(input_buffer), '\n', boost::bind(&Tcp::TcpClient::HandleReceive, this, _1, _2));
//		std::cout << "End start receive" << '\n';
    }

    void TcpClient::HandleReceive(const std::error_code &error, [[maybe_unused]]std::size_t bytes_transferred)
    {
        if (_stopped)
        {
            std::cout << "handle_receive stop" << '\n';
            return;
        }
        if (!error)
        {
            _tcpSafeQueue.push(_buff);
            StartReceive();
        }
        else
        {
            Log::Error("TcpClient::handle_receive:", error);
            Stop();
        }
    }

    void TcpClient::Send(Network::Protocol::Packet &packet)
    {
        std::size_t bytes_transferred = 0;
        const std::error_code error;
        if (_stopped)
            return;
        //_socket.async_send(boost::asio::buffer(reinterpret_cast<NtwClient::Protocol::NetworkPacket &>(packet).buffer),
        //    std::bind(&NetworkClient::Tcp::TcpClient::HandleWrite, shared_from_this(), error));
        std::cout << "packetCast.sizeOfPacket = " << reinterpret_cast<Network::Protocol::NetworkPacket &>(packet).sizeOfPacket << "\n";
        boost::asio::async_write(_socket, boost::asio::buffer(reinterpret_cast<Network::Protocol::NetworkPacket &>(packet).buffer, reinterpret_cast<Network::Protocol::NetworkPacket &>(packet).sizeOfPacket), boost::bind(&NetworkClient::Tcp::TcpClient::HandleWrite, this, error, bytes_transferred));
    }

    void TcpClient::HandleWrite(const std::error_code &error, std::size_t bytes_transferred)
    {
        if (_stopped)
            return;

        if (!error)
        {
//            std::cout << "Packet of size " << bytes_transferred  << " is Sending to server" << std::endl;
            //heartbeat_timer.expires_after(boost::asio::chrono::seconds(10));
//			history.RegisterSentMessage(message.size());
            //heartbeat_timer.async_wait(boost::bind(&NetworkClient::Tcp::TcpClient::Send, this, message));
        }
        else
        {
            std::cout << "Error on heartbeat: " << error.message() << "\n";
            Stop();
        }
    }

    bool TcpClient::HasMessages()
    {
        return !_tcpSafeQueue.empty();
    }

    unsigned char *TcpClient::PopMessage()
    {
        if (_tcpSafeQueue.empty())
            throw std::logic_error("No messages to pop");
        return _tcpSafeQueue.pop();
    }

    void TcpClient::RunService()
    {
        while (!_io_context.stopped())
        {
            try
            {
                _io_context.run();
            }
            catch (const std::exception &e)
            {
                Log::Warning("TcpClient: network exception: ", e.what());
            }
            catch (...)
            {
                Log::Error("Unknown exception in TcpClient network thread");
            }
        }
    }

    void TcpClient::CheckDeadline()
    {
        if (_stopped)
            return;

        // Check whether the deadline has passed. We compare the deadline against
        // the current time since a new asynchronous operation may have moved the
        // deadline before this actor had a chance to run.
        if (_deadline.expiry() <= boost::asio::steady_timer::clock_type::now())
        {
            // The deadline has passed. The socket is closed so that any outstanding
            // asynchronous operations are cancelled.
            _socket.close();

            // There is no longer an active deadline. The expiry is set to the
            // maximum time point so that the actor takes no action until a new
            // deadline is set.
            _deadline.expires_at(boost::asio::steady_timer::time_point::max());
        }

        // Put the actor back to sleep.
        _deadline.async_wait(boost::bind(&TcpClient::TcpClient::CheckDeadline, this));
    }

    std::pair<bool, std::size_t> TcpClient::getInLobby() {
        return _inLobby;
    }

	void TcpClient::setInLobby(const bool inLobby, const std::size_t lobbyId) {
		_inLobby.first = inLobby;
		_inLobby.second = lobbyId;
	}

	bool TcpClient::IsStopped() const {
		return _stopped;
	}
}