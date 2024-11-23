//============================================================================
// APP example
//============================================================================
#include "qpcpp.hpp"  // QP/C++ real-time embedded framework
#include "system_manager.hpp"
#include "common.hpp"

#include <cstdint>

static SystemManager systemManager;
static Uart qsUart{2};

static QP::QSTimeCtr qsTickTime;
static QP::QSTimeCtr qsTickPeriod;

//............................................................................
int main()
{
    QP::QF::init();  // initialize the framework

    QS_GLB_FILTER(QP::QS_SM_RECORDS);
    QS_GLB_FILTER(QP::QS_SC_RECORDS);

    return QP::QF::run();  // run the QF application
}

//............................................................................

namespace QP::QF
{
void onStartup()
{
    NVIC_SetPriorityGrouping(0U);

    // set up the SysTick timer to fire at BSP_TICKS_PER_SEC rate
    SysTick_Config(SystemCoreClock / ticksPerSec);

    // Uart for QSPY
    qsUart.ConfigureBaudrate(115200);
    qsUart.Configure(Uart::WordLength::Length_8);
    qsUart.Configure(Uart::StopBits::Bit_1_0);
    qsUart.Configure(Uart::Parity::None);
    qsUart.Configure(Uart::HardwareControl::None);
    qsUart.Configure(Uart::Mode::TxRx);
    qsUart.Configure(Uart::OneBitSample::Disable);
    qsUart.Configure(Uart::Oversampling::Oversampling16);

    // lUartHandle.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;

    qsUart.Open();

    systemManager.run();
}

void onCleanup() {}
}  // namespace QP::QF

void QP::QV::onIdle()
{
#ifdef Q_SPY
    // interrupts still disabled
    QS::rxParse();  // parse all the received bytes
    QF_INT_ENABLE();

    if (!qsUart.isTxBusy())
    {  // TXE empty?
        QF_INT_DISABLE();
        std::uint16_t b = QS::getByte();
        std::uint8_t *bytePtr = reinterpret_cast<std::uint8_t *>(&b);
        std::span<std::uint8_t> byteSpan(bytePtr, 1);
        QF_INT_ENABLE();

        if (b != QS_EOD)
        {  // not End-Of-Data?
            qsUart.StartWrite(byteSpan);
        }
    }
#elif defined NDEBUG
    QV_CPU_SLEEP();  // atomically go to sleep and enable interrupts
#else
    QF_INT_ENABLE();  // just enable interrupts
#endif
}

//............................................................................

namespace QP::QS
{
void onCleanup() {}

void onReset() { NVIC_SystemReset(); }

void onCommand(std::uint8_t cmdId, std::uint32_t param1, std::uint32_t param2,
               std::uint32_t param3)
{
    Q_UNUSED_PAR(cmdId);
    Q_UNUSED_PAR(param1);
    Q_UNUSED_PAR(param2);
    Q_UNUSED_PAR(param3);
}

void onFlush()
{
    for (;;)
    {
        std::uint16_t b = QP::QS::getByte();
        if (b != QS_EOD)
        {
            while (qsUart.isTxBusy())
            {
            }

            std::uint8_t *bytePtr = reinterpret_cast<std::uint8_t *>(&b);
            std::span<std::uint8_t> byteSpan(bytePtr, 1);

            qsUart.StartWrite(byteSpan);
        }
        else
        {
            break;
        }
    }
}

QP::QSTimeCtr onGetTime()
{  // NOTE: invoked with interrupts DISABLED
    if ((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) == 0)
    {  // not set?
        return qsTickTime - (QP::QSTimeCtr)SysTick->VAL;
    }
    else
    {  // the rollover occurred, but the SysTick_ISR did not run yet
        return qsTickTime + qsTickPeriod - (QP::QSTimeCtr)SysTick->VAL;
    }
}
}  // namespace QP::QS

//............................................................................

extern "C"
{
    Q_NORETURN Q_onError(char const *const module, int_t const id)
    {
        // NOTE: this implementation of the assertion handler is intended only
        // for debugging and MUST be changed for deployment of the application
        // (assuming that you ship your production code with assertions
        // enabled).
        Q_UNUSED_PAR(module);
        Q_UNUSED_PAR(id);
        QS_ASSERTION(module, id, 10000U);

#ifndef NDEBUG
        // light up the user LED
        // for debugging, hang on in an endless loop...
        for (;;)
        {
        }
#endif

        NVIC_SystemReset();
    }
    //............................................................................
    void assert_failed(char const *const module, int_t const id);  // prototype
    void assert_failed(char const *const module, int_t const id)
    {
        Q_onError(module, id);
    }

    // ISRs used in the application
    // ==============================================
    void SysTick_Handler(void);  // prototype
    void SysTick_Handler(void)
    {
        QP::QTimeEvt::TICK_X(0U, nullptr);  // process time events at rate 0

        QV_ARM_ERRATUM_838869();
    }

}  // extern "C"