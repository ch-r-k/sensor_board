#include "device_manager.hpp"

DeviceManager::DeviceManager(HardwareManager& hardwareManager)
{
    userIndication.setOutputPin(hardwareManager.getLedPin());
    test.setSpiInterface(hardwareManager.getSensorSpi());
    test.setI2cInterface(hardwareManager.getSensorI2c());
}

DeviceManager::~DeviceManager() {}

UserIndication& DeviceManager::getUserIndication() { return userIndication; }
Test& DeviceManager::getTest() { return test; }