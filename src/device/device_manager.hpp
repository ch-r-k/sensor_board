#ifndef DEVICE_MANAGER_HPP
#define DEVICE_MANAGER_HPP

#include "hardware_manager.hpp"
#include "sensors/aht10/aht10.hpp"
#include "user_indication/user_indication.hpp"
#include "serial_commander/serial_commander.hpp"
#include "device/sensors/test/test.hpp"
#include "device/display/ssd1306/ssd1306.hpp"

class DeviceManager
{
   private:
    UserIndication userIndication;
    SerialCommander serialCommander;
    Aht10 aht10;

    // Ssd1306 ssd1306;
    // Test test;

   public:
    DeviceManager(HardwareManager& hardwareManager);
    ~DeviceManager();
    UserIndication& getUserIndication();
    // Test& getTest();
    Aht10& getAht10();

    void start();
};

#endif  // DEVICE_MANAGER_HPP