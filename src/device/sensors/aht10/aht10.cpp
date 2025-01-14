#include "aht10.hpp"
#include <cassert>
#include <cstdint>
#include "sensors/icb_sensor.hpp"
#include "serial_commander/i_serial_commander.hpp"

Aht10::Aht10() {}
Aht10::~Aht10() {}

void Aht10::Open()
{
    assert(!open && "already open");

    open = true;
    Init();
}

void Aht10::Close()
{
    open = false;
    initialized = false;  // Reset initialization state
}

void Aht10::Init()
{
    assert(open && "must be open");

    const std::uint8_t commandContinuationByte =
        *(reinterpret_cast<std::uint8_t*>(&commandContinuation));

    ISerialCommander::Command command;
    command.instruction = ISerialCommander::Instructions::Write;
    command.data[0] = static_cast<std::uint8_t>(Command::INIT);
    command.data[1] = commandContinuationByte;
    command.data[2] = 0x00U;
    command.data_length = 3;

    iSerial->SetCommand(command);
    iSerial->StartCommands();

    initialized = true;
}

void Aht10::TriggerMeasurement()
{
    assert(open && "must be open");
    assert(initialized && "sensor must be initialized");

    const std::uint8_t commandContinuationByte =
        *(reinterpret_cast<std::uint8_t*>(&commandContinuation));

    // Trigger a measurement

    ISerialCommander::Command command;
    command.instruction = ISerialCommander::Instructions::Write;
    command.data[0] = static_cast<std::uint8_t>(Command::MEASURE);
    command.data[1] = commandContinuationByte;
    command.data[2] = 0x00U;
    command.data_length = 3;

    iSerial->SetCommand(command);
    iSerial->StartCommands();
}

void Aht10::TriggerRead()
{
    assert(open && "must be open");
    assert(initialized && "sensor must be initialized");

    ISerialCommander::Command command;
    command.instruction = ISerialCommander::Instructions::Read;
    command.data_length = 6;

    iSerial->SetCommand(command);
    iSerial->StartCommands();
}

void Aht10::setSerialInterface(ISerialCommander& i_serial)
{
    iSerial = &i_serial;
}

void Aht10::setIcbSensor(IcbSensor& icb_sensor) { icbSensor = &icb_sensor; }

void Aht10::WriteDone() { icbSensor->done(); }

void Aht10::ReadDone()
{
    temperature.value = (((readBuffer[3] & 0x0F) << 16) | (readBuffer[4] << 8) |
                         readBuffer[5]) *
                            200.0 / (1 << 20) -
                        50;

    humidity.value =
        ((readBuffer[1] << 12) | (readBuffer[2] << 4) | (readBuffer[3] >> 4)) *
        100.0 / (1 << 20);

    icbSensor->done();
}

ISensor::SensorData Aht10::GetMeasurement(Quantities quantity)
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
