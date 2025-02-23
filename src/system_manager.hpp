#ifndef SYSTEM_MANAGER_HPP
#define SYSTEM_MANAGER_HPP

#include "application/application_manager.hpp"
#include "device/device_manager.hpp"
#include "hardware/hardware_manager.hpp"

namespace manager_
{

class SystemManager
{
   private:
    manager::HardwareManager hardwareManager;
    manager::DeviceManager deviceManager;
    manager::ApplicationManager applicationManager;

   public:
    SystemManager();
    ~SystemManager();
    void run();
};

}  // namespace manager_

#endif  // SYSTEM_MANAGER_HPP
