#ifndef AHT10_HPP
#define AHT10_HPP

#include "../i_sensor.hpp"
#include "../icb_sensor.hpp"
#include "serial_commander/i_serial_commander.hpp"
#include "serial_commander/icb_serial_commander.hpp"
#include <array>
#include <cassert>
#include <cstdint>

class Aht10 : public ISensor, public IcbSerialCommander
{
   public:
    // Enum for AHT10 commands
    enum class Command : std::uint8_t
    {
        INIT = 0xE1,     // Initialization command
        MEASURE = 0xAC,  // Trigger measurement command
        RESET = 0xBA     // Soft reset command
    };

    enum class Mode : std::uint8_t
    {
        NOR = 0b00,
        CYC = 0b01,
        CMD = 0b10
    };

    // Bitfield for the "command continuation" byte
    struct CommandContinuation
    {
        std::uint8_t reserved1 : 3;  // Reserved bits, always 0
        bool calibrationEnable : 1;  // Enable bit (set to 1)
        std::uint8_t reserved2 : 1;  // Reserved bits, always 0
        std::uint8_t mode : 2;       // Reserved bits, always 0
        std::uint8_t reserved3 : 1;  // Second enable bit (set to 1)
    };

   private:
    bool open = false;
    bool initialized = false;
    ISerialCommander* iSerial = nullptr;
    ISensor::Operation activeInstruction = ISensor::Operation::NOP;
    IcbSensor* icbSensor = nullptr;

    std::uint8_t address = 0x38;

    SensorData humidity = {0.0f, Quantities::HUMIDITY};
    SensorData temperature = {0.0f, Quantities::TEMPERATURE};

    // Default "command continuation" byte value
    CommandContinuation commandContinuation = {.reserved1 = 0,
                                               .calibrationEnable = 1,
                                               .reserved2 = 0,
                                               .mode = 1,
                                               .reserved3 = 0};

   public:
    Aht10();
    ~Aht10();

    void Open() override;
    void Close() override;
    void Init();
    void TriggerMeasurement() override;

    void ReadDone();

    void Done(ReturnValue return_value) override;

    void setSerialInterface(ISerialCommander& i_serial);
    void setIcbSensor(IcbSensor& icb_sensor);
    SensorData GetMeasurement(Quantities quantity) override;
};

#endif  // AHT10_HPP
