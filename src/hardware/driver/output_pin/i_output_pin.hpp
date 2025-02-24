#ifndef I_OUTPUT_PIN_HPP
#define I_OUTPUT_PIN_HPP

namespace hardware_layer
{

class IOutputPin
{
   public:
    virtual void set() = 0;
    virtual void reset() = 0;
};

}  // namespace hardware_layer

#endif  // I_OUTPUT_PIN_HPP