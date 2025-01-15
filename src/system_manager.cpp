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

void SystemManager::run()
{
        // initialize event pools
    static QF_MPOOL_EL(APP::SensorEvent) smlPoolSto[20];
    QP::QF::poolInit(smlPoolSto, sizeof(smlPoolSto), sizeof(smlPoolSto[0]));

    static QF_MPOOL_EL(CommandEvent) largePoolSto[10];
    QP::QF::poolInit(largePoolSto, sizeof(largePoolSto), sizeof(largePoolSto[0]));
    
    applicationManager.start();
    deviceManager.start();
}
