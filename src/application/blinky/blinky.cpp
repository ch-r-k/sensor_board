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
#include "blinky.hpp"
#include "qpcpp_callbacks.hpp"

// unnamed namespace for local definitions with internal linkage
namespace
{
// Q_DEFINE_THIS_FILE

}  // unnamed namespace

namespace APP
{
//............................................................................
Blinky::Blinky() : QP::QActive(&initial), m_timeEvt(this, TIMEOUT_SIG, 0U)
{
    // empty
}

// HSM definition ------------------------------------------------------------
Q_STATE_DEF(Blinky, initial)
{
    (void)e;  // unused parameter

    // arm the time event to expire in half a second and every half second
    m_timeEvt.armX(TICKS_PER_SEC / 2U, TICKS_PER_SEC / 2U);
    return tran(&off);
}
//............................................................................
Q_STATE_DEF(Blinky, off)
{
    QP::QState status;
    switch (e->sig)
    {
        case Q_ENTRY_SIG:
        {
            userIndication->reset();
            status = Q_RET_HANDLED;
            break;
        }
        case TIMEOUT_SIG:
        {
            status = tran(&on);
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
Q_STATE_DEF(Blinky, on)
{
    QP::QState status;
    switch (e->sig)
    {
        case Q_ENTRY_SIG:
        {
            userIndication->set();
            status = Q_RET_HANDLED;
            break;
        }
        case TIMEOUT_SIG:
        {
            status = tran(&off);
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
void Blinky::setUserIndication(IUserIndication& initUserIndication)
{
    userIndication = &initUserIndication;
}

}  // namespace APP
