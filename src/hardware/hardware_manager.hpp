#ifndef HARDWARE_MANAGER_HPP
#define HARDWARE_MANAGER_HPP

#include "output_pin/output_pin.hpp"
#include "driver/spi/spi.hpp"
#include "driver/uart/uart.hpp"
#include "driver/i2c/i2c.hpp"
#include "stm32l4xx_hal_def.h"

class HardwareManager
{
   private:
    hardware_layer::OutputPin ledPin;
    hardware_layer::OutputPin TEST;
    hardware_layer::Spi sensorSpi{2};
    hardware_layer::I2c sensorI2c{2};
    hardware_layer::I2c displayI2c{3};

    void systemClockConfig();

   public:
    HardwareManager();
    ~HardwareManager();
    void run();
    hardware_layer::OutputPin& getLedPin();
    hardware_layer::Spi& getSensorSpi();
    hardware_layer::I2c& getSensorI2c();
    hardware_layer::I2c& getDisplayI2c();

    static void errorCallback([[maybe_unused]] int error_code)
    {
        UNUSED(error_code);
        assert(false && "error handling not implemented");

        // Add error-specific handling logic here if needed
    }
};

#endif  // HARDWARE_MANAGER_HPP