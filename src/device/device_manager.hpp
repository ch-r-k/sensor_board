#ifndef DEVICE_MANAGER_HPP
#define DEVICE_MANAGER_HPP

#include "hardware_manager.hpp"
#include "sensors/aht10/aht10.hpp"
#include "user_indication/user_indication.hpp"
#include "device/sensors/test/test.hpp"

class DeviceManager
{
   private:
    UserIndication userIndication;
    Aht10 aht10;
    // Test test;

   public:
    DeviceManager(HardwareManager& hardwareManager);
    ~DeviceManager();
    UserIndication& getUserIndication();
    // Test& getTest();
    Aht10& getAht10();
};

#endif  // DEVICE_MANAGER_HPP