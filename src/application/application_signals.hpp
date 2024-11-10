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
    MAX_PUB_SIG,  // the last published signal

    BLINKY_TIMEOUT,
    SENSOR_TIMEOUT,
    MAX_SIG  // the last signal
};
}

#endif  // APPLICATION_SIGNALS_HPP_