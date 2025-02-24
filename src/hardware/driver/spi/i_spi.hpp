#ifndef I_SPI_HPP
#define I_SPI_HPP

#include <span>
#include <cstdint>

namespace hardware_layer
{

class ISpi
{
   public:
    virtual void open() = 0;
    virtual void close() = 0;
    virtual void startWrite(const std::span<const std::uint8_t> data) = 0;
    virtual void startRead(const std::span<std::uint8_t> data) = 0;
};

}  // namespace hardware_layer
#endif  // I_SPI_HPP