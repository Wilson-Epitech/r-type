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

    std::unordered_map<std::size_t, asio::ip::udp::endpoint> connectedClients = server.getConnectedClients();

    static std::size_t previousConnectedClientsSize;
    std::size_t connectedClientsSize = connectedClients.size();

    static std::size_t previousLastConnectedClientId;
    std::size_t lastConnectedClientId = server.getLastConnectedClientId();

    DataToSend newEntitiesData = Lib::fillNewEntitiesDataToSend(reg);
    DataToSend allEntitiesData = Lib::fillAllEntitiesDataToSend(reg);

    try {
        for (const auto &it : connectedClients) {
            std::size_t id = it.first;
            if (((connectedClientsSize != previousConnectedClientsSize) && (lastConnectedClientId == id) && !allEntitiesData.empty())||
                ((lastConnectedClientId != previousLastConnectedClientId) && (lastConnectedClientId == id) && !allEntitiesData.empty())) {
                    entity_t newPlayer = reg.spawnEntity();
                    reg.addPlayer(lastConnectedClientId, newPlayer);
                    std::size_t playerId = reg.getPlayerByClientId(lastConnectedClientId);
                    reg.runInitPlayer();
                    allEntitiesData = Lib::fillAllEntitiesDataToSend(reg);
                    std::string allEntititiesSerialized = Lib::serializeEntityDataVector(allEntitiesData);
                    previousConnectedClientsSize = connectedClientsSize;
                    previousLastConnectedClientId = lastConnectedClientId;
                    server.sendToAClient(id, std::to_string(playerId));
                    server.sendToAClient(id, allEntititiesSerialized);
            } else if (!newEntitiesData.empty()) {
                std::string newEntititiesSerialized = Lib::serializeEntityDataVector(newEntitiesData);
                server.sendToAClient(id, newEntititiesSerialized);
            }
        }
    } catch (const std::exception &e) {
        throw;
    }
}

int main(int ac, char **av) {
    if (ac != 2 || av[1] == NULL) {
        std::cerr << "Usage: ./server [port]" << std::endl;
        return -1;
    }
    try {
        asio::io_context io_context;
        asio::thread_pool pool(4);
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
