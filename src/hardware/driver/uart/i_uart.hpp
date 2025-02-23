#ifndef I_UART_HPP
#define I_UART_HPP

#include <span>
#include <cstdint>

namespace hardware_layer
{
namespace interface
{
class IUart
{
   public:
    virtual ~IUart() = default;

    virtual void Open() = 0;
    virtual void Close() = 0;
    virtual void StartWrite(const std::span<std::uint8_t> data) = 0;
    virtual void StartRead(const std::span<std::uint8_t> data) = 0;
};
}  // namespace interface
}  // namespace hardware_layer
#endif  // I_UART_HPP