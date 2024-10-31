#ifndef I_SPI_HPP
#define I_SPI_HPP

#include <span>
#include <cstdint>

class ISpi
{
   public:
    virtual void Open() = 0;
    virtual void Close() = 0;
    virtual void StartWrite(const std::span<const std::uint8_t> data) = 0;
    virtual void StartRead(const std::span<std::uint8_t> data) = 0;
};

#endif  // I_SPI_HPP