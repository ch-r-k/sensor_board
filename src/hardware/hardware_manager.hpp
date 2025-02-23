#ifndef HARDWARE_MANAGER_HPP
#define HARDWARE_MANAGER_HPP

#include "driver/output_pin/output_pin.hpp"
#include "driver/spi/spi.hpp"
#include "driver/uart/uart.hpp"
#include "driver/i2c/i2c.hpp"
#include "serial_commander/serial_commander.hpp"
#include "stm32l4xx_hal_def.h"

namespace manager
{

class HardwareManager
{
   private:
    hardware_layer::OutputPin ledPin;
    hardware_layer::OutputPin TEST;
    hardware_layer::Spi sensorSpi{2};
    hardware_layer::I2c sensorI2c{2};
    hardware_layer::I2c displayI2c{3};
    hardware_layer::SerialCommander aoSensor;
    hardware_layer::SerialCommander aoDisplay;

    void systemClockConfig();

   public:
    HardwareManager();
    ~HardwareManager();
    void run();
    hardware_layer::OutputPin& getLedPin();
    hardware_layer::Spi& getSensorSpi();
    hardware_layer::SerialCommander& getDisplaySerial();
    hardware_layer::SerialCommander& getSensorSerial();

    static void errorCallback([[maybe_unused]] int error_code)
    {
        UNUSED(error_code);
        assert(false && "error handling not implemented");

        // Add error-specific handling logic here if needed
    }
};

}  // namespace manager

#endif  // HARDWARE_MANAGER_HPP