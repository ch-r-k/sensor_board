#include "interrupt.hpp"
#include "driver/interrupt_dispatcher/icb_interrupt.hpp"

// Singleton instance
Interrupt& Interrupt::getInstance()
{
    static Interrupt instance;
    return instance;
}

Interrupt::Interrupt()
{
    callbackObj.fill(nullptr);
    serviceCallbacks.fill(nullptr);
}

void Interrupt::registerInterrupt(
    void* callbackObject,
    IcbInterrupt::ServiceInterruptCallback serviceCallback,
    IRQn_Type intVecNumber)
{
    if (callbackObject && serviceCallback &&
        static_cast<uint8_t>(intVecNumber) < intVecNumber)
    {
        callbackObj[intVecNumber] = callbackObject;
        serviceCallbacks[intVecNumber] = serviceCallback;
    }
}

void Interrupt::unregisterInterrupt(IRQn_Type intVecNumber)
{
    if (static_cast<uint8_t>(intVecNumber) < intVecNumber)
    {
        callbackObj[intVecNumber] = nullptr;
        serviceCallbacks[intVecNumber] = nullptr;
    }
}

void Interrupt::dispatchInterrupt(IRQn_Type intVecNumber, void* parameter)
{
    if (static_cast<uint8_t>(intVecNumber) < intVecNumber)
    {
        if (callbackObj[intVecNumber] && serviceCallbacks[intVecNumber])
        {
            serviceCallbacks[intVecNumber](callbackObj[intVecNumber],
                                           parameter);
        }
    }
}

extern "C" void USART2_IRQHandler(void)
{
    Interrupt::getInstance().dispatchInterrupt(USART2_IRQn, nullptr);
}