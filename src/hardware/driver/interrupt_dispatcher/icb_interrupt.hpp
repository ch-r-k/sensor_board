#ifndef ICB_INTERRUPT_HPP
#define ICB_INTERRUPT_HPP

#include <functional>

namespace hardware_layer
{
class IcbInterrupt
{
   public:
    using ServiceInterruptCallback =
        std::function<void(void* callbackObject, void* parameter)>;
};

}  // namespace hardware_layer

#endif  // ICB_INTERRUPT_HPP
