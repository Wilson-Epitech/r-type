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

void sendUpdatedDataVectorServer(ecs::Registry &reg, Server &server) {

    std::map<std::size_t, asio::ip::udp::endpoint> connectedClients = server.getConnectedClients();

    static std::size_t previousConnectedClientsSize;
    std::size_t connectedClientsSize = connectedClients.size();

    static std::size_t previousLastConnectedClientId;
    std::size_t lastConnectedClientId = server.getLastConnectedClientId();
    std::unordered_map<std::size_t, bool> areClientsWorldSet = server.getAreClientsWorldSet();

    for (auto rit = connectedClients.rbegin(); rit != connectedClients.rend(); rit++) {
        std::size_t clientId = rit->first;
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
    for (auto rit = connectedClients.rbegin(); rit != connectedClients.rend(); rit++) {
        std::size_t clientId = rit->first;
        if (areClientsWorldSet[clientId] && !newEntitiesData.empty()) {
            std::string newEntititiesSerialized = Lib::serializeEntityDataVector(newEntitiesData);
            if (!newEntitiesData.empty()) {
                server.sendToAClient(clientId, newEntititiesSerialized);
            }
        }
    }
}

int main(int ac, char **av) {
    if (ac != 2 || av[1] == NULL) {
        std::cerr << "Usage: ./server [port]" << std::endl;
        return -1;
    }
    try {
        asio::io_context io_context;
        asio::thread_pool pool(NB_CLIENTS_AUTHORIZED);
        Server server(io_context, std::stoi(av[1]), pool);
        game::RtypeGameServer rtype;
        ecs::Registry reg = rtype.getRegistry();
        static std::chrono::_V2::steady_clock::time_point time0 = std::chrono::steady_clock::now();

        SetTraceLogLevel(TraceLogLevel::LOG_FATAL);
        while (1) {
            std::chrono::_V2::steady_clock::time_point time1 = std::chrono::steady_clock::now();
            std::chrono::milliseconds timeElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(time1 - time0);
            if (!(timeElapsed.count() >= Lib::toMilliseconds(UPDATE_SERVER))) {
                continue;
            }
            time0 = std::chrono::steady_clock::now();
            static DataToSend lastDataReceived;
            DataToSend dataReceived;
            dataReceived = server.getEntityData();
            if (!dataReceived.empty() && lastDataReceived != dataReceived) {
                for (auto &entity : dataReceived.entitiesData) {
                    if (entity.entityID != 0) {
                        Lib::updateEntities(reg, entity);
                    }
                }
                lastDataReceived = dataReceived;
            }
            if (server.getNbClients() > 0) {
                sendUpdatedDataVectorServer(reg, server);
                reg.clearActualTurnNewEntities();
                reg.clearActualTurnDeadEntities();
            }
            reg.runSystems();
        }
        pool.join();
    } catch (std::exception &e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return -1;
    }

    return 0;
}
