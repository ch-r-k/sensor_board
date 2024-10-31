#ifndef I_I2C_HPP
#define I_I2C_HPP

#include <span>
#include <cstdint>

class II2c
{
   public:
    virtual void Open() = 0;
    virtual void Close() = 0;
    virtual void StartWrite(const std::span<std::uint8_t> data) = 0;
    virtual void StartRead(const std::span<std::uint8_t> data) = 0;
};

#endif  // I_I2C_HPP