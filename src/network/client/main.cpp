/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** main
*/

#include "Client.hpp"
#include "RtypeGameClient.hpp"
#include <chrono>
#include <csignal>
#include <iostream>

static sig_atomic_t handleClient(const volatile sig_atomic_t new_status) {
    static volatile sig_atomic_t status = 1;

    if (new_status != 1)
        status = new_status;
    return status;
}

static void handleCtrlC(int signal) {
    if (signal == SIGINT)
        exit(0);
}

void sendUpdatedDataVectorClient(ecs::Registry &reg, Client &client, DataToSend &lastDataReceived) {
    DataToSend data = Lib::fillNewEntitiesDataToSend(reg);
    static bool hasSendWorldSet = 0;

    Lib::deleteSimilarDataLines(data, lastDataReceived);
    if (!lastDataReceived.empty() && !hasSendWorldSet) {
        client.sendMessage("World set");
        hasSendWorldSet = true;
    }
    if (!data.empty()) {
        std::string serialized = Lib::serializeEntityDataVector(data);
        client.sendMessage(serialized);
    }
}

int main(int ac, char **av) {
    if (ac != 2 || av[1] == NULL) {
        std::cerr << "Usage: ./client [port]" << std::endl;
        return -1;
    }
    try {
        asio::io_context ioContext;
        Client client(ioContext, "127.0.0.1", av[1]);
        game::RtypeGameClient rtype;
        ecs::Registry &reg = rtype.getRegistry();
        static std::chrono::_V2::steady_clock::time_point time0 = std::chrono::steady_clock::now();

        SetTraceLogLevel(TraceLogLevel::LOG_FATAL);
        SetConfigFlags(FLAG_WINDOW_RESIZABLE);
        InitWindow(GetScreenWidth(), GetScreenHeight(), rtype.getGameName().c_str());
        MaximizeWindow();
        InitAudioDevice();
        signal(SIGINT, handleCtrlC);
        while (handleClient(1) && !WindowShouldClose()) {
            if (!client.getIsRunning()) {
                client.sendMessage("Client disconnected");
                break;
            }
            std::chrono::_V2::steady_clock::time_point time1 = std::chrono::steady_clock::now();
            std::chrono::milliseconds timeElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(time1 - time0);

            if (!(timeElapsed.count() >= Lib::toMilliseconds(UPDATE_CLIENT))) {
                continue;
            }
            time0 = std::chrono::steady_clock::now();
            BeginDrawing();
            ClearBackground(RAYWHITE);
            static DataToSend lastDataReceived;
            DataToSend dataReceived = client.getData();
            std::size_t playerId = client.getPlayerId();
            std::map<std::size_t, entity_t> player = reg.getPlayers();
            if (playerId != 0 && player.size() == 0)
                reg.addPlayer(0, playerId);
            if (!dataReceived.empty() && !(lastDataReceived == dataReceived)) {
              for (auto &deadEntity : dataReceived.deadEntities) {
                    if (reg.entityExist(deadEntity))
                        reg.killEntity(deadEntity);
                }
                for (auto &entity : dataReceived.entitiesData) {
                    if (entity.entityID != 0) {
                        Lib::updateEntities(reg, entity);
                    }
                }
                lastDataReceived = dataReceived;
            }
            sendUpdatedDataVectorClient(reg, client, lastDataReceived);
            reg.clearActualTurnNewEntities();
            reg.clearActualTurnDeadEntities();
            reg.runSystems();
            EndDrawing();
        }
        CloseWindow();
    } catch (std::exception &e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return -1;
    }

    return 0;
}
