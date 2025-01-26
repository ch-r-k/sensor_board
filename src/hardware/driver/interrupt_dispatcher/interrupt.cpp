#include "interrupt.hpp"
#include "driver/interrupt_dispatcher/icb_interrupt.hpp"
#include "stm32l476xx.h"
#include "qpcpp.hpp"

namespace hardware_layer
{
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
    IRQn_Type int_vec_number)
{
    if (callbackObject && serviceCallback &&
        static_cast<uint8_t>(int_vec_number) < intVecNumberMax)
    {
        callbackObj[int_vec_number] = callbackObject;
        serviceCallbacks[int_vec_number] = serviceCallback;
    }
}

void Interrupt::unregisterInterrupt(IRQn_Type int_vec_number)
{
    if (static_cast<uint8_t>(int_vec_number) < intVecNumberMax)
    {
        callbackObj[int_vec_number] = nullptr;
        serviceCallbacks[int_vec_number] = nullptr;
    }
}

void Interrupt::dispatchInterrupt(IRQn_Type int_vec_number, void* parameter)
{
    if (static_cast<uint8_t>(int_vec_number) < intVecNumberMax)
    {
        if (callbackObj[int_vec_number] && serviceCallbacks[int_vec_number])
        {
            serviceCallbacks[int_vec_number](callbackObj[int_vec_number],
                                             parameter);
        }
    }
}

extern "C" void USART2_IRQHandler(void)
{
    Interrupt::getInstance().dispatchInterrupt(USART2_IRQn, nullptr);
}

extern "C" void SPI2_IRQHandler(void)
{
    Interrupt::getInstance().dispatchInterrupt(SPI2_IRQn, nullptr);
}

extern "C" void SPI3_IRQHandler(void)
{
    Interrupt::getInstance().dispatchInterrupt(SPI3_IRQn, nullptr);
}

extern "C" void I2C1_EV_IRQHandler(void)
{
    Interrupt::getInstance().dispatchInterrupt(I2C1_EV_IRQn, nullptr);
}

extern "C" void I2C2_EV_IRQHandler(void)
{
    Interrupt::getInstance().dispatchInterrupt(I2C2_EV_IRQn, nullptr);
}

extern "C" void I2C3_EV_IRQHandler(void)
{
    Interrupt::getInstance().dispatchInterrupt(I2C3_EV_IRQn, nullptr);
}

}  // namespace hardware_layer