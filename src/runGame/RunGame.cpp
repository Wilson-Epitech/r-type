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
    if (_networkInstance->getNetworkType() == network::Type::SERVER) {
        runServer();
    } else if (_networkInstance->getNetworkType() == network::Type::CLIENT) {
        runClient();
    }
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
        dataReceivedList = _networkInstance->getData();
        _networkInstance->clearData();
        for (auto dataReceived : dataReceivedList) {
            if (!dataReceived.empty()) {
                network::HandleData::updateEntitiesData(dataReceived, _reg);
            }
        }
        if (_networkInstance->isConnectedInNetwork()) {
            _networkInstance->sendUpdatedData(_reg, dataReceivedList);
            _reg.clearActualTurnNewEntities();
            _reg.clearActualTurnDeadEntities();
        }
        _reg.runSystems();
        disconnectedPlayerEntity = _networkInstance->checkConnectedHeartbeat();
        if (disconnectedPlayerEntity != 0)
            _reg.killEntity(disconnectedPlayerEntity);
    }
}

void RunGame::runClient() {
    static std::list<DataToSend> lastDataReceivedList;
    std::list<DataToSend> dataReceivedList;
    std::map<std::size_t, entity_t> player;
    std::size_t playerId = 0;

    while (_networkInstance->handleNetwork(1) && !WindowShouldClose()) {
        if (!_networkInstance->isConnectedInNetwork()) {
            _networkInstance->sendMessage("Client disconnected", 2);
            break;
        }
        if (!isUpdateTimeElapsed())
            continue;
        BeginDrawing();
        ClearBackground(RAYWHITE);
        dataReceivedList = _networkInstance->getData();
        _networkInstance->clearData();
        player = _reg.getPlayers();
        playerId = _networkInstance->getPlayerId();

        if (playerId != 0 && player.size() == 0)
            _reg.addPlayer(0, playerId);
        for (auto dataReceived : dataReceivedList) {
            if (!dataReceived.empty()) {
                network::HandleData::updateDeadEntities(dataReceived, _reg);
                network::HandleData::updateEntitiesData(dataReceived, _reg);
            }
            lastDataReceivedList = dataReceivedList;
        }

        _networkInstance->sendUpdatedData(_reg, lastDataReceivedList);
        _reg.clearActualTurnNewEntities();
        _reg.clearActualTurnDeadEntities();
        _reg.runSystems();
        EndDrawing();
    }
    CloseWindow();
}
