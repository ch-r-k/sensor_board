#include "device_manager.hpp"
#include <optional>
#include "sensors/aht10/aht10.hpp"

DeviceManager::DeviceManager(HardwareManager& hardwareManager)
{
    userIndication.setOutputPin(hardwareManager.getLedPin());
    // test.setSpiInterface(hardwareManager.getSensorSpi());
    // test.setI2cInterface(hardwareManager.getSensorI2c());

    aht10.setI2cInterface(hardwareManager.getSensorI2c());
    hardwareManager.getSensorI2c().SetIcb(aht10);
}

DeviceManager::~DeviceManager() {}

UserIndication& DeviceManager::getUserIndication() { return userIndication; }
// Test& DeviceManager::getTest() {}
Aht10& DeviceManager::getAht10() { return aht10; };