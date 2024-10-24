#include "system_manager.hpp"

SystemManager::SystemManager()
    : hardwareManager(),
      deviceManager(hardwareManager),
      applicationManager(deviceManager)
{
}

SystemManager::~SystemManager() {}

void SystemManager::run() { applicationManager.start(); }
