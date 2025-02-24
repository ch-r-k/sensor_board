#ifndef ICB_I2C_HPP
#define ICB_I2C_HPP

#include <span>
#include <cstdint>

namespace hardware_layer
{

class IcbI2c
{
   public:
    virtual ~IcbI2c() = default;
    virtual void writeDone() = 0;
    virtual void readDone() = 0;
};

}  // namespace hardware_layer
#endif  // ICB_I2C_HPP