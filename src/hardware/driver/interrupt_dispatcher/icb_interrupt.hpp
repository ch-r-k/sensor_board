#ifndef ICB_INTERRUPT_HPP
#define ICB_INTERRUPT_HPP

#include <functional>

class IcbInterrupt
{
   public:
    using ServiceInterruptCallback =
        std::function<void(void* callbackObject, void* parameter)>;
};

#endif  // ICB_INTERRUPT_HPP
