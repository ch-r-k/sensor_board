#ifndef HARDWARE_MANAGER_HPP
#define HARDWARE_MANAGER_HPP

#include "output_pin/output_pin.hpp"
#include "driver/spi/spi.hpp"
#include "driver/uart/uart.hpp"
#include "driver/i2c/i2c.hpp"

class HardwareManager
{
   private:
    OutputPin ledPin;
    OutputPin TEST;
    Spi sensorSpi{2};
    I2c sensorI2c{2};
    I2c displayI2c{3};

    void SystemClock_Config();

   public:
    HardwareManager();
    ~HardwareManager();
    void run();
    OutputPin& getLedPin();
    Spi& getSensorSpi();
    I2c& getSensorI2c();
    I2c& getDisplayI2c();
};

#endif  // HARDWARE_MANAGER_HPP