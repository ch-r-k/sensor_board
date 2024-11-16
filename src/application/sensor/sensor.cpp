//============================================================================
// QP/C++ Real-Time Embedded Framework
//============================================================================
//! @date Last updated on: 2022-08-25
//! @version Last updated Zephyr 3.1.99 and @ref qpcpp_7_1_0
//!
//! @file
//! @brief Blinky example
//!
#include "qpcpp.hpp"
#include "sensor.hpp"

// unnamed namespace for local definitions with internal linkage
namespace
{
// Q_DEFINE_THIS_FILE

}  // unnamed namespace

namespace APP
{
//............................................................................
Sensor::Sensor() : QP::QActive(&initial), m_timeEvt(this, SENSOR_TIMEOUT, 0U)
{
    // empty
}

// HSM definition ------------------------------------------------------------
Q_STATE_DEF(Sensor, initial)
{
    (void)e;  // unused parameter

    // arm the time event to expire in half a second and every half second
    m_timeEvt.armX(ticksPerSec / 2U, ticksPerSec / 2U);
    return tran(&idle);
}
//............................................................................
Q_STATE_DEF(Sensor, idle)
{
    QP::QState status;
    switch (e->sig)
    {
        case Q_ENTRY_SIG:
        {
            status = Q_RET_HANDLED;
            break;
        }
        case SENSOR_START:
        {
            iSensor->Open();

            static QP::QEvt const myEvt{AppSignals::SENSOR_DONE};
            QP::QActive::PUBLISH(&myEvt, this);

            status = Q_RET_HANDLED;
            break;
        }
        default:
        {
            status = super(&top);
            break;
        }
    }
    return status;
}
//............................................................................
Q_STATE_DEF(Sensor, main)
{
    QP::QState status;
    switch (e->sig)
    {
        case Q_ENTRY_SIG:
        {
            status = Q_RET_HANDLED;
            break;
        }
        case SENSOR_TIMEOUT:
        {
            iSensor->StartSensor();
            status = Q_RET_HANDLED;
            break;
        }
        default:
        {
            status = super(&top);
            break;
        }
    }
    return status;
}
//............................................................................
void Sensor::setSensorInterface(ISensor& i_sensor) { iSensor = &i_sensor; }

}  // namespace APP
