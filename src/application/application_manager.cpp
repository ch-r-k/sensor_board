#include "application_manager.hpp"

ApplicationManager::ApplicationManager(DeviceManager& deviceManager)
{
    blinky.setUserIndication(deviceManager.getUserIndication());
}

ApplicationManager::~ApplicationManager() {}

void ApplicationManager::start()
{
    blinky.run();
}