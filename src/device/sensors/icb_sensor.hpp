#ifndef ICB_SENSOR_HPP
#define ICB_SENSOR_HPP

class IcbSensor
{
   private:
   public:
    virtual ~IcbSensor() = default;
    virtual void initDone() = 0;
    virtual void readDone() = 0;
};

#endif  // ICB_SENSOR_HPP