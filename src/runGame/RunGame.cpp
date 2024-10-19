/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** RunGame
*/

#include "RunGame.hpp"
#include <chrono>
#include <csignal>

void RunGame::run() {
    if (_server)
        runServer();
    else
        runClient();
}

bool RunGame::isUpdateTimeElapsed() {
    _time1 = std::chrono::steady_clock::now();
    _timeElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(_time1 - _time0);
    if (_timeElapsed.count() < Lib::toMilliseconds(UPDATE_TIME))
        return false;
    _time0 = std::chrono::steady_clock::now();
    return true;
}

void RunGame::runServer() {
    std::list<DataToSend> dataReceivedList;
    std::size_t disconnectedPlayerEntity = 0;

    while (1) {
        if (!isUpdateTimeElapsed())
            continue;
        dataReceivedList = _server->getData();
        _server->clearData();
        for (auto dataReceived : dataReceivedList) {
            if (!dataReceived.empty()) {
                network::HandleData::updateEntitiesData(dataReceived, _reg);
            }
        }
        if (_server->isConnectedInNetwork()) {
            _server->sendUpdatedData(_reg, dataReceivedList);
            _reg.clearActualTurnNewEntities();
            _reg.clearActualTurnDeadEntities();
        }
        _reg.runSystems();
        disconnectedPlayerEntity = _server->checkConnectedHeartbeat();
        if (disconnectedPlayerEntity != 0)
            _reg.killEntity(disconnectedPlayerEntity);
    }
}

void RunGame::runClient() {
    static std::list<DataToSend> lastDataReceivedList;
    std::list<DataToSend> dataReceivedList;
    std::map<std::size_t, entity_t> player;
    std::size_t playerId = 0;

    while (_client->handleNetwork(1) && !WindowShouldClose()) {
        if (!_client->isConnectedInNetwork()) {
            _client->sendMessage("Client disconnected", 2);
            break;
        }
        if (!isUpdateTimeElapsed())
            continue;
        BeginDrawing();
        ClearBackground(RAYWHITE);
        dataReceivedList = _client->getData();
        _client->clearData();
        player = _reg.getPlayers();
        playerId = _client->getPlayerId();

        if (playerId != 0 && player.size() == 0)
            _reg.addPlayer(0, playerId);
        for (auto dataReceived : dataReceivedList) {
            if (!dataReceived.empty()) {
                network::HandleData::updateDeadEntities(dataReceived, _reg);
                network::HandleData::updateEntitiesData(dataReceived, _reg);
            }
            lastDataReceivedList = dataReceivedList;
        }

        _client->sendUpdatedData(_reg, lastDataReceivedList);
        _reg.clearActualTurnNewEntities();
        _reg.clearActualTurnDeadEntities();
        _reg.runSystems();
        EndDrawing();
    }
    CloseWindow();
}
