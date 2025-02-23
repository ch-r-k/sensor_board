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

    subscribe(system_layer::BLINKY_DONE);
    subscribe(system_layer::SENSOR_DONE);
    subscribe(system_layer::GUI_DONE);

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
            static QP::QEvt const my_evt{system_layer::BLINKY_START};
            QP::QActive::PUBLISH(&my_evt, this);

            status = Q_RET_HANDLED;
            break;
        }
        case system_layer::BLINKY_DONE:
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
            static QP::QEvt const my_evt{system_layer::SENSOR_START};
            QP::QActive::PUBLISH(&my_evt, this);

            status = Q_RET_HANDLED;
            break;
        }
        case system_layer::SENSOR_DONE:
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
            static QP::QEvt const my_evt{system_layer::GUI_START};
            QP::QActive::PUBLISH(&my_evt, this);

            status = Q_RET_HANDLED;
            break;
        }
        case system_layer::GUI_DONE:
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
        case system_layer::SENSOR_DONE:
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
