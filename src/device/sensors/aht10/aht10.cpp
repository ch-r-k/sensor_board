#include "aht10.hpp"
#include <cassert>
#include <cstdint>
#include "sensors/icb_sensor.hpp"
#include "serial_commander/i_serial_commander.hpp"

namespace device_layer
{
Aht10::Aht10() {}
Aht10::~Aht10() {}

void Aht10::open()
{
    assert(!isOpen && "already open");

    isOpen = true;
    init();
}

void Aht10::close()
{
    isOpen = false;
    initialized = false;  // Reset initialization state
}

void Aht10::init()
{
    assert(isOpen && "must be open");

    const std::uint8_t commandContinuationByte =
        *(reinterpret_cast<std::uint8_t*>(&commandContinuation));

    hardware_layer::ISerialCommander::Command command;
    command.instruction = hardware_layer::ISerialCommander::Instructions::WRITE;
    command.data[0] = static_cast<std::uint8_t>(Command::INIT);
    command.data[1] = commandContinuationByte;
    command.data[2] = 0x00U;
    command.data_length = 3;

    iSerial->setCommand(command);

    iSerial->setI2CAddress(address);
    iSerial->startCommands();

    activeInstruction = ISensor::Operation::INIT;
    initialized = true;
}

void Aht10::triggerMeasurement()
{
    assert(isOpen && "must be open");
    assert(initialized && "sensor must be initialized");

    const std::uint8_t commandContinuationByte =
        *(reinterpret_cast<std::uint8_t*>(&commandContinuation));

    // Trigger a measurement

    hardware_layer::ISerialCommander::Command command;

    // Start Measurement
    command.instruction = hardware_layer::ISerialCommander::Instructions::WRITE;
    command.data[0] = static_cast<std::uint8_t>(Command::MEASURE);
    command.data[1] = commandContinuationByte;
    command.data[2] = 0x00U;
    command.data_length = 3;
    iSerial->setCommand(command);

    // Pause
    command.instruction = hardware_layer::ISerialCommander::Instructions::PAUSE;
    command.data_length = 0;
    command.pauseTime = 100;
    iSerial->setCommand(command);

    // Read Measurement
    command.instruction = hardware_layer::ISerialCommander::Instructions::READ;
    command.data_length = 6;
    iSerial->setCommand(command);

    // Start
    activeInstruction = ISensor::Operation::MEASURE;
    iSerial->setI2CAddress(address);
    iSerial->startCommands();
}

void Aht10::setSerialInterface(hardware_layer::ISerialCommander& i_serial)
{
    iSerial = &i_serial;
}

void Aht10::setIcbSensor(IcbSensor& icb_sensor) { icbSensor = &icb_sensor; }

void Aht10::done(ReturnValue return_value)
{
    //
    switch (activeInstruction)
    {
        case Operation::INIT:
        {
            icbSensor->initDone();
            break;
        }
        case Operation::MEASURE:
        {
            temperature.value =
                (((return_value.data[3] & 0x0F) << 16) |
                 (return_value.data[4] << 8) | return_value.data[5]) *
                    200.0 / (1 << 20) -
                50;

            humidity.value =
                ((return_value.data[1] << 12) | (return_value.data[2] << 4) |
                 (return_value.data[3] >> 4)) *
                100.0 / (1 << 20);

            icbSensor->readDone();
            break;
        }
        case Operation::NOP:
        {
            assert(false);
            break;
        }
    }

    activeInstruction = ISensor::Operation::NOP;
}

ISensor::SensorData Aht10::getMeasurement(Quantities quantity)
{
    switch (quantity)
    {
        case ISensor::Quantities::TEMPERATURE:
            return temperature;
            break;

        case ISensor::Quantities::HUMIDITY:
            return humidity;
            break;
        default:
            assert(false && "not doesn't include this quantity");
            break;
    }
}

}  // namespace device_layer