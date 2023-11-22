//#include "client/Factory.hpp"
#include <iostream>
#include "client/core/CoreClient.hpp"

// static std::unique_ptr<NetworkClient::IClient> CreateUdpClient(std::string host, short port)
// {
//     std::cout << "host = " << host << " port = " << port << "\n";
//     return NtwClient::Factory::CreateUdpClient(host, port, 0);
// }

// static std::shared_ptr<NetworkClient::IClient> CreateTcpClient(std::string host, short port)
// {
//     std::cout << "host = " << host << " port = " << port << "\n";
//     return NtwClient::Factory::CreateTcpClient(host, port, 0);
// }

int mainzebi(int ac, char **av)
{
    try {//direct co en udp
        auto aled = std::make_unique<NetworkClient::Udp::UdpClient>(av[1], atoi(av[2]));
        
        //send des msg quand je veux

        //auto packet = ...
        //aled->Send(packet);
        
    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
        std::cout << "aurevoir" << std::endl;
    }
    return 0;
    /*
    std::string message("waitForResponse");
    try {
        NetworkClient::CoreClient CoreClient(av[1], atoi(av[2]));
        CoreClient.Run();
        std::cout << "adieu" << std::endl;
        return 1;
    }
    catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
        std::cout << "aurevoir" << std::endl;
    }
    return 0;*/
}