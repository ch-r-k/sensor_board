#ifndef SENSOR_HPP_
#define SENSOR_HPP_

// std libs
#include <cstdint>

// 3d party libs
#include "qpcpp.hpp"

// app layer interfaces
#include "application/gui/i_gui.hpp"

// device layer interface
#include "device/sensors/i_sensor.hpp"
#include "device/sensors/icb_sensor.hpp"

// signals
#include "system_signals.hpp"

namespace app
{
class SensorEvent : public QP::QEvt
{
   public:
    device_layer::interface::ISensor::SensorData data;
};
class Sensor : public QP::QActive, public device_layer::interface::IcbSensor
{
   private:
    QP::QTimeEvt m_timeEvt;
    device_layer::interface::ISensor* iSensor = nullptr;
    IGui* iGui = nullptr;

    void initDone() override;
    void readDone() override;

   public:
    Sensor();
    void setSensorInterface(device_layer::interface::ISensor& i_sensor);
    void setGuiInterface(IGui& i_gui);

   protected:
    Q_STATE_DECL(initial);
    Q_STATE_DECL(idle);
    Q_STATE_DECL(initialize);
    Q_STATE_DECL(read_measurement);
};

}  // namespace app

#endif  // SENSOR_HPP_
