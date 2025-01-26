#include "gui.hpp"
#include <bits/c++config.h>
#include "application_signals.hpp"
#include "common.hpp"
#include "display/i_display.hpp"

namespace app
{
//............................................................................
Gui::Gui() : QP::QActive(&initial), m_timeEvt(this, BLINKY_TIMEOUT, 0U)
{
    // empty
}

// HSM definition ------------------------------------------------------------
Q_STATE_DEF(Gui, initial)
{
    (void)e;  // unused parameter
    subscribe(AppSignals::GUI_START);

    QS_FUN_DICTIONARY(&idle);
    QS_FUN_DICTIONARY(&initial);
    QS_FUN_DICTIONARY(&update);

    return tran(&idle);
}
//............................................................................
Q_STATE_DEF(Gui, idle)
{
    QP::QState status;
    switch (e->sig)
    {
        case Q_ENTRY_SIG:
        {
            status = Q_RET_HANDLED;
            break;
        }
        case AppSignals::GUI_START:
        {
            static QP::QEvt const my_evt{AppSignals::GUI_DONE};
            QP::QActive::PUBLISH(&my_evt, this);

            status = tran(&initialize);
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
Q_STATE_DEF(Gui, initialize)
{
    QP::QState status;
    switch (e->sig)
    {
        case Q_ENTRY_SIG:
        {
            iDisplay->open();
            status = Q_RET_HANDLED;
            break;
        }
        case GUI_INIT_DONE:
        {
            status = tran(&update);
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
Q_STATE_DEF(Gui, update)
{
    QP::QState status;
    switch (e->sig)
    {
        case Q_ENTRY_SIG:
        {
            for (std::size_t it = 0; it < 32; it++)
            {
                iDisplay->drawPixel(it, it);
            }
            iDisplay->update();
            status = Q_RET_HANDLED;
            break;
        }
        case GUI_UPDATE_DONE:
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
//............................................................................
void Gui::setDisplayInterface(IDisplay& i_display) { iDisplay = &i_display; }

void Gui::initDone()
{
    static QP::QEvt const my_evt{AppSignals::GUI_INIT_DONE};
    this->POST(&my_evt, this);
}

void Gui::printDone()
{
    static QP::QEvt const my_evt{AppSignals::GUI_UPDATE_DONE};
    this->POST(&my_evt, this);
}

}  // namespace app
