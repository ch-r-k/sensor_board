#ifndef APPLICATION_SIGNALS_HPP_
#define APPLICATION_SIGNALS_HPP_

#include "qpcpp.hpp"

namespace APP
{
enum AppSignals : QP::QSignal
{
    DUMMY_SIG = QP::Q_USER_SIG,
    SENSOR_START,
    SENSOR_DONE,
    BLINKY_START,
    BLINKY_DONE,
    SERIAL_COMMANDER_SET,
    SERIAL_COMMANDER_START,
    MAX_PUB_SIG,  // the last published signal

    SENSOR_READ_DONE,
    BLINKY_TIMEOUT,
    SENSOR_TIMEOUT,
    SERIAL_COMMANDER_TIMEOUT,
    SERIAL_COMMANDER_DONE,
    MAX_SIG  // the last signal
};
}

#endif  // APPLICATION_SIGNALS_HPP_