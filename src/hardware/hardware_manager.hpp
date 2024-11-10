#ifndef HARDWARE_MANAGER_HPP
#define HARDWARE_MANAGER_HPP

#include "output_pin/output_pin.hpp"
#include "driver/spi/spi.hpp"

class HardwareManager
{
   private:
    OutputPin ledPin;
    OutputPin TEST;
    Spi sensorSpi;

   public:
    HardwareManager();
    ~HardwareManager();
    void run();
    OutputPin& getLedPin();
    Spi& getSensorSpi();
};

#endif  // HARDWARE_MANAGER_HPP