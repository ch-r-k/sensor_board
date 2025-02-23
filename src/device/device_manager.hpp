#ifndef DEVICE_MANAGER_HPP
#define DEVICE_MANAGER_HPP

#include "hardware_manager.hpp"

#include "sensors/aht10/aht10.hpp"
#include "display/ssd1306/ssd1306.hpp"
#include "user_indication/user_indication.hpp"

namespace manager
{

class DeviceManager
{
   private:
    device_layer::UserIndication userIndication;
    device_layer::Aht10 aht10;
    device_layer::Ssd1306 ssd1306;

    // Test test;

   public:
    DeviceManager(HardwareManager& hardware_manager);
    ~DeviceManager();
    device_layer::UserIndication& getUserIndication();
    device_layer::Aht10& getAht10();
    device_layer::Ssd1306& getSsd1306();

    void start();
};

}  // namespace manager

#endif  // DEVICE_MANAGER_HPP