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

    while (!_networkInstance->isSigInt()) {
        if (!isUpdateTimeElapsed())
            continue;
        dataReceivedList = _networkInstance->getData();
        _networkInstance->updateData(_reg);
        if (_networkInstance->isConnectedInNetwork()) {
            _networkInstance->sendUpdatedData(_reg, dataReceivedList);
            _reg.clearActualTurnNewEntities();
            _reg.clearActualTurnDeadEntities();
        }
        _reg.runSystems();
        disconnectedPlayerEntity = _networkInstance->manageConnection();
        if (disconnectedPlayerEntity != 0)
            _reg.killEntity(disconnectedPlayerEntity);
    }
}

void RunGame::runClient() {
    std::list<DataToSend> dataReceivedList;

    while (!_networkInstance->isSigInt() && !WindowShouldClose()) {
        if (!_networkInstance->isConnectedInNetwork()) {
            _networkInstance->sendMessage("Client disconnected", 2);
            break;
        }
        if (!isUpdateTimeElapsed())
            continue;
        BeginDrawing();
        ClearBackground(RAYWHITE);
        dataReceivedList = _networkInstance->getData();
        _networkInstance->updateData(_reg);
        _networkInstance->sendUpdatedData(_reg, dataReceivedList);
        _reg.clearActualTurnNewEntities();
        _reg.clearActualTurnDeadEntities();
        _reg.runSystems();
        _networkInstance->manageConnection();
        EndDrawing();
    }
    CloseWindow();
}
