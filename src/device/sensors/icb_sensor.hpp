#ifndef ICB_SENSOR_HPP
#define ICB_SENSOR_HPP
namespace device_layer
{
class IcbSensor
{
   private:
   public:
    virtual ~IcbSensor() = default;
    virtual void initDone() = 0;
    virtual void readDone() = 0;
};

}  // namespace device_layer

#endif  // ICB_SENSOR_HPP