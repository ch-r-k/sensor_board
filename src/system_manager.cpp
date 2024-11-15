#include "system_manager.hpp"
#include "qpcpp.hpp"

SystemManager::SystemManager()
    : hardwareManager(),
      deviceManager(hardwareManager),
      applicationManager(deviceManager)
{
    static std::uint8_t qsTxBuf[2 * 1024];  // buffer for QS-TX channel
    QP::QS::initBuf(qsTxBuf, sizeof(qsTxBuf));

    static std::uint8_t qsRxBuf[100];  // buffer for QS-RX channel
    QP::QS::rxInitBuf(qsRxBuf, sizeof(qsRxBuf));
}

SystemManager::~SystemManager() {}

void SystemManager::run() { applicationManager.start(); }
