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
#include "common.hpp"

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
    subscribe(AppSignals::SENSOR_START);

    QS_FUN_DICTIONARY(&idle);
    QS_FUN_DICTIONARY(&init);
    QS_FUN_DICTIONARY(&start_measurement);
    QS_FUN_DICTIONARY(&pause);
    QS_FUN_DICTIONARY(&read_measurement);
    QS_FUN_DICTIONARY(&pause);
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

            status = tran(init);
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
Q_STATE_DEF(Sensor, init)
{
    QP::QState status;
    switch (e->sig)
    {
        case Q_ENTRY_SIG:
        {
            m_timeEvt.armX(ticksPerSec / 2U, 0);
            status = Q_RET_HANDLED;
            break;
        }
        case SENSOR_TIMEOUT:
        {
            status = tran(start_measurement);
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
Q_STATE_DEF(Sensor, start_measurement)
{
    QP::QState status;
    switch (e->sig)
    {
        case Q_ENTRY_SIG:
        {
            iSensor->TriggerMeasurement();
            status = Q_RET_HANDLED;
            break;
        }
        case SENSOR_READ_DONE:
        {
            status = tran(pause);
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
Q_STATE_DEF(Sensor, pause)
{
    QP::QState status;
    switch (e->sig)
    {
        case Q_ENTRY_SIG:
        {
            m_timeEvt.armX(ticksPerSec / 2U, 0);
            status = Q_RET_HANDLED;
            break;
        }
        case SENSOR_TIMEOUT:
        {
            status = tran(read_measurement);
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
Q_STATE_DEF(Sensor, read_measurement)
{
    QP::QState status;
    switch (e->sig)
    {
        case Q_ENTRY_SIG:
        {
            iSensor->TriggerRead();
            status = Q_RET_HANDLED;
            break;
        }
        case SENSOR_READ_DONE:
        {
            status = tran(init);
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

void Sensor::done()
{
    static QP::QEvt const myEvt{AppSignals::SENSOR_READ_DONE};
    QP::QActive::POST(&myEvt, this);
}

}  // namespace APP
