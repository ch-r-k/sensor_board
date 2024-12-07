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

namespace APP
{
//............................................................................
Startup::Startup() : QP::QActive(&initial), m_timeEvt(this, SENSOR_TIMEOUT, 0U)
{
    // empty
}

// HSM definition ------------------------------------------------------------
Q_STATE_DEF(Startup, initial)
{
    (void)e;  // unused parameter

    subscribe(AppSignals::BLINKY_DONE);
    subscribe(AppSignals::SENSOR_DONE);

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
            static QP::QEvt const myEvt{AppSignals::BLINKY_START};
            QP::QActive::PUBLISH(&myEvt, this);

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
            static QP::QEvt const myEvt{AppSignals::SENSOR_START};
            QP::QActive::PUBLISH(&myEvt, this);

            status = Q_RET_HANDLED;
            break;
        }
        case AppSignals::SENSOR_DONE:
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
        case SENSOR_TIMEOUT:
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

}  // namespace APP
