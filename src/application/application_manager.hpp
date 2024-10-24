#ifndef APPLICATION_MANAGER_HPP
#define APPLICATION_MANAGER_HPP

#include "blinky/blinky.hpp"
#include "device_manager.hpp"

class ApplicationManager
{
   private:
    APP::Blinky blinky;

   public:
    ApplicationManager(DeviceManager& deviceManager);
    ~ApplicationManager();

    void start();
};

#endif  // APPLICATION_MANAGER_HPP
