#include "qpcpp.hpp"
#include "stm32l4xx.h"  // CMSIS-compliant header file for the MCU used
#include "stm32l4xx_hal.h"

// Error handler and ISRs...

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
