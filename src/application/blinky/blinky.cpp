#include "blinky.hpp"
#include "common.hpp"

namespace app
{
//............................................................................
Blinky::Blinky()
    : QP::QActive(&initial), m_timeEvt(this, system_layer::BLINKY_TIMEOUT, 0U)
{
    // empty
}

// HSM definition ------------------------------------------------------------
Q_STATE_DEF(Blinky, initial)
{
    (void)e;  // unused parameter
    subscribe(system_layer::BLINKY_START);

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
        case system_layer::BLINKY_START:
        {
            // arm the time event to expire in half a second and every half
            // second
            m_timeEvt.armX(TICKS_PER_SEC / 2U, TICKS_PER_SEC / 2U);

            static QP::QEvt const my_evt{system_layer::BLINKY_DONE};
            QP::QActive::PUBLISH(&my_evt, this);

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
        case system_layer::BLINKY_TIMEOUT:
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
        case system_layer::BLINKY_TIMEOUT:
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
void Blinky::setUserIndication(
    device_layer::IUserIndication& init_user_indication)
{
    userIndication = &init_user_indication;
}

}  // namespace app
