/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** main
*/

#include "RtypeGameServer.hpp"
#include "Server.hpp"
#include "raylib.h"
#include <chrono>
#include <iostream>

void sendUpdatedDataVectorServer(ecs::Registry &reg, network::Server &server) {

    std::map<std::size_t, asio::ip::udp::endpoint> connectedClients = server.getConnectedClients();

    static std::size_t previousConnectedClientsSize;
    std::size_t connectedClientsSize = connectedClients.size();

    static std::size_t previousLastConnectedClientId;
    std::size_t lastConnectedClientId = server.getLastConnectedClientId();
    std::unordered_map<std::size_t, bool> areClientsWorldSet = server.getAreClientsWorldSet();

    for (auto it = connectedClients.begin(); it != connectedClients.end(); it++) {
        std::size_t clientId = it->first;
        if (((connectedClientsSize > previousConnectedClientsSize) && (lastConnectedClientId == clientId) &&
             lastConnectedClientId != previousLastConnectedClientId) &&
            (lastConnectedClientId == clientId)) {
            entity_t newPlayer = reg.spawnEntity();
            reg.addPlayer(lastConnectedClientId, newPlayer);
            std::size_t playerId = reg.getPlayerByClientId(lastConnectedClientId);
            reg.runInitPlayerSystem();
            DataToSend allEntitiesData = Lib::fillAllEntitiesDataToSend(reg);
            std::string allEntititiesSerialized = Lib::serializeEntityDataVector(allEntitiesData);
            previousConnectedClientsSize = connectedClientsSize;
            previousLastConnectedClientId = lastConnectedClientId;
            server.sendToAClient(clientId, "playerId:" + std::to_string(playerId));
            if (!allEntitiesData.empty())
                server.sendToAClient(clientId, allEntititiesSerialized);
        }
    }

    DataToSend newEntitiesData = Lib::fillNewEntitiesDataToSend(reg);
    for (auto it = connectedClients.begin(); it != connectedClients.end(); it++) {
        std::size_t clientId = it->first;
        if (areClientsWorldSet[clientId - 1] && !newEntitiesData.empty()) {
            std::string newEntititiesSerialized = Lib::serializeEntityDataVector(newEntitiesData);
            server.sendToAClient(clientId, newEntititiesSerialized);
        }
    }
}

void checkConnectedHeartbeat(network::Server &server) {
    std::unordered_map<std::size_t, std::chrono::steady_clock::time_point> lastSeenClient = server.getHeartbeatClient();
    auto now = std::chrono::steady_clock::now();
    auto it = lastSeenClient.begin();

    while (it != lastSeenClient.end()) {
        if (std::chrono::duration_cast<std::chrono::seconds>(now - it->second).count() > 5) {
            server.sendToAllClient("Disconnected: " + std::to_string(it->first));
            it = lastSeenClient.erase(it);
        } else {
            it++;
        }
    }
    server.setHeartBeatmMap(lastSeenClient);
}

int main(int ac, char **av)
{
    if (ac != 3 || av[1] == NULL || av[2] == NULL) {
        std::cerr << "Usage: ./server [ip] [port]" << std::endl;
        return -1;
    }
    try {
        asio::io_context io_context;
        asio::thread_pool pool(NB_CLIENTS_AUTHORIZED);
        network::Server server(io_context, av[1], std::stoi(av[2]), pool);
        game::RtypeGameServer rtype;
        ecs::Registry reg = rtype.getRegistry();
        static std::chrono::steady_clock::time_point time0 = std::chrono::steady_clock::now();

        SetTraceLogLevel(TraceLogLevel::LOG_FATAL);
        while (1) {
            std::chrono::steady_clock::time_point time1 = std::chrono::steady_clock::now();
            std::chrono::milliseconds timeElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(time1 - time0);
            if (!(timeElapsed.count() >= Lib::toMilliseconds(UPDATE_SERVER))) {
                continue;
            }
            time0 = std::chrono::steady_clock::now();
            static DataToSend lastDataReceived;
            DataToSend dataReceived = server.getEntityData();
            bool dataHasBeenUsed = server.getDataHasBeenUsed();
            if (!dataReceived.empty() && !dataHasBeenUsed) {
                for (auto &entity : dataReceived.entitiesData) {
                    Lib::updateEntities(reg, entity);
                }
                lastDataReceived = dataReceived;
                server.setDataHasBeenUsed(true);
            }
            if (server.getNbClients() > 0) {
                sendUpdatedDataVectorServer(reg, server);
                reg.clearActualTurnNewEntities();
                reg.clearActualTurnDeadEntities();
            }
            reg.runSystems();
            checkConnectedHeartbeat(server);
        }
        pool.join();
    } catch (std::exception &e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return -1;
    }

    return 0;
}
