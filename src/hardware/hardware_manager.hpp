#ifndef HARDWARE_MANAGER_HPP
#define HARDWARE_MANAGER_HPP

#include "output_pin/output_pin.hpp"
#include "driver/spi/spi.hpp"
#include "driver/uart/uart.hpp"

class HardwareManager
{
   private:
    OutputPin ledPin;
    OutputPin TEST;
    Spi sensorSpi{2};

    void SystemClock_Config();

   public:
    HardwareManager();
    ~HardwareManager();
    void run();
    OutputPin& getLedPin();
    Spi& getSensorSpi();
};

#endif  // HARDWARE_MANAGER_HPP