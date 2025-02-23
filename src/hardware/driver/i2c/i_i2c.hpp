#ifndef I_I2C_HPP
#define I_I2C_HPP

#include <span>
#include <cstdint>

namespace hardware_layer
{
namespace interface
{
class II2c
{
   public:
    virtual ~II2c() = default;
    virtual void open() = 0;
    virtual void close() = 0;
    virtual void setAddress(const std::uint8_t) = 0;
    virtual void startWrite(const std::span<const std::uint8_t> data) = 0;
    virtual void startRead(const std::span<std::uint8_t> data) = 0;
};
}  // namespace interface
}  // namespace hardware_layer
#endif  // I_I2C_HPP