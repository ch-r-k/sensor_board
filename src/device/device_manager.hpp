#ifndef DEVICE_MANAGER_HPP
#define DEVICE_MANAGER_HPP

#include "hardware_manager.hpp"

#include "serial_commander/serial_commander.hpp"

#include "sensors/test/test.hpp"
#include "sensors/aht10/aht10.hpp"
#include "display/ssd1306/ssd1306.hpp"
#include "user_indication/user_indication.hpp"

class DeviceManager
{
   private:
    UserIndication userIndication;
    SerialCommander aoSensor;
    SerialCommander aoDisplay;
    Aht10 aht10;
    Ssd1306 ssd1306;

    // Test test;

   public:
    DeviceManager(HardwareManager& hardwareManager);
    ~DeviceManager();
    UserIndication& getUserIndication();
    // Test& getTest();
    Aht10& getAht10();
    Ssd1306& getSsd1306();

    void start();
};

#endif  // DEVICE_MANAGER_HPP