#ifndef SENSOR_HPP_
#define SENSOR_HPP_

#include <cstdint>
#include "qpcpp.hpp"
#include "device/sensors/i_sensor.hpp"
#include "device/sensors/icb_sensor.hpp"

#include "system_signals.hpp"

namespace app
{
class SensorEvent : public QP::QEvt
{
   public:
    device_layer::ISensor::SensorData data;
};
class Sensor : public QP::QActive, public device_layer::IcbSensor
{
   private:
    QP::QTimeEvt m_timeEvt;
    device_layer::ISensor* iSensor = nullptr;
    void initDone() override;
    void readDone() override;

   public:
    Sensor();
    void setSensorInterface(device_layer::ISensor& i_sensor);

   protected:
    Q_STATE_DECL(initial);
    Q_STATE_DECL(idle);
    Q_STATE_DECL(initialize);
    Q_STATE_DECL(read_measurement);
};

}  // namespace app

#endif  // SENSOR_HPP_
