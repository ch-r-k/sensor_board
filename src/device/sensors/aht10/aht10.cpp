#include "aht10.hpp"
#include <cassert>
#include <cstdint>
#include "sensors/icb_sensor.hpp"

Aht10::Aht10() {}
Aht10::~Aht10() {}

void Aht10::Open()
{
    assert(!open && "already open");
    assert(iI2c != nullptr && "instance can't be nullptr");

    open = true;
    iI2c->SetAddress(address);
    iI2c->Open();
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

    // Construct the initialization command
    const std::array<std::uint8_t, 3> initCmd = {
        static_cast<std::uint8_t>(Command::INIT),  // Initialization command
        commandContinuationByte,  // "Command continuation" byte
        0x00                      // Reserved byte
    };
    iI2c->StartWrite(initCmd);

    initialized = true;
}

void Aht10::TriggerMeasurement()
{
    assert(open && "must be open");
    assert(initialized && "sensor must be initialized");

    const std::uint8_t commandContinuationByte =
        *(reinterpret_cast<std::uint8_t*>(&commandContinuation));

    // Trigger a measurement
    const std::array<std::uint8_t, 3> measureCmd = {
        static_cast<std::uint8_t>(
            Command::MEASURE),    // Trigger measurement command
        commandContinuationByte,  // "Command continuation" byte
        0x00                      // Additional options
    };
    iI2c->StartWrite(measureCmd);
}

void Aht10::TriggerRead()
{
    assert(open && "must be open");
    assert(initialized && "sensor must be initialized");

    iI2c->StartRead(readBuffer);
}

void Aht10::setI2cInterface(II2c& i_i2c) { iI2c = &i_i2c; }

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
