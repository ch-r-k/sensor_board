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
#include "startup.hpp"

// unnamed namespace for local definitions with internal linkage
namespace
{
// Q_DEFINE_THIS_FILE

}  // unnamed namespace

namespace app
{
//............................................................................
Startup::Startup() : QP::QActive(&initial)
{
    // empty
}

// HSM definition ------------------------------------------------------------
Q_STATE_DEF(Startup, initial)
{
    (void)e;  // unused parameter

    subscribe(AppSignals::BLINKY_DONE);
    subscribe(AppSignals::SENSOR_DONE);
    subscribe(AppSignals::GUI_DONE);

    QS_FUN_DICTIONARY(&blinky);
    QS_FUN_DICTIONARY(&sensor);
    QS_FUN_DICTIONARY(&done);

    return tran(&blinky);
}
//............................................................................
Q_STATE_DEF(Startup, blinky)
{
    QP::QState status;
    switch (e->sig)
    {
        case Q_ENTRY_SIG:
        {
            static QP::QEvt const my_evt{AppSignals::BLINKY_START};
            QP::QActive::PUBLISH(&my_evt, this);

            status = Q_RET_HANDLED;
            break;
        }
        case AppSignals::BLINKY_DONE:
        {
            status = tran(&sensor);
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
Q_STATE_DEF(Startup, sensor)
{
    QP::QState status;
    switch (e->sig)
    {
        case Q_ENTRY_SIG:
        {
            static QP::QEvt const my_evt{AppSignals::SENSOR_START};
            QP::QActive::PUBLISH(&my_evt, this);

            status = Q_RET_HANDLED;
            break;
        }
        case AppSignals::SENSOR_DONE:
        {
            status = tran(&display);
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
Q_STATE_DEF(Startup, display)
{
    QP::QState status;
    switch (e->sig)
    {
        case Q_ENTRY_SIG:
        {
            static QP::QEvt const my_evt{AppSignals::GUI_START};
            QP::QActive::PUBLISH(&my_evt, this);

            status = Q_RET_HANDLED;
            break;
        }
        case AppSignals::GUI_DONE:
        {
            status = tran(&done);
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
Q_STATE_DEF(Startup, done)
{
    QP::QState status;
    switch (e->sig)
    {
        case Q_ENTRY_SIG:
        {
            status = Q_RET_HANDLED;
            break;
        }
        case SENSOR_DONE:
        {
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

}  // namespace app
