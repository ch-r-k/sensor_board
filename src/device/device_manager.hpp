#ifndef DEVICE_MANAGER_HPP
#define DEVICE_MANAGER_HPP

#include "hardware_manager.hpp"
#include "user_indication/user_indication.hpp"

class DeviceManager
{
   private:
    UserIndication userIndication;

   public:
    DeviceManager(HardwareManager& hardwareManager);
    ~DeviceManager();
    UserIndication& getUserIndication();
};

#endif  // DEVICE_MANAGER_HPP