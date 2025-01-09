#ifndef I_SENSOR_HPP
#define I_SENSOR_HPP

#include <cstdint>
class ISensor
{
   public:
    enum class Quantities : std::uint8_t
    {
        TEMPERATURE,
        HUMIDITY
    };
    struct SensorData
    {
        float value;
        Quantities quantity;
    };

   private:
   public:
    virtual void Open() = 0;
    virtual void Close() = 0;
    virtual void TriggerMeasurement() = 0;
    virtual void TriggerRead() = 0;
    virtual SensorData GetMeasurement(Quantities quantity) = 0;
};

#endif  // I_SENSOR_HPP