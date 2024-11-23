#ifndef INTERRUPT_DISPATCHER_HPP
#define INTERRUPT_DISPATCHER_HPP

#include "icb_interrupt.hpp"
#include "stm32l476xx.h"

#include <array>

#include <cstddef>

class Interrupt
{
   public:
    static Interrupt& getInstance();

    void registerInterrupt(
        void* callbackObject,
        IcbInterrupt::ServiceInterruptCallback serviceCallback,
        IRQn_Type intVecNumber);

    void unregisterInterrupt(IRQn_Type intVecNumber);

    void dispatchInterrupt(IRQn_Type intVecNumber, void* parameter);

   private:
    Interrupt();
    Interrupt(const Interrupt&) = delete;
    Interrupt& operator=(const Interrupt&) = delete;

    static constexpr size_t intVecNumber = 82;  // Adjust as per actual size

    std::array<void*, intVecNumber> callbackObj;
    std::array<IcbInterrupt::ServiceInterruptCallback, intVecNumber>
        serviceCallbacks;
};

extern "C" void USART2_IRQHandler(void);

#endif  // INTERRUPT_DISPATCHER_HPP