#ifndef DEVICE_MANAGER_HPP
#define DEVICE_MANAGER_HPP

#include "hardware_manager.hpp"
#include "user_indication/user_indication.hpp"
#include "device/sensors/test/test.hpp"

class DeviceManager
{
   private:
    UserIndication userIndication;
    Test test;

   public:
    DeviceManager(HardwareManager& hardwareManager);
    ~DeviceManager();
    UserIndication& getUserIndication();
    Test& getTest();
};

#endif  // DEVICE_MANAGER_HPP