#ifndef ICB_I2C_HPP
#define ICB_I2C_HPP

#include <span>
#include <cstdint>

class IcbI2c
{
   public:
    virtual ~IcbI2c() = default;
    virtual void writeDone() = 0;
    virtual void readDone() = 0;
};

#endif  // ICB_I2C_HPP