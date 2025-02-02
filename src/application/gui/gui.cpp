#include "gui.hpp"
#include <bits/c++config.h>
#include "system_signals.hpp"
#include "common.hpp"
#include "display/i_display.hpp"

namespace app
{
//............................................................................
Gui::Gui()
    : QP::QActive(&initial), m_timeEvt(this, system_layer::GUI_TIMEOUT, 0U)
{
    // empty
}

// HSM definition ------------------------------------------------------------
Q_STATE_DEF(Gui, initial)
{
    (void)e;  // unused parameter
    subscribe(system_layer::GUI_START);

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
        case system_layer::GUI_START:
        {
            static QP::QEvt const my_evt{system_layer::GUI_DONE};
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
        case system_layer::GUI_INIT_DONE:
        {
            status = tran(&update);
            x = 0;
            y = 0;
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
            iDisplay->clear();

            iDisplay->drawPixel(x, y);

            iDisplay->update();

            m_timeEvt.armX(TICKS_PER_SEC / 50U, 0);

            status = Q_RET_HANDLED;
            break;
        }
        case system_layer::GUI_TIMEOUT:
        {
            iDisplay->clear();

            iDisplay->drawPixel(x, y);

            iDisplay->update();

            m_timeEvt.armX(TICKS_PER_SEC / 50U, 0);

            x++;

            if (x >= 127)
            {
                y++;
                x = 0;
            }
            if (y >= 63)
            {
                y = 0;
                x = 0;
            }

            status = Q_RET_HANDLED;
            break;
        }
        case system_layer::GUI_UPDATE_DONE:
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
void Gui::setDisplayInterface(device_layer::IDisplay& i_display)
{
    iDisplay = &i_display;
}

void Gui::initDone()
{
    static QP::QEvt const my_evt{system_layer::GUI_INIT_DONE};
    this->POST(&my_evt, this);
}

void Gui::printDone()
{
    static QP::QEvt const my_evt{system_layer::GUI_UPDATE_DONE};
    this->POST(&my_evt, this);
}

}  // namespace app
