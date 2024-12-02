//============================================================================
// Blinky example
//============================================================================

#ifndef SENSOR_HPP_
#define SENSOR_HPP_

#include <cstdint>
#include "qpcpp.hpp"
#include "device/sensors/i_sensor.hpp"
#include "application_signals.hpp"

namespace APP
{
class Sensor : public QP::QActive
{
   private:
    QP::QTimeEvt m_timeEvt;
    ISensor* iSensor = nullptr;

   public:
    Sensor();
    void setSensorInterface(ISensor& i_sensor);

   protected:
    Q_STATE_DECL(initial);
    Q_STATE_DECL(idle);
    Q_STATE_DECL(main);
};

}  // namespace APP

#endif  // SENSOR_HPP_
