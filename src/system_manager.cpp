#include "system_manager.hpp"
#include "qpcpp.hpp"

namespace manager_
{

SystemManager::SystemManager()
    : hardwareManager(),
      deviceManager(hardwareManager),
      applicationManager(deviceManager)
{
    static std::uint8_t qs_tx_buf[2 * 1024];  // buffer for QS-TX channel
    QP::QS::initBuf(qs_tx_buf, sizeof(qs_tx_buf));

    static std::uint8_t qs_rx_buf[100];  // buffer for QS-RX channel
    QP::QS::rxInitBuf(qs_rx_buf, sizeof(qs_rx_buf));
}

SystemManager::~SystemManager() {}

void SystemManager::run()
{
    // initialize event pools
    static QF_MPOOL_EL(app::SensorEvent) sml_pool_sto[20];
    QP::QF::poolInit(sml_pool_sto, sizeof(sml_pool_sto),
                     sizeof(sml_pool_sto[0]));

    static QF_MPOOL_EL(hardware_layer::CommandEvent) large_pool_sto[20];
    QP::QF::poolInit(large_pool_sto, sizeof(large_pool_sto),
                     sizeof(large_pool_sto[0]));

    applicationManager.start();
    deviceManager.start();
    hardwareManager.run();
}

}  // namespace manager_