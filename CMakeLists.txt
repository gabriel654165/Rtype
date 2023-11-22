cmake_minimum_required(VERSION 3.16)
project(B-CPP-501-PAR-5-1-rtype-maxime.dodin)
add_definitions("-std=c++17")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
#set(CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})

#set(CMAKE_AUTOMOC ON)
#set(CMAKE_AUTORCC ON)
#set(CMAKE_AUTOUIC ON)

include(build/conanbuildinfo.cmake)
#conan_set_vs_runtime()
#conan_set_libcxx()
#conan_output_dirs_setup()
conan_basic_setup(Rtype_server Rtype_client ecs)
#find_package(Sqlite3 REQUIRED)

if (UNIX)
    add_definitions("-fPIC -shared -Wall -Wextra -Wuninitialized -Wunreachable-code")#-Wno-dev
endif (UNIX)

#set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/)
#set(LD_LIBRARY_PATH=$(/lib) && ./babel_client)
#set(LD_LIBRARY_PATH=$(/lib) && ./babel_server)

#apple
if (APPLE)
    set(CMAKE_INSTALL_RPATH "@executable_path/../lib")
endif (APPLE)
#windows
if (WIN32)
    set(CMAKE_WIN32_EXECUTABLE ON)
endif ()

#include_directories(${CONAN_INCLUDE_DIRS})

# server
include_directories(network)
include_directories(network/client/)
include_directories(network/client/udp/)
include_directories(network/client/tcp/)
include_directories(network/client/core/Lobby/Lobby/)
include_directories(network/client/core/)
include_directories(network/factory/)
include_directories(network/server/)
include_directories(network/server/tcp/)
include_directories(network/server/udp/)
include_directories(network/server/CoreServer/)
include_directories(network/server/PortManager/)
include_directories(network/history/)
include_directories(exception/)
include_directories(common/)
include_directories(common/protocol/)
include_directories(common/packets/)
include_directories(common/thread/)
include_directories(common/utils)
include_directories(common/signalHandler/)
include_directories(ecs/)
include_directories(ecs/Client/)
include_directories(ecs/Server/)
include_directories(ecs/Systems/)
include_directories(ecs/Systems/Server/)
include_directories(ecs/Components/)
include_directories(ecs/Graphic/)
include_directories(ecs/Network/)
include_directories(ecs/EntityLoad/)
include_directories(shared_obj/)

file (GLOB CORE_SHARED
        shared_obj/AComponents.hpp
        shared_obj/AComponents.cpp
        )

file(GLOB RTYPE_ERROR
        exception/Error.cpp
        exception/Error.hpp
        exception/Error.cpp
        exception/Error.hpp)

        file(GLOB SHARED_OBJ
        ecs/Graphic/Graphics.cpp
        ecs/Graphic/Graphics.hpp
        common/utils/MyClock.cpp
        common/utils/MyClock.hpp
        shared_obj/AComponents.hpp
        shared_obj/AComponents.cpp
        ecs/sparseArray.cpp
        ecs/Components/**
        )

file(GLOB ENEMIES_BASIC
        shared_obj/Enemies/EnemieBasic.cpp
        shared_obj/Enemies/EnemieBasic.hpp
        ${SHARED_OBJ}
        )

file(GLOB ENEMIES_ADVANCE
        shared_obj/Enemies/EnemieAdvance.cpp
        shared_obj/Enemies/EnemieAdvance.hpp
        ${SHARED_OBJ}
        )

file(GLOB RTYPE_ECS_COMMON
        ecs/sparseArray.impl
        ecs/sparseArray.hpp
        ecs/Registry.hpp
        ecs/Registry.hpp
        ecs/Components/**
        common/utils/MyClock.cpp
        common/utils/MyClock.hpp
        ecs/EntityLoad/EntityLoad.cpp
        ecs/EntityLoad/EntityLoad.hpp
        )


file(GLOB RTYPE_ECS_CLIENT
        ecs/Client/Main.cpp
        ecs/Graphic/Graphics.cpp
        ecs/Graphic/Graphics.hpp
        ecs/Network/NtwClient.cpp
        ecs/Network/NtwClient.hpp
        ecs/Systems/AddSystems.cpp
        ecs/Systems/Animations.cpp
        ecs/Systems/Camera.cpp
        ecs/Systems/Draw.cpp
        ecs/Systems/Events.cpp
        ecs/Systems/Follow.cpp
        ecs/Systems/Movement.cpp
        ecs/Systems/NetworkClient.cpp
        ecs/Systems/Player.cpp
        ecs/Systems/Text.cpp
        #ecs/Systems/Click.cpp
        ecs/Systems/AnimationUI.cpp
        ecs/Systems/TcpClientReceiveSystem.cpp
        ecs/Systems/GameEvent.cpp
        ecs/Systems/MainMenuButton.cpp
        ecs/Systems/JoinLobbyMenuButton.cpp
        ecs/Systems/CreateLobbyMenuButton.cpp
        ecs/Systems/LobbyMenuButton.cpp
        ecs/Systems/JoinLobby.cpp
        ecs/Systems/lobby_return_to_menu_button.cpp
        ecs/Systems/Paralaxe.cpp
        ecs/Systems/Levels.cpp
        ecs/Systems/PlayerShips.cpp
        ecs/Systems/Collisions.cpp
        ecs/Systems/Spawner.cpp
        )

file(GLOB RTYPE_ECS_SERVER
        ecs/Server/Main.cpp
        ecs/Server/EcsServer.cpp
        ecs/Server/EcsServer.hpp
        ecs/Systems/Server/NetworkServer.cpp
#        ecs/Systems/Server/AddSystemsServer.cpp
        ecs/Systems/Server/MovementServer.cpp
        ecs/Systems/Follow.cpp
        ecs/Systems/PlayerShips.cpp
        ecs/Systems/Collisions.cpp
        ecs/Systems/Spawner.cpp
        )

add_executable(Rtype_server
        common/packets/Packet.cpp
        common/packets/Packet.hpp
        common/protocol/IProtocol.hpp
        common/protocol/Manager.cpp
        common/protocol/Manager.hpp
        common/thread/ThreadPair.hpp
        network/server/Data.hpp
        network/server/IServer.hpp
        network/server/tcp/TcpConnection.cpp
        network/server/tcp/TcpConnection.hpp
        network/server/tcp/TcpServer.cpp
        network/server/tcp/TcpServer.hpp
        network/server/udp/UdpServer.cpp
        network/server/udp/UdpServer.hpp
        network/server/udp/UdpSession.cpp
        network/server/udp/UdpSession.hpp
        network/server/PortManager/PortManager.cpp
        network/server/PortManager/PortManager.hpp
        network/server/CoreServer/CoreServer.cpp
        network/server/CoreServer/CoreServer.hpp
        network/server/tcp/ManagerTcpServer.cpp
        network/server/tcp/ManagerTcpServer.hpp
        common/utils/MyClock.cpp
        common/utils/MyClock.hpp
        ${CORE_SHARED}
        ${RTYPE_ERROR}
        ${RTYPE_ECS_COMMON}
        ${RTYPE_ECS_SERVER}
        ecs/Systems/Power.cpp ecs/Components/Level.hpp)


set(enemie_basic_src ${ENEMIES_BASIC})
add_library(_enemies_one SHARED ${enemie_basic_src})
conan_target_link_libraries(_enemies_one ${CONAN_LIBS})

set(enemie_advance_src ${ENEMIES_ADVANCE})
add_library(_enemies_two SHARED ${enemie_advance_src})
conan_target_link_libraries(_enemies_two ${CONAN_LIBS})

target_link_libraries(Rtype_server)
target_link_libraries(Rtype_server _enemies_one)
target_link_libraries(Rtype_server _enemies_two)
target_include_directories(Rtype_server PUBLIC ${CONAN_INCLUDE_DIRS})
target_link_libraries(Rtype_server ${CONAN_LIBS})

add_executable(Rtype_client
        common/packets/Packet.cpp
        common/packets/Packet.hpp
        common/protocol/IProtocol.hpp
        common/protocol/Manager.cpp
        common/protocol/Manager.hpp
        network/client/udp/UdpClient.cpp
        network/client/udp/UdpClient.hpp
        network/client/tcp/TcpClient.cpp
        network/client/tcp/TcpClient.hpp
        network/client/core/CoreClient.cpp
        network/client/core/CoreClient.hpp
        common/signalHandler/signalHandler.cpp
        common/signalHandler/signalHandler.hpp
        network/client/IClient.hpp
        network/client/Log.hpp
        network/history/History.cpp
        network/history/History.hpp
        common/thread/SafeQueue.hpp
        network/client/core/Lobby/Lobby.hpp
        ${CORE_SHARED}
        ${RTYPE_ERROR}
        ${RTYPE_ECS_COMMON}
        ${RTYPE_ECS_CLIENT}
        ecs/Systems/Power.cpp ecs/Components/Level.hpp)

target_compile_definitions(Rtype_client PRIVATE CLIENT=1)
target_compile_definitions(_enemies_one PRIVATE CLIENT=1)
target_compile_definitions(_enemies_two PRIVATE CLIENT=1)
target_link_libraries(Rtype_client)
target_link_libraries(Rtype_client _enemies_one)
target_link_libraries(Rtype_client _enemies_two)
target_include_directories(Rtype_client PUBLIC ${CONAN_INCLUDE_DIRS})
target_link_libraries(Rtype_client ${CONAN_LIBS})