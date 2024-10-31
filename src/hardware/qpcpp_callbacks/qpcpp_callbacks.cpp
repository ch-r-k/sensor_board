#include "qpcpp.hpp"
#include "qpcpp_callbacks.hpp"
#include "stm32l4xx.h"  // CMSIS-compliant header file for the MCU used
#include "stm32l4xx_hal.h"

//============================================================================
// namespace QP
namespace QP
{
// QF callbacks...
void QF::onStartup()
{
    // set up the SysTick timer to fire at BSP::TICKS_PER_SEC rate
    SysTick_Config(SystemCoreClock / TICKS_PER_SEC);

    // assign all priority bits for preemption-prio. and none to sub-prio.
    // NOTE: this might have been changed by STM32Cube.
    NVIC_SetPriorityGrouping(0U);

    // set up the SysTick timer to fire at BSP_TICKS_PER_SEC rate
    SysTick_Config(SystemCoreClock / TICKS_PER_SEC);

    // enable IRQs...
#ifdef Q_SPY
    NVIC_EnableIRQ(USART3_IRQn);  // UART interrupt used for QS-RX
#endif
}
//............................................................................
void QF::onCleanup() {}
//............................................................................
void QV::onIdle()
{  // CAUTION: called with interrupts DISABLED, see NOTE0

#ifdef Q_SPY
   // interrupts still disabled
    QS::rxParse();  // parse all the received bytes
    QF_INT_ENABLE();

    if ((l_uartHandle.Instance->ISR & UART_FLAG_TXE) != 0U)
    {  // TXE empty?
        QF_INT_DISABLE();
        std::uint16_t b = QS::getByte();
        QF_INT_ENABLE();

        if (b != QS_EOD)
        {                                    // not End-Of-Data?
            l_uartHandle.Instance->TDR = b;  // put into TDR
        }
    }
#elif defined NDEBUG
   // Put the CPU and peripherals to the low-power mode.
    // you might need to customize the clock management for your application,
    // see the datasheet for your particular Cortex-M MCU.
    QV_CPU_SLEEP();  // atomically go to sleep and enable interrupts
#else
    QF_INT_ENABLE();  // just enable interrupts
#endif
}

//============================================================================
// QS callbacks...
#ifdef Q_SPY
namespace QS
{
//............................................................................
bool onStartup(void const *arg)
{
    Q_UNUSED_PAR(arg);

    return true;  // return success
}
//............................................................................
void onCleanup() {}
//............................................................................
QSTimeCtr onGetTime()
{  // NOTE: invoked with interrupts DISABLED
    if ((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) == 0)
    {  // not set?
        return QS_tickTime_ - (QSTimeCtr)SysTick->VAL;
    }
    else
    {  // the rollover occurred, but the SysTick_ISR did not run yet
        return QS_tickTime_ + QS_tickPeriod_ - (QSTimeCtr)SysTick->VAL;
    }
}
//............................................................................
// NOTE:
// No critical section in QS::onFlush() to avoid nesting of critical sections
// in case QS::onFlush() is called from Q_onError().
void onFlush()
{
    for (;;)
    {
        std::uint16_t b = getByte();
        if (b != QS_EOD)
        {
            while ((l_uartHandle.Instance->ISR & UART_FLAG_TXE) == 0U)
            {
            }
            l_uartHandle.Instance->TDR = b;
        }
        else
        {
            break;
        }
    }
}
//............................................................................
void onReset() { NVIC_SystemReset(); }
//............................................................................
void onCommand(std::uint8_t cmdId, std::uint32_t param1, std::uint32_t param2,
               std::uint32_t param3)
{
    Q_UNUSED_PAR(cmdId);
    Q_UNUSED_PAR(param1);
    Q_UNUSED_PAR(param2);
    Q_UNUSED_PAR(param3);
}

}  // namespace QS
#endif  // Q_SPY
//----------------------------------------------------------------------------

}  // namespace QP