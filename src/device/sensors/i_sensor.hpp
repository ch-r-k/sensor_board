#ifndef I_SENSOR_HPP
#define I_SENSOR_HPP

class ISensor
{
   private:
   public:
    virtual void Open() = 0;
    virtual void Close() = 0;
    virtual void StartSensor() = 0;
};

#endif  // I_SENSOR_HPP