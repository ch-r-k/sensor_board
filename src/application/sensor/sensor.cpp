#include "system_signals.hpp"
#include "qpcpp.hpp"
#include "sensor.hpp"
#include "common.hpp"

namespace app
{
//............................................................................
Sensor::Sensor()
    : QP::QActive(&initial), m_timeEvt(this, system_layer::SENSOR_TIMEOUT, 0U)
{
    // empty
}

// HSM definition ------------------------------------------------------------
Q_STATE_DEF(Sensor, initial)
{
    (void)e;  // unused parameter

    // arm the time event to expire in half a second and every half second
    subscribe(system_layer::SENSOR_START);

    QS_FUN_DICTIONARY(&idle);
    QS_FUN_DICTIONARY(&initialize);
    QS_FUN_DICTIONARY(&read_measurement);

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
        case system_layer::SENSOR_START:
        {
            static QP::QEvt const my_evt{system_layer::SENSOR_DONE};
            QP::QActive::PUBLISH(&my_evt, this);

            status = tran(initialize);
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
Q_STATE_DEF(Sensor, initialize)
{
    QP::QState status;
    switch (e->sig)
    {
        case Q_ENTRY_SIG:
        {
            iSensor->open();
            status = Q_RET_HANDLED;
            break;
        }
        case system_layer::SENSOR_INIT_DONE:
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
            m_timeEvt.armX(TICKS_PER_SEC / 2U, 0);
            status = Q_RET_HANDLED;
            break;
        }
        case system_layer::SENSOR_TIMEOUT:
        {
            iSensor->triggerMeasurement();
            status = Q_RET_HANDLED;
            break;
        }
        case system_layer::SENSOR_READ_DONE:
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
void Sensor::setSensorInterface(device_layer::ISensor& i_sensor)
{
    iSensor = &i_sensor;
}

void Sensor::initDone()
{
    static QP::QEvt const my_evt{system_layer::SENSOR_INIT_DONE};
    this->POST(&my_evt, this);
}

void Sensor::readDone()
{
    iSensor->getMeasurement(device_layer::ISensor::Quantities::HUMIDITY);

    SensorEvent* sensorEvent =
        Q_NEW(SensorEvent, system_layer::SENSOR_READ_DONE);

    sensorEvent->data.value =
        iSensor->getMeasurement(device_layer::ISensor::Quantities::TEMPERATURE)
            .value;

    sensorEvent->data.quantity =
        iSensor->getMeasurement(device_layer::ISensor::Quantities::TEMPERATURE)
            .quantity;

    this->POST(sensorEvent, this);
}

}  // namespace app
