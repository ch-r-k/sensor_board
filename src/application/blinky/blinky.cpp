//============================================================================
// QP/C++ Real-Time Embedded Framework
//============================================================================
//! @date Last updated on: 2022-08-25
//! @version Last updated Zephyr 3.1.99 and @ref qpcpp_7_1_0
//!
//! @file
//! @brief Blinky example
//!
#include "blinky.hpp"
#include "common.hpp"

// unnamed namespace for local definitions with internal linkage
namespace
{
// Q_DEFINE_THIS_FILE

}  // unnamed namespace

namespace APP
{
//............................................................................
Blinky::Blinky() : QP::QActive(&initial), m_timeEvt(this, BLINKY_TIMEOUT, 0U)
{
    // empty
}

// HSM definition ------------------------------------------------------------
Q_STATE_DEF(Blinky, initial)
{
    (void)e;  // unused parameter
    subscribe(AppSignals::BLINKY_START);

    QS_FUN_DICTIONARY(&idle);
    QS_FUN_DICTIONARY(&off);
    QS_FUN_DICTIONARY(&on);

    return tran(&idle);
}
//............................................................................
Q_STATE_DEF(Blinky, idle)
{
    QP::QState status;
    switch (e->sig)
    {
        case Q_ENTRY_SIG:
        {
            status = Q_RET_HANDLED;
            break;
        }
        case AppSignals::BLINKY_START:
        {
            // arm the time event to expire in half a second and every half
            // second
            m_timeEvt.armX(ticksPerSec / 2U, ticksPerSec / 2U);

            static QP::QEvt const myEvt{AppSignals::BLINKY_DONE};
            QP::QActive::PUBLISH(&myEvt, this);

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
        case BLINKY_TIMEOUT:
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
        case BLINKY_TIMEOUT:
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
