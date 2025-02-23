#include <cstring>
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
            m_timeEvt.armX(TICKS_PER_SEC / 2U, TICKS_PER_SEC / 2U);
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
            device_layer::interface::ISensor::SensorData data;

            data = Q_EVT_CAST(SensorEvent)->data;

            if (data.quantity ==
                device_layer::interface::ISensor::Quantities::TEMPERATURE)
            {
                iGui->setTempLabel(Q_EVT_CAST(SensorEvent)->data.value);
            }
            else if (data.quantity ==
                     device_layer::interface::ISensor::Quantities::HUMIDITY)
            {
                iGui->setHumidityLabel(Q_EVT_CAST(SensorEvent)->data.value);
            }

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
void Sensor::setSensorInterface(device_layer::interface::ISensor& i_sensor)
{
    iSensor = &i_sensor;
}

void Sensor::setGuiInterface(IGui& i_gui) { iGui = &i_gui; }

void Sensor::initDone()
{
    static QP::QEvt const my_evt{system_layer::SENSOR_INIT_DONE};
    this->POST(&my_evt, this);
}

void Sensor::readDone()
{
    iSensor->getMeasurement(
        device_layer::interface::ISensor::Quantities::HUMIDITY);

    SensorEvent* temperatureEvent =
        Q_NEW(SensorEvent, system_layer::SENSOR_READ_DONE);

    temperatureEvent->data.value =
        iSensor
            ->getMeasurement(
                device_layer::interface::ISensor::Quantities::TEMPERATURE)
            .value;

    temperatureEvent->data.quantity =
        iSensor
            ->getMeasurement(
                device_layer::interface::ISensor::Quantities::TEMPERATURE)
            .quantity;

    this->POST(temperatureEvent, this);

    SensorEvent* humidityEvent =
        Q_NEW(SensorEvent, system_layer::SENSOR_READ_DONE);

    humidityEvent->data.value =
        iSensor
            ->getMeasurement(
                device_layer::interface::ISensor::Quantities::HUMIDITY)
            .value;

    humidityEvent->data.quantity =
        iSensor
            ->getMeasurement(
                device_layer::interface::ISensor::Quantities::HUMIDITY)
            .quantity;

    this->POST(humidityEvent, this);
}

}  // namespace app
