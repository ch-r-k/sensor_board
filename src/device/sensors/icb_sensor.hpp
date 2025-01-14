#ifndef ICB_SENSOR_HPP
#define ICB_SENSOR_HPP

class IcbSensor
{
   private:
   public:
    virtual ~IcbSensor() = default;
    virtual void done() = 0;
};

#endif  // ICB_SENSOR_HPP