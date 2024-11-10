#ifndef APPLICATION_MANAGER_HPP
#define APPLICATION_MANAGER_HPP

#include "qpcpp.hpp"
#include "blinky/blinky.hpp"
#include "sensor/sensor.hpp"
#include "startup/startup.hpp"

#include "device_manager.hpp"

class ApplicationManager
{
   private:
    APP::Blinky aoBlinky;
    APP::Sensor aoSensor;
    APP::Startup aoStartup;

   public:
    ApplicationManager(DeviceManager& deviceManager);
    ~ApplicationManager();

    void start();
};

#endif  // APPLICATION_MANAGER_HPP
