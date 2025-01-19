#include "device_manager.hpp"
#include <optional>
#include "sensors/aht10/aht10.hpp"

DeviceManager::DeviceManager(HardwareManager& hardwareManager)
{
    userIndication.setOutputPin(hardwareManager.getLedPin());
    // test.setSpiInterface(hardwareManager.getSensorSpi());
    // test.setI2cInterface(hardwareManager.getSensorI2c());

    // serial commander to devices
    aht10.setSerialInterface(serialCommander);
    serialCommander.setIcbSerialCommander(aht10);
    ssd1306.setSerialCommanderInterface(serialCommander);
    serialCommander.setIcbSerialCommander(ssd1306);

    // serial commander to hardware
    serialCommander.setSerialInterface(hardwareManager.getSensorI2c());
    hardwareManager.getSensorI2c().SetIcb(serialCommander);
}

DeviceManager::~DeviceManager() {}

UserIndication& DeviceManager::getUserIndication() { return userIndication; }
// Test& DeviceManager::getTest() {}
Aht10& DeviceManager::getAht10() { return aht10; };

void DeviceManager::start()
{
    static QP::QEvt const* serialCommanderQueueSto[10];
    serialCommander.start(
        4U,                              // QP prio. of the AO
        serialCommanderQueueSto,         // event queue storage
        Q_DIM(serialCommanderQueueSto),  // queue length [events]
        nullptr, 0U,                     // no stack storage
        nullptr);                        // no initialization param
    QS_OBJ_DICTIONARY(&serialCommander);
}