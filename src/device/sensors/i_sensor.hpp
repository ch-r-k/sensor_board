#ifndef I_SENSOR_HPP
#define I_SENSOR_HPP

#include <cstdint>

namespace device_layer
{
namespace interface
{
class ISensor
{
   public:
    enum class Operation : std::uint8_t
    {
        NOP,
        INIT,
        MEASURE,
    };
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
    virtual ~ISensor() = default;
    virtual void open() = 0;
    virtual void close() = 0;
    virtual void triggerMeasurement() = 0;
    virtual SensorData getMeasurement(Quantities quantity) = 0;
};
}  // namespace interface
}  // namespace device_layer

#endif  // I_SENSOR_HPP