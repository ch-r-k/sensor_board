//============================================================================
// Blinky example
//============================================================================

#ifndef SENSOR_HPP_
#define SENSOR_HPP_

#include <cstdint>
#include "qpcpp.hpp"
#include "device/sensors/i_sensor.hpp"
#include "device/sensors/icb_sensor.hpp"

#include "application_signals.hpp"

namespace APP
{
class Sensor : public QP::QActive, public IcbSensor
{
   private:
    QP::QTimeEvt m_timeEvt;
    ISensor* iSensor = nullptr;
    void done() override;

   public:
    Sensor();
    void setSensorInterface(ISensor& i_sensor);

   protected:
    Q_STATE_DECL(initial);
    Q_STATE_DECL(idle);
    Q_STATE_DECL(init);
    Q_STATE_DECL(start_measurement);
    Q_STATE_DECL(pause);
    Q_STATE_DECL(read_measurement);
};

}  // namespace APP

#endif  // SENSOR_HPP_
