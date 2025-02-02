#ifndef SYSTEM_SIGNALS_HPP_
#define SYSTEM_SIGNALS_HPP_

#include "qpcpp.hpp"

namespace system_layer
{
enum Signals : QP::QSignal
{
    DUMMY_SIG = QP::Q_USER_SIG,
    SENSOR_START,
    SENSOR_DONE,
    SENSOR_TIMEOUT,
    BLINKY_START,
    BLINKY_DONE,
    GUI_START,
    GUI_DONE,
    SERIAL_COMMANDER_SET,
    SERIAL_COMMANDER_START,
    MAX_PUB_SIG,  // the last published signal

    SENSOR_INIT_DONE,
    SENSOR_READ_DONE,
    GUI_UPDATE_DONE,
    GUI_INIT_DONE,
    BLINKY_TIMEOUT,
    GUI_TIMEOUT,
    SERIAL_COMMANDER_TIMEOUT,
    SERIAL_COMMANDER_DONE,
    MAX_SIG  // the last signal
};
}

#endif  // SYSTEM_SIGNALS_HPP_