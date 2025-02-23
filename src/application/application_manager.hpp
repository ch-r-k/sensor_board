#ifndef APPLICATION_MANAGER_HPP
#define APPLICATION_MANAGER_HPP

#include "qpcpp.hpp"

#include "blinky/blinky.hpp"
#include "sensor/sensor.hpp"
#include "gui/gui.hpp"
#include "startup/startup.hpp"

#include "device_manager.hpp"

namespace manager
{

class ApplicationManager
{
   private:
    app::Blinky aoBlinky;
    app::Sensor aoSensor;
    app::Startup aoStartup;
    app::Gui aoGui;

   public:
    ApplicationManager(DeviceManager& device_manager);
    ~ApplicationManager();

    void start();
};

}  // namespace manager

#endif  // APPLICATION_MANAGER_HPP
