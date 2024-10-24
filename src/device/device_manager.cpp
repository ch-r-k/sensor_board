#include "device_manager.hpp"

DeviceManager::DeviceManager(HardwareManager& hardwareManager)
{
    userIndication.setOutputPin(hardwareManager.getLedPin());
}

DeviceManager::~DeviceManager() {}

UserIndication& DeviceManager::getUserIndication() { return userIndication; }